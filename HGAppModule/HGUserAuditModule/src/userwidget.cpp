#include "userwidget.h"
#include <QHeaderView>
#include <QMessageBox>
#include "common.h"
#include <QDialog>
#include "globalsingleton.h"


UserWidget::UserWidget(std::string lang,QWidget *parent) : QWidget(parent),
m_lang(lang),
m_userInfoEditWidget(nullptr)
{
    std::string enterUsersManageName=GlobalSingleton::instance().getSystemInfo("enterUsersManageName");
    std::string authority = GlobalSingleton::instance().getUserAuthority(enterUsersManageName);
    permissionInfo = GlobalSingleton::instance().getAuthorityDetail(authority);

    fnInit();
    fnReadDB();
}
void UserWidget::fnInit()
{
    m_userLayout=new QGridLayout();
    this->setLayout(m_userLayout);
    
    // 
    m_userManageLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"UserManage")));
    QStringList headers={
        QString::fromStdString(loadTranslation(m_lang,"UserNo")),
        QString::fromStdString(loadTranslation(m_lang,"UserAccount")),
        QString::fromStdString(loadTranslation(m_lang,"UserJob")),
        QString::fromStdString(loadTranslation(m_lang,"Department")),
        QString::fromStdString(loadTranslation(m_lang,"Authority")),
        QString::fromStdString(loadTranslation(m_lang,"AccountManagement"))
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
            SAFE_DELETE(m_userInfoEditWidget);
        }
    }
    fnWriteDB();
    return true;
}

