#ifndef HGDISPLAYCHARTWIDGET_H
#define HGDISPLAYCHARTWIDGET_H

#include <QWidget>
#include "hgqlabel.h"
#include <QGroupBox>
#include <QGridLayout>
#include "mychartview.h"
#include "DragDialog.h"
#include <QTimer>
#include <QBarSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QCategoryAxis>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
using namespace QtCharts;
#endif

struct recordLabelWithSeries{
    recordLabel label;
    QLineSeries* seriesLine;
    QBarSeries* seriesBar;
    int voltageMax;
    int voltageMin;
    int cubeMin;
    int cubeMax;
    int cubeDiffMin;
    int cubeDiffMax;
    float voltageDiffWithCubeMin;
    float voltageDiffWithCubeMax;
    float balanceMax;
    float balanceMin;
    float waterWeightMin;
    float waterWeightMax;
    recordLabelWithSeries(){
        label.Clear();
        seriesLine=NULL;
        seriesBar=NULL;
        cubeMin=0;
        cubeMax=0;
        cubeDiffMin=0;
        cubeDiffMax=0;
        voltageMax=0;
        voltageMin=0;
        balanceMax=.0;
        balanceMin=.0;
        waterWeightMin=.0;
        waterWeightMax=.0;
        voltageDiffWithCubeMax=.0;
        voltageDiffWithCubeMin=.0;
    }
};

class HGDisplayChartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGDisplayChartWidget(std::string,const std::vector<std::string> &names,QWidget *parent = nullptr);
    bool closeWindow();
    ~HGDisplayChartWidget();
    void showDataInfo(const std::map<std::string,std::string> &info);
    void showTestInfo(const std::string &dbName);
    void clearChart();
    
signals:

private slots:
    void slotInTimeDisplayType();
    void slotDisplayType();
    void slotLabelClicked(std::string);
    void fnCreateLineChart();
    void fnCreateBarChart();
    void slotUpdateChart();
    void slotExportData();
    void onChartClicked(const QPoint& pos);

    protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    void fnUpdateDisplay(bool,std::string);
    bool hasAxisHor(std::string name);
    bool hasAxisVer(std::string name);
    void removeXAxisByName(QChart* chart,const std::string &name);
    void removeYAxisByName(QChart* chart,const std::string &name);
    bool isDeleteFlag(int type,std::string name);
    void fnWriteDB();
    void fnReadDB();
    void drawTimeAndVoltage(const std::string &name,const std::map<std::string,std::string> &dataInfos);
    void drawTimeAndCubeDiff(const std::string &name,const std::map<std::string,std::string> &dataInfos);
    void drawVoltageAndVoltageDiffWithCube(const std::string &name,const std::map<std::string,std::string> &dataInfos);
    void drawTimeAndSignal(const std::string &name,const std::vector<std::map<std::string,std::string>> &dataInfos);
    void drawTimeAndOneDiff(const std::string &name,const std::vector<std::map<std::string,std::string>> &dataInfos);
    void drawCubeAndVoltage(const std::string &name,const std::map<std::string,std::string> &dataInfos);
    void drawTimeAndCube(const std::string &name,const std::map<std::string,std::string> &dataInfos);
    void drawTimeAndTwoDiff(const std::string &name,const std::vector<std::map<std::string,std::string>> &dataInfos);
    void drawWaterWeightAndVoltage(const std::string &name,const std::map<std::string,std::string> &dataInfos);
    void drawTimeAndWaterWeigth(const std::string &name,const std::map<std::string,std::string> &dataInfos);
    void drawTimeAndBalanceSpeed(const std::string &name,const std::map<std::string,std::string> &dataInfos);
    void fnUpdateChartData(const std::map<std::string,std::string> &dataInfos);
    int isLabelExist(QCategoryAxis *axis, const QString &label);
    QString fnGetTimeLabel(const int &index);

private:
    HGQLabel* m_displayTypeLabel, *m_closeLabel, *inTimeDisplayLabel;
    HGQLabel* m_exportLabel;
    QGridLayout* m_layout;
    QChart* m_chart;
    MyChartView* m_chartView;
    // QCategoryAxis* m_xAxisTime;
    // QDateTimeAxis* m_xAxisTime;
    QValueAxis *m_xAxisTime;
    QValueAxis *m_yAxisSignal, *m_yAxisDiff, *m_yAxisDiff2,*m_xAxisCube,*m_yAxisCube;
    QValueAxis *m_yAxisDiffCubeWithTime;
    QValueAxis *m_xAxisDiffVoltageWithCube;
    QValueAxis *m_yVoltage,*m_yWaterWeigh,*m_yBalanceSpeed;
    QValueAxis *m_xWaterWeigh;
    std::map<std::string,recordLabelWithSeries> m_displayLabels;
    std::map<std::string,recordLabel> m_recordLabels;
    DraggableDialog *m_dragDialog, *m_displayDialog;

    std::vector<float> m_voltages;
    std::vector<float> m_cubeValues;

    std::vector<std::string> m_displayNames;
    QTimer *m_timer;
    bool m_startDrawFlag;
    std::string m_lang;
};

#endif // HGDISPLAYCHARTWIDGET_H
