#include "hgrunningwidget.h"
#include <QGraphicsRectItem>
#include <QMessageBox>
#include <QButtonGroup>
#include "common.h"

HGRunningWidget::HGRunningWidget(std::string lang,QWidget *parent) : QWidget(parent),
m_lang(lang),
m_curChannelIndex(1)
{
    std::string enterStr=loadTranslation(m_lang,"Enter");
    std::string runningStr=loadTranslation(m_lang,"RunningStatus");

    RWDb::writeAuditTrailLog(loadTranslation(m_lang,"Enter")+loadTranslation(m_lang,"RunningStatus"));

    m_layout=new QGridLayout();
    m_layout->setVerticalSpacing(0);

    fnInitChartType();
    fnInitChartElement();
    m_stackedWidget=new QStackedWidget();
    m_views.resize(MAX_VIEW_NUMBER);
    m_scenes.resize(MAX_VIEW_NUMBER);
    m_charts.resize(MAX_VIEW_NUMBER);
    m_dataInfoLabel=new QLabel();
    m_methodInfoLabel=new QLabel();
    m_resultInfoLabel=new QLabel();

    for (int i=0;i<MAX_VIEW_NUMBER;i++){
        m_views[i]=new QGraphicsView();
        m_views[i]->setDragMode(QGraphicsView::ScrollHandDrag);
        m_scenes[i]=new MyScene();
        m_views[i]->setScene(m_scenes[i]);
        // 设置 QGraphicsView 透明
        m_views[i]->setStyleSheet("background: transparent; border: none;");
        m_views[i]->setMouseTracking(true);
        // 设置 QGraphicsScene 透明
        m_scenes[i]->setBackgroundBrush(Qt::transparent);
        fnDisplayRunningStatus(i);
        m_charts[i]=new HGRunningChartWidget(m_lang);
        #ifdef ENABLE_CAS6
        m_chart=NULL;
        m_chart=new HGDisplayChartWidget(m_lang);
        m_stackedWidget->addWidget(m_chart);
        #else
        m_stackedWidget->addWidget(m_views[i]);
        m_stackedWidget->addWidget(m_charts[i]);
        #endif
    }

    m_radioBtnChannel1 = new LabelWithImg(IMGRIGHT,12,getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"),loadTranslation(m_lang,"Channel")+std::to_string(1));
    connect(m_radioBtnChannel1,SIGNAL(clickLeftName(std::string)),this,SLOT(slotLabelClicked(std::string)));
    #ifdef ENABLE_CAS6

    #else
    m_radioBtnChannel2 = new LabelWithImg(IMGRIGHT,12,getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"),loadTranslation(m_lang,"Channel")+std::to_string(2));
    m_radioBtnChannel3 = new LabelWithImg(IMGRIGHT,12,getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"),loadTranslation(m_lang,"Channel")+std::to_string(3));
    m_radioBtnChannel4 = new LabelWithImg(IMGRIGHT,12,getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"),loadTranslation(m_lang,"Channel")+std::to_string(4));
    
    connect(m_radioBtnChannel2,SIGNAL(clickLeftName(std::string)),this,SLOT(slotLabelClicked(std::string)));
    connect(m_radioBtnChannel3,SIGNAL(clickLeftName(std::string)),this,SLOT(slotLabelClicked(std::string)));
    connect(m_radioBtnChannel4,SIGNAL(clickLeftName(std::string)),this,SLOT(slotLabelClicked(std::string)));
    #endif
    m_radioBtnChannel1->setImg(getPath("/resources/V1/@setting.png"));
    
    m_startLabel=new HGQLabel(false,getPath("/resources/V1/@1xif-play-alt-2 1.png"));
    m_pauseLabel=new HGQLabel(false,getPath("/resources/V1/@1xif-stop 1.png"));
    connect(m_startLabel,SIGNAL(leftClicked()),this,SLOT(slotStart()));
    connect(m_pauseLabel,SIGNAL(leftClicked()),this,SLOT(slotPause()));

    m_layout->addWidget(m_dataInfoLabel,0,1,1,6);
    m_layout->addWidget(m_methodInfoLabel,0,7,1,1);
    m_layout->addWidget(m_resultInfoLabel,0,8,1,1);
    m_layout->addWidget(m_stackedWidget,1,1,10,10);
    m_layout->addWidget(m_radioBtnChannel1,12,0);
    #ifdef ENABLE_CAS6
 
    #else
    m_layout->addWidget(m_radioBtnChannel2,12,1);
    m_layout->addWidget(m_radioBtnChannel3,12,2);
    m_layout->addWidget(m_radioBtnChannel4,12,3);
    #endif
    m_layout->addWidget(m_startLabel,11,10);
    m_layout->addWidget(m_pauseLabel,12,10);
    this->setLayout(m_layout);

    m_timer=new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(slotUpdateDataInfo()));
    m_timer->start(100);

    slotUpdateDataInfo();
}

bool HGRunningWidget::closeWindow()
{
    return true;
}
HGRunningWidget::~HGRunningWidget()
{

}
void HGRunningWidget::fnDisplayRunningStatus(int index)
{
    switch (index)
    {
    case 0:
    {
        #ifdef ENABLE_CAS6
        #else
        // 测试：添加一个矩形（看透明效果）
        MyRectangleItem *itemAction = new MyRectangleItem(QRectF(0, 0, 200, 200),
                                                          "ActionPool", m_lang, QColor(255, 255, 255, 0));
        MyRectangleItem *itemRecycle = new MyRectangleItem(QRectF(0, 0, 100, 80),
                                                           "DiscardPool", m_lang, QColor(255, 255, 255, 0));
        MyRectangleItem *itemElectrode1 = new MyRectangleItem(QRectF(0, 0, 10, 200),
                                                              "E1", m_lang, QColor(0, 255, 0, 255));
        MyRectangleItem *itemElectrode2 = new MyRectangleItem(QRectF(0, 0, 10, 200),
                                                              "E2", m_lang, QColor(50, 255, 50, 255));

        MyHighAccuracyPumpItem *itemHighAccuracymPump = new MyHighAccuracyPumpItem(QRectF(0, 0, 100, 200),
                                                                                   "HighAccuracyPump", m_lang, QColor(255, 255, 255, 255));
        MyAuxiliaryPumpItem *itemAuxiliaryPump = new MyAuxiliaryPumpItem(QRectF(0, 0, 100, 100),
                                                                         "AuxiliaryPump", m_lang, QColor(0, 255, 0, 0));
        // 创建折线
        QVector<QPointF> points = {{50, 50}, {150, 100}, {250, 50}};
        PolylineItem *itemPolyline = new PolylineItem();

        m_scenes[index]->addItem(itemAction);
        m_scenes[index]->addItem(itemRecycle);
        m_scenes[index]->addItem(itemElectrode1);
        m_scenes[index]->addItem(itemElectrode2);
        m_scenes[index]->addItem(itemHighAccuracymPump);
        m_scenes[index]->addItem(itemAuxiliaryPump);
        m_scenes[index]->addItem(itemPolyline);
        #endif
    }
    break;
    case 1:
    {
    }
    break;
    case 2:
    {
    }
    break;
    case 3:
    {
    }
    break;
    default:
        break;
    }
}
void HGRunningWidget::fnInitChartElement()
{

}
void HGRunningWidget::fnInitChartType()
{
    QButtonGroup *btnGroup=new QButtonGroup();
    btnGroup->setExclusive(true);

    m_deviceStatusBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"DeviceStatus")));
    m_chartBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"SpectralData")));
    btnGroup->addButton(m_deviceStatusBtn);
    btnGroup->addButton(m_chartBtn);
    m_deviceStatusBtn->setCheckable(true);
    m_chartBtn->setCheckable(true);

    m_deviceStatusBtn->setStyleSheet("QPushButton {"
                               "background-color: #4CAF50;"
                               "color: white;"
                               "border: none;"
                               "padding: 10px;"
                               "font-size: 16px;"
                               "border-radius: 10px;"
                               "}"
                               "QPushButton:checked {"
                               "background-color: #FF5722;"
                               "}");
    m_chartBtn->setStyleSheet("QPushButton {"
                                "background-color: #4CAF50;"
                                "color: white;"
                                "border: none;"
                                "padding: 10px;"
                                "font-size: 16px;"
                                "border-radius: 10px;"
                                "}"
                                "QPushButton:checked {"
                                "background-color: #FF5722;"
                                "}");
    m_deviceStatusBtn->setChecked(true);
    connect(m_deviceStatusBtn,&QPushButton::clicked,[=](){
        fnDisplayDeviceStatus();
    });
    connect(m_chartBtn,&QPushButton::clicked,[=](){
        fnDisplayChart();
    });

    m_layout->addWidget(m_deviceStatusBtn,0,0);
    #ifdef ENABLE_CAS6
    #else
    m_layout->addWidget(m_chartBtn,1,0);
    #endif
}
void HGRunningWidget::slotLabelClicked(std::string name)
{
    std::string channelname=loadTranslation(m_lang,"Channel");
    std::string number=name.substr(name.find_first_of(channelname)+channelname.length());
    int inumber=std::atoi(number.c_str());
    m_curChannelIndex=inumber;
    #ifdef ENABLE_CAS6
    #else
    switch (inumber){
        case 1:{
        m_radioBtnChannel1->setImg(getPath("/resources/V1/@setting.png"));
        m_radioBtnChannel2->setImg(getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"));
        m_radioBtnChannel3->setImg(getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"));
        m_radioBtnChannel4->setImg(getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"));
        break;
        }
        case 2:{
        m_radioBtnChannel2->setImg(getPath("/resources/V1/@setting.png"));
        m_radioBtnChannel1->setImg(getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"));
        m_radioBtnChannel3->setImg(getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"));
        m_radioBtnChannel4->setImg(getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"));
        break;
        }
        case 3:{
        m_radioBtnChannel3->setImg(getPath("/resources/V1/@setting.png"));
        m_radioBtnChannel2->setImg(getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"));
        m_radioBtnChannel1->setImg(getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"));
        m_radioBtnChannel4->setImg(getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"));
        break;
        }
        case 4:{
        m_radioBtnChannel4->setImg(getPath("/resources/V1/@setting.png"));
        m_radioBtnChannel2->setImg(getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"));
        m_radioBtnChannel3->setImg(getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"));
        m_radioBtnChannel1->setImg(getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"));
        break;
        }
        default: break;
    }
    #endif
    switch (m_runningStatusType){
        case RUNNING_DEVICE_TYPE:{
            m_stackedWidget->setCurrentWidget(m_views[inumber-1]);
            break;
        }
        case RUNNING_CHART_TYPE:{
            m_stackedWidget->setCurrentWidget(m_charts[inumber-1]);
            break;
        }
        default: break;
    }
}
void HGRunningWidget::fnDisplayDeviceStatus(){
    m_runningStatusType=RUNNING_DEVICE_TYPE;
    m_stackedWidget->setCurrentWidget(m_views[m_curChannelIndex-1]);
}
void HGRunningWidget::fnDisplayChart(){
    m_runningStatusType=RUNNING_CHART_TYPE;
    m_stackedWidget->setCurrentWidget(m_charts[m_curChannelIndex-1]);
}
void HGRunningWidget::setRunningMethodInfo(const QString& strstatus){
    m_methodInfoLabel->setText(strstatus);
}
void HGRunningWidget::slotStart(){
    if (getConnectWithMCUStatus()!=LINK_ONLINE) {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), QString::fromStdString(loadTranslation(m_lang,"NotConnectWithMCU")));
        return;
    }
    // emit signalRunning();
#ifdef ENABLE_CAS6
    int taskCount=getTaskCount();
    if (taskCount<=0) {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), QString::fromStdString(loadTranslation(m_lang,"NoTaskToDetect")));
        return;
    }
    setStartRunningTaskSequence(true);
    return;
#else
#endif
}
void HGRunningWidget::slotPause(){

}
void HGRunningWidget::slotUpdateDataInfo(){
    setRequestData();
    // running status
    bool runningFlag=makeNextSample();
    if (runningFlag) {
        m_startLabel->changePixmap(getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"),24);
        
    }
    // data information
    QString strstatus;
    // method inforamtion
    std::map<std::string,std::string> taskInfo=getCurRunningTaskInfo();
    std::string curMethod=taskInfo.find("method")!=taskInfo.end()?taskInfo["method"]:"";
    double sampleQuantity= taskInfo.find("quantity")!=taskInfo.end()?std::atof(taskInfo["quantity"].c_str()):1.2345;
    strstatus.clear();
    strstatus = QString(
        "当前方法： %1\n"
        "样品总量： %2 g")
        .arg(QString::fromStdString(curMethod))
        .arg(sampleQuantity);
    m_methodInfoLabel->setText(strstatus);
    strstatus.clear();
    std::map<std::string,std::string> values = getCAS6RawValues();
    if (!values.empty())
    {
        int voltage = values.find("电压") != values.end() ? std::atoi(values["电压"].c_str()) : 0;
        double dianjieV = values.find("电解速度") != values.end() ? std::atof(values["电解速度"].c_str()) : 0.0;
        double curTemp = values.find("当前温度") != values.end() ? std::atof(values["当前温度"].c_str()) : 0.0;
        double setTemp = values.find("加热温度") != values.end() ? std::atof(values["加热温度"].c_str()) : 0.0;
        double zaiqiV = values.find("气流速度") != values.end() ? std::atof(values["气流速度"].c_str()) : 0.0; // 26.3;
        double driftV = values.find("漂移") != values.end() ? std::atof(values["漂移"].c_str()) : 0.0;         // 212.12;
        std::istringstream iss(values.find("风扇状态") != values.end() ? values["风扇状态"] : "0 0");
        std::string first = "", second = "";
        iss >> first >> second;
        std::string fanStatus = (first == "1" ? "开" : "关");

        double autoblankV = values.find("空白") != values.end() ? std::atof(values["空白"].c_str()) : 0.0; // 0.55;
        strstatus = QString(
                        "电压： %1 mv "
                        "电解： %2 ug/m "
                        "温度： %3/%4 ℃ "
                        "载气： %5 mL/m "
                        "漂移： %6 ug/m "
                        "风扇： %7 "
                        "自动空白： %8 ug")
                        .arg(voltage)
                        .arg(dianjieV)
                        .arg(curTemp)
                        .arg(setTemp)
                        .arg(zaiqiV)
                        .arg(driftV)
                        .arg(QString::fromStdString(fanStatus))
                        .arg(autoblankV);
        m_dataInfoLabel->setText(strstatus);

        // result inforamtion
        std::string analysisTime = values.find("时间") != values.end() ? values["时间"] : "00:00:00";
        double waterWeight = values.find("水质量") != values.end() ? std::atof(values["水质量"].c_str()) : 0.0; // 2.0;
        double waterPercent = 0.0;
        if (fabs(sampleQuantity) < 0.00001)
            waterPercent = 0.0;
        else
            waterPercent = (waterWeight / sampleQuantity) * 100.0;
        strstatus.clear();
        strstatus = QString(
                        "分析时长： %1\n"
                        "水质量： %2 ug\n"
                        "含水结果： %3 %")
                        .arg(QString::fromStdString(analysisTime))
                        .arg(waterWeight)
                        .arg(waterPercent);
        m_resultInfoLabel->setText(strstatus);
    }
    #ifdef ENABLE_CAS6
    m_chart->showDataInfo(values);
    #endif
}