void UserWidget::slotHasUserNo(std::string userNo){
    std::vector<std::string> names=GlobalSingleton::instance().getUsersNo();
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
    std::vector<std::string> nos = GlobalSingleton::instance().getUsersNo();
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
    for (int i=0;i<int(GlobalSingleton::instance().getAuthorityInfo().size());i++)
    {
        if (GlobalSingleton::instance().getAuthorityField(i,"GroupName") == findTranslationKey(m_lang, authorityName))
        {
            if (GlobalSingleton::instance().getAuthorityField(i,"GroupPerson").find(text)!= std::string::npos) break;
            GlobalSingleton::instance().setAuthorityField(i,"GroupPerson", 
                GlobalSingleton::instance().getAuthorityField(i,"GroupPerson") + text + ":");
            std::string groupPerson = GlobalSingleton::instance().getAuthorityField(i,"GroupPerson");
            std::vector<std::string> persons = splitStr(groupPerson, ':');
            int count=0;
            for (int m=0;m<int(persons.size());m++){
                if (persons[m]=="") continue;
                count++;
            }
            GlobalSingleton::instance().setAuthorityField(i,"GroupPNumber",
                std::to_string(count));
            GlobalSingleton::instance().setAuthorityField(i,"LastModifyTime",getStandardCurTime());
            GlobalSingleton::instance().setAuthorityField(i,"LastModifier",GlobalSingleton::instance().getSystemInfo("enterUsersManageName"));
            break;
        }
    }
}
void UserWidget::fnRemoveUsrsGroupUser(const std::string &authorityName, const std::string &text)
{
    for (int i=0;i<int(GlobalSingleton::instance().getAuthorityInfo().size());i++)
    {
        if (GlobalSingleton::instance().getAuthorityField(i,"GroupName") == findTranslationKey(m_lang, authorityName))
        {
            std::vector<std::string> persons = splitStr(GlobalSingleton::instance().getAuthorityField(i,"GroupPerson"), ':');
            for (auto person : persons)
            {
                if (person == text)
                {
                    persons.erase(std::find(persons.begin(), persons.end(), person));
                    break;
                }
            }
            GlobalSingleton::instance().setAuthorityField(i,"GroupPerson","");
            for (auto person : persons)
            {
                GlobalSingleton::instance().addAuthorityField(i,"GroupPerson",person + ":");
            }
            std::string groupPerson = GlobalSingleton::instance().getAuthorityField(i,"GroupPerson");
            persons.clear();
            persons = splitStr(groupPerson, ':');
            int count = 0;
            for (int m=0;m<int(persons.size());m++){
                if (persons[m]=="") continue;
                count++;
            }
            GlobalSingleton::instance().setAuthorityField(i,"GroupPNumber",
                std::to_string(count));
            GlobalSingleton::instance().setAuthorityField(i,"LastModifyTime",getStandardCurTime());
            GlobalSingleton::instance().setAuthorityField(i,"LastModifier",GlobalSingleton::instance().getSystemInfo("enterUsersManageName"));
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
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), QString::fromStdString(loadTranslation(m_lang,"SelectOneRecord")));
        return;
    }

    std::string scanUserAccount = m_userManageTableW->item(row,1)->text().toStdString();
    int index = GlobalSingleton::instance().getUserFieldIndex(scanUserAccount);
    if (index < 0) return;

    removeWidgetsFromLayout(m_userLayout);
    m_userInfoEditWidget=new UserInfoEditWidget(USER_SCAN,m_lang,GlobalSingleton::instance().getUserInfo(index),this);
    m_userLayout->addWidget(m_userInfoEditWidget,0,0);
    connect(m_userInfoEditWidget,SIGNAL(signalUserInfo(const std::map<std::string,std::string> &)),
        this,SLOT(slotUserInfo(const std::map<std::string,std::string> &)));
    connect(m_userInfoEditWidget,SIGNAL(signalUserNo(std::string)),this,SLOT(slotHasUserNo(std::string)));
    connect(m_userInfoEditWidget,SIGNAL(signalBack()),this,SLOT(returnToList()));
    connect(m_userInfoEditWidget,SIGNAL(updateAuthority()),this,SLOT(slotUpdateAuthority()));
    RWDb::writeAuditTrailLog("查看用户"+GlobalSingleton::instance().getUserField(index,"UserAccount"));
}
void UserWidget::editUser(){
    int row=getSelectedRow(m_userManageTableW);
    if (row<0||row>=m_userManageTableW->rowCount()) {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), QString::fromStdString(loadTranslation(m_lang,"SelectOneRecord")));
        return;
    }
    std::string editUserAccount=m_userManageTableW->item(row,1)->text().toStdString(); 
    int index = GlobalSingleton::instance().getUserFieldIndex(editUserAccount);
    if (index < 0) return;
    std::string editauthority = GlobalSingleton::instance().getUserAuthority(editUserAccount);
    if (editauthority.find("所有权限")!=std::string::npos){
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "厂家账号不可编辑");
        return;
    }
    removeWidgetsFromLayout(m_userLayout);
    m_userInfoEditWidget=new UserInfoEditWidget(USER_RECTIFY,m_lang,GlobalSingleton::instance().getUserInfo(index),this);
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
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), QString::fromStdString(loadTranslation(m_lang,"SelectOneRecord")));
        return;
    }
    std::string delUserAccount = m_userManageTableW->item(row,1)->text().toStdString();
    int index = GlobalSingleton::instance().getUserFieldIndex(delUserAccount);
    if (index < 0) return;
    std::string delauthority = GlobalSingleton::instance().getUserAuthority(delUserAccount);
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
    std::string authority=GlobalSingleton::instance().getUserField(index,"Authority");
    GlobalSingleton::instance().deleteUserInfo(delUserAccount);
    
    std::vector<std::string> authorities = splitStr(authority,';');
    for (int i=0;i<int(authorities.size());i++){
        if (authorities[i]=="") continue;
        fnRemoveUsrsGroupUser(authorities[i], delUserAccount);
    }
    RWDb::writeAuditTrailLog("删除用户:"+delUserAccount);
    if (GlobalSingleton::instance().getSystemInfo("loginNames").find(delUserAccount)!=std::string::npos){
        std::string loginNames = GlobalSingleton::instance().getSystemInfo("loginNames");
        std::string target=delUserAccount+':';
        size_t pos =loginNames.find(target);
        if (pos!=std::string::npos){
            loginNames.erase(pos,target.length());
        }
        GlobalSingleton::instance().setSystemInfo("loginNames",loginNames);
    }
}
void UserWidget::returnToList(){
    if (m_userInfoEditWidget) {
        disconnect(m_userInfoEditWidget, nullptr, this, nullptr);
        SAFE_DELETE(m_userInfoEditWidget);
    }
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
    RWDb::writeAuditTrailLog("创建用户");
}
void UserWidget::slotUpdateAuthority(){
    emit updateAuthority();
}
void UserWidget::slotUserInfo(const std::map<std::string,std::string>& userInfo){
    int index=-1;
    std::vector<std::map<std::string,std::string>> fillContent=GlobalSingleton::instance().getUsersInfo();
    for (int i=0;i<int(fillContent.size());i++){
        if (fillContent[i].at("UserNo")==userInfo.at("UserNo")){
            index=i;
            break;
        }
    }
    std::string authority="";
    if (index==-1) {
        m_userManageTableW->insertRow(m_userManageTableW->rowCount());
        GlobalSingleton::instance().addUserRecord(userInfo);
        index=m_userManageTableW->rowCount()-1;
        authority = userInfo.at("Authority");
    } else {
        authority=GlobalSingleton::instance().getUserField(index,"Authority");
        GlobalSingleton::instance().setUserInfoWithIndex(index,userInfo);
    }
    for (auto content : userInfo)
    {
        QString name=QString::fromStdString(loadTranslation(m_lang,content.first));
        int nameColIndex = getColumnIndexByName(m_userManageTableW, name);
        if (nameColIndex < 0 || nameColIndex >= m_userManageTableW->columnCount())
            continue;
        if (content.first=="Authority" || content.first=="AccountManagement") 
            m_userManageTableW->setItem(index, nameColIndex, 
                new QTableWidgetItem(QString::fromStdString(loadTranslation(m_lang,content.second))));
        else 
            m_userManageTableW->setItem(index, nameColIndex, 
                new QTableWidgetItem(QString::fromStdString(content.second)));
    }

    fnAddUsrsGroupUser(authority, userInfo.at("UserAccount"));
    QMessageBox::information(this, QString::fromStdString(HG_DEVICE_NAME), "账户保存完成");
    RWDb::writeAuditTrailLog(userInfo.at("UserAccount")+"账户保存完成");
    fnWriteDB();
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
    std::vector<std::map<std::string, std::string>> fillContent=GlobalSingleton::instance().getUsersInfo();
    for (int i = 0; i < int(fillContent.size()); i++)
    {
        if (fillContent[i]["Authority"].find("所有权限")!=std::string::npos 
            && fillContent[i]["UserAccount"].find("XXXXX")!=std::string::npos)
            continue;
        m_userManageTableW->insertRow(m_userManageTableW->rowCount());
        for (auto content : fillContent[i])
        {
            QString name = QString::fromStdString(loadTranslation(m_lang, content.first));
            int nameColIndex = getColumnIndexByName(m_userManageTableW, name);
            if (nameColIndex < 0 || nameColIndex >= m_userManageTableW->columnCount())
                continue;
            if (content.first=="Authority" || content.first=="AccountManagement") {
                std::vector<std::string> authorities = splitStr(content.second, ';');
                std::string temp="";
                for (int j=0;j<int(authorities.size());j++){
                    temp+=loadTranslation(m_lang, authorities[j])+';';
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
    GlobalSingleton::instance().loadUsersInfo();
    fnFillUserList();
}
void UserWidget::fnWriteDB()
{
    GlobalSingleton::instance().saveSystemInfo();
    GlobalSingleton::instance().saveUsersInfo();
}

UserWidget::~UserWidget()
{
    SAFE_DELETE(m_userLayout);
    SAFE_DELETE(m_userManageLabel);
    SAFE_DELETE(m_userManageTableW);
    SAFE_DELETE(m_newUserLabel);
    SAFE_DELETE(m_deleteUserLabel);
    SAFE_DELETE(m_editUserLabel);
    SAFE_DELETE(m_scanUserLabel);
    SAFE_DELETE(m_userInfoEditWidget);
}