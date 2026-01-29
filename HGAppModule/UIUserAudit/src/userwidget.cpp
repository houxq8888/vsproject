#include "userwidget.h"
#include <QHeaderView>
#include <QMessageBox>
#include "common.h"
#include <QDialog>
#include "UserAuditManager.h"
#include "loginterface.h"
#include "SvcFactory.h"
#include "SystemDataManager.h"

using namespace HGMACHINE;


UserWidget::UserWidget(std::string lang,QWidget *parent) : QWidget(parent),
m_lang(lang),
m_userInfoEditWidget(nullptr)
{
    std::string enterUsersManageName=SystemDataManager::instance().get().getSystemInfo("enterUsersManageName");
    std::string authority = UserAuditManager::instance().get().getUserAuthority(enterUsersManageName);
    permissionInfo = UserAuditManager::instance().get().getAuthorityDetail(authority);

    fnInit();
    fnReadDB();
}
void UserWidget::fnInit()
{
    m_userLayout=new QGridLayout();
    this->setLayout(m_userLayout);
    
    // 
    m_userManageLabel=new QLabel(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"UserManage")));
    QStringList headers={
        QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"UserNo")),
        QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"UserAccount")),
        QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"UserName")), 
        QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"UserJob")),
        QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Department")),
        QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Authority")),
        QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"AccountManagement"))
    };
    m_userManageTableW=new QTableWidget(0,headers.size());
    m_userManageTableW->setHorizontalHeaderLabels(headers);
    m_userManageTableW->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_userManageTableW->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_userManageTableW->verticalHeader()->setVisible(false);
    m_userManageTableW->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_userManageTableW->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_userManageTableW->resizeRowsToContents();
    m_userManageTableW->setMouseTracking(true);
    connect(m_userManageTableW,&QTableWidget::cellClicked,this,&UserWidget::onUserManageClicked);

    m_newUserLabel=new HGQLabel(false,getPath("/resources/V1/@1xze-plus 1.png"));
    m_deleteUserLabel=new HGQLabel(false,getPath("/resources/V1/@1xif-ui-delete 1.png"));
    m_editUserLabel=new HGQLabel(false,getPath("/resources/V1/@1xze-edit 1.png"));
    m_scanUserLabel=new HGQLabel(false,getPath("/resources/V1/@1xze-eye-o 1.png"));
    connect(m_newUserLabel,&HGQLabel::leftClicked,this,&UserWidget::newUser);
    connect(m_deleteUserLabel,&HGQLabel::leftClicked,this,&UserWidget::deleteUser);
    connect(m_editUserLabel,&HGQLabel::leftClicked,this,&UserWidget::editUser);
    connect(m_scanUserLabel,&HGQLabel::leftClicked,this,&UserWidget::scanUser);

    fnAddUserListW();
}
void UserWidget::fnAddUserListW()
{
    m_userLayout->addWidget(m_userManageLabel,0,0);
    m_userLayout->addWidget(m_newUserLabel,0,5);
    m_userLayout->addWidget(m_deleteUserLabel,0,6);
    m_userLayout->addWidget(m_editUserLabel,0,7);
    m_userLayout->addWidget(m_scanUserLabel,0,8);
    m_userLayout->addWidget(m_userManageTableW,1,0,1,8);
    m_userManageLabel->show();
    m_newUserLabel->show();
    m_deleteUserLabel->show();
    m_editUserLabel->show();
    m_scanUserLabel->show();
    m_userManageTableW->show();
}
bool UserWidget::closeWindow()
{
    if (m_userInfoEditWidget){
        if (m_userInfoEditWidget->closeWindow()){
            if  (m_userInfoEditWidget) {
                delete (m_userInfoEditWidget);
                m_userInfoEditWidget = nullptr;
            }
        }
    }
    fnWriteDB();
    return true;
}

