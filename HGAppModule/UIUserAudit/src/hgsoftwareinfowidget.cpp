#include "hgsoftwareinfowidget.h"
#include <QMessageBox>
#include "common.h"
#include "loginterface.h"
#include "SystemDataManager.h"
#include "SvcFactory.h"
#include "securityinterface.h"

using namespace HGMACHINE;

HGSoftwareInfoWidget::HGSoftwareInfoWidget(std::string lang,QWidget *parent) : QWidget(parent),
    // m_loginAuthority(HGOnlineRWDB::readLoginAuthority()),
    m_lang(lang)
{
    m_authorityDeadLineCtrl=NULL;
    m_isEnable=false;
    fnInit();
    fnReadDB();
}
void HGSoftwareInfoWidget::fnInit()
{
    m_layout=new QGridLayout();
    m_widgetLayout=new QGridLayout();
    this->setLayout(m_widgetLayout);
    m_groupBox=new QGroupBox(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Software"))/*"应用软件"*/,this);
    m_groupBox->setStyleSheet("QGroupBox { font-size: 12pt; font-weight:bold;}");

    m_groupBox->setLayout(m_layout);

    m_productIDQLabel=new QLabel();
    m_productIDQLabel->setText(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"ActionCode")));//"激活码");
    m_productIDEdit=new QLineEdit();
    m_productIDEdit->setPlaceholderText(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Input")));//"请输入");
    // m_productIDEdit->setEchoMode(QLineEdit::Password);

    m_productKeyQLabel=new QLabel();
    m_productKeyQLabel->setText(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"ValidityPeriod")));//"有效期限");
    m_productKeyEdit=new QLineEdit();
    m_productKeyEdit->setPlaceholderText(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Input")));//"请输入");
    m_productKeyEdit->installEventFilter(this);

    m_enableBtn=new QPushButton(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Activate")));//"激活");
    connect(m_enableBtn,        SIGNAL(clicked()),this,SLOT(clickEnable()));

    if (!m_isEnable){
        m_enableStatusLabel=new LabelWithImg(IMGRIGHT,12,getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"),
            SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Unactivated"));//"未激活");
    } else {
        m_enableStatusLabel=new LabelWithImg(IMGRIGHT,12,getPath("/resources/V1/@1xze-certificate 1.png"),
            SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Activated"));//"已激活");
    }

    m_layout->addWidget(m_productIDQLabel,0,0,1,3);
    m_layout->addWidget(m_productIDEdit,0,2,1,2);
    m_layout->addWidget(m_enableBtn,0,5,1,1);
    m_layout->addWidget(m_productKeyQLabel,1,2,1,1);
    m_layout->addWidget(m_productKeyEdit,1,4,1,2);
    m_widgetLayout->addWidget(m_enableStatusLabel,0,6,1,1);
    m_widgetLayout->addWidget(m_groupBox,1,0,1,7);

}
HGSoftwareInfoWidget::~HGSoftwareInfoWidget()
{
    if (m_layout){
        delete m_authorityDeadLineCtrl;
        m_authorityDeadLineCtrl=NULL;
    }
    if (m_widgetLayout){
        delete m_authorityDeadLineCtrl;
        m_authorityDeadLineCtrl=NULL;
    }
    if (m_groupBox){
        delete m_authorityDeadLineCtrl;
        m_authorityDeadLineCtrl=NULL;
    }
    if (m_productIDQLabel){
        delete m_authorityDeadLineCtrl;
        m_authorityDeadLineCtrl=NULL;
    }
    if (m_productIDEdit){
        delete m_authorityDeadLineCtrl;
        m_authorityDeadLineCtrl=NULL;
    }
    if (m_productKeyQLabel){
        delete m_authorityDeadLineCtrl;
        m_authorityDeadLineCtrl=NULL;
    }
    if (m_productKeyEdit){
        delete m_authorityDeadLineCtrl;
        m_authorityDeadLineCtrl=NULL;
    }
    if (m_enableBtn){
        delete m_authorityDeadLineCtrl;
        m_authorityDeadLineCtrl=NULL;
    }
    if (m_enableStatusLabel){
        delete m_authorityDeadLineCtrl;
        m_authorityDeadLineCtrl=NULL;
    }
    if (m_authorityDeadLineCtrl){
        delete m_authorityDeadLineCtrl;
        m_authorityDeadLineCtrl=NULL;
    }
}

bool HGSoftwareInfoWidget::closeWindow()
{
    return true;
}
void HGSoftwareInfoWidget::clickEnable()
{
    fnWriteDB();
}
void HGSoftwareInfoWidget::fnWriteDB(){
    SystemDataManager::instance().get().setSystemInfo("激活码",m_productIDEdit->text().toStdString());
    if (SECURITY_IF.isRightAuthority(SystemDataManager::instance().get().getSystemInfo("激活码")))
    {
        TimeInfo curTimer = SvcFactory::CreateTimeService()->GetCurrentTime();
        std::ostringstream curTimeS;
        curTimeS << curTimer.year << curTimer.month << curTimer.day;
    
        SystemDataManager::instance().get().setSystemInfo("授权日期", curTimeS.str());
        SystemDataManager::instance().get().setSystemInfo("授权期限", m_productKeyEdit->text().toStdString());
        SystemDataManager::instance().get().setSystemInfo("AuthorityStatus","true");
        curTimeS.str("");
        curTimeS<<"激活成功，期限["<<SystemDataManager::instance().get().getSystemInfo("授权日期")<<",";
        if (SystemDataManager::instance().get().getSystemInfo("授权期限")==""){
            curTimeS<<"长期]";
        } else {
            curTimeS<<SystemDataManager::instance().get().getSystemInfo("授权期限")<<"]";
        }

        m_enableStatusLabel->setImg(getPath("/resources/V1/@1xze-certificate 1.png")); // ,"已激活");
        m_enableStatusLabel->setLabelText((SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Activated")));//"已激活");
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),
                             QString::fromStdString(curTimeS.str()));
        LOG_IF.writeAuditTrailLog(curTimeS.str());
    } else
    {
        m_enableStatusLabel->setImg(getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"));//,"未激活");
        m_enableStatusLabel->setLabelText((SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Unactivated")));//"未激活");
        SystemDataManager::instance().get().setSystemInfo("AuthorityStatus","false");

        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),
                             QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"AuthorityUnactivate")));//"授权码不正确，激活失败！");
        LOG_IF.writeAuditTrailLog(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"AuthorityUnactivate"));
    }

    SystemDataManager::instance().get().saveSystemInfo();
}
void HGSoftwareInfoWidget::fnReadDB(){
    m_productIDEdit->setText(QString::fromStdString(SystemDataManager::instance().get().getSystemInfo("激活码")));
    if (SECURITY_IF.isRightAuthority(SystemDataManager::instance().get().getSystemInfo("激活码")))
    {
        m_productKeyEdit->setText(QString::fromStdString(SystemDataManager::instance().get().getSystemInfo("授权期限")));
        if (SystemDataManager::instance().get().getSystemInfo("授权期限") == "长期")
        {
            m_enableStatusLabel->setImg(getPath("/resources/V1/@1xze-certificate 1.png")); // ,"已激活");
            m_enableStatusLabel->setLabelText(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Activated"));//"已激活");
            m_isEnable=true;
        }
        else
        {
            TimeInfo curTimer = SvcFactory::CreateTimeService()->GetCurrentTime();
            TimeInfo deadlineTimer = SvcFactory::CreateTimeService()->GetCurrentTime();
            if (SystemDataManager::instance().get().getSystemInfo("授权期限").length() >= 8) {
                deadlineTimer.year = atoi(SystemDataManager::instance().get().getSystemInfo("授权期限").substr(0, 4).c_str());
                deadlineTimer.month = atoi(SystemDataManager::instance().get().getSystemInfo("授权期限").substr(4, 2).c_str());
                deadlineTimer.day = atoi(SystemDataManager::instance().get().getSystemInfo("授权期限").substr(6, 2).c_str());
            }
            if (curTimer > deadlineTimer)
            {
                m_isEnable = false;
                m_enableStatusLabel->setImg(getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png")); //,"未激活,授权过期");
                m_enableStatusLabel->setLabelText(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"UnactivatedAndExpired"));//"未激活,授权过期");
            }
            else
            {
                deadlineTimer.day-=7;
                if ((deadlineTimer.day-7) <= 0){
                    if (deadlineTimer.month<=1){
                        deadlineTimer.year--;
                        deadlineTimer.month=12;
                        deadlineTimer.day=31-(7-deadlineTimer.day);
                    } else {
                        deadlineTimer.month--;
                        if (deadlineTimer.month==1||deadlineTimer.month==3||deadlineTimer.month==5||deadlineTimer.month==7||deadlineTimer.month==8||deadlineTimer.month==10||deadlineTimer.month==12)
                        {
                            deadlineTimer.day=31-(7-deadlineTimer.day);
                        } else if (deadlineTimer.month==4||deadlineTimer.month==6||deadlineTimer.month==9||deadlineTimer.month==11){
                            deadlineTimer.day=30-(7-deadlineTimer.day);
                        } else {
                            if ((deadlineTimer.year%4==0&&deadlineTimer.year%100!=0)||deadlineTimer.year%400==0)
                                deadlineTimer.day=29-(7-deadlineTimer.day);
                            else
                                deadlineTimer.day=28-(7-deadlineTimer.day);
                        }
                    }
                }
                if (curTimer>deadlineTimer){
                    m_isEnable = true;
                    m_enableStatusLabel->setImg(getPath("/resources/V1/@1xze-certificate 1.png")); // ,"已激活");
                    m_enableStatusLabel->setLabelText(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"ActivatedAndOnlyOneWeek"));//"已激活[距离截止日期只剩最后一周，请及时续期]");
                } else {
                    m_isEnable = true;
                    m_enableStatusLabel->setImg(getPath("/resources/V1/@1xze-certificate 1.png")); // ,"已激活");
                    m_enableStatusLabel->setLabelText(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Activated"));//"已激活");
                }
            }
        }
    }
    else
    {
        m_isEnable=false;
        m_enableStatusLabel->setImg(getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"));//,"未激活");
        m_enableStatusLabel->setLabelText(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Unactivated"));//"未激活");
    }
    SystemDataManager::instance().get().setSystemInfo("AuthorityStatus", (m_isEnable?"true":"false"));
}
bool HGSoftwareInfoWidget::eventFilter(QObject* obj,QEvent* event)
{
    if (event->type()==QEvent::MouseButtonPress){
        QMouseEvent* mouseEvent=static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton){
            if (obj==m_productKeyEdit){
                // if (!isPermitted(m_loginAuthority,"设备信息")){
                //     QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), 
                //         QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"NoPermission"))); //"无此权限，请联系管理员开通！");
                //     return QWidget::eventFilter(obj,event);
                // }
                m_authorityDeadLineCtrl=new KBTimeEdit(true);
                int passPosX = m_productKeyEdit->mapToGlobal(QPoint(0,0)).x();
                int passPosY = m_productKeyEdit->mapToGlobal(QPoint(0,0)).y() + m_productKeyEdit->height();
                // if (passPosX+m_authorityDeadLineCtrl->width()>m_width) passPosX=m_width-m_authorityDeadLineCtrl->width();
                // if (passPosY+m_authorityDeadLineCtrl->height()>m_height) passPosY=m_height-m_authorityDeadLineCtrl->height();
                m_authorityDeadLineCtrl->move(passPosX,passPosY);
                m_authorityDeadLineCtrl->setValue(m_productKeyEdit->text().toStdString());
                m_authorityDeadLineCtrl->show();
                connect(m_authorityDeadLineCtrl,SIGNAL(MykeyboardSignal(QString)),
                        this,SLOT(slotAuthorityDeadLine(QString)));
            }
        }
    }
    return QWidget::eventFilter(obj,event);
}
void HGSoftwareInfoWidget::slotAuthorityDeadLine(QString str) {
// #if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
//     if (!IsValidString(str)) {
//         QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),"windows系统下不支持中文或特殊字符，请重新输入");
//         return;
//     }
// #endif
    m_productKeyEdit->setText(str);
}