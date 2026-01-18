#ifndef _UTIL_LOG_H
#define _UTIL_LOG_H


#include "Singleton.h"

#include <QDebug>
#include <QDateTime>
#include <QMutexLocker>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTimer>
#include <QTextStream>

// 不再需要按大小切割，保留全局日志大小常量可删或保留备用
// const int g_logLimitSize = 5;

struct LogHandlerPrivate {
    LogHandlerPrivate();
    ~LogHandlerPrivate();

    // 打开当天的日志文件（按小时生成日志文件）
    void openLogFile();

    // 自动删除30天前日志
    void autoDeleteLog();

    // 消息处理函数
    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

    QDir   logBaseDir;           // 根日志文件夹，例如 "log"
    QTimer renameLogFileTimer;   // 每分钟检查是否需要切换小时文件
    QTimer flushLogFileTimer;    // 定时刷新输出到日志文件
    QDate  currentDate;          // 当前日志的日期
    QString currentHourFile;     // 当前小时日志文件名

    static QFile *logFile;       // 当前日志文件
    static QTextStream *logOut;  // 输出日志的 QTextStream
    static QMutex logMutex;      // 同步使用的 mutex
};

class LogHandler : public Singleton<LogHandler>{
    friend class Singleton<LogHandler>;
public:
    void installMessageHandler();   // 给Qt安装消息处理函数
    void uninstallMessageHandler(); // 取消安装消息处理函数并释放资源
private:
    LogHandler();

    LogHandlerPrivate *d;
};

#define G_Logger        LogHandler::getInstance()

#endif
