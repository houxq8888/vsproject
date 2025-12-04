#include "dateandtimewidget.h"
#include "common.h"
#include <QMessageBox>


DateAndTimeWidget::DateAndTimeWidget(std::string lang,QWidget *parent) : BaseWidget(parent),
    m_lang(lang)
    ,m_isEnableAutoSetTime(true),
    m_isEnableAutoSetRegion(true),
    m_manualSetTimeStr(""),
    m_systemTimeCtrl(nullptr)
{
    m_autoSetRegionLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"AutoSetZone")));//"自动设置时区");
    m_regionLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"Zone")));//"时区");
    m_autoSetTimeLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"AutoSetTime")));//"自动设置时间");
    m_manualSetLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"ManualTime")));//"手动设置日期和时间");
 
// 关 
    m_autoSetRegionImg=new LabelWithImg(IMGRIGHT,12,getPath("/resources/V1/@1xIOS开关_enable.png"),
        loadTranslation(m_lang,"On"));//"开");
    m_autoSetTimeImg=new LabelWithImg(IMGRIGHT,12,getPath("/resources/V1/@1xIOS开关_enable.png"),
        loadTranslation(m_lang,"Off"));//"开");
    connect(m_autoSetRegionImg,SIGNAL(clickImgLabel()),this,SLOT(clickEnableAutoSetRegion()));
    connect(m_autoSetTimeImg,SIGNAL(clickImgLabel()),this,SLOT(clickEnableAutoSetTime()));

    m_regionComboBox=new QComboBox();
    m_regionComboBox->addItem("America/New_York");
    m_regionComboBox->addItem("Canada/Toronto");
    m_regionComboBox->addItem("China/Shanghai");
    m_regionComboBox->addItem("Europe/Berlin");
    m_regionComboBox->addItem("Europe/London");
    m_regionComboBox->addItem("Europe/Paris");
    m_regionComboBox->addItem("Europe/Rome");
    m_regionComboBox->addItem("Europe/Vienna");
    m_regionComboBox->addItem("Europe/Zurich");
    m_manualSetBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Change")));//"更改");

    connect(m_regionComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(slotRegionChanged(int)));
    connect(m_manualSetBtn,SIGNAL(clicked()),this,SLOT(clickManualSetTime()));

    m_layout=new QGridLayout();
    this->setLayout(m_layout);
    m_layout->addWidget(m_autoSetRegionLabel,0,0);
    m_layout->addWidget(m_autoSetRegionImg,0,1);
    m_layout->addWidget(m_regionLabel,1,0);
    m_layout->addWidget(m_regionComboBox,1,1);
    m_layout->addWidget(m_autoSetTimeLabel,2,0);
    m_layout->addWidget(m_autoSetTimeImg,2,1);
    m_layout->addWidget(m_manualSetLabel,3,0);
    m_layout->addWidget(m_manualSetBtn,3,1);

    fnReadDB();
}
DateAndTimeWidget::~DateAndTimeWidget(){
    closeWindow();
}
bool DateAndTimeWidget::closeWindow()
{
    fnWriteDB();
    return true;
}
void DateAndTimeWidget::fnReadDB()
{
    m_isEnableAutoSetRegion=GlobalSingleton::instance().getSystemInfo("自动设置时区")=="true"?true:false;
    m_isEnableAutoSetTime=GlobalSingleton::instance().getSystemInfo("自动设置时间")=="true"?true:false;

    setControlStatus();
}
void DateAndTimeWidget::fnWriteDB()
{
    GlobalSingleton::instance().setSystemInfo("自动设置时区",m_isEnableAutoSetRegion?"true":"false");
    GlobalSingleton::instance().setSystemInfo("时区",m_regionComboBox->currentText().toStdString());
    GlobalSingleton::instance().setSystemInfo("自动设置时间",m_isEnableAutoSetTime?"true":"false");
    GlobalSingleton::instance().setSystemInfo("手动设置日期和时间",m_manualSetTimeStr);
}
void DateAndTimeWidget::setControlStatus()
{   
    if (!m_isEnableAutoSetRegion){
        m_autoSetRegionImg->setImg(getPath("/resources/V1/@1xIOS开关.png"));
        m_autoSetRegionImg->getTextLabel()->setText(QString::fromStdString(loadTranslation(m_lang,"Off")));
    } else {
        m_autoSetRegionImg->setImg(getPath("/resources/V1/@1xIOS开关_enable.png"));
        m_autoSetRegionImg->getTextLabel()->setText(QString::fromStdString(loadTranslation(m_lang,"On")));
    }

    m_regionComboBox->setEnabled(!m_isEnableAutoSetRegion);

    if (!m_isEnableAutoSetTime){
        m_autoSetTimeImg->setImg(getPath("/resources/V1/@1xIOS开关.png"));
        m_autoSetTimeImg->getTextLabel()->setText(QString::fromStdString(loadTranslation(m_lang,"Off")));
    } else {
        m_autoSetTimeImg->setImg(getPath("/resources/V1/@1xIOS开关_enable.png"));
        m_autoSetTimeImg->getTextLabel()->setText(QString::fromStdString(loadTranslation(m_lang,"On")));
    }

    m_manualSetBtn->setEnabled(!m_isEnableAutoSetTime);
}
void DateAndTimeWidget::clickEnableAutoSetRegion()
{
    m_isEnableAutoSetRegion=!m_isEnableAutoSetRegion;
    RWDb::writeAuditTrailLog(m_isEnableAutoSetRegion?("打开"+m_autoSetRegionLabel->text().toStdString()): \
        "关闭"+m_autoSetRegionLabel->text().toStdString());
    setControlStatus();
}