void UserWidget::slotHasUserNo(std::string userNo){
    std::vector<std::string> names=UserAuditManager::instance().get().getUsersNo();
    for (const auto &name:names){
        if (name==userNo){
            m_userInfoEditWidget->hasUserNo(true);
            return;
        }
    }
    m_userInfoEditWidget->hasUserNo(false);
}

int  UserWidget::findUserAuthorityIndex(const std::string &userNo){
    int index=-1;
    std::vector<std::string> nos = UserAuditManager::instance().get().getUsersNo();
    for (int i=0;i<int(nos.size());i++){
        if (nos[i]==userNo){
            index=i;
            break;
        }
    }
    return index;
}
void UserWidget::findNextRowAndCol(QTableWidget* tableW,std::string key,int& row,int& col){
    for (int i=0;i<tableW->rowCount();i++){
        for (int j=0;j<tableW->columnCount();j++){
            if (tableW->item(i,j)){
                if (tableW->item(i,j)->text().toStdString()==key){
                    row=-1;
                    col=-1;
                    return;
                } 
            } 
        }
    }
    for (int i=0;i<tableW->rowCount();i++){
        for (int j=0;j<tableW->columnCount();j++){
            if (tableW->item(i,j)){
                if (tableW->item(i,j)->text()==""){
                    row=i;
                    col=j;
                    return;
                }
            } else {
                row=i;
                col=j;
                return;
            }
        }
    }
}

