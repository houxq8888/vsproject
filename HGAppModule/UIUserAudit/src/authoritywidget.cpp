#include "authoritywidget.h"
#include <QHeaderView>
#include <QMessageBox>
#include "common.h"
#include <QDialog>
#include "UserAuditManager.h"
#include "loginterface.h"
#include "SvcFactory.h"
#include "SystemDataManager.h"

using namespace HGMACHINE;

AuthorityWidget::AuthorityWidget(std::string lang,const std::vector<std::string> &wholeAuthority,QWidget *parent) : QWidget(parent),
m_lang(lang),
m_whole_authority(wholeAuthority),
m_setAuthorityFlag(false)
{
    UserAuditManager::instance().get().setWholeAuthority(m_whole_authority);
    std::string enterUsersManageName=SystemDataManager::instance().get().getSystemInfo("enterUsersManageName");
    std::string authority = UserAuditManager::instance().get().getUserAuthority(enterUsersManageName);
    permissionInfo = UserAuditManager::instance().get().getAuthorityDetail(authority);

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

    m_selfDefineBtn=new QPushButton(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"SelfDefine")));
    m_moreInfoBtn=new QPushButton(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"MoreInfo")));
    m_backBtn=new QPushButton(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Back")));

    connect(m_selfDefineBtn,SIGNAL(clicked()),this,SLOT(clickSelfDefine()));
    connect(m_moreInfoBtn,SIGNAL(clicked()),this,SLOT(clickMoreInfo()));
    connect(m_backBtn,SIGNAL(clicked()),this,SLOT(clickBack()));

    m_usersLabel=new QLabel(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Users")));
    m_usersComboBox=new QComboBox();


#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    connect(m_usersComboBox,&QComboBox::activated,this,
        &AuthorityWidget::onUsersComboBoxChanged);
#else
    connect(m_usersComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated),
            this, &AuthorityWidget::onUsersComboBoxChanged);
#endif

    m_userLabel=new QLabel(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"GroupUsers")));
    m_addUserBtn=new QPushButton(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"AddUser")));
    m_removeUserBtn=new QPushButton(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"DeleteUser")));
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
            QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Authority")),
            QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Select")),
            QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Authority")),
            QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Select")),
            QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Authority")),
            QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Select")),
            QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Authority")),
            QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Select"))
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
    std::vector<std::string> permissionInfoTemp = UserAuditManager::instance().get().getWholeAuthority();
    for (int mm=0;mm<int(permissionInfoTemp.size());mm++){
        int row = mm / 4 + 1;
        int col = (2*mm)%8;
        QString text = QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,permissionInfoTemp[mm]));
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

    m_setAuthorityBtn=new QPushButton(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Set")));
    connect(m_setAuthorityBtn,&QPushButton::clicked,this,&AuthorityWidget::setAuthority);

    // 
    m_scanAuthorityInfoBtn=new QPushButton(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"ScanAuthorityInfo")));
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
    if (UserAuditManager::instance().get().isAuthorityEditing()) {
        UserAuditManager::instance().get().rollbackAuthorityEdit();
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
        m_usersLabel->setText(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Users")));
        slotScanAuthorityInfo();
    }
}
bool AuthorityWidget::fnHasUserManageAuthority()
{
    std::string  enterUsersManageName=SystemDataManager::instance().get().getSystemInfo("enterUsersManageName");
    std::string authority=UserAuditManager::instance().get().getUserAuthority(enterUsersManageName);
    std::vector<std::string> userPermission = UserAuditManager::instance().get().getAuthorityDetail(authority);

    if (SystemDataManager::instance().get().getSystemInfo("免密登录")!="true"&&
        std::find(userPermission.begin(),userPermission.end(),"UserManage")==userPermission.end()){
        std::ostringstream ss;
        QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),"没有"+QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"UserManage"))+"权限，请联系管理员开通！");
        LOG_IF.writeAuditTrailLog("没有"+SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"UserManage")+"权限，请联系管理员开通！");
        return false;
    }
    return true;
}
bool AuthorityWidget::fnHasUsersManageAuthority()
{
    return true;
    std::string  enterUsersManageName=SystemDataManager::instance().get().getSystemInfo("enterUsersManageName");
    std::string authority=UserAuditManager::instance().get().getUserAuthority(enterUsersManageName);
    std::vector<std::string> userPermission = UserAuditManager::instance().get().getAuthorityDetail(authority);

    if (SystemDataManager::instance().get().getSystemInfo("免密登录")!="true"&&
        std::find(userPermission.begin(),userPermission.end(),"UsersManage")==userPermission.end()){
        std::ostringstream ss;
        QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),"没有"+QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"UsersManage"))+"权限，请联系管理员开通！");
        LOG_IF.writeAuditTrailLog("没有"+SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"UsersManage")+"权限，请联系管理员开通！");
        return false;
    }
    return true;
}

