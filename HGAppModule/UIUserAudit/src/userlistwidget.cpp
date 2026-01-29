#include "userlistwidget.h"
#include <QHeaderView>
#include <QMessageBox>
#include "common.h"
#include <QDialog>
#include "UserAuditManager.h"
#include "SvcFactory.h"



UserListWidget::UserListWidget(std::string lang,const std::vector<std::string>& authority,QWidget *parent) : QWidget(parent),
m_lang(lang),
m_whole_authority(authority)
{
    m_curPhase = USER_INIT;
    fnInit();
}
void UserListWidget::fnInit()
{
    m_tabWidget = new QTabWidget();
    m_layout=new QGridLayout();
    this->setLayout(m_layout);

    m_backBtn=new QPushButton(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Back")));
    connect(m_backBtn,SIGNAL(clicked()),this,SLOT(clickBack()));

    m_authorityW=new AuthorityWidget(m_lang,m_whole_authority);
    connect(m_authorityW,SIGNAL(updateAuthority()),this,SLOT(slotUpdateAuthority()));

    m_userW=new UserWidget(m_lang);
    connect(m_userW,SIGNAL(updateAuthority()),this,SLOT(slotUpdateAuthority()));

    m_tabWidget->addTab(m_authorityW,"用户组管理");
    m_tabWidget->addTab(m_userW,"用户管理");
    m_layout->addWidget(m_backBtn,0,6,1,1);
    m_layout->addWidget(m_tabWidget,1,0,1,8);
}
void UserListWidget::slotUpdateAuthority()
{
    emit updateAuthority();
}
bool UserListWidget::closeWindow()
{
    bool result = true;
    if (m_userW){
        result &= m_userW->closeWindow();
        if (m_userW){
            delete m_userW;
            m_userW=nullptr;
        }
    }
    if (m_authorityW){
        result &= m_authorityW->closeWindow();
        if (m_authorityW){
            delete m_authorityW;
            m_authorityW=nullptr;
        }
    }
    return result;
}
void UserListWidget::closeEvent(QCloseEvent *event)
{
    if (closeWindow()){
        event->accept();
    }else{
        event->ignore();
    }
}
void UserListWidget::clickBack()
{
    emit signalBack();
}

UserListWidget::~UserListWidget()
{
    if (m_tabWidget);{
        delete m_tabWidget;
        m_tabWidget=nullptr;
    }
    if (m_layout);{
        delete m_layout;
        m_layout=nullptr;
    }
    if (m_backBtn);{
        delete m_backBtn;
        m_backBtn=nullptr;
    }
    if (m_userW);{
        delete m_userW;
        m_userW=nullptr;
    }
    if (m_authorityW);{
        delete m_authorityW;
        m_authorityW=nullptr;
    }
}