void UserWidget::fnAddUsrsGroupUser(const std::string &authorityName, const std::string &text)
{
    for (int i=0;i<int(UserAuditManager::instance().get().getAuthorityInfo().size());i++)
    {
        if (UserAuditManager::instance().get().getAuthorityField(i,"GroupName") == SvcFactory::CreateConfigService()->FindTranslationKey(m_lang, authorityName))
        {
            if (UserAuditManager::instance().get().getAuthorityField(i,"GroupPerson").find(text)!= std::string::npos) break;
            UserAuditManager::instance().get().setAuthorityField(i,"GroupPerson", 
                UserAuditManager::instance().get().getAuthorityField(i,"GroupPerson") + text + ":");
            std::string groupPerson = UserAuditManager::instance().get().getAuthorityField(i,"GroupPerson");
            std::vector<std::string> persons = SvcFactory::CreateCommonService()->SplitString(groupPerson, ':');
            int count=0;
            for (int m=0;m<int(persons.size());m++){
                if (persons[m]=="") continue;
                count++;
            }
            UserAuditManager::instance().get().setAuthorityField(i,"GroupPNumber",
                std::to_string(count));
            UserAuditManager::instance().get().setAuthorityField(i,"LastModifyTime",SvcFactory::CreateCommonService()->GetStandardCurTime());
            UserAuditManager::instance().get().setAuthorityField(i,"LastModifier",SystemDataManager::instance().get().getSystemInfo("enterUsersManageName"));
            break;
        }
    }
}
void UserWidget::fnRemoveUsrsGroupUser(const std::string &authorityName, const std::string &text)
{
    for (int i=0;i<int(UserAuditManager::instance().get().getAuthorityInfo().size());i++)
    {
        if (UserAuditManager::instance().get().getAuthorityField(i,"GroupName") == SvcFactory::CreateConfigService()->FindTranslationKey(m_lang, authorityName))
        {
            std::vector<std::string> persons = SvcFactory::CreateCommonService()->SplitString(UserAuditManager::instance().get().getAuthorityField(i,"GroupPerson"), ':');
            for (auto person : persons)
            {
                if (person == text)
                {
                    persons.erase(std::find(persons.begin(), persons.end(), person));
                    break;
                }
            }
            UserAuditManager::instance().get().setAuthorityField(i,"GroupPerson","");
            for (auto person : persons)
            {
                UserAuditManager::instance().get().addAuthorityField(i,"GroupPerson",person + ":");
            }
            std::string groupPerson = UserAuditManager::instance().get().getAuthorityField(i,"GroupPerson");
            persons.clear();
            persons = SvcFactory::CreateCommonService()->SplitString(groupPerson, ':');
            int count = 0;
            for (int m=0;m<int(persons.size());m++){
                if (persons[m]=="") continue;
                count++;
            }
            UserAuditManager::instance().get().setAuthorityField(i,"GroupPNumber",
                std::to_string(count));
            UserAuditManager::instance().get().setAuthorityField(i,"LastModifyTime",SvcFactory::CreateCommonService()->GetStandardCurTime());
            UserAuditManager::instance().get().setAuthorityField(i,"LastModifier",SystemDataManager::instance().get().getSystemInfo("enterUsersManageName"));
            break;
        }
    }
}
void UserWidget::onUserManageClicked(int row,int column){
    if (row<0||row>=m_userManageTableW->rowCount()) return;
    m_userManageTableW->selectRow(row);
}
void UserWidget::scanUser(){
    int row=getSelectedRow(m_userManageTableW);
    if (row<0||row>=m_userManageTableW->rowCount()) {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"SelectOneRecord")));
        return;
    }

    std::string scanUserAccount = m_userManageTableW->item(row,1)->text().toStdString();
    int index = UserAuditManager::instance().get().getUserFieldIndex(scanUserAccount);
    if (index < 0) return;

    removeWidgetsFromLayout(m_userLayout);
    m_userInfoEditWidget=new UserInfoEditWidget(USER_SCAN,m_lang,UserAuditManager::instance().get().getUserInfo(index),this);
    m_userLayout->addWidget(m_userInfoEditWidget,0,0);
    connect(m_userInfoEditWidget,SIGNAL(signalUserInfo(const std::map<std::string,std::string> &)),
        this,SLOT(slotUserInfo(const std::map<std::string,std::string> &)));
    connect(m_userInfoEditWidget,SIGNAL(signalUserNo(std::string)),this,SLOT(slotHasUserNo(std::string)));
    connect(m_userInfoEditWidget,SIGNAL(signalBack()),this,SLOT(returnToList()));
    connect(m_userInfoEditWidget,SIGNAL(updateAuthority()),this,SLOT(slotUpdateAuthority()));
    LOG_IF.writeAuditTrailLog("查看用户"+UserAuditManager::instance().get().getUserField(index,"UserAccount"));
}
void UserWidget::editUser(){
    int row=getSelectedRow(m_userManageTableW);
    if (row<0||row>=m_userManageTableW->rowCount()) {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"SelectOneRecord")));
        return;
    }
    std::string editUserAccount=m_userManageTableW->item(row,1)->text().toStdString(); 
    int index = UserAuditManager::instance().get().getUserFieldIndex(editUserAccount);
    if (index < 0) return;
    std::string editauthority = UserAuditManager::instance().get().getUserAuthority(editUserAccount);
    if (editauthority.find("所有权限")!=std::string::npos){
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "厂家账号不可编辑");
        return;
    }
    removeWidgetsFromLayout(m_userLayout);
    m_userInfoEditWidget=new UserInfoEditWidget(USER_RECTIFY,m_lang,UserAuditManager::instance().get().getUserInfo(index),this);
    m_userLayout->addWidget(m_userInfoEditWidget,0,0);
    connect(m_userInfoEditWidget,SIGNAL(signalUserInfo(const std::map<std::string,std::string> &)),
        this,SLOT(slotUserInfo(const std::map<std::string,std::string> &)));
    connect(m_userInfoEditWidget,SIGNAL(signalUserNo(std::string)),this,SLOT(slotHasUserNo(std::string)));
    connect(m_userInfoEditWidget,SIGNAL(signalBack()),this,SLOT(returnToList()));
    connect(m_userInfoEditWidget,SIGNAL(updateAuthority()),this,SLOT(slotUpdateAuthority()));
}
void UserWidget::deleteUser(){
    int row=getSelectedRow(m_userManageTableW);
    if (row<0||row>=m_userManageTableW->rowCount()) {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"SelectOneRecord")));
        return;
    }
    std::string delUserAccount = m_userManageTableW->item(row,1)->text().toStdString();
    int index = UserAuditManager::instance().get().getUserFieldIndex(delUserAccount);
    if (index < 0) return;
    std::string delauthority = UserAuditManager::instance().get().getUserAuthority(delUserAccount);
    if (delauthority.find("所有权限")!=std::string::npos){
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "厂家账号不可删除");
        return;
    }
    if (QMessageBox::No == QMessageBox::question(this,QString::fromStdString(HG_DEVICE_NAME),
                        "确定要删除账户["+QString::fromStdString(delUserAccount)+"]吗?",
                    QMessageBox::Yes|QMessageBox::No))
    {
        return;
    }
    m_userManageTableW->removeRow(row);
    std::string authority=UserAuditManager::instance().get().getUserField(index,"Authority");
    UserAuditManager::instance().get().deleteUserInfo(delUserAccount);

    std::vector<std::string> authorities = SvcFactory::CreateCommonService()->SplitString(authority,';');
    for (int i=0;i<int(authorities.size());i++){
        if (authorities[i]=="") continue;
        fnRemoveUsrsGroupUser(authorities[i], delUserAccount);
    }
    LOG_IF.writeAuditTrailLog("删除用户:"+delUserAccount);
    if (SystemDataManager::instance().get().getSystemInfo("loginNames").find(delUserAccount)!=std::string::npos){
        std::string loginNames = SystemDataManager::instance().get().getSystemInfo("loginNames");
        std::string target=delUserAccount+':';
        size_t pos =loginNames.find(target);
        if (pos!=std::string::npos){
            loginNames.erase(pos,target.length());
        }
        SystemDataManager::instance().get().setSystemInfo("loginNames",loginNames);
    }
}
void UserWidget::returnToList(){
    removeWidgetsFromLayout(m_userLayout);
    fnAddUserListW();
}
void UserWidget::newUser(){
    removeWidgetsFromLayout(m_userLayout);
    m_userInfoEditWidget=new UserInfoEditWidget(USER_CREATE,m_lang,std::map<std::string,std::string>(), this);
    m_userLayout->addWidget(m_userInfoEditWidget,0,0);
    connect(m_userInfoEditWidget,SIGNAL(signalUserInfo(const std::map<std::string,std::string> &)),
        this,SLOT(slotUserInfo(const std::map<std::string,std::string> &)));
    connect(m_userInfoEditWidget,SIGNAL(signalUserNo(std::string)),this,SLOT(slotHasUserNo(std::string)));
    connect(m_userInfoEditWidget,SIGNAL(signalBack()),this,SLOT(returnToList()));
    connect(m_userInfoEditWidget,SIGNAL(updateAuthority()),this,SLOT(slotUpdateAuthority()));
    LOG_IF.writeAuditTrailLog("创建用户");
}
void UserWidget::slotUpdateAuthority(){
    emit updateAuthority();
}
void UserWidget::slotUserInfo(const std::map<std::string,std::string>& userInfo){
    int index=-1;
    std::vector<std::map<std::string,std::string>> fillContent=UserAuditManager::instance().get().getUsersInfo();
    for (int i=0;i<int(fillContent.size());i++){
        if (fillContent[i].at("UserNo")==userInfo.at("UserNo")){
            index=i;
            break;
        }
    }
    std::string authority="";
    if (index==-1) {
        m_userManageTableW->insertRow(m_userManageTableW->rowCount());
        UserAuditManager::instance().get().addUserRecord(userInfo);
        index=m_userManageTableW->rowCount()-1;
        authority = userInfo.at("Authority");
    } else {
        authority=UserAuditManager::instance().get().getUserField(index,"Authority");
        UserAuditManager::instance().get().setUserInfoWithIndex(index,userInfo);
    }
    for (auto content : userInfo)
    {
        QString name=QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,content.first));
        int nameColIndex = getColumnIndexByName(m_userManageTableW, name);
        if (nameColIndex < 0 || nameColIndex >= m_userManageTableW->columnCount())
            continue;
        if (content.first=="Authority" || content.first=="AccountManagement") 
            m_userManageTableW->setItem(index, nameColIndex, 
                new QTableWidgetItem(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,content.second))));
        else 
            m_userManageTableW->setItem(index, nameColIndex, 
                new QTableWidgetItem(QString::fromStdString(content.second)));
    }

    fnAddUsrsGroupUser(authority, userInfo.at("UserAccount"));
    QMessageBox::information(this, QString::fromStdString(HG_DEVICE_NAME), "账户保存完成");
    LOG_IF.writeAuditTrailLog(userInfo.at("UserAccount")+"账户保存完成");
    fnWriteDB();
}
void UserWidget::closeEvent(QCloseEvent *event)
{
    fnWriteDB();
    if (closeWindow()){
        event->accept();
    }else{
        event->ignore();
    }
}
int UserWidget::getColumnIndexByName(QTableWidget* table,const QString &columnName){
    int columnCount=table->columnCount();
    for (int i=0;i<columnCount;++i){
        if (table->horizontalHeaderItem(i)->text()==columnName){
            return i;
        }
    }
    return -1;
}
void UserWidget::fnFillUserList()
{
    m_userManageTableW->setRowCount(0);
    std::vector<std::map<std::string, std::string>> fillContent=UserAuditManager::instance().get().getUsersInfo();
    for (int i = 0; i < int(fillContent.size()); i++)
    {
        if (fillContent[i]["Authority"].find("所有权限")!=std::string::npos 
            && fillContent[i]["UserAccount"].find("XXXXX")!=std::string::npos)
            continue;
        m_userManageTableW->insertRow(m_userManageTableW->rowCount());
        for (auto content : fillContent[i])
        {
            QString name = QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang, content.first));
            int nameColIndex = getColumnIndexByName(m_userManageTableW, name);
            if (nameColIndex < 0 || nameColIndex >= m_userManageTableW->columnCount())
                continue;
            if (content.first=="Authority" || content.first=="AccountManagement") {
                std::vector<std::string> authorities = SvcFactory::CreateCommonService()->SplitString(content.second, ';');
                std::string temp="";
                for (int j=0;j<int(authorities.size());j++){
                    temp+=SvcFactory::CreateConfigService()->LoadTranslation(m_lang, authorities[j])+';';
                }
                m_userManageTableW->setItem(m_userManageTableW->rowCount() - 1, nameColIndex, 
                    new QTableWidgetItem(QString::fromStdString(temp)));
            }
            else
                m_userManageTableW->setItem(m_userManageTableW->rowCount() - 1, nameColIndex, new QTableWidgetItem(QString::fromStdString(content.second)));
        }
    }
}
void UserWidget::fnReadDB()
{
    UserAuditManager::instance().get().loadUsersInfo();
    fnFillUserList();
}
void UserWidget::fnWriteDB()
{
    SystemDataManager::instance().get().saveSystemInfo();
    UserAuditManager::instance().get().saveUsersInfo();
    UserAuditManager::instance().get().saveUserGroupInfo();
}

UserWidget::~UserWidget()
{
    if (m_userLayout);{
        delete m_userLayout;
        m_userLayout=nullptr;
    }
    if (m_userManageLabel);{
        delete m_userManageLabel;
        m_userManageLabel=nullptr;
    }
    if (m_userManageTableW);{
        delete m_userManageTableW;
        m_userManageTableW=nullptr;
    }
    if (m_newUserLabel);{
        delete m_newUserLabel;
        m_newUserLabel=nullptr;
    }
    if (m_deleteUserLabel);{
        delete m_deleteUserLabel;
        m_deleteUserLabel=nullptr;
    }
    if (m_editUserLabel);{
        delete m_editUserLabel;
        m_editUserLabel=nullptr;
    }
    if (m_scanUserLabel);{
        delete m_scanUserLabel;
        m_scanUserLabel=nullptr;
    }
    if (m_userInfoEditWidget);{
        delete m_userInfoEditWidget;
        m_userInfoEditWidget=nullptr;
    }
}