#ifndef _UTILS_H
#define _UTILS_H

#include <QString>
#include <QDateTime>
#include <QTextEdit>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QBrush>
#include <QLabel>
#include <QWidget>


struct FontSizeInfo{
    int size;
    float scale;

    FontSizeInfo(int s = 12, float sc = 1.0f)
    : size(s)
    , scale(sc){}
};

//获取ms字符串
QString getCurrentTimeMsString(QString separator=":");
//获取s字符串
QString getCurrentTimeSecString(QString separator=":");
//获取当天日期
QString getCurrentDateString();

QString getCurrentTime();

QString formatFixedWidth(double value, int width, int decimals);

QString formatIntFixedWidth(int value, int width);
//返回文件夹内所有文件夹只有一层
QStringList listFolderNonRecursive(const QString &folder_path);

QJsonObject loadJsonParameter(QString file_path);
//计算字体函数
int calcFontSize(int base, float scale);

double getCurrentSec();

double StrToSec(QString str);

void fontResize(QWidget *w, int size);

void writeRealTimeTemperatureData(double value);

void appendLog(QTextEdit *edit, const QString &text, const QColor &color);

template <typename T>
bool inRange(const T& value, const T& minValue, const T& maxValue){
    return (value >= minValue) && (value <= maxValue);
}

#endif
