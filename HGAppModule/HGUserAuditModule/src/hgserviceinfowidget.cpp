#include "hgserviceinfowidget.h"
#include "common.h"


HGServiceInfoWidget::HGServiceInfoWidget(std::string lang,QWidget *parent) : QWidget(parent),
m_lang(lang)
{
    m_userIdentifierLabel=new QLabel();
    m_userIdentifierLabel->setText(QString::fromStdString(loadTranslation(m_lang,"UserIdentity")));//"用户标识");
    m_userIdentifierEdit=new QLineEdit();
    m_userIdentifierEdit->setPlaceholderText(QString::fromStdString(loadTranslation(m_lang,"Input")));//"请输入");

    m_serviceProviderLabel=new QLabel();
    m_serviceProviderLabel->setText(QString::fromStdString(loadTranslation(m_lang,"Provider")));//"服务商");
    m_serviceProviderEdit=new QLineEdit();
    m_serviceProviderEdit->setText("XXXXXX");

    m_customerServiceLabel=new QLabel();
    m_customerServiceLabel->setText(QString::fromStdString(loadTranslation(m_lang,"AccountSpecialist")));//"客服专员");
    m_customerServiceEdit=new QLineEdit();
    m_customerServiceEdit->setText("XXXXXXX");

    m_layout=new QGridLayout();

    m_layout->addWidget(m_userIdentifierLabel,0,0);
    m_layout->addWidget(m_userIdentifierEdit,0,1);
    m_layout->addWidget(m_serviceProviderLabel,1,0);
    m_layout->addWidget(m_serviceProviderEdit,1,1);
    m_layout->addWidget(m_customerServiceLabel,2,0);
    m_layout->addWidget(m_customerServiceEdit,2,1);

    this->setLayout(m_layout);

    fnReadDB();
}
bool HGServiceInfoWidget::closeWindow()
{
    fnWriteDB();
    return true;
}

void HGServiceInfoWidget::fnWriteDB(){
    if (GlobalSingleton::instance().getSystemInfo("用户标识")!=
        m_userIdentifierEdit->text().toStdString()) 
        RWDb::writeAuditTrailLog("修改用户标识:["+GlobalSingleton::instance().getSystemInfo("用户标识")+"]->["+m_userIdentifierEdit->text().toStdString()+"]");
    if (GlobalSingleton::instance().getSystemInfo("服务商")!=
        m_serviceProviderEdit->text().toStdString()) 
        RWDb::writeAuditTrailLog("修改服务商:["+GlobalSingleton::instance().getSystemInfo("服务商")+"]->["+m_serviceProviderEdit->text().toStdString()+"]");
    if (GlobalSingleton::instance().getSystemInfo("客服专员")!=
        m_customerServiceEdit->text().toStdString()) 
        RWDb::writeAuditTrailLog("修改客服专员:["+GlobalSingleton::instance().getSystemInfo("客服专员")+"]->[]"+m_customerServiceEdit->text().toStdString()+"]");
    GlobalSingleton::instance().setSystemInfo("用户标识",m_userIdentifierEdit->text().toStdString());
    GlobalSingleton::instance().setSystemInfo("服务商",m_serviceProviderEdit->text().toStdString());
    GlobalSingleton::instance().setSystemInfo("客服专员",m_customerServiceEdit->text().toStdString());

    GlobalSingleton::instance().saveSystemInfo();
}
void HGServiceInfoWidget::fnReadDB(){
    m_userIdentifierEdit->setText(QString::fromStdString(GlobalSingleton::instance().getSystemInfo("用户标识")));
    m_serviceProviderEdit->setText(QString::fromStdString(GlobalSingleton::instance().getSystemInfo("服务商")));
    m_customerServiceEdit->setText(QString::fromStdString(GlobalSingleton::instance().getSystemInfo("客服专员")));
}