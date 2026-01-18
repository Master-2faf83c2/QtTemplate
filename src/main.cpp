#include "Config.h"
#include "MainCore.h"
#include "Log.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
    MainCore main;

    //安装日志
    G_Logger.installMessageHandler();
    //目录初始化
    G_Config.directoryInit();
    //加载翻译文件
    G_Config.loadTranslator(a);

    main.ShowUi();

    int ret = a.exec();
    G_Logger.uninstallMessageHandler();

    return ret;
}
