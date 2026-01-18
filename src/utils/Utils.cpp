#include "Utils.h"
#include "Config.h"

#include <algorithm> 

#include <QFile>
#include <QComboBox>
#include <QFileDialog>
#include <QJsonParseError>

QString getCurrentTime(){
    return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
}

QString getCurrentDateString(){
    QDateTime current = QDateTime::currentDateTime();

    QString dateStr = current.toString("yyyy-MM-dd");

    return dateStr;
}

QString getCurrentTimeSecString(QString separator) {
    QDateTime now = QDateTime::currentDateTime();

    QString str = now.toString("yyyy-MM-dd HH" + separator + "mm" + separator + "ss");
    return str;
}

QString formatFixedWidth(double value, int width, int decimals){
    //分离符号
    bool negative = (value < 0);
    double absValue = negative ? -value : value;

    //格式化绝对值
    QString s = QString::number(absValue, 'f', decimals);

    //计算总长度（符号 + 数字）
    int totalLen = s.length() + (negative ? 1 : 1); 
    //这里保证符号位(+/-)都占宽度

    // 补足前导零（符号后的部分）
    int need = width - totalLen;
    if (need > 0) {
        s = QString(need, '0') + s;
    }

    //加回符号
    s = (negative ? "-" : "0") + s;

    return s;
}

QString formatIntFixedWidth(int value, int width){
    QString s = QString::number(value);

    int need = width - s.length();
    if (need > 0) {
        s = QString(need, '0') + s;
    }

    return s;
}

QStringList listFolderNonRecursive(const QString &folder_path){
    QDir dir(folder_path);
    QStringList file_list;

    if (!dir.exists()) {
        qDebug() << "目录不存在:" << folder_path;
        return file_list;
    }

    file_list = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    return file_list;
}

QString getCurrentTimeMsString(QString separator) {
    QDateTime now = QDateTime::currentDateTime();

    QString str = now.toString("yyyy-MM-dd HH" + separator + "mm" + separator + "ss.zzz");
    return str;
}

QJsonObject loadJsonParameter(QString file_path){
    QFile file(file_path);

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "无法打开文件:" << file_path;
        return {};
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << "JSON 解析错误:" << parseError.errorString();
        return {};
    }

    if (!doc.isObject()) {
        qDebug() << "JSON 不是对象!";
        return {};
    }

    QJsonObject obj = doc.object();

    return obj;
}

int calcFontSize(int base, float scale){
    return qMax(1, static_cast<int>(base * scale));
}

double getCurrentSec(){
    qint64 ms = QDateTime::currentDateTime().toMSecsSinceEpoch();
    double s = ms / 1000.0;

    return s;
}

double StrToSec(QString str){
    QDateTime dt = QDateTime::fromString(
        str,
        "yyyy-MM-dd HH:mm:ss.zzz"
    );
    dt.setTimeZone(QTimeZone::systemTimeZone());
    qint64 ms = dt.toMSecsSinceEpoch();
    double sec = ms / 1000.0;

    return sec;
}

void fontResize(QWidget* w, int size) {
    if (!w) return;

    size = std::max(size, 35);

    QFont f = w->font();
    f.setPointSize(size);
    w->setFont(f);
}

void writeRealTimeTemperatureData(double value){
    auto path = G_Config.getTemperatureRecordsPath();
    QDir dir(path);
    auto date_str = getCurrentDateString() + ".csv";
    auto t_path = dir.filePath(date_str);

    QFile file(t_path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
        qDebug() << "无法打开文件:" << file.errorString();
        return;
    }

    auto current_time = getCurrentTimeMsString(":");

    QTextStream out(&file);

    out << current_time << "," << value << "\n";

    file.close();
}

void appendLog(QTextEdit *edit, const QString &text, const QColor &color){

    QString time = getCurrentTime();

    // 创建格式
    QTextCharFormat format;
    format.setForeground(QBrush(color));

    // 移动光标到末尾
    QTextCursor cursor = edit->textCursor();
    cursor.movePosition(QTextCursor::End);

    // 插入时间（灰色）
    QTextCharFormat timeFormat;
    timeFormat.setForeground(QBrush(Qt::gray));
    cursor.insertText("[" + time + "] ", timeFormat);

    // 插入主文本（彩色）
    cursor.insertText(text + "\n", format);

    // 自动滚动到底部
    edit->moveCursor(QTextCursor::End);
}

