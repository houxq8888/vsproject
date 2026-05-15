#ifndef HGRUNNINGWIDGET_H
#define HGRUNNINGWIDGET_H

#include <QWidget>
#include "hgqlabel.h"
#include "labelwithimg.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QListWidget>
#include <QPushButton>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QRadioButton>
#include <QStackedWidget>
#include "myitem.h"
#include "myscene.h"
#include "hgonlinerwDB.h"
#include "PointHandle.h"
#include "PolylineItem.h"
#include "hgrunningchartwidget.h"
#include "hgdisplaychartwidget.h"
#include "HGOnlinePlatformInterface.h"
#include "common.h"

using namespace HGMACHINE;

#ifdef ENABLE_CAS6
#define MAX_VIEW_NUMBER 1
#else
#define MAX_VIEW_NUMBER 4
#endif

enum {
    RUNNING_DEVICE_TYPE,
    RUNNING_CHART_TYPE
};

class HGRunningWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGRunningWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();
    ~HGRunningWidget();
    void setRunningMethodInfo(const QString& info);

signals:
    void signalRunning();

private slots:
    void slotLabelClicked(std::string);
    void slotStart();
    void slotPause();
    void slotUpdateDataInfo();
    
private:
    void fnDisplayDeviceStatus();
    void fnDisplayChart();
    void fnInitChartType();
    void fnInitChartElement();
    void fnDisplayRunningStatus(int index);

private:
    QPushButton *m_deviceStatusBtn, *m_chartBtn;
    std::vector<QGraphicsView*> m_views;
    std::vector<MyScene*> m_scenes;
    std::vector<HGRunningChartWidget*> m_charts;
    QGridLayout* m_layout;
    std::string m_lang;

    
    HGQLabel *m_startLabel, *m_pauseLabel;
    #ifdef ENABLE_CAS6
    LabelWithImg *m_radioBtnChannel1;
    HGDisplayChartWidget *m_chart;
    #else
    LabelWithImg *m_radioBtnChannel1, *m_radioBtnChannel2,*m_radioBtnChannel3,*m_radioBtnChannel4;
    #endif

    QLabel* m_dataInfoLabel;
    QLabel* m_methodInfoLabel;
    QLabel* m_resultInfoLabel;
    QStackedWidget *m_stackedWidget;

    int m_runningStatusType;
    int m_curChannelIndex;

    QTimer *m_timer;
};

#endif // HGRUNNINGWIDGET_H
