#include "hgserviceinfowidget.h"
#include "common.h"
#include "loginterface.h"
#include "SystemDataManager.h"
#include "SvcFactory.h"

using namespace HGMACHINE;


HGServiceInfoWidget::HGServiceInfoWidget(std::string lang,QWidget *parent) : QWidget(parent),
m_lang(lang)
{
    m_userIdentifierLabel=new QLabel();
    m_userIdentifierLabel->setText(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"UserIdentity")));//"用户标识");
    m_userIdentifierEdit=new QLineEdit();
    m_userIdentifierEdit->setPlaceholderText(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Input")));//"请输入");

    m_serviceProviderLabel=new QLabel();
    m_serviceProviderLabel->setText(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Provider")));//"服务商");
    m_serviceProviderEdit=new QLineEdit();
    m_serviceProviderEdit->setText("XXXXXX");

    m_customerServiceLabel=new QLabel();
    m_customerServiceLabel->setText(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"AccountSpecialist")));//"客服专员");
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
    if (SystemDataManager::instance().get().getSystemInfo("用户标识")!=
        m_userIdentifierEdit->text().toStdString()) 
        LOG_IF.writeAuditTrailLog("修改用户标识:["+SystemDataManager::instance().get().getSystemInfo("用户标识")+"]->["+m_userIdentifierEdit->text().toStdString()+"]");
    if (SystemDataManager::instance().get().getSystemInfo("服务商")!=
        m_serviceProviderEdit->text().toStdString()) 
        LOG_IF.writeAuditTrailLog("修改服务商:["+SystemDataManager::instance().get().getSystemInfo("服务商")+"]->["+m_serviceProviderEdit->text().toStdString()+"]");
    if (SystemDataManager::instance().get().getSystemInfo("客服专员")!=
        m_customerServiceEdit->text().toStdString()) 
        LOG_IF.writeAuditTrailLog("修改客服专员:["+SystemDataManager::instance().get().getSystemInfo("客服专员")+"]->["+m_customerServiceEdit->text().toStdString()+"]");
    SystemDataManager::instance().get().setSystemInfo("用户标识",m_userIdentifierEdit->text().toStdString());
    SystemDataManager::instance().get().setSystemInfo("服务商",m_serviceProviderEdit->text().toStdString());
    SystemDataManager::instance().get().setSystemInfo("客服专员",m_customerServiceEdit->text().toStdString());

    SystemDataManager::instance().get().saveSystemInfo();
}
void HGServiceInfoWidget::fnReadDB(){
    m_userIdentifierEdit->setText(QString::fromStdString(SystemDataManager::instance().get().getSystemInfo("用户标识")));
    m_serviceProviderEdit->setText(QString::fromStdString(SystemDataManager::instance().get().getSystemInfo("服务商")));
    m_customerServiceEdit->setText(QString::fromStdString(SystemDataManager::instance().get().getSystemInfo("客服专员")));
}