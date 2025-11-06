#include "hgdisplaychartwidget.h"
#include <QtCharts/QCategoryAxis>
#include <sstream>
#include <QDateTime>
#include "common.h"

#define MAX_POINT_COUNT 10

std::vector<std::string> displayNames={
    "时间~信号值",
    "时间~信号微分值",
    "体积~信号值",
    "时间~体积",
    "时间~信号二次微分",
    "时间~电压",
    "水质量~电压",
    "时间~水质量",
    "时间~电解速率"
};

std::vector<std::string> recordInTimeNames={"测定实时","趋势实时"};


HGDisplayChartWidget::HGDisplayChartWidget(std::string lang,const std::vector<std::string> &names,QWidget *parent) : QWidget(parent), 
    m_lang(lang),
    m_startDrawFlag(false),
    m_dragDialog(nullptr),
    m_displayDialog(nullptr),
    m_layout(new QGridLayout()),
    m_displayTypeLabel(new HGQLabel(false,getPath("/resources/V1/@1xze-bars 1.png"))),
    m_exportLabel(new HGQLabel(false,getPath("/resources/V1/@down.png"))),
    inTimeDisplayLabel(new HGQLabel(false,getPath("/resources/V1/@1xze-apps-o 1.png"))),
    m_closeLabel(new HGQLabel(false,getPath("/resources/V1/@1xze-cross 1.png"))),
    m_chartView(new MyChartView()),
    m_chart(new QChart()),
    // m_xAxisTime(new QCategoryAxis()),
    m_xAxisTime(new QValueAxis()),
    m_yAxisSignal(new QValueAxis()),
    m_yAxisDiff(new QValueAxis()),
    m_yAxisDiff2(new QValueAxis()),
    m_xAxisCube(new QValueAxis()),
    m_yAxisCube(new QValueAxis()),
    m_yVoltage(new QValueAxis()),
    m_yAxisDiffCubeWithTime(new QValueAxis()),
    m_xAxisDiffVoltageWithCube(new QValueAxis()),
    m_yWaterWeigh(new QValueAxis()),
    m_yBalanceSpeed(new QValueAxis()),
    m_xWaterWeigh(new QValueAxis()),
    m_timer(new QTimer(this))
{
    m_displayNames=names;
    m_voltages.clear();
    m_cubeValues.clear();
    m_yBalanceSpeed->setTickInterval(1);
    m_yBalanceSpeed->setGridLineVisible(false);
    m_yBalanceSpeed->setMinorGridLineVisible(false);
    m_yVoltage->setTickInterval(1);
    m_yVoltage->setGridLineVisible(true);
    m_yVoltage->setMinorGridLineVisible(false);
    m_xWaterWeigh->setTickInterval(1);
    m_xWaterWeigh->setGridLineVisible(false);
    m_xWaterWeigh->setMinorGridLineVisible(false);
    m_yWaterWeigh->setTickInterval(1);
    m_yWaterWeigh->setGridLineVisible(false);
    m_yWaterWeigh->setMinorGridLineVisible(false);
    // m_xAxisTime->setFormat("hh:mm");
    // m_xAxisTime->setTickCount(6);
    m_xAxisTime->setGridLineVisible(true);
    m_xAxisTime->setLabelFormat("%d");

    std::map<std::string,QColor> displayColors;
    // 预定义一组常用的图表颜色，确保颜色之间有足够的区分度
    QList<QColor> predefinedColors = {QColor(255, 0, 0),      // 红色
                                      QColor(0, 128, 0),      // 深绿色
                                      QColor(0, 0, 255),      // 蓝色
                                      QColor(255, 165, 0),    // 橙色
                                      QColor(128, 0, 128),    // 紫色
                                      QColor(0, 255, 255),    // 青色
                                      QColor(255, 255, 0),    // 黄色
                                      QColor(255, 192, 203),  // 粉色
                                      QColor(165, 42, 42),    // 棕色
                                      QColor(0, 255, 0)};     // 浅绿色
    // 为每个显示名称分配固定颜色（通过索引取模确保颜色循环使用）
    int colorIndex = 0;
    for (const auto& name : displayNames){
        displayColors[name] = predefinedColors[colorIndex % predefinedColors.size()];
        colorIndex++;
    }
    try {
        for (const auto& name : displayNames) {
            if (std::find(m_displayNames.begin(),m_displayNames.end(),name)==m_displayNames.end()) continue;
            m_displayLabels[name] = recordLabelWithSeries();
            m_displayLabels[name].seriesLine = new QLineSeries();
            m_displayLabels[name].seriesLine->setName(QString::fromStdString(name));
            m_displayLabels[name].seriesLine->setPointsVisible(false);
            m_displayLabels[name].seriesLine->setPen(QPen(displayColors[name]));
            m_displayLabels[name].seriesBar = new QBarSeries();
            m_displayLabels[name].seriesBar->setName(QString::fromStdString(name));
        }

        for (const auto& name : recordInTimeNames) {
            m_recordLabels[name] = recordLabel();
        }

        fnReadDB();

        m_layout->setParent(this);
        this->setLayout(m_layout);

        connect(m_displayTypeLabel, &HGQLabel::leftClicked, this, &HGDisplayChartWidget::slotDisplayType);
        connect(inTimeDisplayLabel, &HGQLabel::leftClicked, this, &HGDisplayChartWidget::slotInTimeDisplayType);
        connect(m_exportLabel, &HGQLabel::leftClicked, this, &HGDisplayChartWidget::slotExportData);

        // m_chart->setRenderHint(QPainter::Antialiasing);
        m_chart->legend()->setBorderColor(QColor(0, 0, 85, 70));
        m_chart->legend()->setAlignment(Qt::AlignBottom);
        m_chart->legend()->setShowToolTips(true);
        m_xAxisTime->setObjectName("时间");
        m_xAxisTime->setTitleText("时间t(s)");
        m_xAxisTime->setRange(0,180);

        m_yVoltage->setTitleText("电压E(mV)");
        m_yVoltage->setObjectName("电压");
        m_yWaterWeigh->setTitleText("水质量W(ug)");
        m_yWaterWeigh->setObjectName("水质量");
        m_yBalanceSpeed->setTitleText("电解速率R(ug/m)");
        m_yBalanceSpeed->setObjectName("电解速率");
        m_xWaterWeigh->setTitleText("水质量W(ug)");
        m_xWaterWeigh->setObjectName("水质量");
        m_yAxisSignal->setTitleText("信号值");
        m_yAxisDiff->setTitleText("信号微分值");
        m_yAxisDiff2->setTitleText("信号二次微分");
        m_xAxisCube->setTitleText("体积(ml)");
        m_xAxisCube->setObjectName("体积");
        m_yAxisCube->setTitleText("体积(ml)");
        m_yAxisCube->setObjectName("体积");
        m_yAxisDiffCubeWithTime->setTitleText("dV/dt");
        m_xAxisDiffVoltageWithCube->setTitleText("dE/dv");
        m_chart->setBackgroundBrush(Qt::NoBrush);
        m_chartView->setChart(m_chart);
        m_chartView->setRubberBand(QChartView::RectangleRubberBand);

        m_chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        connect(m_chartView, &MyChartView::createLineChart, this, &HGDisplayChartWidget::fnCreateLineChart);
        connect(m_chartView, &MyChartView::createBarChart, this, &HGDisplayChartWidget::fnCreateBarChart);
        connect(m_chartView, &MyChartView::chartClicked, this, &HGDisplayChartWidget::onChartClicked);

        m_layout->addWidget(inTimeDisplayLabel, 0, 1);
        m_layout->addWidget(m_displayTypeLabel, 0, 2);
        m_layout->addWidget(m_exportLabel, 0, 3);
        // m_layout->addWidget(m_closeLabel, 0, 2);
        m_layout->addWidget(m_chartView, 1, 0, 2, 3);

        connect(m_timer, &QTimer::timeout, this, &HGDisplayChartWidget::slotUpdateChart);
        // m_timer->start(1000);

        for (const auto& name : displayNames) {
            if (std::find(m_displayNames.begin(),m_displayNames.end(),name)==m_displayNames.end()) continue;
            fnUpdateDisplay(m_displayLabels[name].label.flag, name);
        }
        m_xAxisCube->setRange(0,2);
        m_yAxisCube->setRange(0,2);
        slotUpdateChart();
    } catch (const std::exception& e) {
        std::ostringstream ss;
        ss<< "Error initializing HGDisplayChartWidget: " << e.what();
        HGLog4Cplus::getLogInstance(LOG_PATH)->logout(ss.str(),LOGERROR);
    }
}

