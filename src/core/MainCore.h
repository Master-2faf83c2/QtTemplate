#ifndef _APP_MAIN_WINDOW_H
#define _APP_MAIN_WINDOW_H

#include "MainWindow.h"

#include "Wave.h"

#include <memory>

#include <QObject>
#include <QTimer>
#include <QQueue>

class MainCore : public QObject{
    Q_OBJECT
public:
    MainCore();
    ~MainCore();

    void ShowUi();
protected:

private slots:
    
signals:
    void waveEmit(double x, double y);
private:
    std::unique_ptr<MainWindow>     m_window;
    std::unique_ptr<WaveBase>       m_wave;
};

#endif
