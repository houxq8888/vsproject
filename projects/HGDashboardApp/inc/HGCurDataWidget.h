#ifndef HGCURDATAWIDGET_H
#define HGCURDATAWIDGET_H


#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QMouseEvent>
#include <QGridLayout>
#include <QGroupBox>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QWidget>
#include <QPalette>
#include <QtCharts>
#include <QChart>
#include <QChartView>
#include <QSplineSeries>
#include <QLineSeries>
#include <QScatterSeries>
#include "mychartview.h"

QRect getScreenGeometry();

class HGCurDataWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HGCurDataWidget(int tickcount,const std::string& name,QWidget *parent = nullptr);
    virtual ~HGCurDataWidget();
    void fnUpdateData(const std::map<std::string, std::string>& msg);

private:

private slots:
    void slotPointHoverd(const QPointF &point, bool state);

private:
    void updateYAxis();
    
private:
    QGridLayout *m_layout;
    MyChartView  *m_chartView;
    QChart *m_chart;
    QLineSeries *m_series;
    QScatterSeries *m_scatterSeries;
    QLabel* m_Alkalinity1Label;

    QDateTimeAxis* m_xAxisDateTime;
    QValueAxis* m_xAxis;
    QValueAxis* m_yAxis;

    QLabel *m_valueLabel;
    
};

#endif // HGCURDATAWIDGET_H
