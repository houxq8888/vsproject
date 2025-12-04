#include "hgsystemsetwidget.h"
#include <QMessageBox>
#include <QDebug>
#include "common.h"
#include "myToolBox.h"


HGSystemSetWidget::HGSystemSetWidget(std::string lang,QWidget *parent) : QWidget(parent),
    // m_loginAuthority(HGOnlineRWDB::readLoginAuthority()),
    m_lang(lang),
    m_userListW(nullptr)
{
    RWDb::writeAuditTrailLog(loadTranslation(m_lang,"Enter")+loadTranslation(m_lang,"SystemManage"));

    permissionInfo = GlobalSingleton::instance().getAuthorityDetail();
    
    m_isNoPwdLogin=false;
    m_trackLogFlag=false;
    m_debugModeFlag=false;

    if (GlobalSingleton::instance().getSystemInfo("免密登录")=="true") m_isNoPwdLogin=true;
    else m_isNoPwdLogin=false;
    if (GlobalSingleton::instance().getSystemInfo("debug")=="true") m_debugModeFlag=true;
    else m_debugModeFlag=false;
    if (GlobalSingleton::instance().getSystemInfo("track")=="true") m_trackLogFlag=true;
    else m_trackLogFlag=false;

    m_toolBox=nullptr;
    m_debugModeWarnLabel=NULL;
    m_trackLogWarnLabel=NULL;
    m_lockLabel=NULL;
    m_hline=NULL;
    m_hline1=NULL;
    m_deviceInfoW=NULL;
    m_softwareInfoW=NULL;
    m_userRegisterW=NULL;
    m_serviceInfoW=NULL;
    m_loginStatusLabel=NULL;
    m_trackLogLabel=NULL;
    m_debugModelLabel=NULL;
    m_choice = SYSTEMSET_DEVICEINFO;
    m_layout= new QGridLayout();
    this->setLayout(m_layout);

    m_inputEdit=new QLineEdit();
    m_inputEdit->setPlaceholderText(QString::fromStdString(loadTranslation(m_lang,"Input"))); //"请输入");
    m_searchLabel=new HGQLabel(false,getPath("/resources/V1/@1xmb-search 1.png"));

    m_deviceInfoLabel=new LabelWithImg(IMGLEFT,20,getPath("/resources/V1/@1xarcoDesign-home 1.png"),
        (loadTranslation(m_lang,"DeviceInfo")));//"设备信息");
    m_systemSetLabel=new LabelWithImg(IMGLEFT,20,getPath("/resources/V1/@1xze-setting-o 1.png"),
        (loadTranslation(m_lang,"SystemSet")));//"系统设置");
    m_searchBeforeLabel=new LabelWithImg(IMGLEFT,20,getPath("/resources/V1/@1xze-friends 1.png"),
        (loadTranslation(m_lang,"Date from")));//"追溯管理");
    m_serviceInfoLabel=new LabelWithImg(IMGLEFT,20,getPath("/resources/V1/@1xiconPark-picture-one 1.png"),
        (loadTranslation(m_lang,"ServiceInfo")));//"服务信息");
    connect(m_deviceInfoLabel,SIGNAL(clickLeftButton()),this,SLOT(slotClickDeviceInfo()));
    connect(m_systemSetLabel,SIGNAL(clickLeftButton()),this,SLOT(slotClickSystemSet()));
    connect(m_searchBeforeLabel,SIGNAL(clickLeftButton()),this,SLOT(slotClickSearchBefore()));
    connect(m_serviceInfoLabel,SIGNAL(clickLeftButton()),this,SLOT(slotClickDServiceInfo()));

    m_vline=new QFrame;
    m_vline->setFrameShape(QFrame::VLine);
    m_vline->setFrameShadow(QFrame::Sunken);

    
    m_layout->addWidget(m_inputEdit,0,0);
    m_layout->addWidget(m_searchLabel,0,1);
    m_layout->addWidget(m_deviceInfoLabel,1,0);
    m_layout->addWidget(m_systemSetLabel,2,0);
    m_layout->addWidget(m_searchBeforeLabel,3,0);
    m_layout->addWidget(m_serviceInfoLabel,4,0);
    m_layout->addWidget(m_vline,0,2,12,1);
    switch (m_choice){
        case SYSTEMSET_DEVICEINFO:
        {
            slotClickDeviceInfo();
            break;
        }    
        case SYSTEMSET_SYSTEMSET:
        {
            slotClickSystemSet();
            break;
        }
        case SYSTEMSET_SEARCH:
        {
            slotClickSearchBefore();
            break;
        }
        case SYSTEMSET_SERVICE:
        {
            slotClickDServiceInfo();
            break;
        }
    }

    m_layout->setColumnStretch(0,1);
    m_layout->setColumnStretch(1,1);
    m_layout->setColumnStretch(2,1);
    m_layout->setColumnStretch(3,10);

}

