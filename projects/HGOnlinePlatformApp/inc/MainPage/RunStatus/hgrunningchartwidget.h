#ifndef HGRUNNINGCHARTWIDGET_H
#define HGRUNNINGCHARTWIDGET_H

#include <QWidget>
#include "hgqlabel.h"
#include "labelwithimg.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QSplitter>
#include <QListWidget>
#include <QChart>
#include "mychartview.h"
#include <QPushButton>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QRadioButton>
#include <QBarSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QCategoryAxis>
#include "myitem.h"
#include "myscene.h"
#include "DragDialog.h"
#include "hgcurvewidget.h"
#include <QColor>

struct curColorStruct{
    std::string curType;
    QColor color;
    curColorStruct(std::string type, QColor color):curType(type),color(color){}
};
struct curLabel{
    HGCurveWidget* label;
    bool flag;
    curLabel(){
        Clear();
    }
    void Clear(){
        label=NULL;
        flag=false;
    }
};

struct recordLabelWithSeries1{
    recordLabel label;
    QLineSeries* seriesLine;
    QBarSeries* seriesBar;
    int voltageMax;
    int voltageMin;
    float balanceMax;
    float balanceMin;
    float waterWeightMin;
    float waterWeightMax;
    recordLabelWithSeries1(){
        label.Clear();
        seriesLine=NULL;
        seriesBar=NULL;
        voltageMax=0;
        voltageMin=0;
        balanceMax=.0;
        balanceMin=.0;
        waterWeightMin=.0;
        waterWeightMax=.0;
    }
};

class HGRunningChartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGRunningChartWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();
    ~HGRunningChartWidget();

signals:

private slots:
    void slotInTimeDisplayType();
    void slotDisplayType();
    void slotLabelClicked(std::string);
    
    
private:
    
private:
    QGridLayout* m_layout;
    QChart* m_chart;
    MyChartView* m_chartView;

    QSplitter* m_horSplitter;
    QGroupBox* m_flowGroup, *m_chartGroup;
    QGridLayout* m_flowLayout, *m_chartLayout;
    std::string m_lang;
    HGQLabel* m_displayTypeLabel,*inTimeDisplayLabel;

    std::map<std::string,recordLabelWithSeries1> m_displayLabels;
    std::map<std::string,curLabel> m_recordLabels;
    DraggableDialog *m_dragDialog, *m_displayDialog;
};

#endif // HGRUNNINGCHARTWIDGET_H
