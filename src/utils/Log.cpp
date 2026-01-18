#include "Log.h"

#include <iostream>

QMutex LogHandlerPrivate::logMutex;
QFile* LogHandlerPrivate::logFile = nullptr;
QTextStream* LogHandlerPrivate::logOut = nullptr;

LogHandlerPrivate::LogHandlerPrivate() {
    logBaseDir.setPath("log"); // 根日志文件夹
    openLogFile();

    // 每分钟检查一次是否需要切换日志文件（按小时切换）
    renameLogFileTimer.setInterval(1000 * 60);
    renameLogFileTimer.start();
    QObject::connect(&renameLogFileTimer, &QTimer::timeout, [this] {
        QMutexLocker locker(&LogHandlerPrivate::logMutex);
        openLogFile();
        autoDeleteLog(); // 删除30天前日志
    });

    // 定时刷新日志文件
    flushLogFileTimer.setInterval(1000); 
    flushLogFileTimer.start();
    QObject::connect(&flushLogFileTimer, &QTimer::timeout, [] {
        QMutexLocker locker(&LogHandlerPrivate::logMutex);
        if (logOut) logOut->flush();
    });
}

LogHandlerPrivate::~LogHandlerPrivate() {
    if (logFile) {
        logFile->flush();
        logFile->close();
        delete logOut;
        delete logFile;
        logOut = nullptr;
        logFile = nullptr;
    }
}

void LogHandlerPrivate::openLogFile() {
    QDate today = QDate::currentDate();
    QString todayDirName = today.toString("yyyy-MM-dd");
    QDir todayDir(logBaseDir.filePath(todayDirName));
    if (!todayDir.exists()) {
        todayDir.mkpath(".");
    }

    QString hourFileName = todayDirName + QTime::currentTime().toString("-HH") + ".log";
    QString logPath = todayDir.absoluteFilePath(hourFileName);

    // 如果当前日志文件是同一小时则不用切换
    if (logFile && logFile->fileName() == logPath) return;

    // 关闭旧日志文件
    if (logFile) {
        logFile->flush();
        logFile->close();
        delete logOut;
        delete logFile;
    }

    logFile = new QFile(logPath);
    logOut = (logFile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
                 ? new QTextStream(logFile)
                 : nullptr;
    if (logOut) logOut->setEncoding(QStringConverter::Utf8);
}

// 自动删除30天前日志
void LogHandlerPrivate::autoDeleteLog() {
    QDate threshold = QDate::currentDate().addDays(-30);
    QFileInfoList dayDirs = logBaseDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (const QFileInfo &dirInfo : dayDirs) {
        QDate dirDate = QDate::fromString(dirInfo.fileName(), "yyyy-MM-dd");
        if (dirDate.isValid() && dirDate < threshold) {
            QDir(dirInfo.absoluteFilePath()).removeRecursively();
        }
    }
}

// 消息处理函数保持不变
void LogHandlerPrivate::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QMutexLocker locker(&LogHandlerPrivate::logMutex);
    QString level;
    switch (type) {
        case QtDebugMsg:    level = "DEBUG"; break;
        case QtInfoMsg:     level = "INFO "; break;
        case QtWarningMsg:  level = "WARN "; break;
        case QtCriticalMsg: level = "ERROR"; break;
        case QtFatalMsg:    level = "FATAL"; break;
        default: break;
    }

#if defined(Q_OS_WIN)
    QStringEncoder encoder("UTF-8");
    QByteArray localMsg = encoder(msg);
#else
    QByteArray localMsg = msg.toLocal8Bit();
#endif
    std::cout << std::string(localMsg) << std::endl;

    if (!logOut) return;

    QString fileName = context.file;
    int index = fileName.lastIndexOf(QDir::separator());
    fileName = fileName.mid(index + 1);

    (*logOut) << QString("%1 - [%2] (%3:%4, %5): %6\n")
                  .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                  .arg(level)
                  .arg(fileName)
                  .arg(context.line)
                  .arg(context.function)
                  .arg(msg);
}

LogHandler::LogHandler() : d(nullptr) {
}

void LogHandler::installMessageHandler() {
    QMutexLocker locker(&LogHandlerPrivate::logMutex);
    if (d == nullptr) {
        d = new LogHandlerPrivate();
        qInstallMessageHandler(LogHandlerPrivate::messageHandler);
    }
}

void LogHandler::uninstallMessageHandler() {
    QMutexLocker locker(&LogHandlerPrivate::logMutex);
    qInstallMessageHandler(nullptr);
    delete d;
    d = nullptr;
}