bool HGSystemSetWidget::closeWindow()
{
    if (m_toolBox){
        if (m_toolBox->closeWindow()){
            SAFE_DELETE(m_toolBox);
        }
    }
    if (m_deviceInfoW){
        if (m_deviceInfoW->closeWindow()){
            SAFE_DELETE(m_deviceInfoW);
        } 
    } 
    if (m_softwareInfoW){
        if (m_softwareInfoW->closeWindow()){
            SAFE_DELETE(m_softwareInfoW);
        } 
    }
    if (m_userRegisterW){
        if (m_userRegisterW->closeWindow()){
            SAFE_DELETE(m_userRegisterW);
        }
    }
    if (m_userListW){
        if (m_userListW->closeWindow()){
            SAFE_DELETE(m_userListW);
        }
    }
    if (m_serviceInfoW){
        if (m_serviceInfoW->closeWindow()){
            SAFE_DELETE(m_serviceInfoW);
        }
    }
    {
        if (m_isNoPwdLogin) {
            GlobalSingleton::instance().setSystemInfo("免密登录", "true");
        }
        else GlobalSingleton::instance().setSystemInfo("免密登录", "false");
        if (m_debugModeFlag) GlobalSingleton::instance().setSystemInfo("debug","true");
        else GlobalSingleton::instance().setSystemInfo("debug","false");
        if (m_trackLogFlag) GlobalSingleton::instance().setSystemInfo("track","true");
        else GlobalSingleton::instance().setSystemInfo("track","false");
        GlobalSingleton::instance().saveSystemInfo();
        SAFE_DELETE(m_loginStatusLabel);
        SAFE_DELETE(m_debugModelLabel);
        SAFE_DELETE(m_trackLogLabel);
        SAFE_DELETE(m_debugModeWarnLabel);
        SAFE_DELETE(m_trackLogWarnLabel);
        SAFE_DELETE(m_lockLabel);
        SAFE_DELETE(m_hline);
        SAFE_DELETE(m_hline1);
    }
    
    return true;
}
HGSystemSetWidget::~HGSystemSetWidget()
{
    SAFE_DELETE(m_deviceInfoW);
    SAFE_DELETE(m_softwareInfoW);
    SAFE_DELETE(m_toolBox);
    SAFE_DELETE(m_userRegisterW);
    SAFE_DELETE(m_userListW);
    SAFE_DELETE(m_serviceInfoW);
}
void HGSystemSetWidget::slotClickDeviceInfo(){
    closeWindow();
    std::string text=m_deviceInfoLabel->getTextLabel()->text().toStdString();
    std::string name=findTranslationKey(m_lang, text);
    // if (!isPermitted(HGOnlineRWDB::readAuthorityInfo(),HGOnlineRWDB::readLoginName(),name)){
    //     QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "无此权限，请联系管理员开通！");
    //     return;
    // }
    m_deviceInfoW=new HGDeviceInfoWidget(m_lang,this);
    m_softwareInfoW=new HGSoftwareInfoWidget(m_lang,this);
    m_layout->addWidget(m_deviceInfoW,0,3,2,1);
    m_layout->addWidget(m_softwareInfoW,3,3,3,1);
    m_deviceInfoLabel->getTextLabel()->setStyleSheet("QLabel{background-color:rgb(154,154,154);}");
    m_systemSetLabel->getTextLabel()->setStyleSheet("QLabel{background-color:rgb(239,239,239);}");
    m_searchBeforeLabel->getTextLabel()->setStyleSheet("QLabel{background-color:rgb(239,239,239);}");
    m_serviceInfoLabel->getTextLabel()->setStyleSheet("QLabel{background-color:rgb(239,239,239);}");
    
}
bool HGSystemSetWidget::isToolBoxNull()
{
    return (m_toolBox==nullptr?true:false);
}

