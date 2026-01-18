#ifndef _UTIL_SERIAL_H
#define _UTIL_SERIAL_H

#include "SafeQueue.h"

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QObject>

class SerialPrivate : public QObject {
    Q_OBJECT
public:
    SerialPrivate(QObject *parent = nullptr);
    ~SerialPrivate();

    int scanPorts(QStringList &scanf_ports);
    int open(const QString &portName, qint32 baudRate);
    int isOpen();
    void testSend();
    void close();
    void sendData(const QByteArray &data);
    void startThread();
    void registerCallback(std::function<void(const std::vector<uint8_t>&)> cb);
protected:
    void parseDataThread();
    void sendDataThread();

private slots:
    void readData();

private:
    std::unique_ptr<QSerialPort>                        m_serial;
    SafeQueue<uint8_t>                                  m_rx_buffer;
    SafeQueue<QByteArray>                               m_tx_buffer;
    std::atomic<bool>                                   m_is_runing;
    std::vector<QThread*>                               m_thread;
    std::vector<std::function<void()>>                  m_func;
    std::function<void(const std::vector<uint8_t>&)>    m_rx_cb;
};

#endif