void HGDisplayChartWidget::fnWriteDB(){
    for (auto name:displayNames){
        if (std::find(m_displayNames.begin(),m_displayNames.end(),name)==m_displayNames.end()) continue;
        if (m_displayLabels[name].label.flag) GlobalSingleton::instance().setDataChartInfo(name, "true");
        else GlobalSingleton::instance().setDataChartInfo(name,"false");
    }
    for (auto name:recordInTimeNames){
        if (m_recordLabels[name].flag) GlobalSingleton::instance().setDataChartInfo(name,"true");
        else GlobalSingleton::instance().setDataChartInfo(name,"false");
    }
    GlobalSingleton::instance().saveDataChartInfo();
}
void HGDisplayChartWidget::fnReadDB(){
    for (auto name:displayNames){
        if (std::find(m_displayNames.begin(),m_displayNames.end(),name)==m_displayNames.end()) continue;
        if (GlobalSingleton::instance().getDataChartInfo(name)=="true") m_displayLabels[name].label.flag=true;
        else m_displayLabels[name].label.flag=false;
    }
    for (auto name:recordInTimeNames){
        if (GlobalSingleton::instance().getDataChartInfo(name)=="true") m_recordLabels[name].flag=true;
        else m_recordLabels[name].flag=false;
    }
}
bool HGDisplayChartWidget::closeWindow()
{
    fnWriteDB();
    return true;
}
HGDisplayChartWidget::~HGDisplayChartWidget()
{
    
}
void HGDisplayChartWidget::clearChart(){ 
    for (auto serie : m_displayLabels){
        serie.second.seriesLine->clear();
        serie.second.seriesBar->clear();
    }
    m_voltages.clear();
    m_cubeValues.clear();
}
void HGDisplayChartWidget::slotExportData(){
    std::vector<std::map<std::string,std::string>> logList;
    std::string outlogPath=FileConfig::getDirPath()+"/outlog/";
    HGMkDir(outlogPath);
    HGExactTime curTime=HGExactTime::currentTime();
    std::string syncslice = curTime.toStringFromYearToSec();
    std::string logname = outlogPath+syncslice;
    logname+=".csv";
    // logList=RWDb::readSampleDetectInfo();
    saveTableToCsv(logList,logname);
}
void HGDisplayChartWidget::slotDisplayType(){
    if (m_dragDialog!=NULL) {
        fnWriteDB();
        SAFE_DELETE(m_dragDialog);
        return;
    }
    m_dragDialog=new DraggableDialog(this);

    QVBoxLayout* layout=new QVBoxLayout(m_dragDialog);

    LabelWithImg* nameLabel=new LabelWithImg(IMGTOP,12,getPath("/resources/V1/@1xze-bars 1.png"),
        loadTranslation(m_lang,"DisplayMethod"));//"显示方式");  

    for (auto name:displayNames){
        if (std::find(m_displayNames.begin(),m_displayNames.end(),name)==m_displayNames.end()) continue;
        if (m_displayLabels[name].label.flag)
            m_displayLabels[name].label.label=new LabelWithImg(IMGLEFT,12,getPath("/resources/V1/@1xze-certificate 1.png"),name);
        else m_displayLabels[name].label.label=new LabelWithImg(IMGLEFT,12,getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"),name);
    }

    layout->addWidget(nameLabel);
    for (auto it:m_displayLabels){
        connect(m_displayLabels[it.first].label.label,SIGNAL(clickLeftName(std::string)),this,SLOT(slotLabelClicked(std::string)));
        layout->addWidget(it.second.label.label);
    }

    m_dragDialog->setLayout(layout);
    m_dragDialog->show();
}
void HGDisplayChartWidget::slotInTimeDisplayType(){
    if (m_displayDialog!=NULL) {
        SAFE_DELETE(m_displayDialog);
        return;
    }
    m_displayDialog=new DraggableDialog(this);

    QVBoxLayout* layout=new QVBoxLayout(m_displayDialog);

    LabelWithImg* nameLabel=new LabelWithImg(IMGTOP,12,getPath("/resources/V1/@1xze-apps-o 1.png"),
        loadTranslation(m_lang,"RealTimeDisplay"));//"实时显示");  
 
    for (auto name:recordInTimeNames){
        if (m_recordLabels[name].flag)
            m_recordLabels[name].label=new LabelWithImg(IMGLEFT,12,getPath("/resources/V1/@1xze-certificate 1.png"),name);
        else m_recordLabels[name].label=new LabelWithImg(IMGLEFT,12,getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"),name);
    }

    layout->addWidget(nameLabel);
    for (auto it:m_recordLabels){
        connect(m_recordLabels[it.first].label,SIGNAL(clickLeftName(std::string)),this,SLOT(slotLabelClicked(std::string)));
        layout->addWidget(it.second.label);
    }

    m_displayDialog->setLayout(layout);
    m_displayDialog->show();
}
void HGDisplayChartWidget::slotLabelClicked(std::string name){
    if (m_recordLabels.find(name) != m_recordLabels.end())
    {
        m_recordLabels[name].flag = !m_recordLabels[name].flag;
        if (!m_recordLabels[name].flag)
            m_recordLabels[name].label->setImg(getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"));
        else
            m_recordLabels[name].label->setImg(getPath("/resources/V1/@1xze-certificate 1.png"));
    }
    else if (m_displayLabels.find(name) != m_displayLabels.end())
    {
        m_displayLabels[name].label.flag = !m_displayLabels[name].label.flag;
        if (!m_displayLabels[name].label.flag){
            m_displayLabels[name].label.label->setImg(getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"));
            fnUpdateDisplay(false,name);
        }
        else{
            m_displayLabels[name].label.label->setImg(getPath("/resources/V1/@1xze-certificate 1.png"));
            fnUpdateDisplay(true,name);
            slotUpdateChart();
        }
    }
}
bool HGDisplayChartWidget::hasAxisHor(std::string name){
    QList<QAbstractAxis *> xAxes=m_chart->axes(Qt::Horizontal);
    for (QAbstractAxis *axis : xAxes){
        std::string text=axis->objectName().toStdString() ;
        if (name==text) return true;
    }
    return false;
}
bool HGDisplayChartWidget::hasAxisVer(std::string name)
{
    QList<QAbstractAxis *> yAxes=m_chart->axes(Qt::Vertical);
    for (QAbstractAxis *axis : yAxes){
        std::string text=axis->objectName().toStdString() ;
        if (name==text) return true;
    }
    return false;
}
enum {
    XAXIS,
    YAXIS
};
bool HGDisplayChartWidget::isDeleteFlag(int type,std::string name){
    for (auto record: m_displayLabels) {
        size_t posSep=record.first.find("~");
        size_t pos=record.first.find(name);
        bool flag=false;
        switch (type){
            case XAXIS:{
                if (pos!=std::string::npos && pos < posSep) flag=true;
            break;
            }
            case YAXIS:{
                if (pos!=std::string::npos && pos > posSep) flag=true;
            break;
            }
        }
        if (flag){
            if (record.second.label.flag) {
                return false;
            } 
        }
    }
    return true;
}
void HGDisplayChartWidget::removeXAxisByName(QChart* chart,const std::string &name){
    QList<QAbstractAxis*> xAxes=chart->axes(Qt::Horizontal);
    for (QAbstractAxis *axis : xAxes){
        std::string text=axis->objectName().toStdString();
        if (name==text){
            chart->removeAxis(axis);
            return;
        }
    }
}
void HGDisplayChartWidget::removeYAxisByName(QChart* chart,const std::string &name){
    QList<QAbstractAxis*> yAxes=chart->axes(Qt::Vertical);
    for (QAbstractAxis *axis : yAxes){
        std::string text=axis->objectName().toStdString();
        if (name==text){
            chart->removeAxis(axis);
            return;
        }
    }
}
void HGDisplayChartWidget::fnUpdateDisplay(bool flag,std::string name){
    if (flag){
        switch (m_chartView->getChartType()){
            case CHART_LINE:
                fnCreateLineChart();
                m_chart->addSeries(m_displayLabels[name].seriesLine);
                break;
            case CHART_BAR:
                fnCreateBarChart();
                m_chart->addSeries(m_displayLabels[name].seriesBar);
                break;
        }
        
        // display
        if (name=="时间~信号值"){
            bool flag=hasAxisHor("时间");
            if (!flag) m_chart->addAxis(m_xAxisTime,Qt::AlignBottom);
            m_displayLabels[name].seriesLine->attachAxis(m_xAxisTime);
            flag=hasAxisVer("信号值");
            if (!flag) m_chart->addAxis(m_yAxisSignal,Qt::AlignLeft);
            m_displayLabels[name].seriesLine->attachAxis(m_yAxisSignal);
        } else if (name=="时间~信号微分值"){
            bool flag=hasAxisHor("时间");
            if (!flag) m_chart->addAxis(m_xAxisTime,Qt::AlignBottom);
            m_displayLabels[name].seriesLine->attachAxis(m_xAxisTime);
            flag=hasAxisVer("信号微分值");
            if (!flag) m_chart->addAxis(m_yAxisDiff,Qt::AlignRight);
            m_displayLabels[name].seriesLine->attachAxis(m_yAxisDiff);
        } else if (name=="时间~dV/dt"){
            bool flag=hasAxisHor("时间");
            if (!flag) m_chart->addAxis(m_xAxisTime,Qt::AlignBottom);
            m_displayLabels[name].seriesLine->attachAxis(m_xAxisTime);
            flag=hasAxisVer("dV/dt");
            if (!flag) m_chart->addAxis(m_yAxisDiffCubeWithTime,Qt::AlignLeft);
            m_displayLabels[name].seriesLine->attachAxis(m_yAxisDiffCubeWithTime);
        } else if (name=="dE/dv~电压"){
            bool flag=hasAxisHor("dE/dv");
            if (!flag) m_chart->addAxis(m_xAxisDiffVoltageWithCube,Qt::AlignBottom);
            m_displayLabels[name].seriesLine->attachAxis(m_xAxisDiffVoltageWithCube);
            flag=hasAxisVer("电压");
            if (!flag) m_chart->addAxis(m_yVoltage,Qt::AlignLeft);
            m_displayLabels[name].seriesLine->attachAxis(m_yVoltage);
        } else if (name=="时间~信号二次微分"){
            bool flag=hasAxisHor("时间");
            if (!flag) m_chart->addAxis(m_xAxisTime,Qt::AlignBottom);
            m_displayLabels[name].seriesLine->attachAxis(m_xAxisTime);   
            flag=hasAxisVer("信号二次微分");
            if (!flag) m_chart->addAxis(m_yAxisDiff2,Qt::AlignLeft);
            m_displayLabels[name].seriesLine->attachAxis(m_yAxisDiff2);
        } else if (name=="体积~电压"){
            bool flag=hasAxisHor("体积");
            if (!flag) m_chart->addAxis(m_xAxisCube,Qt::AlignTop);
            m_displayLabels[name].seriesLine->attachAxis(m_xAxisCube);
            flag=hasAxisVer("电压");
            if (!flag) m_chart->addAxis(m_yVoltage,Qt::AlignLeft);
            m_displayLabels[name].seriesLine->attachAxis(m_yVoltage);
        } else if (name=="时间~体积"){
            bool flag=hasAxisHor("时间");
            if (!flag) m_chart->addAxis(m_xAxisTime,Qt::AlignBottom);
            m_displayLabels[name].seriesLine->attachAxis(m_xAxisTime);
            flag=hasAxisVer("体积");
            if (!flag) m_chart->addAxis(m_yAxisCube,Qt::AlignRight);
            m_displayLabels[name].seriesLine->attachAxis(m_yAxisCube);
        }  else if (name=="时间~电压"){
            bool flag=hasAxisHor("时间");
            if (!flag) m_chart->addAxis(m_xAxisTime,Qt::AlignBottom);
            m_displayLabels[name].seriesLine->attachAxis(m_xAxisTime);
            flag=hasAxisVer("电压");
            if (!flag) m_chart->addAxis(m_yVoltage,Qt::AlignLeft);
            m_displayLabels[name].seriesLine->attachAxis(m_yVoltage);
        } else if (name=="水质量~电压"){
            bool flag=hasAxisHor("水质量");
            if (!flag) m_chart->addAxis(m_xWaterWeigh,Qt::AlignBottom);
            m_displayLabels[name].seriesLine->attachAxis(m_xWaterWeigh);
            flag=hasAxisVer("电压");
            if (!flag) m_chart->addAxis(m_yVoltage,Qt::AlignLeft);
            m_displayLabels[name].seriesLine->attachAxis(m_yVoltage);
        } else if (name=="时间~水质量"){
            bool flag=hasAxisHor("时间");
            if (!flag) m_chart->addAxis(m_xAxisTime,Qt::AlignBottom);
            m_displayLabels[name].seriesLine->attachAxis(m_xAxisTime);
            flag=hasAxisVer("水质量");
            if (!flag) m_chart->addAxis(m_yWaterWeigh,Qt::AlignRight);
            m_displayLabels[name].seriesLine->attachAxis(m_yWaterWeigh);
        } else if (name=="时间~电解速率"){
            bool flag=hasAxisHor("时间");
            if (!flag) m_chart->addAxis(m_xAxisTime,Qt::AlignBottom);
            m_displayLabels[name].seriesLine->attachAxis(m_xAxisTime);
            flag=hasAxisVer("电解速率");
            if (!flag) m_chart->addAxis(m_yBalanceSpeed,Qt::AlignRight);
            m_displayLabels[name].seriesLine->attachAxis(m_yBalanceSpeed);
        }
    } else {
        // dis visible
        if (m_chart->series().size() <=0){
            m_chart->setTitle("");
        }
        if (!m_chart->series().contains(m_displayLabels[name].seriesLine)) return;
        if (name=="时间~信号值"){
            m_displayLabels[name].seriesLine->detachAxis(m_xAxisTime);
            m_displayLabels[name].seriesLine->detachAxis(m_yAxisSignal);
            if (isDeleteFlag(XAXIS,"时间")) removeXAxisByName(m_chart,"时间");
            if (isDeleteFlag(YAXIS,"信号值")) removeYAxisByName(m_chart,"信号值");
        } else if (name=="时间~信号微分值"){
            m_displayLabels[name].seriesLine->detachAxis(m_xAxisTime);
            m_displayLabels[name].seriesLine->detachAxis(m_yAxisDiff);
            if (isDeleteFlag(XAXIS,"时间")) removeXAxisByName(m_chart,"时间");
            if (isDeleteFlag(YAXIS,"信号微分值")) removeYAxisByName(m_chart,"信号微分值");
        } else if (name=="时间~dV/dt"){
            m_displayLabels[name].seriesLine->detachAxis(m_xAxisTime);
            m_displayLabels[name].seriesLine->detachAxis(m_yAxisDiffCubeWithTime);
            if (isDeleteFlag(XAXIS,"时间")) removeXAxisByName(m_chart,"时间");
            if (isDeleteFlag(YAXIS,"dV/dt")) removeYAxisByName(m_chart,"dV/dt");
        } else if (name=="dE/dv~电压"){
            m_displayLabels[name].seriesLine->detachAxis(m_xAxisDiffVoltageWithCube);
            m_displayLabels[name].seriesLine->detachAxis(m_yVoltage);
            if (isDeleteFlag(XAXIS,"dE/dv")) removeXAxisByName(m_chart,"dE/dv");
            if (isDeleteFlag(YAXIS,"电压")) removeYAxisByName(m_chart,"电压");
        } else if (name=="时间~信号二次微分"){
            m_displayLabels[name].seriesLine->detachAxis(m_xAxisTime);
            m_displayLabels[name].seriesLine->detachAxis(m_yAxisDiff2);
            if (isDeleteFlag(XAXIS,"时间")) removeXAxisByName(m_chart,"时间");
            if (isDeleteFlag(YAXIS,"信号二次微分")) removeYAxisByName(m_chart,"信号二次微分");
        } else if (name=="体积~电压"){
            m_displayLabels[name].seriesLine->detachAxis(m_xAxisCube);
            m_displayLabels[name].seriesLine->detachAxis(m_yVoltage);
            if (isDeleteFlag(XAXIS,"体积")) removeXAxisByName(m_chart,"体积");
            if (isDeleteFlag(YAXIS,"电压")) removeYAxisByName(m_chart,"电压");
        } else if (name=="时间~体积"){
            m_displayLabels[name].seriesLine->detachAxis(m_xAxisTime);
            m_displayLabels[name].seriesLine->detachAxis(m_yAxisCube);
            if (isDeleteFlag(XAXIS,"时间")) removeXAxisByName(m_chart,"时间");
            if (isDeleteFlag(YAXIS,"体积")) removeYAxisByName(m_chart,"体积");
        }  else if (name=="时间~电压"){
            m_displayLabels[name].seriesLine->detachAxis(m_xAxisTime);
            m_displayLabels[name].seriesLine->detachAxis(m_yVoltage);
            if (isDeleteFlag(XAXIS,"时间")) removeXAxisByName(m_chart,"时间");
            if (isDeleteFlag(YAXIS,"电压")) removeYAxisByName(m_chart,"电压");
        } else if (name=="水质量~电压"){
            m_displayLabels[name].seriesLine->detachAxis(m_xWaterWeigh);
            m_displayLabels[name].seriesLine->detachAxis(m_yVoltage);
            if (isDeleteFlag(XAXIS,"水质量")) removeXAxisByName(m_chart,"水质量");
            if (isDeleteFlag(YAXIS,"电压")) removeYAxisByName(m_chart,"电压");
        } else if (name=="时间~水质量"){
            m_displayLabels[name].seriesLine->detachAxis(m_xAxisTime);
            m_displayLabels[name].seriesLine->detachAxis(m_yWaterWeigh);
            if (isDeleteFlag(XAXIS,"时间")) removeXAxisByName(m_chart,"时间");
            if (isDeleteFlag(YAXIS,"水质量")) removeYAxisByName(m_chart,"水质量");
        } else if (name=="时间~电解速率"){
            m_displayLabels[name].seriesLine->detachAxis(m_xAxisTime);
            m_displayLabels[name].seriesLine->detachAxis(m_yBalanceSpeed);
            if (isDeleteFlag(XAXIS,"时间")) removeXAxisByName(m_chart,"时间");
            if (isDeleteFlag(YAXIS,"电解速率")) removeYAxisByName(m_chart,"电解速率");
        }
        
        m_chart->removeSeries(m_displayLabels[name].seriesLine);
    }
}
void HGDisplayChartWidget::fnCreateBarChart(){
    m_chart->setTitle(QString::fromStdString(loadTranslation(m_lang,"Bar")));//"柱状图");
}
void HGDisplayChartWidget::fnCreateLineChart()
{
    m_chart->setTitle(QString::fromStdString(loadTranslation(m_lang,"Line")));//"折线图");
}
void HGDisplayChartWidget::drawTimeAndSignal(const std::string &name,const std::vector<std::map<std::string,std::string>> &dataInfos){
    int minY = 0.0, maxY = 0.0;
    for (int i = 0; i < int(dataInfos.size()); i++)
    {
        int voltage = 0;
        if (dataInfos[i].find("电压")!=dataInfos[i].end()) voltage=std::atoi(dataInfos[i].at("电压").c_str());
        if (voltage < minY)
            minY = voltage;
        if (voltage > maxY)
            maxY = voltage;
    }

    m_yVoltage->setRange(minY, maxY);

    int dataInfosSize = int(dataInfos.size());
    int startI = (dataInfosSize - MAX_POINT_COUNT) < 0 ? 0 : (dataInfosSize - MAX_POINT_COUNT);
    for (int i = startI; i < int(dataInfos.size()); i++)
    {
        // printf("i:%d\n",i-startI);
        QString strTime = QString::fromStdString(dataInfos[i].at("时间"));
        // printf("time:%s\n",dataInfos[i]["时间"].c_str());

        m_displayLabels[name].seriesLine->append(i - startI, std::atof(dataInfos[i].at("电压").c_str()));
        // m_xAxisTime->append(strTime, i - startI);
        m_chart->update();
    }
}
void HGDisplayChartWidget::drawTimeAndOneDiff(const std::string &name,const std::vector<std::map<std::string,std::string>> &dataInfos){    
    int minY = 0.0, maxY = 0.0;
    for (int i = 0; i < int(dataInfos.size()); i++)
    {
        int voltage = 0;
        if (dataInfos[i].find("电压")!=dataInfos[i].end()) voltage=std::atoi(dataInfos[i].at("电压").c_str());
        if (voltage < minY)
            minY = voltage;
        if (voltage > maxY)
            maxY = voltage;
    }

    m_yVoltage->setRange(minY, maxY);

    int dataInfosSize = int(dataInfos.size());
    int startI = (dataInfosSize - MAX_POINT_COUNT) < 0 ? 0 : (dataInfosSize - MAX_POINT_COUNT);
    for (int i = startI; i < int(dataInfos.size()); i++)
    {
        // printf("i:%d\n",i-startI);
        QString strTime = QString::fromStdString(dataInfos[i].at("时间"));
        // printf("time:%s\n",dataInfos[i]["时间"].c_str());

        m_displayLabels[name].seriesLine->append(i - startI, std::atof(dataInfos[i].at("电压").c_str()));
        // m_xAxisTime->append(strTime, i - startI);
        m_chart->update();
    }
}
void HGDisplayChartWidget::drawCubeAndVoltage(const std::string &name,const std::map<std::string,std::string> &dataInfos){
    if (m_cubeValues.size() <=0 || m_voltages.size() <= 0) return;
    float minY = 0.0, maxY = 0.0;
    float sumCubeValue=0.0;
    for (int i = 0; i < int(m_cubeValues.size()); i++)
    {
        if (m_cubeValues[i] < minY)
            minY = m_cubeValues[i];
        if (m_cubeValues[i] > maxY)
            maxY = m_cubeValues[i];
    }
    sumCubeValue=m_cubeValues[m_cubeValues.size()-1];
    maxY = sumCubeValue;
    if (maxY < 2) maxY = 2;
    m_xAxisCube->setRange(minY,maxY);
    minY = 0,maxY=0;
    for (int i =0;i<int(m_voltages.size());i++){
        if (m_voltages[i] < minY)
            minY = m_voltages[i];
        if (m_voltages[i] > maxY)
            maxY = m_voltages[i];
    }
    m_yVoltage->setRange(minY, maxY);

    switch (m_chartView->getChartType()){
        case CHART_LINE:
        m_displayLabels[name].seriesLine->append(sumCubeValue,m_voltages[m_voltages.size()-1]);
        break;
        case CHART_BAR:
        break;
    }
    m_chart->update();
}
void HGDisplayChartWidget::drawVoltageAndVoltageDiffWithCube(const std::string &name,const std::map<std::string,std::string> &dataInfos){
    if (m_voltages.size() <=2 || m_cubeValues.size() <= 2) return;
    float minY = 0.0, maxY = 0.0;
    for (int i =0;i<int(m_voltages.size());i++){
        if (m_voltages[i] < minY)
            minY = m_voltages[i];
        if (m_voltages[i] > maxY)
            maxY = m_voltages[i];
    }
    m_yVoltage->setRange(minY, maxY);

    float voltagediffWithCube = 0.0;
    if (m_cubeValues[m_cubeValues.size()-1]==m_cubeValues[m_cubeValues.size()-2]) voltagediffWithCube = 0.0;
    else voltagediffWithCube = float(m_voltages[m_voltages.size()-1]-m_voltages[m_voltages.size()-2]) /
        float(m_cubeValues[m_cubeValues.size()-1]-m_cubeValues[m_cubeValues.size()-2]);
    if (voltagediffWithCube < m_displayLabels[name].voltageDiffWithCubeMin)
        m_displayLabels[name].voltageDiffWithCubeMin = voltagediffWithCube;
    if (voltagediffWithCube > m_displayLabels[name].voltageDiffWithCubeMax)
        m_displayLabels[name].voltageDiffWithCubeMax = voltagediffWithCube;

    if (m_displayLabels[name].voltageDiffWithCubeMax <= m_displayLabels[name].voltageDiffWithCubeMin)
        m_xAxisDiffVoltageWithCube->setRange(0,1);
    else 
        m_xAxisDiffVoltageWithCube->setRange(m_displayLabels[name].voltageDiffWithCubeMin,m_displayLabels[name].voltageDiffWithCubeMax);

    switch (m_chartView->getChartType()){
        case CHART_LINE:
        m_displayLabels[name].seriesLine->append(voltagediffWithCube,m_voltages[m_voltages.size()-1]);
        break;
        case CHART_BAR:
        break;
    }
    m_chart->update();
}
void HGDisplayChartWidget::drawTimeAndCubeDiff(const std::string &name,const std::map<std::string,std::string> &dataInfos){
    if (m_cubeValues.size() <=2) return;
    int cubediff = m_cubeValues[m_cubeValues.size()-1] - m_cubeValues[m_cubeValues.size()-2];
    if (cubediff < m_displayLabels[name].cubeDiffMin)
        m_displayLabels[name].cubeDiffMin = cubediff;
    if (cubediff > m_displayLabels[name].cubeDiffMax)
        m_displayLabels[name].cubeDiffMax = cubediff;

    if (m_displayLabels[name].cubeDiffMax <= m_displayLabels[name].cubeDiffMin)
        m_yAxisDiffCubeWithTime->setRange(0,1);
    else 
        m_yAxisDiffCubeWithTime->setRange(m_displayLabels[name].cubeDiffMin,m_displayLabels[name].cubeDiffMax);

    QDateTime now = QDateTime::currentDateTime();
    switch (m_chartView->getChartType()){
        case CHART_LINE:
        m_displayLabels[name].seriesLine->append(now.toMSecsSinceEpoch(), cubediff);
        break;
        case CHART_BAR:
        break;
    }

    QDateTime fiveMinutesBefore = QDateTime(now.addSecs(-120));
    m_xAxisTime->setRange(0, 180);
    m_chart->update();
}
void HGDisplayChartWidget::drawTimeAndCube(const std::string &name,const std::map<std::string,std::string> &dataInfos){
    int duration = 0;
    if (dataInfos.find("检测时长") != dataInfos.end()) duration = std::atoi(dataInfos.at("检测时长").c_str());
    float sumCubeValue=0.0;

    m_displayLabels[name].cubeMin = 0.0;
    for (int i=0;i<int(m_cubeValues.size());i++){
        sumCubeValue+=m_cubeValues[i];
    }
    sumCubeValue=m_cubeValues[m_cubeValues.size()-1];
    m_displayLabels[name].cubeMax = sumCubeValue;
    if (m_displayLabels[name].cubeMax < 2) m_displayLabels[name].cubeMax=2;

    if (m_displayLabels[name].cubeMax <= m_displayLabels[name].cubeMin)
        m_yAxisCube->setRange(0,2);
    else
        m_yAxisCube->setRange(m_displayLabels[name].cubeMin, m_displayLabels[name].cubeMax);

    switch (m_chartView->getChartType()){
        case CHART_LINE:
        m_displayLabels[name].seriesLine->append(duration,sumCubeValue);
        break;
        case CHART_BAR:
        break;
    }

    if (duration < 180) m_xAxisTime->setRange(0,180);
    else m_xAxisTime->setRange(0,duration);
    m_chart->update();
}
void HGDisplayChartWidget::drawTimeAndTwoDiff(const std::string &name,const std::vector<std::map<std::string,std::string>> &dataInfos){
    int dataInfosSize = int(dataInfos.size());
    int startI = (dataInfosSize - MAX_POINT_COUNT) < 0 ? 0 : (dataInfosSize - MAX_POINT_COUNT);
    for (int i = startI; i < int(dataInfos.size()); i++)
    {
        QString strTime = QString::fromStdString(dataInfos[i].at("时间"));

        m_displayLabels[name].seriesLine->append(i - startI, std::atof(dataInfos[i].at("电压").c_str()));
        m_chart->update();
    }
}
void HGDisplayChartWidget::drawWaterWeightAndVoltage(const std::string &name,const std::map<std::string,std::string> &dataInfos){    
    int voltage = 0;
    if (dataInfos.find("电压")!=dataInfos.end()) voltage=std::atoi(dataInfos.at("电压").c_str());
    if (voltage < m_displayLabels[name].voltageMin)
        m_displayLabels[name].voltageMin = voltage;
    if (voltage > m_displayLabels[name].voltageMax)
        m_displayLabels[name].voltageMax = voltage;

    if (m_displayLabels[name].voltageMax <= m_displayLabels[name].voltageMin)
        m_yVoltage->setRange(-1,1);
    else 
        m_yVoltage->setRange(m_displayLabels[name].voltageMin, m_displayLabels[name].voltageMax);

   
    float waterWeigh = .0;
    if (dataInfos.find("水质量")!=dataInfos.end()) waterWeigh=std::atof(dataInfos.at("水质量").c_str());
    if (waterWeigh < m_displayLabels[name].waterWeightMin)
        m_displayLabels[name].waterWeightMin = waterWeigh;
    if (waterWeigh > m_displayLabels[name].waterWeightMax)
        m_displayLabels[name].waterWeightMax = waterWeigh;

    printf("waterWeight:(min:%.2f,max:%.2f)\n",
        m_displayLabels[name].waterWeightMin,
        m_displayLabels[name].waterWeightMax);

    if ((m_displayLabels[name].waterWeightMax-m_displayLabels[name].waterWeightMin) < 0.00001) 
        m_xWaterWeigh->setRange(-1,1);
    else 
        m_xWaterWeigh->setRange(m_displayLabels[name].waterWeightMin, m_displayLabels[name].waterWeightMax);

    if (m_displayLabels[name].seriesLine->count() >= MAX_POINT_COUNT) {
        m_displayLabels[name].seriesLine->clear();
    }
    m_displayLabels[name].seriesLine->append(waterWeigh, voltage);
    m_chart->update();
}
void HGDisplayChartWidget::drawTimeAndWaterWeigth(const std::string &name,const std::map<std::string,std::string> &dataInfos){
    int duration = 0;
    if (dataInfos.find("检测时长")!=dataInfos.end()) duration=std::atoi(dataInfos.at("检测时长").c_str());
    float waterWeigh = .0;
    if (dataInfos.find("水质量")!=dataInfos.end()) waterWeigh = std::atof(dataInfos.at("水质量").c_str());
    if (waterWeigh < m_displayLabels[name].waterWeightMin)
        m_displayLabels[name].waterWeightMin = waterWeigh;
    if (waterWeigh > m_displayLabels[name].waterWeightMax)
        m_displayLabels[name].waterWeightMax = waterWeigh;

    if ((m_displayLabels[name].waterWeightMax-m_displayLabels[name].waterWeightMin) < 0.00001) 
        m_yWaterWeigh->setRange(-1,1);
    else 
        m_yWaterWeigh->setRange(m_displayLabels[name].waterWeightMin, m_displayLabels[name].waterWeightMax);

    switch (m_chartView->getChartType())
    {
    case CHART_LINE:
        m_displayLabels[name].seriesLine->append(duration, waterWeigh);
        break;
    case CHART_BAR:
        break;
    }

    if (duration < 180) m_xAxisTime->setRange(0,180);
    else m_xAxisTime->setRange(0,duration);
    m_chart->update();
}

void HGDisplayChartWidget::drawTimeAndBalanceSpeed(const std::string &name,const std::map<std::string,std::string> &dataInfos){
    int duration = 0;
    if (dataInfos.find("检测时长")!=dataInfos.end()) duration=std::atoi(dataInfos.at("检测时长").c_str());
    float balanceSpeed = .0;
    if (dataInfos.find("电解速率")!=dataInfos.end()) balanceSpeed=std::atof(dataInfos.at("电解速率").c_str());
    if (balanceSpeed < m_displayLabels[name].balanceMin)
        m_displayLabels[name].balanceMin = balanceSpeed;
    if (balanceSpeed > m_displayLabels[name].balanceMin)
        m_displayLabels[name].balanceMax = balanceSpeed;

    if ((m_displayLabels[name].balanceMax-m_displayLabels[name].balanceMin) < 0.00001) 
        m_yBalanceSpeed->setRange(0,1);
    else 
        m_yBalanceSpeed->setRange(m_displayLabels[name].balanceMin, m_displayLabels[name].balanceMax);

    switch (m_chartView->getChartType())
    {
    case CHART_LINE:
        m_displayLabels[name].seriesLine->append(duration, balanceSpeed);
        break;
    case CHART_BAR:
        break;
    }

    if (duration < 180) m_xAxisTime->setRange(0,180);
    else m_xAxisTime->setRange(0,duration);
    m_chart->update();
}
void HGDisplayChartWidget::drawTimeAndVoltage(const std::string &name,const std::map<std::string,std::string> &dataInfos)
{
    int duration = 0;
    if (dataInfos.find("检测时长")!=dataInfos.end()) duration=std::atoi(dataInfos.at("检测时长").c_str());
    int voltage = 0;
    if (dataInfos.find("电压")!=dataInfos.end()) voltage=std::atoi(dataInfos.at("电压").c_str());
    else return;
    
    if (voltage < m_displayLabels[name].voltageMin)
        m_displayLabels[name].voltageMin = voltage;
    if (voltage > m_displayLabels[name].voltageMax)
        m_displayLabels[name].voltageMax = voltage;

    if (m_displayLabels[name].voltageMax <= m_displayLabels[name].voltageMin)
        m_yVoltage->setRange(0,1);
    else 
        m_yVoltage->setRange(m_displayLabels[name].voltageMin, m_displayLabels[name].voltageMax);

    switch (m_chartView->getChartType())
    {
    case CHART_LINE:
        m_displayLabels[name].seriesLine->append(duration, voltage);
        break;
    case CHART_BAR:
        break;
    }

    if (duration < 180) m_xAxisTime->setRange(0,180);
    else m_xAxisTime->setRange(0,duration);
    m_chart->update();
}
void HGDisplayChartWidget::showTestInfo(const std::string &dbName)
{
    // if (HGOnlineRWDB::getTaskRunFlag() == "true")
    //     return;
    // // 动态清空标签
    // QStringList labels = m_xAxisTime->categoriesLabels();
    // for (const QString &label : labels)
    // {
    //     m_xAxisTime->remove(label); // 移除每个标签
    // }
    // for (auto name : displayNames)
    // {
    //     if (m_displayLabels[name].label.flag)
    //     {
    //         if (m_displayLabels[name].seriesLine->count() >= MAX_POINT_COUNT)
    //         {
    //             m_displayLabels[name].seriesLine->clear();
    //         }
    //     }
    //     else
    //         continue;
    // }
    // std::vector<std::map<std::string,std::string>> dataInfos=HGOnlineRWDB::readDeviceDataInfo(dbName);

    // if (int(dataInfos.size()) <= 0) return;

    // for (int i=0;i<int(dataInfos.size());i++){
    //     fnUpdateChartData(dataInfos[i]);
    // }
}
void HGDisplayChartWidget::showDataInfo(const std::map<std::string,std::string> &info){
    if (info.size()>0)  fnUpdateChartData(info);
}
void HGDisplayChartWidget::fnUpdateChartData(const std::map<std::string,std::string> &dataInfos)
{
    if (dataInfos.size() <=0) {
        printf("no data\n");
        return;
    }
    bool flag=false;
    float cube = 0.0;
    if (dataInfos.find("体积")!=dataInfos.end()) cube = std::stof(dataInfos.at("体积").c_str());
    m_cubeValues.push_back(cube);
    float voltage = 0;
    if (dataInfos.find("电压")!=dataInfos.end()) voltage = std::stof(dataInfos.at("电压").c_str());
    m_voltages.push_back(voltage);

    for (auto name : displayNames)
    {
        if (std::find(m_displayNames.begin(),m_displayNames.end(),name)==m_displayNames.end()) continue;
        if (m_displayLabels[name].label.flag)
        {
            flag=true;
            if (name=="时间~信号值"){
                // drawTimeAndSignal(name,dataInfos);
            } else if (name=="时间~信号微分值"){
                // drawTimeAndOneDiff(name,dataInfos);
            } else if (name=="时间~体积"){
                drawTimeAndCube(name,dataInfos);
            } else if (name=="时间~电压"){
                drawTimeAndVoltage(name,dataInfos);
            } else if (name=="体积~信号值"){
                // drawCubeAndSignal(name,dataInfos);
            } else if (name=="体积~电压"){
                drawCubeAndVoltage(name,dataInfos);
            } else if (name=="时间~dV/dt"){
                drawTimeAndCubeDiff(name,dataInfos);
            } else if (name=="dE/dv~电压"){
                drawVoltageAndVoltageDiffWithCube(name,dataInfos);
            } else if (name=="时间~信号二次微分"){
                // drawTimeAndTwoDiff(name,dataInfos);
            } else if (name=="水质量~电压"){
                drawWaterWeightAndVoltage(name,dataInfos);
            } else if (name=="时间~水质量"){
                drawTimeAndWaterWeigth(name,dataInfos);
            } else if (name=="时间~电解速率"){
                drawTimeAndBalanceSpeed(name,dataInfos);
            }
        }
        else
            continue;
    }
    if (flag){
        m_chart->scroll(1,0);
    }
}
void HGDisplayChartWidget::slotUpdateChart()
{
    // std::string dbName=HGOnlineRWDB::getTaskRunRecordDataDB();

    // std::vector<std::map<std::string,std::string>> dataInfos=HGOnlineRWDB::readDeviceDataInfo(dbName);

    // if (int(dataInfos.size()) <= 0) return;

    // for (int i=0;i<int(dataInfos.size());i++){
    //     fnUpdateChartData(dataInfos[i]);
    // }
}
void HGDisplayChartWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (!m_displayTypeLabel->geometry().contains(event->pos())){
            if (m_dragDialog!=NULL){
                fnWriteDB();
                SAFE_DELETE(m_dragDialog);
            }
            return;
        }
    }
    QWidget::mousePressEvent(event);
}
void HGDisplayChartWidget::onChartClicked(const QPoint& pos)
{
    if (!m_displayTypeLabel->geometry().contains(pos)){
        if (m_dragDialog!=NULL){
            fnWriteDB();
            SAFE_DELETE(m_dragDialog);
        }
    }
}
QString HGDisplayChartWidget::fnGetTimeLabel(const int &index){
    QString str;
    return str;
    // QStringList labels=m_xAxisTime->categoriesLabels();
    // if (index <0||index >=m_xAxisTime->count()) return "";
    // else return labels[index];
}
int HGDisplayChartWidget::isLabelExist(QCategoryAxis *axis, const QString &label) {
    // 获取所有标签
    QStringList labels = axis->categoriesLabels();
    // 检查目标标签是否在列表中
    for (int i = 0; i < labels.size(); ++i) {
        if (labels[i]==label) {
            return i;
        }
    }

    if (axis->count() >= MAX_POINT_COUNT)
    {
        // 动态清空标签
        QStringList labels = axis->categoriesLabels();
        for (const QString &label : labels)
        {
            axis->remove(label); // 移除每个标签
        }
        for (auto name : displayNames)
        {
            if (std::find(m_displayNames.begin(),m_displayNames.end(),name)==m_displayNames.end()) continue;
            if (m_displayLabels[name].label.flag)
            {
                if (m_displayLabels[name].seriesLine->count() >= MAX_POINT_COUNT) {
                    m_displayLabels[name].seriesLine->clear();
                }
            }
            else
                continue;
        }
    }
    return -1;
}