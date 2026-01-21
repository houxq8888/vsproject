#include "authoritywidget.h"
#include <QHeaderView>
#include <QMessageBox>
#include "common.h"
#include <QDialog>
#include "globalsingleton.h"


AuthorityWidget::AuthorityWidget(std::string lang,const std::vector<std::string> &wholeAuthority,QWidget *parent) : QWidget(parent),
m_lang(lang),
m_whole_authority(wholeAuthority),
m_setAuthorityFlag(false)
{
    GlobalSingleton::instance().setWholeAuthority(m_whole_authority);
    std::string enterUsersManageName=GlobalSingleton::instance().getSystemInfo("enterUsersManageName");
    std::string authority = GlobalSingleton::instance().getUserAuthority(enterUsersManageName);
    permissionInfo = GlobalSingleton::instance().getAuthorityDetail(authority);

    m_curPhase = USER_INIT;
    fnInit();
    fnReadDB();
    fnFillAuthorityInfo();
}
void AuthorityWidget::fnInit()
{
    m_usersLayout=new QGridLayout();
    this->setLayout(m_usersLayout);
    
    m_usersAuthorityInfoW=new QTableWidget(3,6);
    m_usersAuthorityInfoW->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_usersAuthorityInfoW->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_usersAuthorityInfoW->verticalHeader()->setVisible(false);
    m_usersAuthorityInfoW->horizontalHeader()->setVisible(false);
    m_usersAuthorityInfoW->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_usersAuthorityInfoW->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_usersAuthorityInfoW->resizeRowsToContents();
    m_usersAuthorityInfoW->setMouseTracking(true);
    m_usersAuthorityInfoW->setSpan(0,0,1,m_usersAuthorityInfoW->columnCount());
    QTableWidgetItem *headerItem = new QTableWidgetItem("用户组权限");
    headerItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    headerItem->setBackground(QBrush(QColor(173,216,230)));
    headerItem->setFont(QFont("Arial",24));
    m_usersAuthorityInfoW->setItem(0,0,headerItem);

    m_selfDefineBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"SelfDefine")));
    m_moreInfoBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"MoreInfo")));
    m_backBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Back")));

    connect(m_selfDefineBtn,SIGNAL(clicked()),this,SLOT(clickSelfDefine()));
    connect(m_moreInfoBtn,SIGNAL(clicked()),this,SLOT(clickMoreInfo()));
    connect(m_backBtn,SIGNAL(clicked()),this,SLOT(clickBack()));

    m_usersLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"Users")));
    m_usersComboBox=new QComboBox();


#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    connect(m_usersComboBox,&QComboBox::activated,this,
        &AuthorityWidget::onUsersComboBoxChanged);
#else
    connect(m_usersComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated),
            this, &AuthorityWidget::onUsersComboBoxChanged);
#endif

    m_userLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"GroupUsers")));
    m_addUserBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"AddUser")));
    m_removeUserBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"DeleteUser")));
    connect(m_addUserBtn,&QPushButton::clicked,this,&AuthorityWidget::slotAddUser);
    connect(m_removeUserBtn,&QPushButton::clicked,this,&AuthorityWidget::slotRemoveUser);

    m_usersAuthorityTableW=new QTableWidget(8,8);
    m_usersAuthorityTableW->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_usersAuthorityTableW->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_usersAuthorityTableW->verticalHeader()->setVisible(false);
    m_usersAuthorityTableW->horizontalHeader()->setVisible(false);
    m_usersAuthorityTableW->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(m_usersAuthorityTableW,&QTableWidget::cellClicked,this,&AuthorityWidget::onUsersAuthorityClicked);
    QString data[1][8]={
        {
            QString::fromStdString(loadTranslation(m_lang,"Authority")),
            QString::fromStdString(loadTranslation(m_lang,"Select")),
            QString::fromStdString(loadTranslation(m_lang,"Authority")),
            QString::fromStdString(loadTranslation(m_lang,"Select")),
            QString::fromStdString(loadTranslation(m_lang,"Authority")),
            QString::fromStdString(loadTranslation(m_lang,"Select")),
            QString::fromStdString(loadTranslation(m_lang,"Authority")),
            QString::fromStdString(loadTranslation(m_lang,"Select"))
        }
    };
    for (int row=0;row<1;row++){
        for (int col=0;col<8;col++){
            QTableWidgetItem* item=new QTableWidgetItem(data[row][col]);
            m_usersAuthorityTableW->setItem(row,col,item);
            if (row==0) item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            if (col%2!=0) item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        }
    }
    std::vector<std::string> permissionInfoTemp = GlobalSingleton::instance().getWholeAuthority();
    for (int mm=0;mm<int(permissionInfoTemp.size());mm++){
        int row = mm / 4 + 1;
        int col = (2*mm)%8;
        QString text = QString::fromStdString(loadTranslation(m_lang,permissionInfoTemp[mm]));
        QTableWidgetItem* item=new QTableWidgetItem(text);
        m_usersAuthorityTableW->setItem(row,col,item);
        col = (2*mm+1)%8;
        text = "";
        item = new QTableWidgetItem(text);
        m_usersAuthorityTableW->setItem(row,col,item);
    }
    m_userTableW=new QTableWidget(4,6);
    m_userTableW->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_userTableW->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_userTableW->verticalHeader()->setVisible(false);
    m_userTableW->horizontalHeader()->setVisible(false);
    m_userTableW->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_userTableW->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_userTableW->resizeRowsToContents();
    m_userTableW->setMouseTracking(true);

    m_setAuthorityBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Set")));
    connect(m_setAuthorityBtn,&QPushButton::clicked,this,&AuthorityWidget::setAuthority);

    // 
    m_scanAuthorityInfoBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"ScanAuthorityInfo")));
    connect(m_scanAuthorityInfoBtn,&QPushButton::clicked,this,&AuthorityWidget::slotScanAuthorityInfo);

    m_usersLayout->addWidget(m_usersLabel,0,0);
    m_usersLayout->addWidget(m_usersComboBox,0,1);
    m_usersLayout->addWidget(m_usersAuthorityInfoW,1,0,1,9);
    m_usersLayout->addWidget(m_userLabel,2,0);
    m_usersLayout->addWidget(m_addUserBtn,3,6,2,1);
    m_usersLayout->addWidget(m_removeUserBtn,5,6,2,1);
    m_usersLayout->addWidget(m_scanAuthorityInfoBtn,3,7,2,1);
    m_usersLayout->addWidget(m_userTableW,3,0,4,5);
}
void AuthorityWidget::closeEvent(QCloseEvent *event) {    
    // 检查是否处于编辑模式，如果是则回滚未确认的修改
    if (GlobalSingleton::instance().isAuthorityEditing()) {
        GlobalSingleton::instance().rollbackAuthorityEdit();
    }
    
    fnWriteDB();
    event->accept();
}