AuthorityWidget::~AuthorityWidget()
{
    if (m_usersLayout);{
        delete (m_usersLayout);
        m_usersLayout = nullptr;
    }
    if (m_usersAuthorityInfoW);{
        delete (m_usersAuthorityInfoW);
        m_usersAuthorityInfoW = nullptr;
    }
    if (m_selfDefineBtn);{
        delete (m_selfDefineBtn);
        m_selfDefineBtn = nullptr;
    }
    if (m_moreInfoBtn);{
        delete (m_moreInfoBtn);
        m_moreInfoBtn = nullptr;
    }
    if (m_usersLabel);{
        delete (m_usersLabel);
        m_usersLabel = nullptr;
    }
    if (m_usersComboBox);{
        delete (m_usersComboBox);
        m_usersComboBox = nullptr;
    }
    if (m_userLabel);{
        delete (m_userLabel);
        m_userLabel = nullptr;
    }
    if (m_addUserBtn);{
        delete (m_addUserBtn);
        m_addUserBtn = nullptr;
    }
    if (m_removeUserBtn);{
        delete (m_removeUserBtn);
        m_removeUserBtn = nullptr;
    }
    if (m_usersAuthorityTableW);{
        delete (m_usersAuthorityTableW);
        m_usersAuthorityTableW = nullptr;
    }
    if (m_userTableW);{
        delete (m_userTableW);
        m_userTableW = nullptr;
    }
    if (m_setAuthorityBtn);{
        delete (m_setAuthorityBtn);
        m_setAuthorityBtn = nullptr;
    }
    if (m_backBtn);{
        delete (m_backBtn);
        m_backBtn = nullptr;
    }
    if (m_scanAuthorityInfoBtn);{
        delete (m_scanAuthorityInfoBtn);
        m_scanAuthorityInfoBtn = nullptr;
    }
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
    dialog.setWindowTitle(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"AddUser")));
    dialog.setWindowModality(Qt::ApplicationModal);
    QListWidget* listW=new QListWidget(&dialog);
    QPushButton* okbtn=new QPushButton(&dialog);
    okbtn->setText(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Ok")));//"确定");
    QPushButton* cancelbtn=new QPushButton(&dialog);
    cancelbtn->setText(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Cancel")));//"取消");

    std::string addUserName;
    connect(okbtn,&QPushButton::clicked,[&](){
       if (listW->count()!=0) addUserName=listW->currentItem()->text().toStdString();
       dialog.close();
    });
    connect(cancelbtn,&QPushButton::clicked,[&](){
        dialog.close();
    });
    std::vector<std::string> names=UserAuditManager::instance().get().getUsersNo();
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
        if (SvcFactory::CreateConfigService()->LoadTranslation(m_lang, UserAuditManager::instance().get().getUserField(index, "Authority")) 
            != m_usersComboBox->currentText().toStdString())
        {
            std::string selectAuthority=SvcFactory::CreateConfigService()->FindTranslationKey(m_lang,m_usersComboBox->currentText().toStdString());
            if (selectAuthority == "Manager") {
                if (SystemDataManager::instance().get().getSystemInfo("免密登录") != "true" &&
                    std::find(permissionInfo.begin(), permissionInfo.end(), "UserManage(Manager)") == permissionInfo.end())
                {
                    std::ostringstream ss;
                    QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "该用户没有权限创建管理员，请联系管理员开通!");
                    LOG_IF.writeAuditTrailLog("该用户没有权限创建管理员，请联系管理员开通!");
                    return;
                }
            } else if (selectAuthority != "SystemManager"&&selectAuthority!="Manager"&&selectAuthority!="所有权限")
            {
                if (SystemDataManager::instance().get().getSystemInfo("免密登录") != "true" &&
                    std::find(permissionInfo.begin(), permissionInfo.end(), "UserManage(Users)") == permissionInfo.end())
                {
                    std::ostringstream ss;
                    QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "该用户没有权限创建非管理员用户，请联系管理员开通！");
                    LOG_IF.writeAuditTrailLog("该用户没有权限创建非管理员用户，请联系管理员开通！");
                    return;
                }
            } else if (selectAuthority=="所有权限"){
                QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "不能创建所有权限账号!");
                LOG_IF.writeAuditTrailLog("不能创建所有权限账号!");
                return;
            } else if (selectAuthority=="SystemManager"){
                std::string enterUsersManageName=SystemDataManager::instance().get().getSystemInfo("enterUsersManageName");
                std::string authority = UserAuditManager::instance().get().getUserAuthority(enterUsersManageName);
                if (authority.find("所有权限")==std::string::npos){
                    QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "只有厂家账号才能创建超级管理员账号!");
                    LOG_IF.writeAuditTrailLog("只有厂家账号才能创建超级管理员账号!");
                    return;
                }
            }
            if (QMessageBox::Yes==QMessageBox::question(this,QString::fromStdString(HG_DEVICE_NAME),
                QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"SureToChangeAccoundAuthority")),
                QMessageBox::Yes|QMessageBox::No))
            {
                std::string attributeName=UserAuditManager::instance().get().getUserField(index,"Authority");

                QString name = QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang, "Authority"));
                if (SvcFactory::CreateCommonService()->SplitString(UserAuditManager::instance().get().getUserField(index,"Authority"),';').size() <=1)
                    UserAuditManager::instance().get().addUserField(index,"Authority",';'+SvcFactory::CreateConfigService()->FindTranslationKey(m_lang,m_usersComboBox->currentText().toStdString())+';');
                else 
                    UserAuditManager::instance().get().addUserField(index,"Authority",SvcFactory::CreateConfigService()->FindTranslationKey(m_lang,m_usersComboBox->currentText().toStdString())+';');
                UserAuditManager::instance().get().setUserField(index,"LastModifyTime",SvcFactory::CreateCommonService()->GetStandardCurTime());
                UserAuditManager::instance().get().setUserField(index,"LastModifier",SystemDataManager::instance().get().getSystemInfo("enterUsersManageName"));
                if (addUserName == SystemDataManager::instance().get().getSystemInfo("loginName")){
                    SystemDataManager::instance().get().setSystemInfo("authority",UserAuditManager::instance().get().getUserField(index,"Authority"));
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
        std::string attributeName=SvcFactory::CreateConfigService()->FindTranslationKey(m_lang,m_usersComboBox->currentText().toStdString());
 
        LOG_IF.writeAuditTrailLog(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,attributeName)+" 增加账户 ["+addUserName+"] ");
        fnAddUsrsGroupUser(m_usersComboBox->currentText().toStdString(),addUserName);
    }
}
int  AuthorityWidget::findUserAuthorityIndex(const std::string &userNo){
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
    for (int i=0;i<int(UserAuditManager::instance().get().getAuthorityInfo().size());i++)
    {
        if (UserAuditManager::instance().get().getAuthorityField(i,"GroupName") == SvcFactory::CreateConfigService()->FindTranslationKey(m_lang, authorityName))
        {
            if (UserAuditManager::instance().get().getAuthorityField(i,"Authority").find(text)!= std::string::npos) break;
            UserAuditManager::instance().get().setAuthorityField(i,"Authority", 
                UserAuditManager::instance().get().getAuthorityField(i,"Authority") + (text + ":"));
            UserAuditManager::instance().get().setAuthorityField(i,"LastModifyTime", SvcFactory::CreateCommonService()->GetStandardCurTime());
            UserAuditManager::instance().get().setAuthorityField(i,"LastModifier",SystemDataManager::instance().get().getSystemInfo("enterUsersManageName"));
            break;
        }
    }
}
void AuthorityWidget::fnAddUsrsGroupUser(const std::string &authorityName, const std::string &text)
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
void AuthorityWidget::fnRemoveAuthorityItem(const std::string &authorityName, const std::string &text)
{
     for (int i=0;i<int(UserAuditManager::instance().get().getAuthorityInfo().size());i++)
    {
        if (UserAuditManager::instance().get().getAuthorityField(i,"GroupName") == SvcFactory::CreateConfigService()->FindTranslationKey(m_lang, authorityName))
        {
            if (UserAuditManager::instance().get().getAuthorityField(i,"Authority").find(text)== std::string::npos) break;
            std::vector<std::string> authoritys = SvcFactory::CreateCommonService()->SplitString(UserAuditManager::instance().get().getAuthorityField(i,"Authority"), ':');
            for (auto authority : authoritys)
            {
                if (authority == text)
                {
                    authoritys.erase(std::find(authoritys.begin(), authoritys.end(), authority));
                    break;
                }
            }
            UserAuditManager::instance().get().setAuthorityField(i,"Authority","");
            for (auto authority : authoritys)
            {
                UserAuditManager::instance().get().addAuthorityField(i,"Authority", authority + ":");
            }
            UserAuditManager::instance().get().setAuthorityField(i,"LastModifyTime",SvcFactory::CreateCommonService()->GetStandardCurTime());
            UserAuditManager::instance().get().setAuthorityField(i,"LastModifier",SystemDataManager::instance().get().getSystemInfo("enterUsersManageName"));
            break;
        }
    }
}
void AuthorityWidget::fnRemoveUsrsGroupUser(const std::string &authorityName, const std::string &text)
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
void AuthorityWidget::slotRemoveUser(){
    std::string attributeName=SvcFactory::CreateConfigService()->FindTranslationKey(m_lang,m_usersComboBox->currentText().toStdString());
    // remove 
    int col=m_userTableW->currentColumn();
    int row=m_userTableW->currentRow();
    if ((row<0||row>=m_userTableW->rowCount()) &&
        (col<0||col>=m_userTableW->columnCount()))
    {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), 
            QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"SelectOneRecord")));
        return;
    }
    if (m_userTableW->item(row,col)){
        std::string text=m_userTableW->item(row,col)->text().toStdString();
        int index = findUserAuthorityIndex(text);

        std::string selectAuthority = SvcFactory::CreateConfigService()->FindTranslationKey(m_lang, m_usersComboBox->currentText().toStdString());
        if (selectAuthority == "Manager")
        {
            if (SystemDataManager::instance().get().getSystemInfo("免密登录") != "true" &&
                std::find(permissionInfo.begin(), permissionInfo.end(), "UserManage(Manager)") == permissionInfo.end())
            {
                std::ostringstream ss;
                QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "该用户没有权限移除管理员，请联系管理员开通!");
                LOG_IF.writeAuditTrailLog("该用户没有权限移除管理员，请联系管理员开通!");
                return;
            }
        }
        else if (selectAuthority != "SystemManager" && selectAuthority != "Manager" && selectAuthority != "所有权限")
        {
            if (SystemDataManager::instance().get().getSystemInfo("免密登录") != "true" &&
                std::find(permissionInfo.begin(), permissionInfo.end(), "UserManage(Users)") == permissionInfo.end())
            {
                std::ostringstream ss;
                QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "该用户没有权限移除非管理员用户，请联系管理员开通！");
                LOG_IF.writeAuditTrailLog("该用户没有权限移除非管理员用户，请联系管理员开通！");
                return;
            }
        }
        else if (selectAuthority == "所有权限")
        {
            QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "不能移除所有权限账号!");
            LOG_IF.writeAuditTrailLog("不能移除所有权限账号!");
            return;
        }
       

        if (QMessageBox::Yes == QMessageBox::question(this,QString::fromStdString(HG_DEVICE_NAME),
            QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"SureToChangeAccoundAuthority")),
            QMessageBox::Yes|QMessageBox::No))
        {
            m_userTableW->item(row,col)->setText("");
            LOG_IF.writeAuditTrailLog(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,attributeName)+" 删除账户 ["+text+"] ");
            fnRemoveUsrsGroupUser(m_usersComboBox->currentText().toStdString(),text);
            std::string authorityTemp = UserAuditManager::instance().get().getUserField(index,"Authority");
            std::vector<std::string> authorities = SvcFactory::CreateCommonService()->SplitString(authorityTemp,';');
            authorityTemp = "";
            for (int mm=0;mm<int(authorities.size());mm++){
                if (authorities[mm]=="") continue;
                if (authorities[mm]==SvcFactory::CreateConfigService()->FindTranslationKey(m_lang,m_usersComboBox->currentText().toStdString())) continue;
                authorityTemp +=SvcFactory::CreateConfigService()->LoadTranslation(m_lang,authorities[mm])+';';
            }
            UserAuditManager::instance().get().setUserField(index,"Authority",authorityTemp);
            if (text==SystemDataManager::instance().get().getSystemInfo("loginName")){
                SystemDataManager::instance().get().setSystemInfo("authority",UserAuditManager::instance().get().getUserField(index,"Authority"));
                emit updateAuthority();
            }
            UserAuditManager::instance().get().setUserField(index,"LastModifyTime",SvcFactory::CreateCommonService()->GetStandardCurTime());
            UserAuditManager::instance().get().setUserField(index,"LastModifier",SystemDataManager::instance().get().getSystemInfo("enterUsersManageName"));
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
    std::vector<std::map<std::string,std::string>> authorityIfno=UserAuditManager::instance().get().getAuthorityInfo();
    for (auto info : authorityIfno)
    {
        if (info.at("GroupName") == SvcFactory::CreateConfigService()->FindTranslationKey(m_lang, authorityName))
        {
            std::vector<std::string> resultvalue;
            resultvalue = SvcFactory::CreateCommonService()->SplitString(info.at("Authority"), ':');
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
                    m_usersAuthorityInfoW->item(row, col)->setText(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang, resultvalue[i])));
                else
                    m_usersAuthorityInfoW->setItem(row, col, new QTableWidgetItem(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang, resultvalue[i]))));
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
    for (auto info : UserAuditManager::instance().get().getAuthorityInfo())
    {
        if (info.at("GroupName") == SvcFactory::CreateConfigService()->FindTranslationKey(m_lang, authorityName))
        {
            std::vector<std::string> resultvalue = SvcFactory::CreateCommonService()->SplitString(info.at("Authority"), ':');
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
                            std::string itemText = SvcFactory::CreateConfigService()->FindTranslationKey(m_lang, item->text().toStdString());
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
                                SvcFactory::CreateConfigService()->LoadTranslation(m_lang, resultvalue[i]))
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
            resultvalue = SvcFactory::CreateCommonService()->SplitString(info.at("GroupPerson"), ':');
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
    std::string usersType = SvcFactory::CreateConfigService()->FindTranslationKey(m_lang, m_usersComboBox->currentText().toStdString());
    int row,col;
    for (row = 1; row < m_usersAuthorityTableW->rowCount(); row++){
        for (col=0; col < m_usersAuthorityTableW->columnCount(); col++){
            if (col % 2 == 0)
                continue;
            if (m_usersAuthorityTableW->item(row,col)==NULL) continue;
            if (m_usersAuthorityTableW->item(row, col - 1)==NULL) continue;
            for (int mm=0;mm<2;mm++){
                QTableWidgetItem *item = m_usersAuthorityTableW->item(row, col-mm);
                std::string itemText = SvcFactory::CreateConfigService()->FindTranslationKey(m_lang,m_usersAuthorityTableW->item(row,col-1)->text().toStdString());
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
    UserAuditManager::instance().get().loadUserGroupInfo();
    std::vector<std::string> userGroupNames={"SystemManager","Manager","Operator","MaintenanceStaff"};
    if (SystemDataManager::instance().get().getSystemInfo("userGroupName")==""){
        SystemDataManager::instance().get().setSystemInfo("userGroupName", "SystemManager:Manager:Operator:MaintenanceStaff:");
        std::map<std::string,std::string> info;
        for (auto name:userGroupNames){
            info["GroupName"] = name;
            info["GroupPNumber"] = "0";
            info["GroupPerson"] = "";
            info["Authority"] = "";
            info["Creator"] = SystemDataManager::instance().get().getSystemInfo("enterUsersManageName");
            info["CreateTime"] = SvcFactory::CreateCommonService()->GetStandardCurTime();
            info["LastModifier"] = "";
            info["LastModifyTime"] = "";
            UserAuditManager::instance().get().addAuthorityRecord(info);
        }
    } else {
        std::vector<std::map<std::string,std::string>> authorityInfo=UserAuditManager::instance().get().getAuthorityInfo();
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
                info["Creator"] = SystemDataManager::instance().get().getSystemInfo("enterUsersManageName");
                info["CreateTime"] = SvcFactory::CreateCommonService()->GetStandardCurTime();
                info["LastModifier"] = "";
                info["LastModifyTime"] = "";
                UserAuditManager::instance().get().addAuthorityRecord(info);
            }
        }
    }
    std::vector<std::string> usersGroupName=SvcFactory::CreateCommonService()->SplitString(SystemDataManager::instance().get().getSystemInfo("userGroupName"),':');
    usersGroupName.erase(usersGroupName.begin()+usersGroupName.size()-1);
    for (int i=0;i<int(usersGroupName.size());i++){
        m_usersComboBox->addItem(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,usersGroupName[i])));
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
            std::string attributeName = SvcFactory::CreateConfigService()->FindTranslationKey(m_lang, m_usersComboBox->currentText().toStdString());
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
                if (UserAuditManager::instance().get().isAuthorityEditing()) {
                    UserAuditManager::instance().get().addAuthorityFieldToEdit(attributeName, SvcFactory::CreateConfigService()->FindTranslationKey(m_lang,
                                                                     m_usersAuthorityTableW->item(row, col - 1)->text().toStdString()));
                } else {
                    fnAddAuthorityItem(attributeName, SvcFactory::CreateConfigService()->FindTranslationKey(m_lang,
                                                                     m_usersAuthorityTableW->item(row, col - 1)->text().toStdString()));
                }
            }
            else
            {
                item->setText("");
                // remove
                if (UserAuditManager::instance().get().isAuthorityEditing()) {
                    UserAuditManager::instance().get().removeAuthorityFieldFromEdit(attributeName, SvcFactory::CreateConfigService()->FindTranslationKey(m_lang,
                                                                        m_usersAuthorityTableW->item(row, col - 1)->text().toStdString()));
                } else {
                    fnRemoveAuthorityItem(attributeName, SvcFactory::CreateConfigService()->FindTranslationKey(m_lang,
                                                                        m_usersAuthorityTableW->item(row, col - 1)->text().toStdString()));
                }
            }
        }
    }
}
void AuthorityWidget::fnWriteDB()
{
    // 仅在非编辑模式下保存数据
    if (!UserAuditManager::instance().get().isAuthorityEditing()) {
        SystemDataManager::instance().get().saveSystemInfo();
        UserAuditManager::instance().get().saveUserGroupInfo();
    }
}
void AuthorityWidget::clickSelfDefine(){
    QDialog dialog(this);
    dialog.setWindowTitle(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang, "SelfDefine")));
    dialog.setWindowModality(Qt::ApplicationModal);
    QLabel *inputLabel = new QLabel(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang, "InputDefineName")));
    QLabel *infoLabel = new QLabel("");
    QLineEdit *inputLineEdit = new QLineEdit();
    QPushButton *okButton = new QPushButton(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang, "Ok")));
    QPushButton *delBtn = new QPushButton(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Delete")));
    connect (delBtn,&QPushButton::clicked,[&](){
        if (inputLineEdit->text().isEmpty()) return;
        if (m_usersComboBox->findText(inputLineEdit->text())==-1){
            infoLabel->setText(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang, "NameNotExisted")));
            return;
        } else {
            QString delQstr = inputLineEdit->text();
            std::string delAuthority=SvcFactory::CreateConfigService()->FindTranslationKey(m_lang,inputLineEdit->text().toStdString());
            if (delAuthority=="SystemManager"||delAuthority=="Manager"||delAuthority=="Operator"||delAuthority=="MaintenanceStaff"){
                infoLabel->setText("预设权限分组不可删除");
                return;
            } else {
                SystemDataManager::instance().get().delSystemInfo("userGroupName",delAuthority+":");
                UserAuditManager::instance().get().delAuthorityRecord(delAuthority);
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
                std::string defineName=SvcFactory::CreateConfigService()->FindTranslationKey(m_lang,inputLineEdit->text().toStdString());
                SystemDataManager::instance().get().addSystemInfo("userGroupName",defineName+":");

                std::map<std::string, std::string> info;
                info["GroupName"] = defineName;
                info["GroupPNumber"] = "0";
                info["GroupPerson"] = "";
                info["Authority"] = "";
                info["Creator"] = SystemDataManager::instance().get().getSystemInfo("enterUsersManageName");
                info["CreateTime"] = SvcFactory::CreateCommonService()->GetStandardCurTime();
                info["LastModifier"] = "";
                info["LastModifyTime"] = "";
                UserAuditManager::instance().get().addAuthorityRecord(info);
                m_usersComboBox->addItem(inputLineEdit->text());
                dialog.close();
            } else infoLabel->setText(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang, "NameExisted"))); 
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
    for (auto info : UserAuditManager::instance().get().getAuthorityInfo()){
        if (info.at("GroupName")==SvcFactory::CreateConfigService()->FindTranslationKey(m_lang,m_usersComboBox->currentText().toStdString())){
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
        for (auto info : UserAuditManager::instance().get().getAuthorityInfo())
        {
            if (info["GroupName"] == SvcFactory::CreateConfigService()->FindTranslationKey(m_lang, m_usersComboBox->currentText().toStdString()))
            {
                curAuthority = info["Authority"];
                authorities = SvcFactory::CreateCommonService()->SplitString(curAuthority, ':');
                break;
            }
        }

        if (!UserAuditManager::instance().get().beginAuthorityEdit())
        {
            // 如果开始编辑失败，保持当前状态不变
            m_setAuthorityFlag = !m_setAuthorityFlag;
            return;
        }

        fnResetAuthorityToEnable();
        ss << "编辑 " + m_usersComboBox->currentText().toStdString() + " 原权限:";
        for (int i = 0; i < int(authorities.size()); i++)
        {
            ss << SvcFactory::CreateConfigService()->LoadTranslation(m_lang, authorities[i]) << ",";
        }
        LOG_IF.writeAuditTrailLog(ss.str());
        m_setAuthorityBtn->setText(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang, "Ok")));

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
                        std::string authorityKey = SvcFactory::CreateConfigService()->FindTranslationKey(m_lang, authorityItem->text().toStdString());
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
            ss << SvcFactory::CreateConfigService()->LoadTranslation(m_lang, authorities[i]) << ",";
        }
        LOG_IF.writeAuditTrailLog(ss.str());
        m_setAuthorityBtn->setText(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang, "Set")));
        
        // 修复：先提交编辑并更新数据库，再刷新表格显示
        m_usersAuthorityTableW->setEditTriggers(QAbstractItemView::NoEditTriggers);  // 再设置不可编辑

        // 提交编辑模式
        if (UserAuditManager::instance().get().commitAuthorityEdit())
        {
            // 提交成功，更新最后修改信息
            for (int mm = 0; mm < int(UserAuditManager::instance().get().getAuthorityInfo().size()); mm++)
            {
                if (UserAuditManager::instance().get().getAuthorityField(mm, "GroupName") == SvcFactory::CreateConfigService()->FindTranslationKey(m_lang, m_usersComboBox->currentText().toStdString()))
                {
                    UserAuditManager::instance().get().setAuthorityField(mm, "Authority", curAuthority);
                    UserAuditManager::instance().get().setAuthorityField(mm, "LastModifier", SystemDataManager::instance().get().getSystemInfo("enterUsersManageName"));
                    UserAuditManager::instance().get().setAuthorityField(mm, "LastModifyTime", SvcFactory::CreateCommonService()->GetStandardCurTime());
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
            UserAuditManager::instance().get().rollbackAuthorityEdit();
            // 即使回滚，也要刷新表格显示当前状态
            fnFillAuthorityList();
        }
    }
}