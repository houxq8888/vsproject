#include "hgsoftwareinfowidget.h"
#include <QMessageBox>
#include "common.h"

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
    m_groupBox=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"Software"))/*"应用软件"*/,this);
    m_groupBox->setStyleSheet("QGroupBox { font-size: 12pt; font-weight:bold;}");

    m_groupBox->setLayout(m_layout);

    m_productIDQLabel=new QLabel();
    m_productIDQLabel->setText(QString::fromStdString(loadTranslation(m_lang,"ActionCode")));//"激活码");
    m_productIDEdit=new QLineEdit();
    m_productIDEdit->setPlaceholderText(QString::fromStdString(loadTranslation(m_lang,"Input")));//"请输入");
    // m_productIDEdit->setEchoMode(QLineEdit::Password);

    m_productKeyQLabel=new QLabel();
    m_productKeyQLabel->setText(QString::fromStdString(loadTranslation(m_lang,"ValidityPeriod")));//"有效期限");
    m_productKeyEdit=new QLineEdit();
    m_productKeyEdit->setPlaceholderText(QString::fromStdString(loadTranslation(m_lang,"Input")));//"请输入");
    m_productKeyEdit->installEventFilter(this);

    m_enableBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Activate")));//"激活");
    connect(m_enableBtn,        SIGNAL(clicked()),this,SLOT(clickEnable()));

    if (!m_isEnable){
        m_enableStatusLabel=new LabelWithImg(IMGRIGHT,12,getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"),
            loadTranslation(m_lang,"Unactivated"));//"未激活");
    } else {
        m_enableStatusLabel=new LabelWithImg(IMGRIGHT,12,getPath("/resources/V1/@1xze-certificate 1.png"),
            loadTranslation(m_lang,"Activated"));//"已激活");
    }

    m_layout->addWidget(m_productIDQLabel,0,0,1,3);
    m_layout->addWidget(m_productIDEdit,0,2,1,2);
    m_layout->addWidget(m_enableBtn,0,5,1,1);
    m_layout->addWidget(m_productKeyQLabel,1,2,1,1);
    m_layout->addWidget(m_productKeyEdit,1,4,1,2);
    m_widgetLayout->addWidget(m_enableStatusLabel,0,6,1,1);
    m_widgetLayout->addWidget(m_groupBox,1,0,1,7);

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
    GlobalSingleton::instance().setSystemInfo("激活码",m_productIDEdit->text().toStdString());
    if (isRightAuthority(GlobalSingleton::instance().getSystemInfo("激活码")))
    {
        HGExactTime curTimer = HGExactTime::currentTime();
        std::ostringstream curTimeS;
        curTimeS << curTimer.tm_year << curTimer.tm_mon << curTimer.tm_mday;
    
        GlobalSingleton::instance().setSystemInfo("授权日期", curTimeS.str());
        GlobalSingleton::instance().setSystemInfo("授权期限", m_productKeyEdit->text().toStdString());
        GlobalSingleton::instance().setSystemInfo("AuthorityStatus","true");
        curTimeS.str("");
        curTimeS<<"激活成功，期限["<<GlobalSingleton::instance().getSystemInfo("授权日期")<<",";
        if (GlobalSingleton::instance().getSystemInfo("授权期限")==""){
            curTimeS<<"长期]";
        } else {
            curTimeS<<GlobalSingleton::instance().getSystemInfo("授权期限")<<"]";
        }

        m_enableStatusLabel->setImg(getPath("/resources/V1/@1xze-certificate 1.png")); // ,"已激活");
        m_enableStatusLabel->setLabelText((loadTranslation(m_lang,"Activated")));//"已激活");
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),
                             QString::fromStdString(curTimeS.str()));
        RWDb::writeAuditTrailLog(curTimeS.str());
    } else
    {
        m_enableStatusLabel->setImg(getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"));//,"未激活");
        m_enableStatusLabel->setLabelText((loadTranslation(m_lang,"Unactivated")));//"未激活");
        GlobalSingleton::instance().setSystemInfo("AuthorityStatus","false");

        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),
                             QString::fromStdString(loadTranslation(m_lang,"AuthorityUnactivate")));//"授权码不正确，激活失败！");
        RWDb::writeAuditTrailLog(loadTranslation(m_lang,"AuthorityUnactivate"));
    }

    GlobalSingleton::instance().saveSystemInfo();
}
void HGSoftwareInfoWidget::fnReadDB(){
    m_productIDEdit->setText(QString::fromStdString(GlobalSingleton::instance().getSystemInfo("激活码")));
    if (isRightAuthority(GlobalSingleton::instance().getSystemInfo("激活码")))
    {
        m_productKeyEdit->setText(QString::fromStdString(GlobalSingleton::instance().getSystemInfo("授权期限")));
        if (GlobalSingleton::instance().getSystemInfo("授权期限") == "长期")
        {
            m_enableStatusLabel->setImg(getPath("/resources/V1/@1xze-certificate 1.png")); // ,"已激活");
            m_enableStatusLabel->setLabelText(loadTranslation(m_lang,"Activated"));//"已激活");
            m_isEnable=true;
        }
        else
        {
            HGExactTime curTimer = HGExactTime::currentTime();
            HGExactTime deadlineTimer = HGExactTime::currentTime();
            if (GlobalSingleton::instance().getSystemInfo("授权期限").length() >= 8) {
                deadlineTimer.tm_year = atoi(GlobalSingleton::instance().getSystemInfo("授权期限").substr(0, 4).c_str());
                deadlineTimer.tm_mon = atoi(GlobalSingleton::instance().getSystemInfo("授权期限").substr(4, 2).c_str());
                deadlineTimer.tm_mday = atoi(GlobalSingleton::instance().getSystemInfo("授权期限").substr(6, 2).c_str());
            }
            if (curTimer > deadlineTimer)
            {
                m_isEnable = false;
                m_enableStatusLabel->setImg(getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png")); //,"未激活,授权过期");
                m_enableStatusLabel->setLabelText(loadTranslation(m_lang,"UnactivatedAndExpired"));//"未激活,授权过期");
            }
            else
            {
                deadlineTimer.tm_mday-=7;
                if ((deadlineTimer.tm_mday-7) <= 0){
                    if (deadlineTimer.tm_mon<=1){
                        deadlineTimer.tm_year--;
                        deadlineTimer.tm_mon=12;
                        deadlineTimer.tm_mday=31-(7-deadlineTimer.tm_mday);
                    } else {
                        deadlineTimer.tm_mon--;
                        if (deadlineTimer.tm_mon==1||deadlineTimer.tm_mon==3||deadlineTimer.tm_mon==5||deadlineTimer.tm_mon==7||deadlineTimer.tm_mon==8||deadlineTimer.tm_mon==10||deadlineTimer.tm_mon==12)
                        {
                            deadlineTimer.tm_mday=31-(7-deadlineTimer.tm_mday);
                        } else if (deadlineTimer.tm_mon==4||deadlineTimer.tm_mon==6||deadlineTimer.tm_mon==9||deadlineTimer.tm_mon==11){
                            deadlineTimer.tm_mday=30-(7-deadlineTimer.tm_mday);
                        } else {
                            if ((deadlineTimer.tm_year%4==0&&deadlineTimer.tm_year%100!=0)||deadlineTimer.tm_year%400==0)
                                deadlineTimer.tm_mday=29-(7-deadlineTimer.tm_mday);
                            else
                                deadlineTimer.tm_mday=28-(7-deadlineTimer.tm_mday);
                        }
                    }
                }
                if (curTimer>deadlineTimer){
                    m_isEnable = true;
                    m_enableStatusLabel->setImg(getPath("/resources/V1/@1xze-certificate 1.png")); // ,"已激活");
                    m_enableStatusLabel->setLabelText(loadTranslation(m_lang,"ActivatedAndOnlyOneWeek"));//"已激活[距离截止日期只剩最后一周，请及时续期]");
                } else {
                    m_isEnable = true;
                    m_enableStatusLabel->setImg(getPath("/resources/V1/@1xze-certificate 1.png")); // ,"已激活");
                    m_enableStatusLabel->setLabelText(loadTranslation(m_lang,"Activated"));//"已激活");
                }
            }
        }
    }
    else
    {
        m_isEnable=false;
        m_enableStatusLabel->setImg(getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"));//,"未激活");
        m_enableStatusLabel->setLabelText(loadTranslation(m_lang,"Unactivated"));//"未激活");
    }
    GlobalSingleton::instance().setSystemInfo("AuthorityStatus", (m_isEnable?"true":"false"));
}
bool HGSoftwareInfoWidget::eventFilter(QObject* obj,QEvent* event)
{
    if (event->type()==QEvent::MouseButtonPress){
        QMouseEvent* mouseEvent=static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton){
            if (obj==m_productKeyEdit){
                // if (!isPermitted(m_loginAuthority,"设备信息")){
                //     QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), 
                //         QString::fromStdString(loadTranslation(m_lang,"NoPermission"))); //"无此权限，请联系管理员开通！");
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