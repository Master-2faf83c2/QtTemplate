#include "Wave.h"
#include <qlogging.h>

#define MAX_COMPENSATE  0.1
#define TIME_RANGE      1

WaveBase::WaveBase(QObject *parent)
    : QObject(parent)
    , m_user_interacting(false)
    , m_is_init(false)
    , m_is_auto_refresh(false)
    , m_move_mode(0)
{

}

void WaveBase::baseInitialize(
    QCustomPlot *plot,
    const QString &title,
    const QString &x_title,
    const QString &y_title,
    int x_range_min,
    int x_range_max,
    double y_range_min,
    double y_range_max,
    bool is_auto_refresh
){

    m_plot = plot;
    m_x_min = x_range_min;
    m_x_max = x_range_max;
    m_y_min = y_range_min;
    m_y_max = y_range_max;
    m_current_y  = 0;
    m_is_auto_refresh = is_auto_refresh;

    // 创建曲线
    m_graph = m_plot->addGraph();
    m_graph->setPen(QPen(Qt::blue, 1));
    m_graph->setAdaptiveSampling(true);

    // 设置坐标轴标题
    m_plot->xAxis->setLabel(x_title);
    m_plot->yAxis->setLabel(y_title);

    QSharedPointer<QCPAxisTickerDateTime> timeTicker(new QCPAxisTickerDateTime);
    timeTicker->setDateTimeFormat("HH:mm:ss.zzz");
    timeTicker->setTickOrigin(0);
    timeTicker->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
    m_plot->xAxis->setTicker(timeTicker);

    // 设置坐标轴范围
    m_plot->xAxis->setRange(m_x_min, m_x_max);
    m_plot->yAxis->setRange(m_y_min, m_y_max);

    // 设置标题
    m_plot->plotLayout()->insertRow(0);
    QCPTextElement *titleElement = new QCPTextElement(m_plot, title, QFont("Arial", 14, QFont::Bold));
    m_plot->plotLayout()->addElement(0, 0, titleElement);

    // 开启交互（滚轮缩放、拖动、选择）
    m_plot->setInteractions(
        QCP::iRangeDrag |
        QCP::iRangeZoom |
        QCP::iSelectPlottables
    );

    // m_plot->setOpenGl(true);

    connect(m_plot, &QCustomPlot::mousePress, this, [=](){ 
        m_user_interacting = true; 
    });

    connect(m_plot, &QCustomPlot::mouseWheel, this, [=](QWheelEvent*) {
        m_user_interacting = true;
    });

    m_is_init = true;
}

void WaveBase::resetPlotRange(){
    if (!m_is_init)
        return;
    m_user_interacting = false; 
    m_plot->xAxis->setRange(m_x_min, m_x_max);
    m_plot->yAxis->setRange(m_y_min, m_y_max + (m_current_y*MAX_COMPENSATE));
    m_plot->replot();
}

void WaveBase::addWaveform(double x, double y){

    m_graph->addData(x, y);
    
    if (x > m_x_max) m_x_max = x;
    if (x < m_x_min) m_x_min = x;
    if (y > m_y_max) m_y_max = y;
    if (y < m_y_min) m_y_min = y;

    m_current_y = y;

    if (!m_user_interacting)
    {   
        bool is_x_set_range = false;
        double x_range_min = 0, x_range_max = 0;

        if (m_move_mode == 0){
            m_x_range = x;
            m_x_min = m_x_range;
            x_range_min = m_x_range;
            x_range_max = m_x_range + TIME_RANGE;
            m_move_mode = 1;
            is_x_set_range = true;
        }else if (m_move_mode == 1){
            if (x > m_x_range+TIME_RANGE){
                m_move_mode = 2;
            }
        }
        
        if (m_move_mode == 2){
            x_range_min = m_x_max - TIME_RANGE;
            x_range_max = m_x_max;
            is_x_set_range = true;
        }
        
        if (is_x_set_range){
            m_plot->xAxis->setRange(x_range_min, x_range_max);
        }
        
        double y_min = m_y_min;
        double y_max = m_y_max;
        for (auto it = m_graph->data()->constBegin(); it != m_graph->data()->constEnd(); ++it)
        {
            if (it->value < y_min) y_min = it->value;
            if (it->value > y_max) y_max = it->value;
        }
        m_plot->yAxis->setRange(y_min, y_max + y_max * MAX_COMPENSATE);
    }

    if (m_is_auto_refresh){
        updateWaveform();
    }
}

void WaveBase::updateWaveform(){
    m_plot->replot();
}

void WaveBase::clear(){
    if (m_graph) {
        m_graph->data()->clear();
    }

    m_plot->xAxis->setRange(m_x_min, m_x_max);
    m_plot->yAxis->setRange(m_y_min, m_y_max);

    m_plot->replot();
}
