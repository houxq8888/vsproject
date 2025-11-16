#include "userlistwidget.h"
#include <QHeaderView>
#include <QMessageBox>
#include "common.h"
#include <QDialog>
#include "globalsingleton.h"


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

    m_backBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Back")));
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
    }
    if (m_authorityW){
        result &= m_authorityW->closeWindow();
    }
    return result;
}
void UserListWidget::clickBack()
{
    emit signalBack();
}

UserListWidget::~UserListWidget()
{
    SAFE_DELETE(m_tabWidget);
    SAFE_DELETE(m_layout);
    SAFE_DELETE(m_backBtn);
    SAFE_DELETE(m_userW);
    SAFE_DELETE(m_authorityW);
}