void DateAndTimeWidget::clickEnableAutoSetTime()
{
    m_isEnableAutoSetTime=!m_isEnableAutoSetTime;
    RWDb::writeAuditTrailLog(m_isEnableAutoSetTime?("打开"+m_autoSetTimeLabel->text().toStdString()): \
        "关闭"+m_autoSetTimeLabel->text().toStdString());
    setControlStatus();
}
void DateAndTimeWidget::slotRegionChanged(int index){
    RWDb::writeAuditTrailLog("切换时区为"+m_regionComboBox->currentText().toStdString());
    setTimezone(m_regionComboBox->currentText().toStdString());
}
void DateAndTimeWidget::clickManualSetTime(){
    m_systemTimeCtrl=new KBTimeWithHourEdit(false);
    int passPosX = m_manualSetBtn->mapToGlobal(QPoint(0,0)).x();
    int passPosY = m_manualSetBtn->mapToGlobal(QPoint(0,0)).y() + m_manualSetBtn->height();
    // if (passPosX+m_authorityDeadLineCtrl->width()>m_width) passPosX=m_width-m_authorityDeadLineCtrl->width();
    // if (passPosY+m_authorityDeadLineCtrl->height()>m_height) passPosY=m_height-m_authorityDeadLineCtrl->height();
    m_systemTimeCtrl->move(passPosX,passPosY);
    m_systemTimeCtrl->show();
    connect(m_systemTimeCtrl,SIGNAL(MykeyboardSignal(const std::string&)),this,SLOT(fnSetSystemTime(const std::string&)));
}
void DateAndTimeWidget::setLanguage(std::string lang){
    m_lang=lang;
    m_autoSetRegionLabel->setText(QString::fromStdString(loadTranslation(m_lang,"AutoSetZone")));//"自动设置时区");
    m_regionLabel->setText(QString::fromStdString(loadTranslation(m_lang,"Zone")));//"时区");
    m_autoSetTimeLabel->setText(QString::fromStdString(loadTranslation(m_lang,"AutoSetTime")));//"自动设置时间");
    m_manualSetLabel->setText(QString::fromStdString(loadTranslation(m_lang,"ManualTime")));//"手动设置日期和时间");
    m_manualSetBtn->setText(QString::fromStdString(loadTranslation(m_lang,"Change")));
}
void DateAndTimeWidget::fnSetSystemTime(const std::string &time){
    HGExactTime setSystemTimer = HGExactTime::currentTime();
    setSystemTimer.tm_year = atoi(time.substr(0, 4).c_str());
    setSystemTimer.tm_mon = atoi(time.substr(4, 2).c_str());
    setSystemTimer.tm_mday = atoi(time.substr(6, 2).c_str());
    // setSystemTimer.tm_hour = atoi(time.substr(8, 2).c_str());
    // setSystemTimer.tm_min = atoi(time.substr(10, 2).c_str());
    // setSystemTimer.tm_sec = atoi(time.substr(12, 2).c_str());
    TIME_STRUECT tmS;
    decodeStandardTime(GlobalSingleton::instance().getSystemInfo("lastLoginTime"),tmS); 
    HGExactTime softwareLastTimer=HGExactTime::currentTime();
    softwareLastTimer.tm_year =tmS.year;
    softwareLastTimer.tm_mon =tmS.month;
    softwareLastTimer.tm_mday =tmS.day;
    bool setflag=false;
    if (GlobalSingleton::instance().getSystemInfo("lastLoginTime")==""){
        setflag=true;
    } else {
        if (setSystemTimer<softwareLastTimer){
            if (QMessageBox::Ok==QMessageBox::question(this, QString::fromStdString(HG_DEVICE_NAME),
                    QString::fromStdString(loadTranslation(m_lang,"loginWarning")),QMessageBox::Ok|QMessageBox::Cancel))
            {
                RWDb::writeAuditTrailLog(loadTranslation(m_lang,"loginWarning")+":[Yes]");
                setflag=true;
            } else {
                RWDb::writeAuditTrailLog(loadTranslation(m_lang,"loginWarning")+":[No]");
            }
        } else {
            setflag=true;
        }
    }
    if (setflag){
        HGExactTime::setSystemTime(setSystemTimer.tm_year,setSystemTimer.tm_mon,setSystemTimer.tm_mday,
                setSystemTimer.tm_hour,setSystemTimer.tm_min,setSystemTimer.tm_sec);
        std::ostringstream ss;
        ss<<setSystemTimer.tm_year<<"-"<<setSystemTimer.tm_mon<<"-"<<setSystemTimer.tm_mday<<"-" \
            <<setSystemTimer.tm_hour<<"-"<<setSystemTimer.tm_min<<"-"<<setSystemTimer.tm_sec;
        RWDb::writeAuditTrailLog("手动设置系统时间为:"+ss.str());
    }
}