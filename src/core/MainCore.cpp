#include "MainCore.h"

#include "Config.h"
#include "Utils.h"

MainCore::MainCore()
    : m_window(std::make_unique<MainWindow>())
    , m_wave(std::make_unique<WaveBase>())
{   
    auto ui = m_window->getUi();
    auto title = getCurrentDateString();
    auto x = getCurrentSec();

    m_wave->baseInitialize(
        ui->graphicsView,
        title,
        "时间(s)",
        "测试温度(℃)",
        x,
        x,
        80,
        100,
        true
    );
    
    connect(this, &MainCore::waveEmit, m_wave.get(), &WaveBase::addWaveform, Qt::QueuedConnection);

    G_Config.loadFromInfoJson();
}

MainCore::~MainCore(){
    
}

void MainCore::ShowUi(){
    m_window->show();
}
