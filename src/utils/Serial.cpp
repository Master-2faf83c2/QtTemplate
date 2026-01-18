#include "Serial.h"

#include <QDebug>
#include <QThread>

SerialPrivate::SerialPrivate(QObject *parent)
    : QObject(parent)
    , m_serial(std::make_unique<QSerialPort>(this))
    , m_is_runing(true)
{
    connect(m_serial.get(), &QSerialPort::readyRead, this, &SerialPrivate::readData);

    m_func.push_back([this](){ parseDataThread(); });
    m_func.push_back([this](){ sendDataThread(); });
}

SerialPrivate::~SerialPrivate(){
    if (m_serial->isOpen())
        m_serial->close();
}

int SerialPrivate::scanPorts(QStringList &scanf_ports){
    
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    if (ports.isEmpty()) {
        qDebug() << "没有可用串口";
        return 1;
    }

    for (const QSerialPortInfo &info : ports){
        scanf_ports.append(info.portName());
    }

    return 0;
}

int SerialPrivate::open(const QString &portName, qint32 baudRate){
    m_serial->setPortName(portName);
    m_serial->setBaudRate(baudRate);
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setStopBits(QSerialPort::OneStop);
    m_serial->setFlowControl(QSerialPort::NoFlowControl);

    if (!m_serial->open(QIODevice::ReadWrite)){
        qDebug() << "串口打开失败:" << m_serial->errorString();
        return 1;
    }
    return 0;
}

int SerialPrivate::isOpen(){
    return (m_serial->isOpen() ? 1 : 0);
}

void SerialPrivate::close(){
    if(m_serial->isOpen())
        m_serial->close();
}

void SerialPrivate::sendData(const QByteArray &data){
    if (m_serial->isOpen()){
        QByteArray d = data;
        d.append(0x0A);
        m_tx_buffer.push(d);
    }
}

void SerialPrivate::startThread(){
    for (size_t i = 0; i < m_func.size(); ++i) {
        //用 QThread::create 启动 lambda
        QThread* thread = QThread::create([func = m_func[i]](){
            func(); // 执行该函数
        });
        m_serial->moveToThread(thread);

        //线程结束后自动删除
        connect(thread, &QThread::finished, m_serial.get(), &QObject::deleteLater);

        //保存线程指针
        m_thread.push_back(thread);

        //启动线程
        thread->start();
    }
}

void SerialPrivate::registerCallback(std::function<void(const std::vector<uint8_t>&)> cb){
    m_rx_cb = cb;
    qDebug() << "已注册数据解析回调函数!";
}

void SerialPrivate::testSend(){
    QByteArray data = "*APA00000";

    sendData(data);
    qDebug() << "发送测试数据";
}

void SerialPrivate::parseDataThread(){

    std::vector<uint8_t> frame;

    qDebug() << "协议数据解析线程启动...";

    while(m_is_runing){
        auto byte = m_rx_buffer.pop();
        frame.push_back(byte);
        if (byte == 0x0A){
            if (m_rx_cb){
                m_rx_cb(frame);
            }else {
                qDebug() << "未注册回调函数";
            }
            // qDebug() << "收到数据:" << frame;
            frame.clear();
        }
    }
}

void SerialPrivate::sendDataThread(){
    qDebug() << "发送数据线程启动...";

    while(m_is_runing){
        auto bytes = m_tx_buffer.pop();
        if(m_serial->isOpen()){
            m_serial->write(bytes);
            m_serial->flush();
            
            // qDebug() << "发送数据: " << bytes;

            QThread::msleep(10);
        }
    }
}

void SerialPrivate::readData(){
    QByteArray bytes = m_serial->readAll();
    // qDebug() << "收到数据:" << bytes;
    for (uint8_t b : bytes) {
        m_rx_buffer.push(b);
    }
}
