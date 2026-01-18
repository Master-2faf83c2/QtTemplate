#include "MainWindow.h"
#include "Config.h"
#include "Utils.h"
#include "Version.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);

    setWindowIcon(QIcon(":/icons/logo.jpg"));
    
    //程序版本号
    QString title = windowTitle();
    setWindowTitle(title + "-" + APP_VERSION);
}

MainWindow::~MainWindow(){
    delete m_ui;
}

Ui::MainWindow* MainWindow::getUi(){
    return m_ui;
}