void AuthorityWidget::clickBack(){
    if (m_curPhase==USER_AUTHORITY){
        m_usersLayout->removeWidget(m_backBtn);
        m_usersLayout->removeWidget(m_usersAuthorityTableW);
        m_usersLayout->removeWidget(m_selfDefineBtn);
        m_usersLayout->removeWidget(m_setAuthorityBtn);
        m_usersLayout->removeWidget(m_moreInfoBtn);

        m_backBtn->hide();
        m_usersAuthorityTableW->hide();
        m_selfDefineBtn->hide();
        m_setAuthorityBtn->hide();
        m_moreInfoBtn->hide();

        m_usersLayout->addWidget(m_usersAuthorityInfoW,1,0,1,9);
        m_usersLayout->addWidget(m_userLabel,2,0);
        m_usersLayout->addWidget(m_userTableW,3,0,4,5);
        m_usersLayout->addWidget(m_addUserBtn,3,6,2,1);
        m_usersLayout->addWidget(m_removeUserBtn,5,6,2,1);
        m_usersLayout->addWidget(m_scanAuthorityInfoBtn,3,7,2,1);

        m_usersAuthorityInfoW->show();
        m_userLabel->show();
        m_userTableW->show();
        m_addUserBtn->show();
        m_removeUserBtn->show();
        m_scanAuthorityInfoBtn->show();
        m_curPhase = USER_INIT;
        if (!m_setAuthorityFlag) fnFillAuthorityInfo();
    } else if (m_curPhase==USER_SCAN_USERS) {
        removeWidgetsFromLayout(m_usersLayout);
        m_usersLayout->addWidget(m_usersLabel,0,0);
        m_usersLayout->addWidget(m_usersComboBox,0,1);
        m_usersLabel->show();
        m_usersComboBox->show();
        m_usersLabel->setText(QString::fromStdString(loadTranslation(m_lang,"Users")));
        slotScanAuthorityInfo();
    }
}
bool AuthorityWidget::fnHasUserManageAuthority()
{
    std::string  enterUsersManageName=GlobalSingleton::instance().getSystemInfo("enterUsersManageName");
    std::string authority=GlobalSingleton::instance().getUserAuthority(enterUsersManageName);
    std::vector<std::string> userPermission = GlobalSingleton::instance().getAuthorityDetail(authority);

    if (GlobalSingleton::instance().getSystemInfo("免密登录")!="true"&&
        std::find(userPermission.begin(),userPermission.end(),"UserManage")==userPermission.end()){
        std::ostringstream ss;
        QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),"没有"+QString::fromStdString(loadTranslation(m_lang,"UserManage"))+"权限，请联系管理员开通！");
        RWDb::writeAuditTrailLog("没有"+loadTranslation(m_lang,"UserManage")+"权限，请联系管理员开通！");
        return false;
    }
    return true;
}
bool AuthorityWidget::fnHasUsersManageAuthority()
{
    return true;
    std::string  enterUsersManageName=GlobalSingleton::instance().getSystemInfo("enterUsersManageName");
    std::string authority=GlobalSingleton::instance().getUserAuthority(enterUsersManageName);
    std::vector<std::string> userPermission = GlobalSingleton::instance().getAuthorityDetail(authority);

    if (GlobalSingleton::instance().getSystemInfo("免密登录")!="true"&&
        std::find(userPermission.begin(),userPermission.end(),"UsersManage")==userPermission.end()){
        std::ostringstream ss;
        QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),"没有"+QString::fromStdString(loadTranslation(m_lang,"UsersManage"))+"权限，请联系管理员开通！");
        RWDb::writeAuditTrailLog("没有"+loadTranslation(m_lang,"UsersManage")+"权限，请联系管理员开通！");
        return false;
    }
    return true;
}

AuthorityWidget::~AuthorityWidget()
{
    SAFE_DELETE(m_usersLayout);
    SAFE_DELETE(m_usersAuthorityInfoW);
    SAFE_DELETE(m_selfDefineBtn);
    SAFE_DELETE(m_moreInfoBtn);
    SAFE_DELETE(m_usersLabel);
    SAFE_DELETE(m_usersComboBox);
    SAFE_DELETE(m_userLabel);
    SAFE_DELETE(m_addUserBtn);
    SAFE_DELETE(m_removeUserBtn);
    SAFE_DELETE(m_usersAuthorityTableW);
    SAFE_DELETE(m_userTableW);
    SAFE_DELETE(m_setAuthorityBtn);
    SAFE_DELETE(m_backBtn);
    SAFE_DELETE(m_scanAuthorityInfoBtn);
}
void AuthorityWidget::slotScanAuthorityInfo()
{
    if (!fnHasUsersManageAuthority()) return;

    m_usersLayout->removeWidget(m_usersAuthorityInfoW); 
    m_usersLayout->removeWidget(m_userLabel);
    m_usersLayout->removeWidget(m_userTableW);
    m_usersLayout->removeWidget(m_addUserBtn);
    m_usersLayout->removeWidget(m_removeUserBtn);
    m_usersLayout->removeWidget(m_scanAuthorityInfoBtn);

    m_usersAuthorityInfoW->hide();
    m_userLabel->hide();
    m_userTableW->hide();
    m_addUserBtn->hide();
    m_removeUserBtn->hide();
    m_scanAuthorityInfoBtn->hide();


    m_usersLayout->addWidget(m_backBtn,0,8);
    m_usersLayout->addWidget(m_usersAuthorityTableW,1,0,1,9);
    m_usersLayout->addWidget(m_selfDefineBtn,2,7);
    m_usersLayout->addWidget(m_setAuthorityBtn,2,8);
    m_usersLayout->addWidget(m_moreInfoBtn,2,10);

    m_backBtn->show();
    m_usersAuthorityTableW->show();
    m_selfDefineBtn->show();
    m_setAuthorityBtn->show();
    m_moreInfoBtn->show();

    m_curPhase = USER_AUTHORITY;
}
bool AuthorityWidget::closeWindow()
{
    // 如果处于编辑模式且有未保存的修改，询问用户是否保存
    fnWriteDB();
    return true;
}

