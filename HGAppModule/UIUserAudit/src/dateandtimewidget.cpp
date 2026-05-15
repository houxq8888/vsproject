#include "dateandtimewidget.h"
#include "common.h"
#include <QMessageBox>
#include "loginterface.h"
#include "SystemDataManager.h"
#include "SvcFactory.h"

using namespace HGMACHINE;
DateAndTimeWidget::DateAndTimeWidget(std::string lang,QWidget *parent) : BaseWidget(parent),
    m_lang(lang)
    ,m_isEnableAutoSetTime(true),
    m_isEnableAutoSetRegion(true),
    m_manualSetTimeStr(""),
    m_systemTimeCtrl(nullptr)
{
    m_autoSetRegionLabel=new QLabel(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"AutoSetZone")));//"自动设置时区");
    m_regionLabel=new QLabel(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Zone")));//"时区");
    m_autoSetTimeLabel=new QLabel(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"AutoSetTime")));//"自动设置时间");
    m_manualSetLabel=new QLabel(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"ManualTime")));//"手动设置日期和时间");
 
// 关 
    m_autoSetRegionImg=new LabelWithImg(IMGRIGHT,12,getPath("/resources/V1/@1xIOS开关_enable.png"),
        SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"On"));//"开");
    m_autoSetTimeImg=new LabelWithImg(IMGRIGHT,12,getPath("/resources/V1/@1xIOS开关_enable.png"),
        SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Off"));//"开");
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
    m_manualSetBtn=new QPushButton(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Change")));//"更改");

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
    m_isEnableAutoSetRegion=SystemDataManager::instance().get().getSystemInfo("自动设置时区")=="true"?true:false;
    m_isEnableAutoSetTime=SystemDataManager::instance().get().getSystemInfo("自动设置时间")=="true"?true:false;

    setControlStatus();
}
void DateAndTimeWidget::fnWriteDB()
{
    SystemDataManager::instance().get().setSystemInfo("自动设置时区",m_isEnableAutoSetRegion?"true":"false");
    SystemDataManager::instance().get().setSystemInfo("时区",m_regionComboBox->currentText().toStdString());
    SystemDataManager::instance().get().setSystemInfo("自动设置时间",m_isEnableAutoSetTime?"true":"false");
    SystemDataManager::instance().get().setSystemInfo("手动设置日期和时间",m_manualSetTimeStr);
}
void DateAndTimeWidget::setControlStatus()
{   
    if (!m_isEnableAutoSetRegion){
        m_autoSetRegionImg->setImg(getPath("/resources/V1/@1xIOS开关.png"));
        m_autoSetRegionImg->getTextLabel()->setText(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Off")));
    } else {
        m_autoSetRegionImg->setImg(getPath("/resources/V1/@1xIOS开关_enable.png"));
        m_autoSetRegionImg->getTextLabel()->setText(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"On")));
    }

    m_regionComboBox->setEnabled(!m_isEnableAutoSetRegion);

    if (!m_isEnableAutoSetTime){
        m_autoSetTimeImg->setImg(getPath("/resources/V1/@1xIOS开关.png"));
        m_autoSetTimeImg->getTextLabel()->setText(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Off")));
    } else {
        m_autoSetTimeImg->setImg(getPath("/resources/V1/@1xIOS开关_enable.png"));
        m_autoSetTimeImg->getTextLabel()->setText(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"On")));
    }

    m_manualSetBtn->setEnabled(!m_isEnableAutoSetTime);
}
void DateAndTimeWidget::clickEnableAutoSetRegion()
{
    m_isEnableAutoSetRegion=!m_isEnableAutoSetRegion;
    LOG_IF.writeAuditTrailLog(m_isEnableAutoSetRegion?("打开"+m_autoSetRegionLabel->text().toStdString()): \
        "关闭"+m_autoSetRegionLabel->text().toStdString());
    setControlStatus();
}

void DateAndTimeWidget::clickEnableAutoSetTime()
{
    m_isEnableAutoSetTime=!m_isEnableAutoSetTime;
    LOG_IF.writeAuditTrailLog(m_isEnableAutoSetTime?("打开"+m_autoSetTimeLabel->text().toStdString()): \
        "关闭"+m_autoSetTimeLabel->text().toStdString());
    setControlStatus();
}
void DateAndTimeWidget::slotRegionChanged(int index){
    LOG_IF.writeAuditTrailLog("切换时区为"+m_regionComboBox->currentText().toStdString());
    SvcFactory::CreateCommonService()->SetTimezone(m_regionComboBox->currentText().toStdString());
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
    m_autoSetRegionLabel->setText(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"AutoSetZone")));//"自动设置时区");
    m_regionLabel->setText(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Zone")));//"时区");
    m_autoSetTimeLabel->setText(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"AutoSetTime")));//"自动设置时间");
    m_manualSetLabel->setText(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"ManualTime")));//"手动设置日期和时间");
    m_manualSetBtn->setText(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Change")));
}
void DateAndTimeWidget::fnSetSystemTime(const std::string &time){
    TimeInfo setSystemTimer = SvcFactory::CreateTimeService()->GetCurrentTime();
    setSystemTimer.year = atoi(time.substr(0, 4).c_str());
    setSystemTimer.month = atoi(time.substr(4, 2).c_str());
    setSystemTimer.day = atoi(time.substr(6, 2).c_str());
    // setSystemTimer.hour = atoi(time.substr(8, 2).c_str());
    // setSystemTimer.minute = atoi(time.substr(10, 2).c_str());
    // setSystemTimer.second = atoi(time.substr(12, 2).c_str());
    std::string lastLoginTime = SystemDataManager::instance().get().getSystemInfo("lastLoginTime");
    TimeInfo softwareLastTimer = SvcFactory::CreateTimeService()->GetCurrentTime();
    if (lastLoginTime.size() >= 8) {
        softwareLastTimer.year = atoi(lastLoginTime.substr(0, 4).c_str());
        softwareLastTimer.month = atoi(lastLoginTime.substr(4, 2).c_str());
        softwareLastTimer.day = atoi(lastLoginTime.substr(6, 2).c_str());
    }
    bool setflag=false;
    if (SystemDataManager::instance().get().getSystemInfo("lastLoginTime")==""){
        setflag=true;
    } else {
        if (setSystemTimer<softwareLastTimer){
            if (QMessageBox::Ok==QMessageBox::question(this, QString::fromStdString(HG_DEVICE_NAME),
                    QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"loginWarning")),QMessageBox::Ok|QMessageBox::Cancel))
            {
                LOG_IF.writeAuditTrailLog(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"loginWarning")+":[Yes]");
                setflag=true;
            } else {
                LOG_IF.writeAuditTrailLog(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"loginWarning")+":[No]");
            }
        } else {
            setflag=true;
        }
    }
    if (setflag){
        SvcFactory::CreateTimeService()->SetSystemTime(setSystemTimer.year,setSystemTimer.month,setSystemTimer.day,
                setSystemTimer.hour,setSystemTimer.minute,setSystemTimer.second);
        std::ostringstream ss;
        ss<<setSystemTimer.year<<"-"<<setSystemTimer.month<<"-"<<setSystemTimer.day<<"-" \
            <<setSystemTimer.hour<<"-"<<setSystemTimer.minute<<"-"<<setSystemTimer.second;
        LOG_IF.writeAuditTrailLog("手动设置系统时间为:"+ss.str());
    }
}