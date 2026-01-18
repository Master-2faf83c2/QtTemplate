#ifndef _UTIL_WAVE_H
#define _UTIL_WAVE_H

#include "Qcustomplot.h"
#include <QObject>

class WaveBase : public QObject {
    Q_OBJECT
public:
    explicit WaveBase(QObject *parent = nullptr);

    void baseInitialize(
        QCustomPlot *plot,
        const QString &title,
        const QString &x_title,
        const QString &y_title,
        int x_range_min = 0,
        int x_range_max = 10,
        double y_range_min = 0.0,
        double y_range_max = 10.0,
        bool is_auto_refresh = false
    );
    void resetPlotRange();
    void addWaveform(double x, double y);
    void updateWaveform();
    void clear();
private:
    QCustomPlot *m_plot = nullptr;
    QCPGraph *m_graph = nullptr;
    
    double m_x_min, m_x_max, m_x_range;
    double m_current_y, m_y_min, m_y_max;

    bool m_user_interacting;
    bool m_is_init;
    bool m_is_auto_refresh;

    int m_move_mode;
};

#endif
