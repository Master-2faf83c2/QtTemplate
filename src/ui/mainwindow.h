#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"

#include <QMainWindow>
#include <QThread>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
    Ui::MainWindow* getUi();
protected:

private:
    Ui::MainWindow *m_ui;
};
#endif // MAINWINDOW_H
