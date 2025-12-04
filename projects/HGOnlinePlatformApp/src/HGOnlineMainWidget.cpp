#include "HGOnlineMainWidget.h"
#include "common.h"
#include <QDebug>
#include <QDialog>
#include "hglog4cplus.h"
#include "HGMacroData.h"
#include "demo.h"
#include "HGOnlinePlatformInterface.h"
#include <QMessageBox>
#include <QFileDialog>
#include "HGAuthorityService.h"
#include "hgthreadManage.h"


using namespace HGMACHINE;

HGOnlineMainWidget::HGOnlineMainWidget(QWidget *parent)
    : QWidget(parent),
    m_soundFlag(false),
    m_loginFlag(false),
    m_loginW(nullptr),
    m_deviceManageW(nullptr),
    m_systemSetW(nullptr),
    m_channelSetW(nullptr),
    m_reagentManageW(nullptr),
    m_runningW(nullptr),
    m_taskManageW(nullptr),
    m_flowW(nullptr),
    m_methodManageW(nullptr),
    m_analysisRecordW(nullptr),
    m_warningW(nullptr),
    m_logW(nullptr),
    m_deviceMaintenanceW(nullptr),
    m_exceptionHandleW(nullptr),
    m_materialManageW(nullptr),
    m_runningTaskSequence(false)
{
    m_width = getScreenGeometry().width();
    m_height = getScreenGeometry().height();
    qDebug()<<m_width<<","<<m_height;
    this->setFixedSize(m_width,m_height);
    this->setWindowFlag(Qt::Window);
    this->showFullScreen();

///////////---interface-------
    QString basePath = qApp->applicationDirPath();
    GlobalSingleton::instance().setSystemInfo("basePath", basePath.toStdString());
    HGOnlineRWDB::writeSerialPortInfo(listAvailableSerialPorts());
    GlobalSingleton::instance().loadUsersInfo();
    createThread();
    startThread();
///////////---interface-------

    m_language=GlobalSingleton::instance().getSystemInfo("显示语言");
    m_language= (m_language=="English"?"en":"zh");

    m_leftIndex = 0;
    m_topIndex = 0;

    m_listenTimer=NULL;
    m_listenTimer=new QTimer();
    connect(m_listenTimer,SIGNAL(timeout()),this,SLOT(fnSlotListenTimerOut()));
    m_listenTimer->start(1000);

    m_titleTimer=new QLabel(this);
    m_titleTimer->setText("00:00:00");
    m_titleTimer->setStyleSheet("QLabel{background-color:rgb(147,210,243);color:black;}");
    m_titleTimer->setVisible(true);
    m_titleTimer->raise();

    m_topLabel = new QLabel(this);
    m_topLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);}");
    m_topLabel->setVisible(true);

    m_leftLabel = new QLabel(this);
    m_leftLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);}");
    m_leftLabel->setVisible(true);

    m_statusLabel = new QLabel(this);
    m_statusLabel->setText("状态：未就绪 缺少信号，请检查仪器"); 
    m_statusLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);color:black;}");
    m_statusLabel->setVisible(true);
    m_statusLabel->raise();

    m_quitLabel=new HGQLabel();
    m_quitLabel->setParent(this);
    printf("basePath:%s",basePath.toStdString().c_str());
    QPixmap pixmap(basePath+"/resources/V1/@1xsemiDesign-semi-icons-quit.png");
    m_quitLabel->setPixmap(pixmap.scaled(55,55,Qt::KeepAspectRatio));
    m_quitLabel->setVisible(true);
    connect(m_quitLabel,SIGNAL(leftClicked()),this,SLOT(clickEscape()));

    m_soundLabel=new HGQLabel();
    m_soundLabel->setParent(this);
    m_soundLabel->setPixmap(QPixmap(basePath+"/resources/V1/@sound-open-main.png").scaled(55,55,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    m_soundLabel->setVisible(true);
    connect(m_soundLabel,SIGNAL(leftClicked()),this,SLOT(clickSound()));

    if (GlobalSingleton::instance().getSystemInfo("声音报警") == "true"&&
        GlobalSingleton::instance().getSystemInfo("声音value")!="0") {
        m_soundFlag=true;
        m_soundLabel->setPixmap(QPixmap(basePath+"/resources/V1/@sound-open-main.png").scaled(55,55,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    } else {
        m_soundFlag=false;
        m_soundLabel->setPixmap(QPixmap(basePath+"/resources/V1/@sound-close-main.png").scaled(55,55,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    }

    m_reloginLabel=new HGQLabel();
    m_reloginLabel->setParent(this);
    m_reloginLabel->setPixmap(QPixmap(basePath+"/resources/V1/@1xfa5-user-circle-fas-undefined 1 (1).png").scaled(30,30,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    m_reloginLabel->setVisible(true);
    connect(m_reloginLabel,SIGNAL(leftClicked()),this,SLOT(clickRelogin()));
    m_curloginLabel=new HGQLabel();
    m_curloginLabel->setParent(this);
    m_curloginLabel->setPixmap(QPixmap(basePath+"/resources/V1/@1xze-manager 1 (1).png").scaled(30,30,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    m_curloginLabel->setVisible(true);
    connect(m_curloginLabel,SIGNAL(leftClicked()),this,SLOT(clickCurlogin()));

    m_downloadDBLabel=new HGQLabel();
    m_downloadDBLabel->setParent(this);
    m_downloadDBLabel->setPixmap(QPixmap(basePath+"/resources/V1/@1xiconPark-to-bottom 1.png").scaled(30,30,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    m_downloadDBLabel->setVisible(true);
    connect(m_downloadDBLabel, SIGNAL(leftClicked()), this, SLOT(clickDownloadDB()));

    m_loginW = new HGLoginWidget(m_language, this);
    m_loginW->move(m_leftLabel->width(), m_topLabel->height());
    m_loginW->setFixedSize(m_width - m_leftLabel->width(), m_height - m_topLabel->height());
    m_loginW->show();

    qRegisterMetaType<UserInfoS>("UserInfoS");
    connect(m_loginW, SIGNAL(loginOn(UserInfoS)), this, SLOT(slotLoginOn(UserInfoS)));

    fnInitPos();
    fnSlotListenTimerOut();
}

HGOnlineMainWidget::~HGOnlineMainWidget()
{


}
void HGOnlineMainWidget::slotLoginOn(UserInfoS userInfo){
    if (GlobalSingleton::instance().getSystemInfo("免密登录") == "true"){
        m_loginFlag=true;
        m_loginAuthority=SYSTEM_MANAGER;
        fnInitSoftwarePage();
        RWDb::writeAuditTrailLog("免密登录成功");
    }
    else
    {
        m_userInfoS = userInfo;
        fnCloseWindow();
        m_loginFlag = true;
        fnInitSoftwarePage();
        m_loginAuthority = (HGOnlineRWDB::readLoginAuthority());
    }
    GlobalSingleton::instance().loadUserGroupInfo();
}
void HGOnlineMainWidget::fnInitSoftwarePage()
{
    int fontSize=m_width*m_height/67000;

    m_deviceLabel = new HGQLabel();
    m_deviceLabel->setParent(this);
    m_deviceLabel->setText(QString::fromStdString(loadTranslation(m_language, "Device"/*"设备"*/)));//
    QFont font=m_deviceLabel->font();
    font.setPointSize(fontSize);
    m_deviceLabel->setFont(font);
    m_deviceLabel->setAlignment(Qt::AlignCenter);
    m_deviceLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    connect(m_deviceLabel,SIGNAL(leftClicked()),this,SLOT(slotDeviceLabelleftClicked()));

    m_mainPageLabel = new HGQLabel();
    m_mainPageLabel->setParent(this);
    m_mainPageLabel->setText(QString::fromStdString(loadTranslation(m_language,"MainPage")));//主页"
    font=m_mainPageLabel->font();
    font.setPointSize(fontSize);
    m_mainPageLabel->setFont(font);
    m_mainPageLabel->setAlignment(Qt::AlignCenter);
    m_mainPageLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    connect(m_mainPageLabel,SIGNAL(leftClicked()),this,SLOT(slotMainPageLabelleftClicked()));

    m_dataLabel = new HGQLabel();
    m_dataLabel->setParent(this);
    m_dataLabel->setText(QString::fromStdString(loadTranslation(m_language,"Data"))); // "数据");
    font=m_dataLabel->font();
    font.setPointSize(fontSize);
    m_dataLabel->setFont(font);
    m_dataLabel->setAlignment(Qt::AlignCenter);
    m_dataLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
   
    connect(m_dataLabel,SIGNAL(leftClicked()),this,SLOT(slotDataLabelleftClicked()));

    m_maintenanceLabel = new HGQLabel();
    m_maintenanceLabel->setParent(this);
    m_maintenanceLabel->setText(QString::fromStdString(loadTranslation(m_language,"Maintenance"))); //"维护");
    font=m_maintenanceLabel->font();
    font.setPointSize(fontSize);
    m_maintenanceLabel->setFont(font);
    m_maintenanceLabel->setAlignment(Qt::AlignCenter);
    m_maintenanceLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}"); 
    connect(m_maintenanceLabel,SIGNAL(leftClicked()),this,SLOT(slotMaintenanceLabelleftClicked()));

    m_systemSetLabel = new HGQLabel();
    m_systemSetLabel->setParent(this);
    m_systemSetLabel->setObjectName("SystemManage");
    m_systemSetLabel->setText(QString::fromStdString(loadTranslation(m_language,
        m_systemSetLabel->objectName().toStdString())));//"系统设置");
    font=m_systemSetLabel->font();
    font.setPointSize(fontSize);
    m_systemSetLabel->setFont(font);
    m_systemSetLabel->setAlignment(Qt::AlignCenter);
    m_systemSetLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    connect(m_systemSetLabel,SIGNAL(leftClicked()),this,SLOT(slotSystemSetLabelleftClicked()));

    m_deviceManageLabel = new HGQLabel();
    m_deviceManageLabel->setParent(this);
    m_deviceManageLabel->setObjectName("DeviceManage");
    m_deviceManageLabel->setText(QString::fromStdString(loadTranslation(m_language,
        m_deviceManageLabel->objectName().toStdString())));//"设备管理");
    font=m_deviceManageLabel->font();
    font.setPointSize(fontSize);
    m_deviceManageLabel->setFont(font);
    m_deviceManageLabel->setAlignment(Qt::AlignCenter);
    m_deviceManageLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    connect(m_deviceManageLabel,SIGNAL(leftClicked()),this,SLOT(slotDeviceManageLabelleftClicked()));

    m_channelSetLabel = new HGQLabel();
    m_channelSetLabel->setObjectName("ChannelSet");
    m_channelSetLabel->setParent(this);
    m_channelSetLabel->setText(QString::fromStdString(loadTranslation(m_language,
        m_channelSetLabel->objectName().toStdString())));//"通道配置");
    font=m_channelSetLabel->font();
    font.setPointSize(fontSize);
    m_channelSetLabel->setFont(font);
    m_channelSetLabel->setAlignment(Qt::AlignCenter);
    m_channelSetLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    connect(m_channelSetLabel,SIGNAL(leftClicked()),this,SLOT(slotChannelSetLabelleftClicked()));

    m_reagentManageLabel = new HGQLabel();
    m_reagentManageLabel->setParent(this);
    m_reagentManageLabel->setObjectName("ReagentManage");
    m_reagentManageLabel->setText(QString::fromStdString(loadTranslation(m_language,
        m_reagentManageLabel->objectName().toStdString())));//"试剂管理");
    font=m_reagentManageLabel->font();
    font.setPointSize(fontSize);
    m_reagentManageLabel->setFont(font);
    m_reagentManageLabel->setAlignment(Qt::AlignCenter);
    m_reagentManageLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    connect(m_reagentManageLabel,SIGNAL(leftClicked()),this,SLOT(slotReagentManageLabelleftClicked()));

    m_runningLabel = new HGQLabel();
    m_runningLabel->setParent(this);
    m_runningLabel->setObjectName("RunningStatus");
    m_runningLabel->setText(QString::fromStdString(loadTranslation(m_language,
        m_runningLabel->objectName().toStdString())));//"运行状态");
    font=m_runningLabel->font();
    font.setPointSize(fontSize);
    m_runningLabel->setFont(font);
    m_runningLabel->setAlignment(Qt::AlignCenter);
    m_runningLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    connect(m_runningLabel,SIGNAL(leftClicked()),this,SLOT(slotRunningLabelleftClicked()));

    m_taskManageLabel = new HGQLabel();
    m_taskManageLabel->setParent(this);
    m_taskManageLabel->setObjectName("TaskSequence");
    m_taskManageLabel->setText(QString::fromStdString(loadTranslation(m_language,
        m_taskManageLabel->objectName().toStdString())));//"任务序列");
    font=m_taskManageLabel->font();
    font.setPointSize(fontSize);
    m_taskManageLabel->setFont(font);
    m_taskManageLabel->setAlignment(Qt::AlignCenter);
    m_taskManageLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    connect(m_taskManageLabel,SIGNAL(leftClicked()),this,SLOT(slotTaskManageLabelleftClicked()));

    m_flowLabel = new HGQLabel();
    m_flowLabel->setParent(this);
    m_flowLabel->setObjectName("Flow");
    m_flowLabel->setText(QString::fromStdString(loadTranslation(m_language,
        m_flowLabel->objectName().toStdString())));//"流程");
    font=m_flowLabel->font();
    font.setPointSize(fontSize);
    m_flowLabel->setFont(font);
    m_flowLabel->setAlignment(Qt::AlignCenter);
    m_flowLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    connect(m_flowLabel,SIGNAL(leftClicked()),this,SLOT(slotFlowLabelleftClicked()));

    m_methodManageLabel = new HGQLabel();
    m_methodManageLabel->setParent(this);
    m_methodManageLabel->setObjectName("Method");
    m_methodManageLabel->setText(QString::fromStdString(loadTranslation(m_language,
        m_methodManageLabel->objectName().toStdString())));//"方法");
    font=m_methodManageLabel->font();
    font.setPointSize(fontSize);
    m_methodManageLabel->setFont(font);
    m_methodManageLabel->setAlignment(Qt::AlignCenter);
    m_methodManageLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    connect(m_methodManageLabel,SIGNAL(leftClicked()),this,SLOT(slotMethodManageLabelleftClicked()));

    m_analysisRecordLabel = new HGQLabel();
    m_analysisRecordLabel->setParent(this);
    m_analysisRecordLabel->setObjectName("AnalysisRecord");
    m_analysisRecordLabel->setText(QString::fromStdString(loadTranslation(m_language,
        m_analysisRecordLabel->objectName().toStdString())));//"分析记录");
    font=m_analysisRecordLabel->font();
    font.setPointSize(fontSize);
    m_analysisRecordLabel->setFont(font);
    m_analysisRecordLabel->setAlignment(Qt::AlignCenter);
    m_analysisRecordLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    connect(m_analysisRecordLabel,SIGNAL(leftClicked()),this,SLOT(slotAnalysisRecordLabelleftClicked()));

    m_warningLabel = new HGQLabel();
    m_warningLabel->setParent(this);
    m_warningLabel->setObjectName("WarningInfo");
    m_warningLabel->setText(QString::fromStdString(loadTranslation(m_language,
        m_warningLabel->objectName().toStdString())));//"报警信息");
    font=m_warningLabel->font();
    font.setPointSize(fontSize);
    m_warningLabel->setFont(font);
    m_warningLabel->setAlignment(Qt::AlignCenter);
    m_warningLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    connect(m_warningLabel,SIGNAL(leftClicked()),this,SLOT(slotWarningLabelleftClicked()));

    m_logLabel = new HGQLabel();
    m_logLabel->setParent(this);
    m_logLabel->setObjectName("Log");
    m_logLabel->setText(QString::fromStdString(loadTranslation(m_language,
        m_logLabel->objectName().toStdString())));//"日志");
    font=m_logLabel->font();
    font.setPointSize(fontSize);
    m_logLabel->setFont(font);
    m_logLabel->setAlignment(Qt::AlignCenter);
    m_logLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    connect(m_logLabel,SIGNAL(leftClicked()),this,SLOT(slotLogLabelleftClicked()));

    m_deviceMaintenanceLabel = new HGQLabel();
    m_deviceMaintenanceLabel->setParent(this);
    m_deviceMaintenanceLabel->setObjectName("DeviceMaintenance");
    m_deviceMaintenanceLabel->setText(QString::fromStdString(loadTranslation(m_language,
        m_deviceMaintenanceLabel->objectName().toStdString())));//"设备维护");
    font=m_deviceMaintenanceLabel->font();
    font.setPointSize(fontSize);
    m_deviceMaintenanceLabel->setFont(font);
    m_deviceMaintenanceLabel->setAlignment(Qt::AlignCenter);
    m_deviceMaintenanceLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    connect(m_deviceMaintenanceLabel,SIGNAL(leftClicked()),this,SLOT(slotDeviceMaintenanceLabelleftClicked()));

    m_exceptionHandleLabel = new HGQLabel();
    m_exceptionHandleLabel->setParent(this);
    m_exceptionHandleLabel->setObjectName("ExceptionHandle");
    m_exceptionHandleLabel->setText(QString::fromStdString(loadTranslation(m_language,
        m_exceptionHandleLabel->objectName().toStdString())));
    font=m_exceptionHandleLabel->font();
    font.setPointSize(fontSize);
    m_exceptionHandleLabel->setFont(font);
    m_exceptionHandleLabel->setAlignment(Qt::AlignCenter);
    m_exceptionHandleLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    connect(m_exceptionHandleLabel,SIGNAL(leftClicked()),this,SLOT(slotExceptionHandleLabelleftClicked()));

    m_materialManageLabel = new HGQLabel();
    m_materialManageLabel->setParent(this);
    m_materialManageLabel->setObjectName("MaterialManage");
    m_materialManageLabel->setText(QString::fromStdString(loadTranslation(m_language,
        m_materialManageLabel->objectName().toStdString())));//"物料管理");
    font=m_materialManageLabel->font();
    font.setPointSize(fontSize);
    m_materialManageLabel->setFont(font);
    m_materialManageLabel->setAlignment(Qt::AlignCenter);
    m_materialManageLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    connect(m_materialManageLabel,SIGNAL(leftClicked()),this,SLOT(slotMaterialManageLabelleftClicked()));

    
    m_deviceLabel->setVisible(true);
    m_mainPageLabel->setVisible(true);
    m_dataLabel->setVisible(true);
    #ifdef ENABLE_CAS6
    m_maintenanceLabel->setVisible(false);
    #else 
    m_maintenanceLabel->setVisible(true); 
    #endif
    fnInitPos();
    switch (m_leftIndex){
        case 0:{
            slotDeviceLabelleftClicked();
            break;
        }
        case 1:{
            slotMainPageLabelleftClicked();
            break;
        }
        case 2:{
            slotDataLabelleftClicked();
            break;
        }
        case 3:{
            slotMainPageLabelleftClicked();
            break;
        }
    }
}
void HGOnlineMainWidget::fnInitPos()
{
    m_quitLabel->move(0.01 * m_width, 0.89 * m_height);
    m_quitLabel->setFixedSize(0.05 * m_width, 0.05 * m_height);
    m_soundLabel->move(0.01 * m_width, 0.94 * m_height);
    m_soundLabel->setFixedSize(0.05 * m_width, 0.05 * m_height);
    m_downloadDBLabel->move(0.01*m_width,0.84*m_height);
    m_downloadDBLabel->setFixedSize(0.05*m_width,0.05*m_height);
    m_reloginLabel->move(0.01*m_width,0.008*m_height);
    m_reloginLabel->setFixedSize(0.03 * m_width, 0.03 * m_height);
    m_curloginLabel->move(0.005*m_width+m_reloginLabel->width(),0.008*m_height);
    m_curloginLabel->setFixedSize(0.03 * m_width, 0.03 * m_height);
    m_titleTimer->setFixedSize(m_width*6/100,m_height*0.05);
    m_titleTimer->move(m_width-m_titleTimer->width(),0);
    m_statusLabel->setFixedSize(m_width*22/100,m_height*0.05);
    m_statusLabel->move(0.71*m_width,0);
    m_topLabel->setFixedSize(m_width*94/100,m_height*0.05);
    m_topLabel->move(0,0);
    m_leftLabel->setFixedSize(m_width*0.08,m_height);
    m_leftLabel->move(0,0);
    if (!m_loginFlag) return;
    m_deviceLabel->setFixedSize(m_width*0.08,m_height*0.1);
    m_deviceLabel->move(0,m_height*0.05);
    m_mainPageLabel->setFixedSize(m_width*0.08,m_height*0.1);
    m_mainPageLabel->move(0,m_topLabel->height()+m_deviceLabel->height());
    m_dataLabel->setFixedSize(m_width*0.08,m_height*0.1);
    m_dataLabel->move(0,m_topLabel->height()+m_deviceLabel->height()+m_mainPageLabel->height());
    m_maintenanceLabel->setFixedSize(m_width*0.08,m_height*0.1);
    m_maintenanceLabel->move(0,m_topLabel->height()+m_deviceLabel->height()+m_mainPageLabel->height()+m_dataLabel->height());
    m_systemSetLabel->setFixedSize(m_width*0.15,m_height*0.05);
    m_systemSetLabel->move(m_mainPageLabel->width(),0);
    m_deviceManageLabel->setFixedSize(m_width*0.15,m_height*0.05);
    m_deviceManageLabel->move(m_mainPageLabel->width()+m_systemSetLabel->width(),0);
    m_channelSetLabel->setFixedSize(m_width*0.15,m_height*0.05);
    m_channelSetLabel->move(m_mainPageLabel->width()+m_systemSetLabel->width()+m_deviceManageLabel->width(),0);
    m_reagentManageLabel->setFixedSize(m_width*0.15,m_height*0.05);
    m_reagentManageLabel->move(m_mainPageLabel->width()+m_systemSetLabel->width()+m_deviceManageLabel->width()+m_channelSetLabel->width(),0);

    m_runningLabel->setFixedSize(m_width*0.15,m_height*0.05);
    m_runningLabel->move(m_mainPageLabel->width(),0);
    m_taskManageLabel->setFixedSize(m_width*0.15,m_height*0.05);
    m_taskManageLabel->move(m_mainPageLabel->width()+m_runningLabel->width(),0);
    m_flowLabel->setFixedSize(m_width*0.15,m_height*0.05);
    m_flowLabel->move(m_mainPageLabel->width()+m_runningLabel->width()+m_taskManageLabel->width(),0);
    m_methodManageLabel->setFixedSize(m_width*0.15,m_height*0.05);
    m_methodManageLabel->move(m_mainPageLabel->width()+m_runningLabel->width()+m_taskManageLabel->width()+m_flowLabel->width(),0);

    m_analysisRecordLabel->setFixedSize(m_width*0.15,m_height*0.05);
    m_analysisRecordLabel->move(m_mainPageLabel->width(),0);
    m_warningLabel->setFixedSize(m_width*0.15,m_height*0.05);
    m_warningLabel->move(m_mainPageLabel->width()+m_analysisRecordLabel->width(),0);
    m_logLabel->setFixedSize(m_width*0.15,m_height*0.05);
    #ifdef ENABLE_CAS6
    m_logLabel->move(m_mainPageLabel->width()+m_analysisRecordLabel->width(),0);
    #else
    m_logLabel->move(m_mainPageLabel->width()+m_analysisRecordLabel->width()+m_warningLabel->width(),0);
    #endif

    m_deviceMaintenanceLabel->setFixedSize(m_width*0.15,m_height*0.05);
    m_deviceMaintenanceLabel->move(m_mainPageLabel->width(),0);
    m_exceptionHandleLabel->setFixedSize(m_width*0.15,m_height*0.05);
    m_exceptionHandleLabel->move(m_mainPageLabel->width()+m_deviceMaintenanceLabel->width(),0);
    m_materialManageLabel->setFixedSize(m_width*0.15,m_height*0.05);
    m_materialManageLabel->move(m_mainPageLabel->width()+m_deviceMaintenanceLabel->width()+m_exceptionHandleLabel->width(),0);

    if (m_systemSetW){
        m_systemSetW->move(m_mainPageLabel->width(), m_topLabel->height());
        m_systemSetW->setFixedSize(m_width-m_mainPageLabel->width(),m_height-m_topLabel->height());
    } else if (m_deviceManageW){
        m_deviceManageW->move(m_mainPageLabel->width(),m_topLabel->height());
        m_deviceManageW->setFixedSize(m_width-m_mainPageLabel->width(),m_height-m_topLabel->height());
    } else if (m_channelSetW){
        m_channelSetW->move(m_mainPageLabel->width(), m_topLabel->height());
        m_channelSetW->setFixedSize(m_width-m_mainPageLabel->width(),m_height-m_topLabel->height());
    } else if (m_reagentManageW){
        m_reagentManageW->move(m_mainPageLabel->width(), m_topLabel->height());
        m_reagentManageW->setFixedSize(m_width-m_mainPageLabel->width(),m_height-m_topLabel->height());
    } else if (m_runningW){
        m_runningW->move(m_mainPageLabel->width(), m_topLabel->height());
        m_runningW->setFixedSize(m_width-m_mainPageLabel->width(),m_height-m_topLabel->height());
    } else if (m_taskManageW){
        m_taskManageW->move(m_mainPageLabel->width(), m_topLabel->height());
        m_taskManageW->setFixedSize(m_width-m_mainPageLabel->width(),m_height-m_topLabel->height());
    } else if (m_flowW){
        m_flowW->move(m_mainPageLabel->width(), m_topLabel->height());
        m_flowW->setFixedSize(m_width-m_mainPageLabel->width(),m_height-m_topLabel->height());
    } else if (m_methodManageW){
        m_methodManageW->move(m_mainPageLabel->width(), m_topLabel->height());
        m_methodManageW->setFixedSize(m_width-m_mainPageLabel->width(),m_height-m_topLabel->height());
    } else if (m_analysisRecordW){
        m_analysisRecordW->move(m_mainPageLabel->width(), m_topLabel->height());
        m_analysisRecordW->setFixedSize(m_width-m_mainPageLabel->width(),m_height-m_topLabel->height());
    } else if (m_warningW){
        m_warningW->move(m_mainPageLabel->width(), m_topLabel->height());
        m_warningW->setFixedSize(m_width-m_mainPageLabel->width(),m_height-m_topLabel->height());
    } else if (m_logW){
        m_logW->move(m_mainPageLabel->width(), m_topLabel->height());
        m_logW->setFixedSize(m_width-m_mainPageLabel->width(),m_height-m_topLabel->height());
    } else if (m_deviceMaintenanceW){
        m_deviceMaintenanceW->move(m_mainPageLabel->width(), m_topLabel->height());
        m_deviceMaintenanceW->setFixedSize(m_width-m_mainPageLabel->width(),m_height-m_topLabel->height());
    } else if (m_materialManageW){
        m_materialManageW->move(m_mainPageLabel->width(), m_topLabel->height());
        m_materialManageW->setFixedSize(m_width-m_mainPageLabel->width(),m_height-m_topLabel->height());
    } else if (m_exceptionHandleW){
        m_exceptionHandleW->move(m_mainPageLabel->width(),m_topLabel->height());
        m_exceptionHandleW->setFixedSize(m_width-m_mainPageLabel->width(),m_height-m_topLabel->height());
    }
}
void HGOnlineMainWidget::fnResize()
{
    int width=getScreenGeometry().width();
    int height=getScreenGeometry().height();
    if (width != m_width || height != m_height) {
        if (width<=0||height<=0) return;
        this->setFixedSize(width, height);
        m_width=this->width();
        m_height=this->height();
        fnInitPos();
    }
}
void HGOnlineMainWidget::fnSlotListenTimerOut()
{
    fnResize();
    // 获取当前时间
    HGExactTime curTimer=HGExactTime::currentTime();
    std::ostringstream curTimeS;
    curTimeS<<"      "<<curTimer.tm_hour<<":"<<curTimer.tm_min<<":"<<curTimer.tm_sec
        <<"\n  "<<curTimer.tm_year<<"/"<<curTimer.tm_mon<<"/"<<curTimer.tm_mday;
    m_titleTimer->setText(QString::fromStdString(curTimeS.str()));
    std::string mcuState=getMCUState();
    printf("mcustate:%s\n",mcuState.c_str());
    m_statusLabel->setText(QString::fromStdString(mcuState));//  "状态：未就绪 缺少信号，请检查仪器");
}
void HGOnlineMainWidget::slotDeviceLabelleftClicked(){
    m_deviceLabel->setStyleSheet("QLabel{background-color:rgb(127,131,247);border:2px solid black;}");
    m_mainPageLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    m_dataLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    m_maintenanceLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    m_systemSetLabel->setVisible(true);
    m_deviceManageLabel->setVisible(true);
    #ifdef ENABLE_CAS6
    m_channelSetLabel->setVisible(false);
    m_reagentManageLabel->setVisible(false);
    #else
    m_channelSetLabel->setVisible(true);
    m_reagentManageLabel->setVisible(true);
    #endif

    m_runningLabel->setVisible(false);
    m_taskManageLabel->setVisible(false);
    m_flowLabel->setVisible(false);
    m_methodManageLabel->setVisible(false);
    m_analysisRecordLabel->setVisible(false);
    m_warningLabel->setVisible(false);
    m_logLabel->setVisible(false);
    m_deviceMaintenanceLabel->setVisible(false);
    m_exceptionHandleLabel->setVisible(false);
    m_materialManageLabel->setVisible(false);

    switch (m_topIndex){
        case 0:
        slotSystemSetLabelleftClicked();
        break;
        case 1:
        slotDeviceManageLabelleftClicked();
        break;
        case 2:
        slotChannelSetLabelleftClicked();
        break;
        case 3:
        slotReagentManageLabelleftClicked();
        break;
    }
    
}
void HGOnlineMainWidget::slotMainPageLabelleftClicked(){
    if (GlobalSingleton::instance().getSystemInfo("AuthorityStatus")!="true") {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), QObject::tr("软件没有授权，或者授权过期!"));
        return;
    }
    m_deviceLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    m_mainPageLabel->setStyleSheet("QLabel{background-color:rgb(127,131,247);border:2px solid black;}");
    m_dataLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    m_maintenanceLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    
    m_systemSetLabel->setVisible(false);
    m_deviceManageLabel->setVisible(false);
    m_channelSetLabel->setVisible(false);
    m_reagentManageLabel->setVisible(false);
    m_runningLabel->setVisible(true);
    m_taskManageLabel->setVisible(true);
    m_flowLabel->setVisible(true);
    m_methodManageLabel->setVisible(true);
    m_analysisRecordLabel->setVisible(false);
    m_warningLabel->setVisible(false);
    m_logLabel->setVisible(false);
    m_deviceMaintenanceLabel->setVisible(false);
    m_exceptionHandleLabel->setVisible(false);
    m_materialManageLabel->setVisible(false);
    switch (m_topIndex){
        case 0:
        slotRunningLabelleftClicked();
        break;
        case 1:
        slotTaskManageLabelleftClicked();
        break;
        case 2:
        slotFlowLabelleftClicked();
        break;
        case 3:
        slotMethodManageLabelleftClicked();
        break;
    }
}
void HGOnlineMainWidget::slotDataLabelleftClicked()
{
    if (GlobalSingleton::instance().getSystemInfo("AuthorityStatus")!="true") {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "软件没有授权，或者授权过期!");
        return;
    }
    m_deviceLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    m_mainPageLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    m_dataLabel->setStyleSheet("QLabel{background-color:rgb(127,131,247);border:2px solid black;}");
    m_maintenanceLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    m_systemSetLabel->setVisible(false);
    m_deviceManageLabel->setVisible(false);
    m_channelSetLabel->setVisible(false);
    m_reagentManageLabel->setVisible(false);
    m_runningLabel->setVisible(false);
    m_taskManageLabel->setVisible(false);
    m_flowLabel->setVisible(false);
    m_methodManageLabel->setVisible(false);
    m_analysisRecordLabel->setVisible(true);
    #ifdef ENABLE_CAS6
    m_warningLabel->setVisible(false);
    #else
    m_warningLabel->setVisible(true);
    #endif
    m_logLabel->setVisible(true);
    m_deviceMaintenanceLabel->setVisible(false);
    m_exceptionHandleLabel->setVisible(false);
    m_materialManageLabel->setVisible(false);
    switch (m_topIndex){
        case 0:
        slotAnalysisRecordLabelleftClicked();
        break;
        case 1:
        slotWarningLabelleftClicked();
        break;
        case 2:
        slotLogLabelleftClicked();
        break;
    }
}
void HGOnlineMainWidget::slotMaintenanceLabelleftClicked()
{
    if (GlobalSingleton::instance().getSystemInfo("AuthorityStatus")!="true") {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "软件没有授权，或者授权过期!");
        return;
    }
    m_deviceLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    m_mainPageLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    m_dataLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    m_maintenanceLabel->setStyleSheet("QLabel{background-color:rgb(127,131,247);border:2px solid black;}");
    m_systemSetLabel->setVisible(false);
    m_deviceManageLabel->setVisible(false);
    m_channelSetLabel->setVisible(false);
    m_reagentManageLabel->setVisible(false);
    m_runningLabel->setVisible(false);
    m_taskManageLabel->setVisible(false);
    m_flowLabel->setVisible(false);
    m_methodManageLabel->setVisible(false);
    m_analysisRecordLabel->setVisible(false);
    m_warningLabel->setVisible(false);
    m_logLabel->setVisible(false);
    m_deviceMaintenanceLabel->setVisible(true);
    m_exceptionHandleLabel->setVisible(true);
    m_materialManageLabel->setVisible(true);
    switch (m_topIndex){
        case 0:
        slotDeviceMaintenanceLabelleftClicked();
        break;
        case 1:
        slotExceptionHandleLabelleftClicked();
        break;
        case 2:
        slotMaterialManageLabelleftClicked();
        break;
    }
}
void HGOnlineMainWidget::slotSystemSetLabelleftClicked()
{
    m_systemSetLabel->setStyleSheet("QLabel{background-color:rgb(184,134,248);border:2px solid black;}");
    m_deviceManageLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    m_channelSetLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    m_reagentManageLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");

    fnCloseWindow();
    m_systemSetW = new HGSystemSetWidget(m_language,this);
    connect(m_systemSetW,&HGSystemSetWidget::signalSoundVolumn,
        this,&HGOnlineMainWidget::slotUpdateSoundVolumn);
    connect(m_systemSetW,&HGSystemSetWidget::updateLanguage,
        this,&HGOnlineMainWidget::slotUpdateLanguage);

    m_systemSetW->move(m_mainPageLabel->width(), m_topLabel->height());
    m_systemSetW->setFixedSize(m_width-m_mainPageLabel->width(),m_height-m_topLabel->height());
    
    m_systemSetW->show();
}
void HGOnlineMainWidget::slotDeviceManageLabelleftClicked()
{
    if (GlobalSingleton::instance().getSystemInfo("AuthorityStatus")!="true") {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "软件没有授权，或者授权过期!");
        return;
    }
    // judge is online or offline
    m_systemSetLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    m_deviceManageLabel->setStyleSheet("QLabel{background-color:rgb(184,134,248);border:2px solid black;}");
    m_channelSetLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    m_reagentManageLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    
    fnCloseWindow();
    // if (!isPermitted(HGOnlineRWDB::readAuthorityInfo(), HGOnlineRWDB::readLoginName(), m_deviceManageLabel->objectName().toStdString()))
    // {
    //     QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "无此权限，请联系管理员开通！");
    //     return;
    // }
    m_deviceManageW = new HGDeviceManageWidget(m_language,this);
    m_deviceManageW->move(m_mainPageLabel->width(),m_topLabel->height());
    m_deviceManageW->setFixedSize(m_width-m_mainPageLabel->width(),m_height-m_topLabel->height());
    m_deviceManageW->show();
}
void HGOnlineMainWidget::slotChannelSetLabelleftClicked()
{
    if (GlobalSingleton::instance().getSystemInfo("AuthorityStatus")!="true") {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "软件没有授权，或者授权过期!");
        return;
    }
    m_systemSetLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    m_deviceManageLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    m_channelSetLabel->setStyleSheet("QLabel{background-color:rgb(184,134,248);border:2px solid black;}");
    m_reagentManageLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    
    fnCloseWindow();
    // if (!isPermitted(HGOnlineRWDB::readAuthorityInfo(),HGOnlineRWDB::readLoginName(),m_channelSetLabel->objectName().toStdString())){
    //     QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "无此权限，请联系管理员开通！");
    //     return;
    // }
    m_channelSetW = new HGChannelSetWidget(m_language,this);
    m_channelSetW->move(m_mainPageLabel->width(), m_topLabel->height());
    m_channelSetW->setFixedSize(m_width-m_mainPageLabel->width(),m_height-m_topLabel->height());
    m_channelSetW->show();
}
void HGOnlineMainWidget::slotReagentManageLabelleftClicked()
{
    if (GlobalSingleton::instance().getSystemInfo("AuthorityStatus")!="true") {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "软件没有授权，或者授权过期!");
        return;
    }
    m_systemSetLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    m_deviceManageLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    m_channelSetLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    m_reagentManageLabel->setStyleSheet("QLabel{background-color:rgb(184,134,248);border:2px solid black;}");
    
    fnCloseWindow();
    // if (!isPermitted(HGOnlineRWDB::readAuthorityInfo(),HGOnlineRWDB::readLoginName(),m_reagentManageLabel->objectName().toStdString()))
    // {
    //     QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "无此权限，请联系管理员开通！");
    //     return;
    // }
    m_reagentManageW = new HGReagentManageWidget(m_language,this);
    m_reagentManageW->move(m_mainPageLabel->width(), m_topLabel->height());
    m_reagentManageW->setFixedSize(m_width-m_mainPageLabel->width(),m_height-m_topLabel->height());
    m_reagentManageW->show();
}

void HGOnlineMainWidget::slotUpdateSoundVolumn(int volumn)
{
    if (volumn==0){
        m_soundLabel->changePixmap(getPath("/resources/V1/@sound-close-main.png"),55);
    } else {
        m_soundLabel->changePixmap(getPath("/resources/V1/@sound-open-main.png"),55);
    }
}
void HGOnlineMainWidget::slotUpdateLanguage(std::string lang)
{
    m_language=lang;
    m_deviceLabel->setText(QString::fromStdString(loadTranslation(m_language, "Device"/*"设备"*/)));
    m_mainPageLabel->setText(QString::fromStdString(loadTranslation(m_language,"MainPage")));
    m_dataLabel->setText(QString::fromStdString(loadTranslation(m_language,"Data"))); 
    m_maintenanceLabel->setText(QString::fromStdString(loadTranslation(m_language,"Maintenance")));
    m_systemSetLabel->setText(QString::fromStdString(loadTranslation(m_language,"SystemManage")));
    m_deviceManageLabel->setText(QString::fromStdString(loadTranslation(m_language,"DeviceManage")));
    m_channelSetLabel->setText(QString::fromStdString(loadTranslation(m_language,"ChannelSet")));
    m_reagentManageLabel->setText(QString::fromStdString(loadTranslation(m_language,"ReagentManage")));
    m_runningLabel->setText(QString::fromStdString(loadTranslation(m_language,"RunningStatus")));
    m_taskManageLabel->setText(QString::fromStdString(loadTranslation(m_language,"TaskSequence")));
    m_flowLabel->setText(QString::fromStdString(loadTranslation(m_language,"Flow")));
    m_methodManageLabel->setText(QString::fromStdString(loadTranslation(m_language,"Method")));
    m_analysisRecordLabel->setText(QString::fromStdString(loadTranslation(m_language,"AnalysisRecord")));
    m_warningLabel->setText(QString::fromStdString(loadTranslation(m_language,"WarningInfo")));
    m_logLabel->setText(QString::fromStdString(loadTranslation(m_language,"Log")));
    m_deviceMaintenanceLabel->setText(QString::fromStdString(loadTranslation(m_language,"DeviceMaintenance")));
    m_exceptionHandleLabel->setText(QString::fromStdString(loadTranslation(m_language,"ExceptionHandle")));
    m_materialManageLabel->setText(QString::fromStdString(loadTranslation(m_language,"MaterialManage")));


}
void HGOnlineMainWidget::clickRelogin(){
    if (!m_loginFlag) return;
    fnCloseWindow();
    m_loginW = new HGLoginWidget(m_language,this);
    m_loginW->move(0, 0);
    m_loginW->setFixedSize(m_width, m_height);
    m_loginW->show();

    std::string loginName=GlobalSingleton::instance().getSystemInfo("loginName");
    int index=GlobalSingleton::instance().getUserFieldIndex(loginName);
    GlobalSingleton::instance().addUserField(index,"QuitTime",getStandardCurTime()+';');
    RWDb::writeAuditTrailLog(loginName+" quit");

    qRegisterMetaType<UserInfoS>("UserInfoS");
    connect(m_loginW, SIGNAL(loginOn(UserInfoS)), this, SLOT(slotLoginOn(UserInfoS)));
}
void HGOnlineMainWidget::clickCurlogin()
{
    if (!m_loginFlag) return;
    std::string userName=(m_userInfoS.username==""?  \
        loadTranslation(m_language,"NoPassLogin") : 
        (loadTranslation(m_language,"UserNo")+"["+m_userInfoS.userno+"]," \
        +loadTranslation(m_language,"UserName")+"["+m_userInfoS.username+"]," \
        +loadTranslation(m_language,"Authority")+"["+loadTranslation(m_language, m_userInfoS.authority)+"]"));
    QMessageBox::information(this, QString::fromStdString(HG_DEVICE_NAME), 
        "当前用户："+QString::fromStdString(userName));
}
void HGOnlineMainWidget::clickDownloadDB(){
    QDialog dialog(this);
    dialog.setWindowTitle(QString::fromStdString(loadTranslation(m_language,"SelectDBFile")));//"选择要录入的db文件");
    dialog.setWindowModality(Qt::ApplicationModal);
    QString dbFileName;
    QLabel* title=new QLabel(QString::fromStdString(loadTranslation(m_language,"SelectDBFileToCopy")));//"选择下列哪些表需要复制");
    QLabel* titleRight=new QLabel(QString::fromStdString(loadTranslation(m_language,"MigrateDBFile")));//"需要移植的表");
    QListWidget* listW=new QListWidget(&dialog);
    QPushButton* insertBtn=new QPushButton(&dialog);
    insertBtn->setText(">>");
    QPushButton* backBtn=new QPushButton(&dialog);
    backBtn->setText("<<");
    QListWidget* listWRight=new QListWidget(&dialog);
    QPushButton* searchbtn=new QPushButton(&dialog);
    searchbtn->setText(QString::fromStdString(loadTranslation(m_language,"Search")));//"浏览");
    QPushButton* okbtn=new QPushButton(&dialog);
    okbtn->setText(QString::fromStdString(loadTranslation(m_language,"Ok")));//"确定");
    QPushButton* cancelbtn=new QPushButton(&dialog);
    cancelbtn->setText(QString::fromStdString(loadTranslation(m_language,"Cancel")));//"取消");
    connect(searchbtn,&QPushButton::clicked,[&](){
        listW->clear();
        listWRight->clear();
        
        dbFileName=QFileDialog::getOpenFileName(NULL,"选择要录入的db文件","./",tr("db files(*.db);;All files(*.*)"));
        HGSaveDataToDB dbOperatorTemp;
        dbOperatorTemp.openDB(dbFileName.toStdString());
        std::vector<std::string> allTableNames=dbOperatorTemp.getAllTables(/*dbFileName.toStdString()*/);
        for (const auto &name:allTableNames){
            if (name.find("Channel_")!=std::string::npos
                ||name.find("Reagent_")!=std::string::npos
                ||name.find("Flow_")!=std::string::npos
                ||name.find("Method_")!=std::string::npos
                ||name.find("TaskSequence_")!=std::string::npos)
            {
                if (name.find("ReagentDevice")!=std::string::npos) continue;
                listW->addItem(QString::fromStdString(name));
            }
        }
        dbOperatorTemp.closeDB();
    });
    connect(okbtn,&QPushButton::clicked,[&](){
        if (listWRight->count()>0) {
            if (QMessageBox::Ok==QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "确定要复制这些表吗?")){
                for (int i=0;i<listWRight->count();i++){
                    QListWidgetItem* item = listWRight->item(i);
                    std::string tableName=item->text().toStdString();
                    HGOnlineRWDB::copyTable(dbFileName.toStdString(),
                        HGOnlineRWDB::readCurDirPath()+DB_PATH,tableName);
                }
            
                dialog.close();
            }
        }
    });
    connect(cancelbtn,&QPushButton::clicked,[&](){
        dialog.close();
    });
    connect(insertBtn,&QPushButton::clicked,[&](){
        int index=listW->currentRow();
        if (index>=0){
            QString curStr=listW->currentItem()->text();
            listW->takeItem(index);
            listWRight->addItem(curStr);
        }
    });
    connect(backBtn,&QPushButton::clicked,[&](){
        int index=listWRight->currentRow();
        if (index >=0){
            QString curStr=listWRight->currentItem()->text();
            listW->addItem(curStr);
            listWRight->takeItem(index);
        }
    });
    // std::vector<std::string> names=HGOnlineRWDB::getAllTables(TaskSequenceDBName);
    // for (const auto &name:names){
    //     listW->addItem(QString::fromStdString(name));
    // }
    // setFocusByName(listW,HgOnlinePlatformModule::getTaskSeqName());

    QGridLayout *layout=new QGridLayout(&dialog);
    layout->addWidget(title,0,0);
    layout->addWidget(titleRight,0,2);
    layout->addWidget(listW,1,0,3,1);
    layout->addWidget(insertBtn,1,1);
    layout->addWidget(backBtn,2,1);
    layout->addWidget(listWRight,1,2,3,1);
    layout->addWidget(searchbtn,6,0);
    layout->addWidget(okbtn,6,1);
    layout->addWidget(cancelbtn,6,2);
    dialog.setLayout(layout);
    dialog.exec();
}
void HGOnlineMainWidget::clickSound()
{
    m_soundFlag=!m_soundFlag;
    if (!m_soundFlag){
        m_soundLabel->changePixmap(getPath("/resources/V1/@sound-close-main.png"),55);
    } else {
        m_soundLabel->changePixmap(getPath("/resources/V1/@sound-open-main.png"),55);
    }
    bool setflag=false;
    if (m_systemSetW){
        if (m_systemSetW->isToolBoxNull()){
            setflag=false;
        } else {
            setflag=true;
        }
    } else {
        setflag=false;
    }
    if (!setflag){
        GlobalSingleton::instance().setSystemInfo("声音value", m_soundFlag?"50":"0");
    }  else  m_systemSetW->setWarnSoundVolumnMute(!m_soundFlag);
}
void HGOnlineMainWidget::clickEscape()
{
    stopThread();
    HGLog4Cplus::getLogInstance(LOG_PATH)->logout("quit HG online platform app",LOGINFO);
    fnCloseWindow();
    printf("close window\n");
    saveConfig();
    printf("save config\n");
    RWDb::resetTaskRunStatus(HgOnlinePlatformModule::getTaskSeqName());
    printf("reset task run status");
    RWDb::resetTaskRunFlag();
    printf("reset task run flag");
    HGOnlineRWDB::setDeviceFromLPCManageOffline();
    printf("set device from lpc manage offline");
    RWDb::setTaskRunRecordDataDB("");

    GlobalSingleton::instance().setSystemInfo("lastLoginTime", getStandardCurTime());

    std::string loginName=GlobalSingleton::instance().getSystemInfo("loginName");
    int index = GlobalSingleton::instance().getUserFieldIndex(loginName);
    GlobalSingleton::instance().addUserField(index,"QuitTime",getStandardCurTime()+';');
    RWDb::writeAuditTrailLog(GlobalSingleton::instance().getSystemInfo("loginName")+" quit");
    GlobalSingleton::instance().setSystemInfo("loginName", "");
    GlobalSingleton::instance().setSystemInfo("authority","");
    GlobalSingleton::instance().saveAll();
    closeDB();
    remove(PARENT_PID_FILE);  // 退出时删除父进程 PID 文件
    this->close();
}

void HGOnlineMainWidget::slotRunningLabelleftClicked()
{
    m_runningLabel->setStyleSheet("QLabel{background-color:rgb(184,134,248);border:2px solid black;}");
    m_taskManageLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    m_flowLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    m_methodManageLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    
    fnCloseWindow();
    // if (!isPermitted(HGOnlineRWDB::readAuthorityInfo(),HGOnlineRWDB::readLoginName(),m_runningLabel->objectName().toStdString())){
    //     QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "无此权限，请联系管理员开通！");
    //     return;
    // }
    m_runningW = new HGRunningWidget(m_language,this);
    m_runningW->move(m_mainPageLabel->width(), m_topLabel->height());
    m_runningW->setFixedSize(m_width-m_mainPageLabel->width(),m_height-m_topLabel->height());
    m_runningW->show();
    // connect(m_runningW,SIGNAL(signalRunning()),this,SLOT(slotRunningTaskSequence()));
}
void HGOnlineMainWidget::slotRunningTaskSequence(){
    #ifdef ENABLE_CAS6
    m_runningTaskSequence=true;
    
    #else

    #endif
}
void HGOnlineMainWidget::slotTaskManageLabelleftClicked()
{
    m_runningLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    m_taskManageLabel->setStyleSheet("QLabel{background-color:rgb(184,134,248);border:2px solid black;}");
    m_flowLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    m_methodManageLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    
    fnCloseWindow();
    // if (!isPermitted(HGOnlineRWDB::readAuthorityInfo(),HGOnlineRWDB::readLoginName(),m_taskManageLabel->objectName().toStdString())){
    //     QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "无此权限，请联系管理员开通！");
    //     return;
    // }
    m_taskManageW = new HGTaskManageWidget(m_language,HgOnlinePlatformModule::getTaskSeqName(), this);
    m_taskManageW->move(m_mainPageLabel->width(), m_topLabel->height());
    m_taskManageW->setFixedSize(m_width-m_mainPageLabel->width(),m_height-m_topLabel->height());
    m_taskManageW->show();
}
void HGOnlineMainWidget::slotFlowLabelleftClicked()
{
    m_runningLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    m_taskManageLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    m_flowLabel->setStyleSheet("QLabel{background-color:rgb(184,134,248);border:2px solid black;}");
    m_methodManageLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    
    fnCloseWindow();
    // if (!isPermitted(HGOnlineRWDB::readAuthorityInfo(),HGOnlineRWDB::readLoginName(),m_flowLabel->objectName().toStdString())){
    //     QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "无此权限，请联系管理员开通！");
    //     return;
    // }
    m_flowW = new HGFlowWidget(m_language,this);
    m_flowW->move(m_mainPageLabel->width(), m_topLabel->height());
    m_flowW->setFixedSize(m_width-m_mainPageLabel->width(),m_height-m_topLabel->height());
    m_flowW->show();
}
void HGOnlineMainWidget::slotMethodManageLabelleftClicked()
{
    m_runningLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    m_taskManageLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    m_flowLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    m_methodManageLabel->setStyleSheet("QLabel{background-color:rgb(184,134,248);border:2px solid black;}");
    
    fnCloseWindow();
    // if (!isPermitted(HGOnlineRWDB::readAuthorityInfo(),HGOnlineRWDB::readLoginName(),m_methodManageLabel->objectName().toStdString())){
    //     QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "无此权限，请联系管理员开通！");
    //     return;
    // }
    m_methodManageW = new HGMethodManageWidget(m_language,this);
    m_methodManageW->move(m_mainPageLabel->width(), m_topLabel->height());
    m_methodManageW->setFixedSize(m_width-m_mainPageLabel->width(),m_height-m_topLabel->height());
    m_methodManageW->show();
}
void HGOnlineMainWidget::slotAnalysisRecordLabelleftClicked(){
    m_analysisRecordLabel->setStyleSheet("QLabel{background-color:rgb(184,134,248);border:2px solid black;}");
    m_warningLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    m_logLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    fnCloseWindow();
    // if (!isPermitted(HGOnlineRWDB::readAuthorityInfo(),HGOnlineRWDB::readLoginName(),m_analysisRecordLabel->objectName().toStdString())){
    //     QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "无此权限，请联系管理员开通！");
    //     return;
    // }
    
    m_analysisRecordW = new HGAnalysisRecordWidget(m_language,this);
    m_analysisRecordW->move(m_mainPageLabel->width(), m_topLabel->height());
    m_analysisRecordW->setFixedSize(m_width-m_mainPageLabel->width(),m_height-m_topLabel->height());
    m_analysisRecordW->show();
}
void HGOnlineMainWidget::slotWarningLabelleftClicked(){
    m_analysisRecordLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    m_warningLabel->setStyleSheet("QLabel{background-color:rgb(184,134,248);border:2px solid black;}");
    m_logLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    
    fnCloseWindow();
    // if (!isPermitted(HGOnlineRWDB::readAuthorityInfo(),HGOnlineRWDB::readLoginName(),m_warningLabel->objectName().toStdString())){
    //     QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "无此权限，请联系管理员开通！");
    //     return;
    // }
    m_warningW = new HGErrorCodeWidget(m_language,this);
    connect(m_warningW,SIGNAL(signalJumpToExceptionPage()),this,SLOT(slotJumpToExceptionPage()));
    m_warningW->move(m_mainPageLabel->width(), m_topLabel->height());
    m_warningW->setFixedSize(m_width-m_mainPageLabel->width(),m_height-m_topLabel->height());
    m_warningW->show();
}
void HGOnlineMainWidget::slotLogLabelleftClicked(){
    m_analysisRecordLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    m_warningLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    m_logLabel->setStyleSheet("QLabel{background-color:rgb(184,134,248);border:2px solid black;}");
    
    fnCloseWindow();
    // if (!isPermitted(HGOnlineRWDB::readAuthorityInfo(),HGOnlineRWDB::readLoginName(),m_logLabel->objectName().toStdString())){
    //     QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "无此权限，请联系管理员开通！");
    //     return;
    // }
    m_logW = new HGLogWidget(m_language,this);
    m_logW->move(m_mainPageLabel->width(), m_topLabel->height());
    m_logW->setFixedSize(m_width-m_mainPageLabel->width(),m_height-m_topLabel->height());
    m_logW->show();
}
void HGOnlineMainWidget::slotDeviceMaintenanceLabelleftClicked(){
    m_deviceMaintenanceLabel->setStyleSheet("QLabel{background-color:rgb(184,134,248);border:2px solid black;}");
    m_exceptionHandleLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    m_materialManageLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    
    fnCloseWindow();
    // if (!isPermitted(HGOnlineRWDB::readAuthorityInfo(),HGOnlineRWDB::readLoginName(),m_deviceMaintenanceLabel->objectName().toStdString())){
    //     QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "无此权限，请联系管理员开通！");
    //     return;
    // }
    m_deviceMaintenanceW = new HGDeviceMaintenanceWidget(m_language,this);
    m_deviceMaintenanceW->move(m_mainPageLabel->width(), m_topLabel->height());
    m_deviceMaintenanceW->setFixedSize(m_width-m_mainPageLabel->width(),m_height-m_topLabel->height());
    m_deviceMaintenanceW->show();
}
void HGOnlineMainWidget::slotJumpToExceptionPage(){
    #ifdef ENABLE_CAS6

    #else
    slotMaintenanceLabelleftClicked();
    slotExceptionHandleLabelleftClicked();
    #endif
}
void HGOnlineMainWidget::slotExceptionHandleLabelleftClicked(){
    m_deviceMaintenanceLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    m_exceptionHandleLabel->setStyleSheet("QLabel{background-color:rgb(184,134,248);border:2px solid black;}");
    m_materialManageLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    
    fnCloseWindow();
    // if (!isPermitted(HGOnlineRWDB::readAuthorityInfo(),HGOnlineRWDB::readLoginName(),m_exceptionHandleLabel->objectName().toStdString())){
    //     QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "无此权限，请联系管理员开通！");
    //     return;
    // }
    m_exceptionHandleW = new HGExceptionHandleWidget(m_language,this);
    m_exceptionHandleW->move(m_mainPageLabel->width(), m_topLabel->height());
    m_exceptionHandleW->setFixedSize(m_width-m_mainPageLabel->width(),m_height-m_topLabel->height());
    m_exceptionHandleW->show();
}
void HGOnlineMainWidget::slotMaterialManageLabelleftClicked(){
    m_deviceMaintenanceLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    m_exceptionHandleLabel->setStyleSheet("QLabel{background-color:rgb(147,210,243);border:2px solid black;}");
    m_materialManageLabel->setStyleSheet("QLabel{background-color:rgb(184,134,248);border:2px solid black;}");
    
    fnCloseWindow();
    // if (!isPermitted(HGOnlineRWDB::readAuthorityInfo(),HGOnlineRWDB::readLoginName(),m_materialManageLabel->objectName().toStdString())){
    //     QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "无此权限，请联系管理员开通！");
    //     return;
    // }
    m_materialManageW = new HGMaterialManageWidget(m_loginAuthority,m_language,this);
    m_materialManageW->move(m_mainPageLabel->width(), m_topLabel->height());
    m_materialManageW->setFixedSize(m_width-m_mainPageLabel->width(),m_height-m_topLabel->height());
    m_materialManageW->show();
}
void HGOnlineMainWidget::fnCloseWindow()
{
    if (m_systemSetW){
        if (m_systemSetW->closeWindow()){
            SAFE_DELETE(m_systemSetW);
        } 
    } else if (m_deviceManageW){
        if (m_deviceManageW->closeWindow()){
            SAFE_DELETE(m_deviceManageW);
        }
    } else if (m_channelSetW){
        if (m_channelSetW->closeWindow()){
            SAFE_DELETE(m_channelSetW);
        }
    } else if (m_reagentManageW){
        if (m_reagentManageW->closeWindow()){
            SAFE_DELETE(m_reagentManageW);
        }
    } else if (m_runningW)
    {
        if (m_runningW->closeWindow())
        {
            SAFE_DELETE(m_runningW);
        }
    } else if (m_taskManageW)
    {
        if (m_taskManageW->closeWindow())
        {
            SAFE_DELETE(m_taskManageW);
        }
    } else if (m_flowW)
    {
        if (m_flowW->closeWindow())
        {
            SAFE_DELETE(m_flowW);
        }
    } else if (m_methodManageW)
    {
        if (m_methodManageW->closeWindow())
        {
            SAFE_DELETE(m_methodManageW);
        }
    } else if (m_analysisRecordW)
    {
        if (m_analysisRecordW->closeWindow())
        {
            SAFE_DELETE(m_analysisRecordW);
        }
    } else if (m_warningW)
    {
        if (m_warningW->closeWindow())
        {
            SAFE_DELETE(m_warningW);
        }
    } else if (m_logW)
    {
        if (m_logW->closeWindow())
        {
            SAFE_DELETE(m_logW);
        }
    } else if (m_deviceMaintenanceW)
    {
        if (m_deviceMaintenanceW->closeWindow())
        {
            SAFE_DELETE(m_deviceMaintenanceW);
        }
    } else if (m_exceptionHandleW){
        if (m_exceptionHandleW->closeWindow()){
            SAFE_DELETE(m_exceptionHandleW);
        }
    } else if (m_materialManageW)
    {
        if (m_materialManageW->closeWindow())
        {
            SAFE_DELETE(m_materialManageW);
        }
    } else if (m_loginW){
        if (m_loginW->closeWindow()){
            SAFE_DELETE(m_loginW);
        }
    }
}

void HGOnlineMainWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->key()==Qt::Key_Escape){
        clickEscape();
    } else {
        QWidget::keyPressEvent(event);
    }
}