void HGSystemSetWidget::slotClickSystemSet(){
    if (GlobalSingleton::instance().getSystemInfo("AuthorityStatus")!="true") {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "软件没有授权，或者授权过期!");
        return;
    }

    closeWindow();

    std::string name=findTranslationKey(m_lang, m_systemSetLabel->getTextLabel()->text().toStdString());
    // if (!isPermitted(HGOnlineRWDB::readAuthorityInfo(),HGOnlineRWDB::readLoginName(),name)){
    //     QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "无此权限，请联系管理员开通！");
    //     return;
    // }

    m_toolBox = new MyToolBox(this);
    m_toolBox->addWidget(m_lang,"DateAndTime",new DateAndTimeWidget(m_lang));
    m_toolBox->addWidget(m_lang,"DisplayAndBrightness", new DisplayAndLightWidget(m_lang));
    m_toolBox->addWidget(m_lang,"Language", new LanguageWidget());
    m_toolBox->addWidget(m_lang,"WarningSet", new WarningWidget(m_lang));
    m_toolBox->addWidget(m_lang,"Internet", new NetworkWidget(m_lang));
    
    connect(m_toolBox,&MyToolBox::signalSoundVolumn,[=](int volumn){
        emit signalSoundVolumn(volumn);
    });
    connect(m_toolBox,&MyToolBox::updateLanguage,[=](std::string lang){
        m_lang=lang;
        WarningWidget* widget=dynamic_cast<WarningWidget*>(m_toolBox->getWidget("WarningSet"));
        if (widget) {
            // 安全调用
            widget->setLanguage(m_lang);
        } else {
            printf("basePtr is not pointing to a Derived object.\n");
        }
        DateAndTimeWidget *widgetDate = dynamic_cast<DateAndTimeWidget *>(m_toolBox->getWidget("DateAndTime"));
        if (widgetDate)
        {
            // 安全调用
            widgetDate->setLanguage(m_lang);
        }
        else
        {
            printf("basePtr is not pointing to a Derived object.\n");
        }
        DisplayAndLightWidget *widgetDisplay = dynamic_cast<DisplayAndLightWidget *>(m_toolBox->getWidget("DisplayAndBrightness"));
        if (widgetDisplay)
        {
            // 安全调用
            widgetDisplay->setLanguage(m_lang);
        }
        else
        {
            printf("basePtr is not pointing to a Derived object.\n");
        }
        NetworkWidget *widgetNetwork = dynamic_cast<NetworkWidget *>(m_toolBox->getWidget("Internet"));
        if (widgetNetwork)
        {
            // 安全调用
            widgetNetwork->setLanguage(m_lang);
        }
        else
        {
            printf("basePtr is not pointing to a Derived object.\n");
        }
        m_toolBox->setLanguage(m_lang);
        m_deviceInfoLabel->setLabelText(loadTranslation(m_lang,"DeviceInfo"));//"设备信息");
        m_systemSetLabel->setLabelText(loadTranslation(m_lang,"SystemSet"));
        m_searchBeforeLabel->setLabelText(loadTranslation(m_lang,"Date from"));//"追溯管理");
        m_serviceInfoLabel->setLabelText(loadTranslation(m_lang,"ServiceInfo"));//"服务信息");
        emit updateLanguage(lang);
    });

    m_layout->addWidget(m_toolBox,0,3,12,1);
    m_deviceInfoLabel->getTextLabel()->setStyleSheet("QLabel{background-color:rgb(239,239,239);}");
    m_systemSetLabel->getTextLabel()->setStyleSheet("QLabel{background-color:rgb(154,154,154);}");
    m_searchBeforeLabel->getTextLabel()->setStyleSheet("QLabel{background-color:rgb(239,239,239);}");
    m_serviceInfoLabel->getTextLabel()->setStyleSheet("QLabel{background-color:rgb(239,239,239);}");
}
void HGSystemSetWidget::setWarnSoundVolumnMute(const bool &flag){
    WarningWidget* widget=dynamic_cast<WarningWidget*>(m_toolBox->getWidget("WarningSet"));
    if (widget) {
        // 安全调用
        widget->setWarnSoundVolumn(flag?0:50);
    } else {
        printf("basePtr is not pointing to a Derived object.\n");
    }
}
void HGSystemSetWidget::slotClickSearchBefore(){
    if (GlobalSingleton::instance().getSystemInfo("AuthorityStatus")!="true") {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "软件没有授权，或者授权过期!");
        return;
    }
    closeWindow();
    std::string name=findTranslationKey(m_lang, m_searchBeforeLabel->getTextLabel()->text().toStdString());
    // if (!isPermitted(HGOnlineRWDB::readAuthorityInfo(),HGOnlineRWDB::readLoginName(),name)){
    //     QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "无此权限，请联系管理员开通！");
    //     return;
    // }
    m_userRegisterW=new UserRegisterWidget(m_lang,m_isNoPwdLogin,this);
    connect(m_userRegisterW,SIGNAL(registerUserManage()),this,SLOT(slotEnterUserManage()));
    

    m_lockLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"Lock")));//"已锁死");
    
    if (!m_isNoPwdLogin){
        m_loginStatusLabel=new LabelWithImg(IMGRIGHT,12,getPath("/resources/V1/@1xIOS开关.png"),
            loadTranslation(m_lang,"NoPassLogin"));
    } else {
        m_loginStatusLabel=new LabelWithImg(IMGRIGHT,12,getPath("/resources/V1/@1xIOS开关_enable.png"),
            loadTranslation(m_lang,"NoPassLogin"));
    }
    connect(m_loginStatusLabel,SIGNAL(clickImgLabel()),this,SLOT(clickNoWdLogin()));

    if (!m_trackLogFlag){
        m_trackLogLabel=new LabelWithImg(IMGRIGHT,12,getPath("/resources/V1/@1xIOS开关.png"),
            loadTranslation(m_lang,"AuditTrail"));
        m_lockLabel->setVisible(false);
    } else {
        m_trackLogLabel=new LabelWithImg(IMGRIGHT,12,getPath("/resources/V1/@1xIOS开关_enable.png"),
            loadTranslation(m_lang,"AuditTrail"));//"审计追踪");
        m_lockLabel->setVisible(true);
    }
    connect(m_trackLogLabel,SIGNAL(clickImgLabel()),this,SLOT(clickTrackLog()));

    if (!m_debugModeFlag){
        m_debugModelLabel=new LabelWithImg(IMGRIGHT,12,getPath("/resources/V1/@1xIOS开关.png"),
            loadTranslation(m_lang,"Debug"));
    } else {
        m_debugModelLabel=new LabelWithImg(IMGRIGHT,12,getPath("/resources/V1/@1xIOS开关_enable.png"),
            loadTranslation(m_lang,"Debug"));
    }
    connect(m_debugModelLabel,SIGNAL(clickImgLabel()),this,SLOT(clickDebugMode()));
    
    m_trackLogWarnLabel=new LabelWithImg(IMGLEFT,12,getPath("/resources/V1/@1xriLine-rocket-2-line 1.png"),
        loadTranslation(m_lang,"OpenCanClose"));//打开后不可关闭!");
    m_debugModeWarnLabel=new LabelWithImg(IMGLEFT,12,getPath("/resources/V1/@1xriLine-rocket-2-line 1.png"),
        loadTranslation(m_lang,"DebugInformation"));//"初期调试时使用!");

    m_hline=new QFrame;
    m_hline->setFrameShape(QFrame::HLine);
    m_hline->setFrameShadow(QFrame::Sunken);
    m_hline1=new QFrame;
    m_hline1->setFrameShape(QFrame::HLine);
    m_hline1->setFrameShadow(QFrame::Sunken);

    m_layout->addWidget(m_userRegisterW,0,3,2,3);
    m_layout->addWidget(m_loginStatusLabel,7,3,1,1);
    m_layout->addWidget(m_trackLogLabel,3,3,1,1);
    m_layout->addWidget(m_lockLabel,3,4,1,1);
    m_layout->addWidget(m_trackLogWarnLabel,3,5,1,1);
    m_layout->addWidget(m_hline,4,3,1,3);
    m_layout->addWidget(m_debugModelLabel,5,3,1,1);
    m_layout->addWidget(m_debugModeWarnLabel,5,5,1,1);
    m_layout->addWidget(m_hline1,6,3,1,3);
    m_deviceInfoLabel->getTextLabel()->setStyleSheet("QLabel{background-color:rgb(239,239,239);}");
    m_systemSetLabel->getTextLabel()->setStyleSheet("QLabel{background-color:rgb(239,239,239);}");
    m_searchBeforeLabel->getTextLabel()->setStyleSheet("QLabel{background-color:rgb(154,154,154);}");
    m_serviceInfoLabel->getTextLabel()->setStyleSheet("QLabel{background-color:rgb(239,239,239);}");
}
void HGSystemSetWidget::slotEnterUserManage(){
    std::vector<std::string> authorityNames={

    };
    m_userListW=new UserListWidget(m_lang,authorityNames,this);
    connect(m_userListW,SIGNAL(signalBack()),this,SLOT(slotBackToSystemSet()));

    m_userRegisterW->hide();
    m_loginStatusLabel->hide();
    m_trackLogLabel->hide();
    m_lockLabel->hide();
    m_trackLogWarnLabel->hide();
    m_hline->hide();
    m_debugModelLabel->hide();
    m_debugModeWarnLabel->hide();
    m_hline1->hide();

    m_layout->removeWidget(m_userRegisterW);
    m_layout->removeWidget(m_loginStatusLabel);
    m_layout->removeWidget(m_trackLogLabel);
    m_layout->removeWidget(m_lockLabel);
    m_layout->removeWidget(m_trackLogWarnLabel);
    m_layout->removeWidget(m_hline);
    m_layout->removeWidget(m_debugModelLabel);
    m_layout->removeWidget(m_debugModeWarnLabel);
    m_layout->removeWidget(m_hline1);
    m_layout->addWidget(m_userListW,0,3,7,4);
}
void HGSystemSetWidget::slotBackToSystemSet()
{
    m_userRegisterW->show();
    m_loginStatusLabel->show();
    m_trackLogLabel->show();
    m_lockLabel->show();
    m_trackLogWarnLabel->show();
    m_hline->show();
    m_debugModelLabel->show();
    m_debugModeWarnLabel->show();
    m_hline1->show();

    m_layout->removeWidget(m_userListW);
    if (m_userListW){
        if (m_userListW->closeWindow()){
            SAFE_DELETE(m_userListW);
        }
    }
    m_layout->addWidget(m_userRegisterW,0,3,2,3);
    m_layout->addWidget(m_loginStatusLabel,7,3,1,1);
    m_layout->addWidget(m_trackLogLabel,3,3,1,1);
    m_layout->addWidget(m_lockLabel,3,4,1,1);
    m_layout->addWidget(m_trackLogWarnLabel,3,5,1,1);
    m_layout->addWidget(m_hline,4,3,1,3);
    m_layout->addWidget(m_debugModelLabel,5,3,1,1);
    m_layout->addWidget(m_debugModeWarnLabel,5,5,1,1);
    m_layout->addWidget(m_hline1,6,3,1,3);
}
void HGSystemSetWidget::clickTrackLog(){
    if (m_trackLogFlag){
        m_lockLabel->setVisible(true);
        return;
    }
    m_trackLogFlag=!m_trackLogFlag;
    if (!m_trackLogFlag){
        m_trackLogLabel->setImg(getPath("/resources/V1/@1xIOS开关.png"));
        GlobalSingleton::instance().setSystemInfo("track", "false");
        RWDb::writeAuditTrailLog("审计追踪关闭");
    }
    else
    {
        // if (m_userInfoS.authority != "系统管理员")
        // {
        //     QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "无此权限，请询问系统管理员!");
        //     m_isNoPwdLogin=!m_isNoPwdLogin;
        //     return;
        // }
        m_trackLogLabel->setImg(getPath("/resources/V1/@1xIOS开关_enable.png"));
        // QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),
        //                      "开启免密登录后，不需要用户名和密码，所有用户都可以登入，请确认！");
        GlobalSingleton::instance().setSystemInfo("track", "true");
        RWDb::writeAuditTrailLog("审计追踪开启");
    }
}
void HGSystemSetWidget::clickDebugMode(){
    m_debugModeFlag=!m_debugModeFlag;
    if (!m_debugModeFlag){
        m_debugModelLabel->setImg(getPath("/resources/V1/@1xIOS开关.png"));
        RWDb::writeAuditTrailLog("调试模式关闭");
        GlobalSingleton::instance().setSystemInfo("debug", "false");
    }
    else
    {
        // if (m_userInfoS.authority != "系统管理员")
        // {
        //     QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "无此权限，请询问系统管理员!");
        //     m_isNoPwdLogin=!m_isNoPwdLogin;
        //     return;
        // }
        m_debugModelLabel->setImg(getPath("/resources/V1/@1xIOS开关_enable.png"));
        // QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),
        //                      "开启免密登录后，不需要用户名和密码，所有用户都可以登入，请确认！");
        RWDb::writeAuditTrailLog("调试模式开启");
        GlobalSingleton::instance().setSystemInfo("debug", "true");
    }
}
void HGSystemSetWidget::clickNoWdLogin()
{
    m_isNoPwdLogin=!m_isNoPwdLogin;
    if (!m_isNoPwdLogin){
        m_loginStatusLabel->setImg(getPath("/resources/V1/@1xIOS开关.png"));
        RWDb::writeAuditTrailLog("免密登录关闭");
        GlobalSingleton::instance().setSystemInfo("免密登录", "false");
    }
    else
    {
        // if (!(m_userInfoS.authority == "SystemManager"||m_userInfoS.authority == "Manager"))
        // {
        //     QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "无此权限，请询问系统管理员!");
        //     m_isNoPwdLogin=!m_isNoPwdLogin;
        //     return;
        // }
        m_loginStatusLabel->setImg(getPath("/resources/V1/@1xIOS开关_enable.png"));
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),
                             "开启免密登录后，不需要用户名和密码，所有用户都可以登入，请确认！");
        RWDb::writeAuditTrailLog("免密登录开启");
        GlobalSingleton::instance().setSystemInfo("免密登录", "true");
    }
}
void HGSystemSetWidget::slotClickDServiceInfo(){
    if (GlobalSingleton::instance().getSystemInfo("AuthorityStatus")!="true") {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "软件没有授权，或者授权过期!");
        return;
    }
    closeWindow();
    std::string name=findTranslationKey(m_lang, m_serviceInfoLabel->getTextLabel()->text().toStdString());
    // if (!isPermitted(HGOnlineRWDB::readAuthorityInfo(),HGOnlineRWDB::readLoginName(),name)){
    //     QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "无此权限，请联系管理员开通！");
    //     return;
    // }
    m_serviceInfoW=new HGServiceInfoWidget(m_lang,this);
    m_layout->addWidget(m_serviceInfoW,0,3,2,1);
    m_deviceInfoLabel->getTextLabel()->setStyleSheet("QLabel{background-color:rgb(239,239,239);}");
    m_systemSetLabel->getTextLabel()->setStyleSheet("QLabel{background-color:rgb(239,239,239);}");
    m_searchBeforeLabel->getTextLabel()->setStyleSheet("QLabel{background-color:rgb(239,239,239);}");
    m_serviceInfoLabel->getTextLabel()->setStyleSheet("QLabel{background-color:rgb(154,154,154);}");
}