void AuthorityWidget::slotAddUser(){
    QDialog dialog(this);
    dialog.setWindowTitle(QString::fromStdString(loadTranslation(m_lang,"AddUser")));
    dialog.setWindowModality(Qt::ApplicationModal);
    QListWidget* listW=new QListWidget(&dialog);
    QPushButton* okbtn=new QPushButton(&dialog);
    okbtn->setText(QString::fromStdString(loadTranslation(m_lang,"Ok")));//"确定");
    QPushButton* cancelbtn=new QPushButton(&dialog);
    cancelbtn->setText(QString::fromStdString(loadTranslation(m_lang,"Cancel")));//"取消");

    std::string addUserName;
    connect(okbtn,&QPushButton::clicked,[&](){
       if (listW->count()!=0) addUserName=listW->currentItem()->text().toStdString();
       dialog.close();
    });
    connect(cancelbtn,&QPushButton::clicked,[&](){
        dialog.close();
    });
    std::vector<std::string> names=GlobalSingleton::instance().getUsersNo();
    for (const auto &name:names){
        listW->addItem(QString::fromStdString(name));
    }

    QVBoxLayout *layout=new QVBoxLayout(&dialog);
    layout->addWidget(listW);
    layout->addWidget(okbtn);
    layout->addWidget(cancelbtn);
    dialog.setLayout(layout);
    dialog.exec();

    if (addUserName!=""){
        bool flag=false;
        int index=findUserAuthorityIndex(addUserName);
        if (loadTranslation(m_lang, GlobalSingleton::instance().getUserField(index, "Authority")) 
            != m_usersComboBox->currentText().toStdString())
        {
            std::string selectAuthority=findTranslationKey(m_lang,m_usersComboBox->currentText().toStdString());
            if (selectAuthority == "Manager") {
                if (GlobalSingleton::instance().getSystemInfo("免密登录") != "true" &&
                    std::find(permissionInfo.begin(), permissionInfo.end(), "UserManage(Manager)") == permissionInfo.end())
                {
                    std::ostringstream ss;
                    QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "该用户没有权限创建管理员，请联系管理员开通!");
                    RWDb::writeAuditTrailLog("该用户没有权限创建管理员，请联系管理员开通!");
                    return;
                }
            } else if (selectAuthority != "SystemManager"&&selectAuthority!="Manager"&&selectAuthority!="所有权限")
            {
                if (GlobalSingleton::instance().getSystemInfo("免密登录") != "true" &&
                    std::find(permissionInfo.begin(), permissionInfo.end(), "UserManage(Users)") == permissionInfo.end())
                {
                    std::ostringstream ss;
                    QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "该用户没有权限创建非管理员用户，请联系管理员开通！");
                    RWDb::writeAuditTrailLog("该用户没有权限创建非管理员用户，请联系管理员开通！");
                    return;
                }
            } else if (selectAuthority=="所有权限"){
                QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "不能创建所有权限账号!");
                RWDb::writeAuditTrailLog("不能创建所有权限账号!");
                return;
            } else if (selectAuthority=="SystemManager"){
                std::string enterUsersManageName=GlobalSingleton::instance().getSystemInfo("enterUsersManageName");
                std::string authority = GlobalSingleton::instance().getUserAuthority(enterUsersManageName);
                if (authority.find("所有权限")==std::string::npos){
                    QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "只有厂家账号才能创建超级管理员账号!");
                    RWDb::writeAuditTrailLog("只有厂家账号才能创建超级管理员账号!");
                    return;
                }
            }
            if (QMessageBox::Yes==QMessageBox::question(this,QString::fromStdString(HG_DEVICE_NAME),
                QString::fromStdString(loadTranslation(m_lang,"SureToChangeAccoundAuthority")),
                QMessageBox::Yes|QMessageBox::No))
            {
                std::string attributeName=GlobalSingleton::instance().getUserField(index,"Authority");

                QString name = QString::fromStdString(loadTranslation(m_lang, "Authority"));
                if (splitStr(GlobalSingleton::instance().getUserField(index,"Authority"),';').size() <=1)
                    GlobalSingleton::instance().addUserField(index,"Authority",';'+findTranslationKey(m_lang,m_usersComboBox->currentText().toStdString())+';');
                else 
                    GlobalSingleton::instance().addUserField(index,"Authority",findTranslationKey(m_lang,m_usersComboBox->currentText().toStdString())+';');
                GlobalSingleton::instance().setUserField(index,"LastModifyTime",getStandardCurTime());
                GlobalSingleton::instance().setUserField(index,"LastModifier",GlobalSingleton::instance().getSystemInfo("enterUsersManageName"));
                if (addUserName == GlobalSingleton::instance().getSystemInfo("loginName")){
                    GlobalSingleton::instance().setSystemInfo("authority",GlobalSingleton::instance().getUserField(index,"Authority"));
                    emit updateAuthority();
                }
                flag=true;
            } else {
                flag=false;
            }
        }
        else
            flag = true;
        if (!flag) return;
        int row=0;
        int col=0;
        findNextRowAndCol(m_userTableW,addUserName,row,col);
        if (row==-1&&col==-1) return;
        m_userTableW->setItem(row,col,new QTableWidgetItem(QString::fromStdString(addUserName)));
        std::string attributeName=findTranslationKey(m_lang,m_usersComboBox->currentText().toStdString());
 
        RWDb::writeAuditTrailLog(loadTranslation(m_lang,attributeName)+" 增加账户 ["+addUserName+"] ");
        fnAddUsrsGroupUser(m_usersComboBox->currentText().toStdString(),addUserName);
    }
}
int  AuthorityWidget::findUserAuthorityIndex(const std::string &userNo){
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
void AuthorityWidget::findNextRowAndCol(QTableWidget* tableW,std::string key,int& row,int& col){
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
void AuthorityWidget::fnAddAuthorityItem(const std::string &authorityName, const std::string &text){
    for (int i=0;i<int(GlobalSingleton::instance().getAuthorityInfo().size());i++)
    {
        if (GlobalSingleton::instance().getAuthorityField(i,"GroupName") == findTranslationKey(m_lang, authorityName))
        {
            if (GlobalSingleton::instance().getAuthorityField(i,"Authority").find(text)!= std::string::npos) break;
            GlobalSingleton::instance().setAuthorityField(i,"Authority", 
                GlobalSingleton::instance().getAuthorityField(i,"Authority") + (text + ":"));
            GlobalSingleton::instance().setAuthorityField(i,"LastModifyTime", getStandardCurTime());
            GlobalSingleton::instance().setAuthorityField(i,"LastModifier",GlobalSingleton::instance().getSystemInfo("enterUsersManageName"));
            break;
        }
    }
}
void AuthorityWidget::fnAddUsrsGroupUser(const std::string &authorityName, const std::string &text)
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
void AuthorityWidget::fnRemoveAuthorityItem(const std::string &authorityName, const std::string &text)
{
     for (int i=0;i<int(GlobalSingleton::instance().getAuthorityInfo().size());i++)
    {
        if (GlobalSingleton::instance().getAuthorityField(i,"GroupName") == findTranslationKey(m_lang, authorityName))
        {
            if (GlobalSingleton::instance().getAuthorityField(i,"Authority").find(text)== std::string::npos) break;
            std::vector<std::string> authoritys = splitStr(GlobalSingleton::instance().getAuthorityField(i,"Authority"), ':');
            for (auto authority : authoritys)
            {
                if (authority == text)
                {
                    authoritys.erase(std::find(authoritys.begin(), authoritys.end(), authority));
                    break;
                }
            }
            GlobalSingleton::instance().setAuthorityField(i,"Authority","");
            for (auto authority : authoritys)
            {
                GlobalSingleton::instance().addAuthorityField(i,"Authority", authority + ":");
            }
            GlobalSingleton::instance().setAuthorityField(i,"LastModifyTime",getStandardCurTime());
            GlobalSingleton::instance().setAuthorityField(i,"LastModifier",GlobalSingleton::instance().getSystemInfo("enterUsersManageName"));
            break;
        }
    }
}
void AuthorityWidget::fnRemoveUsrsGroupUser(const std::string &authorityName, const std::string &text)
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
void AuthorityWidget::slotRemoveUser(){
    std::string attributeName=findTranslationKey(m_lang,m_usersComboBox->currentText().toStdString());
    // remove 
    int col=m_userTableW->currentColumn();
    int row=m_userTableW->currentRow();
    if ((row<0||row>=m_userTableW->rowCount()) &&
        (col<0||col>=m_userTableW->columnCount()))
    {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), 
            QString::fromStdString(loadTranslation(m_lang,"SelectOneRecord")));
        return;
    }
    if (m_userTableW->item(row,col)){
        std::string text=m_userTableW->item(row,col)->text().toStdString();
        int index = findUserAuthorityIndex(text);

        std::string selectAuthority = findTranslationKey(m_lang, m_usersComboBox->currentText().toStdString());
        if (selectAuthority == "Manager")
        {
            if (GlobalSingleton::instance().getSystemInfo("免密登录") != "true" &&
                std::find(permissionInfo.begin(), permissionInfo.end(), "UserManage(Manager)") == permissionInfo.end())
            {
                std::ostringstream ss;
                QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "该用户没有权限移除管理员，请联系管理员开通!");
                RWDb::writeAuditTrailLog("该用户没有权限移除管理员，请联系管理员开通!");
                return;
            }
        }
        else if (selectAuthority != "SystemManager" && selectAuthority != "Manager" && selectAuthority != "所有权限")
        {
            if (GlobalSingleton::instance().getSystemInfo("免密登录") != "true" &&
                std::find(permissionInfo.begin(), permissionInfo.end(), "UserManage(Users)") == permissionInfo.end())
            {
                std::ostringstream ss;
                QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "该用户没有权限移除非管理员用户，请联系管理员开通！");
                RWDb::writeAuditTrailLog("该用户没有权限移除非管理员用户，请联系管理员开通！");
                return;
            }
        }
        else if (selectAuthority == "所有权限")
        {
            QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "不能移除所有权限账号!");
            RWDb::writeAuditTrailLog("不能移除所有权限账号!");
            return;
        }
       

        if (QMessageBox::Yes == QMessageBox::question(this,QString::fromStdString(HG_DEVICE_NAME),
            QString::fromStdString(loadTranslation(m_lang,"SureToChangeAccoundAuthority")),
            QMessageBox::Yes|QMessageBox::No))
        {
            m_userTableW->item(row,col)->setText("");
            RWDb::writeAuditTrailLog(loadTranslation(m_lang,attributeName)+" 删除账户 ["+text+"] ");
            fnRemoveUsrsGroupUser(m_usersComboBox->currentText().toStdString(),text);
            std::string authorityTemp = GlobalSingleton::instance().getUserField(index,"Authority");
            std::vector<std::string> authorities = splitStr(authorityTemp,';');
            authorityTemp = "";
            for (int mm=0;mm<int(authorities.size());mm++){
                if (authorities[mm]=="") continue;
                if (authorities[mm]==findTranslationKey(m_lang,m_usersComboBox->currentText().toStdString())) continue;
                authorityTemp +=loadTranslation(m_lang,authorities[mm])+';';
            }
            GlobalSingleton::instance().setUserField(index,"Authority",authorityTemp);
            if (text==GlobalSingleton::instance().getSystemInfo("loginName")){
                GlobalSingleton::instance().setSystemInfo("authority",GlobalSingleton::instance().getUserField(index,"Authority"));
                emit updateAuthority();
            }
            GlobalSingleton::instance().setUserField(index,"LastModifyTime",getStandardCurTime());
            GlobalSingleton::instance().setUserField(index,"LastModifier",GlobalSingleton::instance().getSystemInfo("enterUsersManageName"));
        }
    }
}
int AuthorityWidget::getColumnIndexByName(QTableWidget* table,const QString &columnName){
    int columnCount=table->columnCount();
    for (int i=0;i<columnCount;++i){
        if (table->horizontalHeaderItem(i)->text()==columnName){
            return i;
        }
    }
    return -1;
}
void AuthorityWidget::fnFillAuthorityInfo()
{
    // clear
    for (int row = 1; row < m_usersAuthorityInfoW->rowCount(); ++row)
    {
        for (int col = 0; col < m_usersAuthorityInfoW->columnCount(); ++col)
        {
            QTableWidgetItem *item = m_usersAuthorityInfoW->item(row, col);
            if (item)
                item->setText("");
        }
    }
    std::string authorityName = m_usersComboBox->currentText().toStdString();
    std::vector<std::map<std::string,std::string>> authorityIfno=GlobalSingleton::instance().getAuthorityInfo();
    for (auto info : authorityIfno)
    {
        if (info.at("GroupName") == findTranslationKey(m_lang, authorityName))
        {
            std::vector<std::string> resultvalue;
            resultvalue = splitStr(info.at("Authority"), ':');
            resultvalue.erase(std::remove(resultvalue.begin(),resultvalue.end(),""),resultvalue.end());

            int row = 0, col = 0;
            for (int i = 0; i < int(resultvalue.size()); i++)
            {
                if (resultvalue[i] == "")
                    continue;
                row = i / 6 + 1;
                col = i % 6;
                if (row >= m_usersAuthorityInfoW->rowCount())
                {
                    m_usersAuthorityInfoW->insertRow(row);
                }
                QTableWidgetItem *item = m_usersAuthorityInfoW->item(row, col);
                if (item)
                    m_usersAuthorityInfoW->item(row, col)->setText(QString::fromStdString(loadTranslation(m_lang, resultvalue[i])));
                else
                    m_usersAuthorityInfoW->setItem(row, col, new QTableWidgetItem(QString::fromStdString(loadTranslation(m_lang, resultvalue[i]))));
            }
            break;
        }
    }
}
void AuthorityWidget::fnFillAuthorityList()
{
    m_userTableW->clearContents();
    fnResetAuthorityToEnable();
    std::string authorityName = m_usersComboBox->currentText().toStdString();
    for (auto info : GlobalSingleton::instance().getAuthorityInfo())
    {
        if (info.at("GroupName") == findTranslationKey(m_lang, authorityName))
        {
            std::vector<std::string> resultvalue = splitStr(info.at("Authority"), ':');
            resultvalue.erase(resultvalue.begin() + resultvalue.size() - 1);

            // 清空所有勾选标记
            for (int row = 1; row < m_usersAuthorityTableW->rowCount(); row++) {
                for (int col = 0; col < m_usersAuthorityTableW->columnCount(); col++) {
                    if (col % 2 != 0) { // 只处理勾选框列
                        QTableWidgetItem* item = m_usersAuthorityTableW->item(row, col);
                        if (item) {
                            item->setText("");
                        }
                    }
                }
            }
             // 根据权限列表设置勾选标记
            for (int i = 0; i < int(resultvalue.size()); i++)
            {
                if (resultvalue[i].empty()) continue;
                
                bool found = false;
                for (int row = 1; row < m_usersAuthorityTableW->rowCount(); row++)
                {
                    for (int col = 0; col < m_usersAuthorityTableW->columnCount(); col++)
                    {
                        if (col % 2 != 0) continue; // 跳过勾选框列
                        
                        QTableWidgetItem* item = m_usersAuthorityTableW->item(row, col);
                        if (item && !item->text().isEmpty())
                        {
                            std::string itemText = findTranslationKey(m_lang, item->text().toStdString());
                            if (itemText == resultvalue[i])
                            {
                                QTableWidgetItem* checkItem = m_usersAuthorityTableW->item(row, col + 1);
                                if (checkItem) {
                                    checkItem->setText("✔");
                                    checkItem->setTextAlignment(Qt::AlignCenter);
                                }
                                found = true;
                                break;
                            }
                        }
                    }
                    if (found) break;
                }
            }
            int row = 0, col = 0;
            // else if (resultkey[1] == "Authority")
            {
                for (int i = 0; i < int(resultvalue.size()); i++)
                {
                    if (resultvalue[i] == "")
                        continue;
                    bool flag = false;
                    for (row = 1; row < m_usersAuthorityTableW->rowCount(); row++)
                    {
                        for (col = 0; col < m_usersAuthorityTableW->columnCount(); col++)
                        {
                            if (col % 2 != 0)
                                continue;
                            if (m_usersAuthorityTableW->item(row, col) == NULL)
                                continue;
                            if (m_usersAuthorityTableW->item(row, col)->text().toStdString() ==
                                loadTranslation(m_lang, resultvalue[i]))
                            {
                                m_usersAuthorityTableW->item(row, col + 1)->setText("✔");
                                m_usersAuthorityTableW->item(row, col + 1)->setTextAlignment(Qt::AlignCenter);
                                flag = true;
                                break;
                            }
                        }
                        if (flag)
                            break;
                    }
                }
                fnInitAuthorityTable();
            }
            resultvalue.clear();
            resultvalue = splitStr(info.at("GroupPerson"), ':');
            resultvalue.erase(resultvalue.begin() + resultvalue.size() - 1);
            row=0; col=0;
            // if (resultkey[1] == "Account")
            {
                for (int i = 0; i < int(resultvalue.size()); i++)
                {
                    if (resultvalue[i] == "")
                        continue;
                    m_userTableW->setItem(row, col, new QTableWidgetItem(QString::fromStdString(resultvalue[i])));
                    col++;
                    if (col >= m_userTableW->columnCount())
                    {
                        col = 0;
                        row++;
                    }
                    if (row >= m_userTableW->rowCount())
                    {
                        continue;
                    }
                }
            }
        }
    }
}
void AuthorityWidget::fnResetAuthorityToEnable(){
    std::string usersType = findTranslationKey(m_lang, m_usersComboBox->currentText().toStdString());
    int row,col;
    for (row = 1; row < m_usersAuthorityTableW->rowCount(); row++){
        for (col=0; col < m_usersAuthorityTableW->columnCount(); col++){
            if (col % 2 == 0)
                continue;
            if (m_usersAuthorityTableW->item(row,col)==NULL) continue;
            if (m_usersAuthorityTableW->item(row, col - 1)==NULL) continue;
            for (int mm=0;mm<2;mm++){
                QTableWidgetItem *item = m_usersAuthorityTableW->item(row, col-mm);
                std::string itemText = findTranslationKey(m_lang,m_usersAuthorityTableW->item(row,col-1)->text().toStdString());
                // if (usersType == "Auditor"){
                //     if (itemText!="ScanAuditTrailRecord")
                //         continue;
                // } else if (usersType == "SystemManager"){

                // } else if (usersType == "Manager"){

                // } else {

                // }
                // recover
                item->setFlags(item->flags() | Qt::ItemIsEditable);
                // reset
                item->setForeground(QBrush(Qt::black));
                // reset to default
                item->setBackground(QBrush());
            }
        }
    }
}
void AuthorityWidget::fnInitAuthorityTable(){
    int row,col;
    for (row = 1; row < m_usersAuthorityTableW->rowCount(); row++){ 
        for (col=0; col < m_usersAuthorityTableW->columnCount(); col++){ 
            if (col % 2 == 0)
                continue;
            if (m_usersAuthorityTableW->item(row, col) == NULL) continue;
            if (m_usersAuthorityTableW->item(row, col - 1) == NULL) continue;
            if (m_usersAuthorityTableW->item(row, col)->text().toStdString() != "✔")
            {
                for (int mm = 0; mm < 2; mm++) {
                    QTableWidgetItem *item = m_usersAuthorityTableW->item(row, col - mm);
                    // recover
                    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                    // reset
                    item->setForeground(QBrush(Qt::gray));
                    // reset to default
                    item->setBackground(QBrush(QColor(240,240,240)));
                }
            } else {
                for (int mm = 0; mm < 2; mm++) {
                    QTableWidgetItem *item = m_usersAuthorityTableW->item(row, col - mm);
                    // recover
                    item->setFlags(item->flags() | Qt::ItemIsEditable);
                    // reset
                    item->setForeground(QBrush(Qt::black));
                    // reset to default
                    item->setBackground(QBrush());
                }
            }
        }
    }
}
void AuthorityWidget::fnReadDB()
{
    GlobalSingleton::instance().loadUserGroupInfo();
    std::vector<std::string> userGroupNames={"SystemManager","Manager","Operator","MaintenanceStaff"};
    if (GlobalSingleton::instance().getSystemInfo("userGroupName")==""){
        GlobalSingleton::instance().setSystemInfo("userGroupName", "SystemManager:Manager:Operator:MaintenanceStaff:");
        std::map<std::string,std::string> info;
        for (auto name:userGroupNames){
            info["GroupName"] = name;
            info["GroupPNumber"] = "0";
            info["GroupPerson"] = "";
            info["Authority"] = "";
            info["Creator"] = GlobalSingleton::instance().getSystemInfo("enterUsersManageName");
            info["CreateTime"] = getStandardCurTime();
            info["LastModifier"] = "";
            info["LastModifyTime"] = "";
            GlobalSingleton::instance().addAuthorityRecord(info);
        }
    } else {
        std::vector<std::map<std::string,std::string>> authorityInfo=GlobalSingleton::instance().getAuthorityInfo();
        for (int j=0;j<int(userGroupNames.size());j++){
            bool flag=false;
            for (int i=0;i<int(authorityInfo.size());i++){
                if (authorityInfo[i]["GroupName"]==userGroupNames[j]){
                    flag=true;
                    break;
                }
            }
            if (!flag){
                std::map<std::string,std::string> info;
                info["GroupName"] = userGroupNames[j];
                info["GroupPNumber"] = "0";
                info["GroupPerson"] = "";
                info["Authority"] = "";
                info["Creator"] = GlobalSingleton::instance().getSystemInfo("enterUsersManageName");
                info["CreateTime"] = getStandardCurTime();
                info["LastModifier"] = "";
                info["LastModifyTime"] = "";
                GlobalSingleton::instance().addAuthorityRecord(info);
            }
        }
    }
    std::vector<std::string> usersGroupName=splitStr(GlobalSingleton::instance().getSystemInfo("userGroupName"),':');
    usersGroupName.erase(usersGroupName.begin()+usersGroupName.size()-1);
    for (int i=0;i<int(usersGroupName.size());i++){
        m_usersComboBox->addItem(QString::fromStdString(loadTranslation(m_lang,usersGroupName[i])));
    }

    fnFillAuthorityList();
}
void AuthorityWidget::onUsersComboBoxChanged(int index)
{
    fnFillAuthorityInfo();
    if (m_setAuthorityFlag) setAuthority();

    for (int row = 0; row < m_usersAuthorityTableW->rowCount(); row++)
    {
        if (row==0) continue;
        for (int col = 0; col < m_usersAuthorityTableW->columnCount(); col++)
        {
            if (col%2==0) continue;
            QTableWidgetItem *item = m_usersAuthorityTableW->item(row, col);
            if (item)
            {
                item->setText("");
            }
        }
    }
    fnFillAuthorityList();
}
bool isCellEditable(QTableWidget *tableWidget,int row,int column){
    QTableWidgetItem *item = tableWidget->item(row, column);
    if (item==nullptr)
        return false;
    return item->flags() & Qt::ItemIsEditable;
}
void AuthorityWidget::onUsersAuthorityClicked(int row, int col)
{
    if (col % 2 == 0)
        return;
    if (m_setAuthorityFlag)
    {
        QTableWidgetItem *item = m_usersAuthorityTableW->item(row, col);
        if (item)
        {
            std::string attributeName = findTranslationKey(m_lang, m_usersComboBox->currentText().toStdString());
            if (m_usersAuthorityTableW->item(row, col - 1) == NULL)
                return;
            if (m_usersAuthorityTableW->item(row, col - 1)->text() == "")
                return;
            bool editable = isCellEditable(m_usersAuthorityTableW, row, col);
            if (!editable)
                return;
            if (item->text() == "")
            {
                item->setText("✔");
                item->setTextAlignment(Qt::AlignCenter);
                // 在编辑模式下，使用临时编辑方法而不是直接修改数据库
                if (GlobalSingleton::instance().isAuthorityEditing()) {
                    GlobalSingleton::instance().addAuthorityFieldToEdit(attributeName, findTranslationKey(m_lang,
                                                                     m_usersAuthorityTableW->item(row, col - 1)->text().toStdString()));
                } else {
                    fnAddAuthorityItem(attributeName, findTranslationKey(m_lang,
                                                                     m_usersAuthorityTableW->item(row, col - 1)->text().toStdString()));
                }
            }
            else
            {
                item->setText("");
                // remove
                if (GlobalSingleton::instance().isAuthorityEditing()) {
                    GlobalSingleton::instance().removeAuthorityFieldFromEdit(attributeName, findTranslationKey(m_lang,
                                                                        m_usersAuthorityTableW->item(row, col - 1)->text().toStdString()));
                } else {
                    fnRemoveAuthorityItem(attributeName, findTranslationKey(m_lang,
                                                                        m_usersAuthorityTableW->item(row, col - 1)->text().toStdString()));
                }
            }
        }
    }
}
void AuthorityWidget::fnWriteDB()
{
    // 仅在非编辑模式下保存数据
    if (!GlobalSingleton::instance().isAuthorityEditing()) {
        GlobalSingleton::instance().saveSystemInfo();
        GlobalSingleton::instance().saveUserGroupInfo();
    }
}
void AuthorityWidget::clickSelfDefine(){
    QDialog dialog(this);
    dialog.setWindowTitle(QString::fromStdString(loadTranslation(m_lang, "SelfDefine")));
    dialog.setWindowModality(Qt::ApplicationModal);
    QLabel *inputLabel = new QLabel(QString::fromStdString(loadTranslation(m_lang, "InputDefineName")));
    QLabel *infoLabel = new QLabel("");
    QLineEdit *inputLineEdit = new QLineEdit();
    QPushButton *okButton = new QPushButton(QString::fromStdString(loadTranslation(m_lang, "Ok")));
    QPushButton *delBtn = new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Delete")));
    connect (delBtn,&QPushButton::clicked,[&](){
        if (inputLineEdit->text().isEmpty()) return;
        if (m_usersComboBox->findText(inputLineEdit->text())==-1){
            infoLabel->setText(QString::fromStdString(loadTranslation(m_lang, "NameNotExisted")));
            return;
        } else {
            QString delQstr = inputLineEdit->text();
            std::string delAuthority=findTranslationKey(m_lang,inputLineEdit->text().toStdString());
            if (delAuthority=="SystemManager"||delAuthority=="Manager"||delAuthority=="Operator"||delAuthority=="MaintenanceStaff"){
                infoLabel->setText("预设权限分组不可删除");
                return;
            } else {
                GlobalSingleton::instance().delSystemInfo("userGroupName",delAuthority+":");
                GlobalSingleton::instance().delAuthorityRecord(delAuthority);
                int index=m_usersComboBox->findText(delQstr);
                if (index!=-1){
                    m_usersComboBox->removeItem(index);
                }
                dialog.close();
            }
        }
    });
    connect(okButton, &QPushButton::clicked, [&](){
            if (inputLineEdit->text().isEmpty()) return;
            if (m_usersComboBox->findText(inputLineEdit->text())==-1){
                std::string defineName=findTranslationKey(m_lang,inputLineEdit->text().toStdString());
                GlobalSingleton::instance().addSystemInfo("userGroupName",defineName+":");

                std::map<std::string, std::string> info;
                info["GroupName"] = defineName;
                info["GroupPNumber"] = "0";
                info["GroupPerson"] = "";
                info["Authority"] = "";
                info["Creator"] = GlobalSingleton::instance().getSystemInfo("enterUsersManageName");
                info["CreateTime"] = getStandardCurTime();
                info["LastModifier"] = "";
                info["LastModifyTime"] = "";
                GlobalSingleton::instance().addAuthorityRecord(info);
                m_usersComboBox->addItem(inputLineEdit->text());
                dialog.close();
            } else infoLabel->setText(QString::fromStdString(loadTranslation(m_lang, "NameExisted"))); 
    });
    QGridLayout *layout = new QGridLayout(&dialog);
    layout->addWidget(inputLabel, 0, 0);
    layout->addWidget(inputLineEdit, 0, 1);
    layout->addWidget(okButton, 1, 0);
    layout->addWidget(delBtn, 1, 1);
    layout->addWidget(infoLabel, 1, 2);
    dialog.setLayout(layout);
    dialog.exec();
}
void AuthorityWidget::clickMoreInfo(){
    std::map<std::string,std::string> groupInfo;
    for (auto info : GlobalSingleton::instance().getAuthorityInfo()){
        if (info.at("GroupName")==findTranslationKey(m_lang,m_usersComboBox->currentText().toStdString())){
            groupInfo = info;
            break;
        }
    }
    std::ostringstream ss;
    ss << "创建信息：" << m_usersComboBox->currentText().toStdString() << "\n" \
        << "用户组人数：" << ((groupInfo.find("GroupPNumber")!=groupInfo.end())?groupInfo.at("GroupPNumber"):"0") + "人\n" \
        << "创建人：" << ((groupInfo.find("Creator")!=groupInfo.end())?groupInfo.at("Creator"):"")+"\n" \
        << "创建日期：" << ((groupInfo.find("CreateTime")!=groupInfo.end())?groupInfo.at("CreateTime"):"")+"\n" \
        << "最后修改人：" << ((groupInfo.find("LastModifier")!=groupInfo.end())?groupInfo.at("LastModifier"):"")+"\n" \
        << "最后修改日期：" << ((groupInfo.find("LastModifyTime")!=groupInfo.end())?groupInfo.at("LastModifyTime"):"")+"\n";

    m_usersLayout->removeWidget(m_usersComboBox);
    m_usersLayout->removeWidget(m_usersAuthorityTableW);
    m_usersLayout->removeWidget(m_selfDefineBtn);
    m_usersLayout->removeWidget(m_setAuthorityBtn);
    m_usersLayout->removeWidget(m_moreInfoBtn);

    m_usersComboBox->hide();
    m_usersAuthorityTableW->hide();
    m_selfDefineBtn->hide();
    m_setAuthorityBtn->hide();
    m_moreInfoBtn->hide();

    m_usersLabel->setText(QString::fromStdString(ss.str()));
    m_curPhase=USER_SCAN_USERS;
}
void AuthorityWidget::setAuthority()
{
    m_setAuthorityFlag = !m_setAuthorityFlag;
    std::string curAuthority;
    std::vector<std::string> authorities;

    std::ostringstream ss;
    if (m_setAuthorityFlag)
    {
        // 开始编辑模式
        for (auto info : GlobalSingleton::instance().getAuthorityInfo())
        {
            if (info["GroupName"] == findTranslationKey(m_lang, m_usersComboBox->currentText().toStdString()))
            {
                curAuthority = info["Authority"];
                authorities = splitStr(curAuthority, ':');
                break;
            }
        }

        if (!GlobalSingleton::instance().beginAuthorityEdit())
        {
            // 如果开始编辑失败，保持当前状态不变
            m_setAuthorityFlag = !m_setAuthorityFlag;
            return;
        }

        fnResetAuthorityToEnable();
        ss << "编辑 " + m_usersComboBox->currentText().toStdString() + " 原权限:";
        for (int i = 0; i < int(authorities.size()); i++)
        {
            ss << loadTranslation(m_lang, authorities[i]) << ",";
        }
        RWDb::writeAuditTrailLog(ss.str());
        m_setAuthorityBtn->setText(QString::fromStdString(loadTranslation(m_lang, "Ok")));

        for (int row = 0; row < m_usersAuthorityTableW->rowCount(); row++)
        {
            for (int col = 0; col < m_usersAuthorityTableW->columnCount(); col++)
            {
                QTableWidgetItem *item = m_usersAuthorityTableW->item(row, col);
                if (item)
                {
                    if (row == 0)
                        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                    if (col % 2 == 0)
                        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                    else
                        item->setFlags(item->flags() & Qt::ItemIsEditable);
                }
            }
        }
    }
    else
    {
        curAuthority = "";
        authorities.clear();
        // 扫描表格中所有打勾的权限项
        for (int row = 1; row < m_usersAuthorityTableW->rowCount(); row++) {
            for (int col = 0; col < m_usersAuthorityTableW->columnCount(); col++) {
                if (col % 2 == 0) continue; // 跳过权限名称列
                
                QTableWidgetItem* item = m_usersAuthorityTableW->item(row, col);
                if (item && item->text() == "✔") {
                    // 获取对应的权限名称
                    QTableWidgetItem* authorityItem = m_usersAuthorityTableW->item(row, col-1);
                    if (authorityItem && !authorityItem->text().isEmpty()) {
                        std::string authorityKey = findTranslationKey(m_lang, authorityItem->text().toStdString());
                        if (!authorityKey.empty()) {
                            authorities.push_back(authorityKey);
                        }
                    }
                }
            }
        }
        // 重新构建curAuthority字符串
        for (const auto& auth : authorities) {
            curAuthority += auth + ":";
        }
        ss << "修改 " + m_usersComboBox->currentText().toStdString() + " 权限:";
        for (int i = 0; i < int(authorities.size()); i++)
        {
            ss << loadTranslation(m_lang, authorities[i]) << ",";
        }
        RWDb::writeAuditTrailLog(ss.str());
        m_setAuthorityBtn->setText(QString::fromStdString(loadTranslation(m_lang, "Set")));
        
        // 修复：先提交编辑并更新数据库，再刷新表格显示
        m_usersAuthorityTableW->setEditTriggers(QAbstractItemView::NoEditTriggers);  // 再设置不可编辑

        // 提交编辑模式
        if (GlobalSingleton::instance().commitAuthorityEdit())
        {
            // 提交成功，更新最后修改信息
            for (int mm = 0; mm < int(GlobalSingleton::instance().getAuthorityInfo().size()); mm++)
            {
                if (GlobalSingleton::instance().getAuthorityField(mm, "GroupName") == findTranslationKey(m_lang, m_usersComboBox->currentText().toStdString()))
                {
                    GlobalSingleton::instance().setAuthorityField(mm, "Authority", curAuthority);
                    GlobalSingleton::instance().setAuthorityField(mm, "LastModifier", GlobalSingleton::instance().getSystemInfo("enterUsersManageName"));
                    GlobalSingleton::instance().setAuthorityField(mm, "LastModifyTime", getStandardCurTime());
                    break;
                }
            }
            // 保存权限数据到数据库
            fnWriteDB();

            // 修复：在数据库更新后，再刷新表格显示
            fnFillAuthorityList();  
        }
        else
        {
            // 提交失败，回滚编辑模式
            GlobalSingleton::instance().rollbackAuthorityEdit();
            // 即使回滚，也要刷新表格显示当前状态
            fnFillAuthorityList();
        }
    }
}