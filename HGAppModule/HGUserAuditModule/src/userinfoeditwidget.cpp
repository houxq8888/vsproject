#include "userinfoeditwidget.h"
#include <QHeaderView>
#include <QMessageBox>
#include "common.h"
#include <QToolTip>


UserInfoEditWidget::UserInfoEditWidget(int type,std::string lang,const std::map<std::string,std::string>& info,QWidget *parent) : QWidget(parent),
    m_lang(lang),
    m_info(info),
    m_type(type)
{
    std::string enterUsersManageName=GlobalSingleton::instance().getSystemInfo("enterUsersManageName");
    std::string authority=GlobalSingleton::instance().getUserAuthority(enterUsersManageName);
    permissionInfo = GlobalSingleton::instance().getAuthorityDetail(authority);

    switch (m_type){
        case USER_CREATE:
        case USER_RECTIFY:
        {
            fnInit();
            m_authorityEdit->setEnabled(false);
            if (m_type==USER_CREATE){
                m_authorityLabel->setVisible(false);
                m_authorityEdit->setVisible(false);
            } else if (m_type==USER_RECTIFY){
                m_authorityLabel->setVisible(true);
                m_authorityEdit->setVisible(true);
                m_userAccountEdit->setEnabled(false);
            }
            if (!m_info.empty())
            {
                m_userNoEdit->setText(QString::fromStdString(m_info.at("UserNo")));
                m_userNoEdit->setReadOnly(true);
                m_userAccountEdit->setText(QString::fromStdString(m_info.find("UserAccount")!=m_info.end()?m_info.at("UserAccount"):""));
                m_passwdEdit->setText(QString::fromStdString(m_info.at("Password")));
                if (m_type == USER_RECTIFY){
                    passwdStrength(m_passwdEdit->text().toStdString());
                }
                m_passwdInputLimitCountEdit->setText(QString::fromStdString(m_info.find("PasswdInputLimitCount")!=m_info.end()?m_info.at("PasswdInputLimitCount"):""));
                m_passwdCycleEdit->setText(QString::fromStdString(splitStr(m_info.at("PasswdCycle"),';')[0]));
                m_userNameEdit->setText(QString::fromStdString(m_info.at("UserName")));
                m_jobEdit->setText(QString::fromStdString(m_info.find("UserJob")!=m_info.end()?m_info.at("UserJob"):""));
                m_departmentEdit->setText(QString::fromStdString(m_info.at("Department")));
                std::string authorityTemp="";
                std::vector<std::string> authoritys=splitStr(m_info.at("Authority"),';');
                for(int i=0;i<int(authoritys.size());i++){
                    if (authoritys[i]=="") continue;
                    authorityTemp+=loadTranslation(m_lang,authoritys[i])+';';
                }
                m_authorityEdit->setText(QString::fromStdString(authorityTemp));
                // m_authorityCombo->setCurrentText(QString::fromStdString(loadTranslation(m_lang, m_info.at("Authority"))));
                m_accountManageCombo->setCurrentText(QString::fromStdString(loadTranslation(m_lang, m_info.at("AccountManagement"))));
                m_userDescriptionEdit->setPlainText(QString::fromStdString(m_info.at("UserDescription")));
            } else {
                std::string nocount = RWDb::getUserInfoLastCount();
                m_userNoEdit->setText(QString::fromStdString(nocount));
            }
            m_userNoEdit->setEnabled(false);
            break;
        }
        case USER_SCAN:{
            fnInitScan();
            std::vector<std::string> resLogin=splitStr(m_info["LoginTime"], ';');
            std::vector<std::string> resQuit=splitStr(m_info["QuitTime"], ';');
            for (int i=0;i<int(resLogin.size())-1;i++){
                // if (resLogin[i].empty()) continue;
                int row=-1, col=-1;
                row=i/3+1;
                col=(i%3) * 3;
                if (row < 0 
                    || col < 0 || col >= m_loginLogTableW->columnCount())
                    continue;
                if (row >=m_loginLogTableW->rowCount()){
                    m_loginLogTableW->insertRow(row);
                }
                m_loginLogTableW->setItem(row,col,new QTableWidgetItem(QString::number(i+1)));
                m_loginLogTableW->setItem(row,col+1,new QTableWidgetItem(QString::fromStdString(resLogin[i])));
                // m_loginLogTableW->setItem(row,col+2,new QTableWidgetItem(QString::fromStdString(resQuit[i])));
            }
            for (int i=0;i<int(resQuit.size())-1;i++){
                // if (resQuit[i].empty()) continue;
                int row=-1, col=-1;
                row=i/3+1;
                col=(i%3) * 3;
                if (row < 0  
                    || col < 0 || col >= m_loginLogTableW->columnCount())
                    continue;
                if (row >=m_loginLogTableW->rowCount()){
                    m_loginLogTableW->insertRow(row);
                }
                // m_loginLogTableW->setItem(row,col,new QTableWidgetItem(QString::number(i)));
                // m_loginLogTableW->setItem(row,col+1,new QTableWidgetItem(QString::fromStdString(resLogin[i])));
                m_loginLogTableW->setItem(row,col+2,new QTableWidgetItem(QString::fromStdString(resQuit[i])));
            }
            
            break;
        }
        default: break;
    }
}
void UserInfoEditWidget::fnInitScan(){
    m_layout=new QGridLayout();
    m_userInfoLayout=new QGridLayout();
    m_groupBox=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"ScanUserInfo")),this);
    m_groupBox->setStyleSheet("QGroupBox { font-size: 12pt; font-weight:bold;}");
    m_groupBox->setLayout(m_userInfoLayout);

    m_backBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Back")));
    connect(m_backBtn,SIGNAL(clicked()),this,SLOT(clickBack()));

    m_accountStateLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang, "AccountManagement")
        +":"+loadTranslation(m_lang,m_info.at("AccountManagement"))));
    m_accountCreaterLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang, "Creator")
        +":"+loadTranslation(m_lang,m_info.at("Creator"))));
    m_accountCreateTimeLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang, "CreateTime")
        +":"+loadTranslation(m_lang,m_info.at("CreateTime"))));
    m_lastRectifyLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang, "LastModifier")
        +":"+loadTranslation(m_lang,m_info.at("LastModifier"))));
    m_lastRectifyTimeLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang, "LastModifyTime")
        +":"+loadTranslation(m_lang,m_info.at("LastModifyTime"))));
    m_loginLogLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang, "ScanLoginRecord")));
    m_loginLogTableW=new QTableWidget(4,9);
    m_loginLogTableW->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_loginLogTableW->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_loginLogTableW->verticalHeader()->setVisible(false);
    m_loginLogTableW->horizontalHeader()->setVisible(false);
    m_loginLogTableW->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_loginLogTableW->setMouseTracking(true);
    connect(m_loginLogTableW, &QTableWidget::cellEntered, [&](int row, int col) {
        if (!m_loginLogTableW->item(row,col)) return;
        QString tooltipText = m_loginLogTableW->item(row, col)->text();
        QToolTip::showText(QCursor::pos(), tooltipText);
    });

    QString data[4][9]={
        {
            QString::fromStdString(loadTranslation(m_lang,"Index")),
            QString::fromStdString(loadTranslation(m_lang,"LoginTime")),
            QString::fromStdString(loadTranslation(m_lang,"QuitTime")),
            QString::fromStdString(loadTranslation(m_lang,"Index")),
            QString::fromStdString(loadTranslation(m_lang,"LoginTime")),
            QString::fromStdString(loadTranslation(m_lang,"QuitTime")),
            QString::fromStdString(loadTranslation(m_lang,"Index")),
            QString::fromStdString(loadTranslation(m_lang,"LoginTime")),
            QString::fromStdString(loadTranslation(m_lang,"QuitTime"))
        },
        {
            "","","","","","","","",""
        },
        {
            "","","","","","","","",""
        },
        {
            "","","","","","","","",""
        }
    };
    for (int row=0;row<4;row++){
        for (int col=0;col<9;col++){
            QTableWidgetItem* item=new QTableWidgetItem(data[row][col]);
            m_loginLogTableW->setItem(row,col,item);
        }
    }

    m_userInfoLayout->addWidget(m_accountStateLabel,0,0,1,1);
    m_userInfoLayout->addWidget(m_accountCreaterLabel,1,0,1,1);
    m_userInfoLayout->addWidget(m_accountCreateTimeLabel,2,0,1,1);
    m_userInfoLayout->addWidget(m_lastRectifyLabel,3,0,1,1);
    m_userInfoLayout->addWidget(m_lastRectifyTimeLabel,4,0,1,1);
    m_userInfoLayout->addWidget(m_loginLogLabel,5,0,1,1);
    m_userInfoLayout->addWidget(m_loginLogTableW,6,0,1,1);

    m_layout->addWidget(m_backBtn,0,6,1,1);
    m_layout->addWidget(m_groupBox,1,0,1,8);
    this->setLayout(m_layout);
}
void UserInfoEditWidget::fnInit()
{
    m_layout=new QGridLayout();
    m_userInfoLayout=new QGridLayout();
    this->setLayout(m_layout);
    m_groupBox=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"NewRectifyUserInfo")),this);
    m_groupBox->setStyleSheet("QGroupBox { font-size: 12pt; font-weight:bold;}");
    m_groupBox->setLayout(m_userInfoLayout);
    
    m_backBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Back")));
    connect(m_backBtn,SIGNAL(clicked()),this,SLOT(clickBack()));
           
    m_userAccountLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"UserAccount")));
    m_passwdInputLimitCountLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"PasswdInputLimitCount")));
    m_userNoLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"UserNo")));
    m_passwdLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"Password")));
    m_eyeLabel=new HGQLabel(false,getPath("/resources/V1/@1xze-eye-o 1.png"));
    connect(m_eyeLabel,SIGNAL(leftClicked()),this,SLOT(slotEyeClicked()));
    
    m_passwdCycleLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"PasswdCycle")));
    m_userNameLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"UserName")));
    m_jobLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"UserJob")));
    m_departmentLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"Department")));
    m_authorityLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"Authority")));
    m_accountManageLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"AccountManagement")));
    m_userDescriptionLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"UserDescription")));
    m_markLabel=new QLabel("*");
    m_markLabel->setStyleSheet("color:red");
    m_markLabel1=new QLabel("*");
    m_markLabel1->setStyleSheet("color:red");
    m_markLabel2=new QLabel(QString::fromStdString(loadTranslation(m_lang,"Day"))+"*");
    m_markLabel2->setStyleSheet("color:red");
    m_markLabel3=new QLabel("*");
    m_markLabel3->setStyleSheet("color:red");
    m_markLabel4=new QLabel("*");
    m_markLabel4->setStyleSheet("color:red");
    m_markLabel5=new QLabel("*");
    m_markLabel5->setStyleSheet("color:red");
    m_markLabel6=new QLabel("*");
    m_markLabel6->setStyleSheet("color:red");
    m_markLabel7=new QLabel("*");
    m_markLabel7->setStyleSheet("color:red");
    m_markLabel8=new QLabel("*");
    m_markLabel8->setStyleSheet("color:red");

    m_userAccountEdit=new QLineEdit();
    m_userAccountEdit->setPlaceholderText(QString::fromStdString(loadTranslation(m_lang,"Input")));
    connect(m_userAccountEdit,&QLineEdit::editingFinished,this,&UserInfoEditWidget::onUserNoEditChanged);
    m_passwdInputLimitCountEdit=new QLineEdit();
    m_passwdInputLimitCountEdit->setPlaceholderText(QString::fromStdString(loadTranslation(m_lang,"Input")));
    m_userNoEdit=new QLineEdit();
    m_userNoEdit->installEventFilter(this);
    m_userNoEdit->setPlaceholderText(QString::fromStdString(loadTranslation(m_lang,"Input")));
    // connect(m_userNoEdit,&QLineEdit::editingFinished,this,&UserInfoEditWidget::onUserNoEditChanged);
    m_passwdEdit=new QLineEdit();
    m_passwdEdit->installEventFilter(this);
    m_passwdEdit->setPlaceholderText(QString::fromStdString(loadTranslation(m_lang,"Input")));
    connect(m_passwdEdit,&QLineEdit::editingFinished,this,&UserInfoEditWidget::onPasswdEditChanged);
    m_passwdEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    m_passwdCycleEdit=new QLineEdit();
    m_passwdCycleEdit->setPlaceholderText(QString::fromStdString(loadTranslation(m_lang,"Input")));
    m_userNameEdit=new QLineEdit();
    m_userNameEdit->setPlaceholderText(QString::fromStdString(loadTranslation(m_lang,"Input")));
    m_jobEdit=new QLineEdit();
    m_jobEdit->setPlaceholderText(QString::fromStdString(loadTranslation(m_lang,"Input")));
    m_departmentEdit=new QLineEdit();
    m_departmentEdit->setPlaceholderText(QString::fromStdString(loadTranslation(m_lang,"Input")));

    m_userDescriptionEdit=new QTextEdit();
    m_userDescriptionEdit->setPlaceholderText(QString::fromStdString(loadTranslation(m_lang,"Input")));

    m_passwdStandardLabel=new QLabel();
    std::ostringstream ss;
    ss << "密码规则：\n"
       << "强：长度>=12且字符类型==4 (包含大写、小写、数字、特殊字符)\n"
       << "中：长度>=8且字符类型>=2 (至少两种字符类型）\n"
       << "弱：其他情况";
    m_passwdStandardLabel->setText(QString::fromStdString(ss.str()));

    // m_authorityCombo=new QComboBox();

    // if (GlobalSingleton::instance().getSystemInfo("userGroupName")==""){
    //     GlobalSingleton::instance().setSystemInfo("userGroupName", "SystemManager:Manager:Operator:MaintenanceStaff:");
    // }

    // std::vector<std::string> usersGroupName=splitStr(GlobalSingleton::instance().getSystemInfo("userGroupName"),':');
    // usersGroupName.erase(usersGroupName.begin()+usersGroupName.size()-1);
    // for (int i=0;i<int(usersGroupName.size());i++){
    //     m_authorityCombo->addItem(QString::fromStdString(loadTranslation(m_lang,usersGroupName[i])));
    // }

          
    m_authorityEdit=new QLineEdit();
    m_accountManageCombo=new QComboBox();
    m_accountManageCombo->addItems({
        QString::fromStdString(loadTranslation(m_lang,"Enable")),
        QString::fromStdString(loadTranslation(m_lang,"Disable")),
        QString::fromStdString(loadTranslation(m_lang,"Locked"))
    });
    connect(m_accountManageCombo,QOverload<int>::of(&QComboBox::currentIndexChanged),this,
        &UserInfoEditWidget::onAccountComboBoxChanged);

    m_okBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Ok")));

    connect(m_okBtn,&QPushButton::clicked,this,&UserInfoEditWidget::slotOk);

    m_userInfoLayout->addWidget(m_userNoLabel,0,0);
    m_userInfoLayout->addWidget(m_userNoEdit,0,1);
    m_userInfoLayout->addWidget(m_markLabel,0,2);
    m_userInfoLayout->addWidget(m_userAccountLabel,m_userInfoLayout->rowCount(),0);
    m_userInfoLayout->addWidget(m_userAccountEdit,m_userInfoLayout->rowCount()-1,1);
    m_userInfoLayout->addWidget(m_markLabel7,m_userInfoLayout->rowCount()-1,2);
    m_userInfoLayout->addWidget(m_passwdLabel,m_userInfoLayout->rowCount(),0);
    m_userInfoLayout->addWidget(m_passwdEdit,m_userInfoLayout->rowCount()-1,1);
    m_userInfoLayout->addWidget(m_markLabel1,m_userInfoLayout->rowCount()-1,2);
    m_userInfoLayout->addWidget(m_eyeLabel,m_userInfoLayout->rowCount()-1,3);
    m_userInfoLayout->addWidget(m_passwdInputLimitCountLabel,m_userInfoLayout->rowCount(),0);
    m_userInfoLayout->addWidget(m_passwdInputLimitCountEdit,m_userInfoLayout->rowCount()-1,1);
    m_userInfoLayout->addWidget(m_markLabel8,m_userInfoLayout->rowCount()-1,2);
    m_userInfoLayout->addWidget(m_passwdCycleLabel,m_userInfoLayout->rowCount(),0);
    m_userInfoLayout->addWidget(m_passwdCycleEdit,m_userInfoLayout->rowCount()-1,1);
    m_userInfoLayout->addWidget(m_markLabel2,m_userInfoLayout->rowCount()-1,2);
    m_userInfoLayout->addWidget(m_userNameLabel,m_userInfoLayout->rowCount(),0);
    m_userInfoLayout->addWidget(m_userNameEdit,m_userInfoLayout->rowCount()-1,1);
    m_userInfoLayout->addWidget(m_markLabel3,m_userInfoLayout->rowCount()-1,2);
    m_userInfoLayout->addWidget(m_jobLabel,m_userInfoLayout->rowCount(),0);
    m_userInfoLayout->addWidget(m_jobEdit,m_userInfoLayout->rowCount()-1,1);
    m_userInfoLayout->addWidget(m_markLabel5,m_userInfoLayout->rowCount()-1,2);
    m_userInfoLayout->addWidget(m_departmentLabel,m_userInfoLayout->rowCount(),0);
    m_userInfoLayout->addWidget(m_departmentEdit,m_userInfoLayout->rowCount()-1,1);
    m_userInfoLayout->addWidget(m_markLabel4,m_userInfoLayout->rowCount()-1,2);
    m_userInfoLayout->addWidget(m_authorityLabel,m_userInfoLayout->rowCount(),0);
    m_userInfoLayout->addWidget(m_authorityEdit,m_userInfoLayout->rowCount()-1,1);
    // m_userInfoLayout->addWidget(m_markLabel5,m_userInfoLayout->rowCount()-1,2);
    m_userInfoLayout->addWidget(m_accountManageLabel,m_userInfoLayout->rowCount(),0);
    m_userInfoLayout->addWidget(m_accountManageCombo,m_userInfoLayout->rowCount()-1,1);
    m_userInfoLayout->addWidget(m_markLabel6,m_userInfoLayout->rowCount()-1,2);
    m_userInfoLayout->addWidget(m_userDescriptionLabel,m_userInfoLayout->rowCount(),0);
    m_userInfoLayout->addWidget(m_userDescriptionEdit,m_userInfoLayout->rowCount()-1,1,2,1);
    m_userInfoLayout->addWidget(m_passwdStandardLabel,m_userInfoLayout->rowCount(),1,2,1);
    m_userInfoLayout->addWidget(m_okBtn,m_userInfoLayout->rowCount()-1,3);

    m_layout->addWidget(m_backBtn,0,6,1,1);
    m_layout->addWidget(m_groupBox,1,0,1,8);
}

UserInfoEditWidget::~UserInfoEditWidget()
{
    SAFE_DELETE(m_layout);
    SAFE_DELETE(m_userInfoLayout);
    SAFE_DELETE(m_groupBox);
    SAFE_DELETE(m_backBtn);
    SAFE_DELETE(m_accountStateLabel);
    SAFE_DELETE(m_accountCreaterLabel);
    SAFE_DELETE(m_accountCreateTimeLabel);
    SAFE_DELETE(m_lastRectifyLabel);
    SAFE_DELETE(m_lastRectifyTimeLabel);
    SAFE_DELETE(m_loginLogLabel);
    SAFE_DELETE(m_loginLogTableW);
    SAFE_DELETE(m_userAccountLabel);
    SAFE_DELETE(m_passwdInputLimitCountLabel);
    SAFE_DELETE(m_userNoLabel);
    SAFE_DELETE(m_passwdLabel);
    SAFE_DELETE(m_eyeLabel);
    SAFE_DELETE(m_passwdCycleLabel);
    SAFE_DELETE(m_userNameLabel);
    SAFE_DELETE(m_jobLabel);
    SAFE_DELETE(m_departmentLabel);
    SAFE_DELETE(m_authorityLabel);
    SAFE_DELETE(m_accountManageLabel);
    SAFE_DELETE(m_userDescriptionLabel);
    SAFE_DELETE(m_markLabel);
    SAFE_DELETE(m_markLabel1);
    SAFE_DELETE(m_markLabel2);
    SAFE_DELETE(m_markLabel3);
    SAFE_DELETE(m_markLabel4);
    SAFE_DELETE(m_markLabel5);
    SAFE_DELETE(m_markLabel6);
    SAFE_DELETE(m_markLabel7);
    SAFE_DELETE(m_markLabel8);
    SAFE_DELETE(m_userAccountEdit);
    SAFE_DELETE(m_passwdInputLimitCountEdit);
    SAFE_DELETE(m_userNoEdit);
    SAFE_DELETE(m_passwdEdit);
    SAFE_DELETE(m_passwdCycleEdit);
    SAFE_DELETE(m_userNameEdit);
    SAFE_DELETE(m_jobEdit);
    SAFE_DELETE(m_departmentEdit);
    SAFE_DELETE(m_userDescriptionEdit);
    SAFE_DELETE(m_passwdStandardLabel);
    SAFE_DELETE(m_authorityEdit);
    SAFE_DELETE(m_accountManageCombo);
    SAFE_DELETE(m_okBtn);
}
bool UserInfoEditWidget::closeWindow()
{
    GlobalSingleton::instance().saveSystemInfo();
    GlobalSingleton::instance().saveUsersInfo();
    return true;
}
void UserInfoEditWidget::slotEyeClicked(){
    if (m_passwdEdit->echoMode() == QLineEdit::Password ||
            m_passwdEdit->echoMode() == QLineEdit::PasswordEchoOnEdit)
    {
        m_passwdEdit->setEchoMode(QLineEdit::Normal);
    } else {
        m_passwdEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    }
}
bool UserInfoEditWidget::eventFilter(QObject *obj, QEvent *event){
    if (event->type() == QEvent::MouseButtonPress){
        QMouseEvent* mouseEvent=static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton){

        }
    } else {
        return QWidget::eventFilter(obj, event);
    }
    return true;
}
void UserInfoEditWidget::onPasswdEditChanged(){
    passwdStrength(m_passwdEdit->text().toStdString());
}
void UserInfoEditWidget::onUserNoEditChanged(){
    emit signalUserNo(m_userAccountEdit->text().toStdString());
}
void UserInfoEditWidget::hasUserNo(bool flag){
    if (m_type==USER_RECTIFY) return;
    if (flag){
        m_userNoEdit->setFocus();
        m_markLabel7->setText(QString::fromStdString(loadTranslation(m_lang,"AlreadyExist")));
    } else {
        m_markLabel7->setText("✔ *");
    }
}
void UserInfoEditWidget::passwdStrength(const std::string &passwd){
    std::string level = checkPasswordStrengthToString(passwd);
    m_markLabel1->setText(QString::fromStdString(loadTranslation(m_lang,level)));
}
void UserInfoEditWidget::slotOk()
{
    std::map<std::string,std::string> userInfoMap;
    // HGOnlineRWDB::deleteDB(USERINFODBNAME);
    if (m_userNoEdit->text().isEmpty()){
        QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),
            QString::fromStdString(loadTranslation(m_lang,"PleaseinputUserNo")));
        m_userNoEdit->setFocus();
        return;
    }
    if (m_passwdEdit->text().isEmpty()){
        QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),
            QString::fromStdString(loadTranslation(m_lang,"PleaseinputPassword")));
        m_passwdEdit->setFocus();
        return;
    }
    if (m_passwdCycleEdit->text().isEmpty()){
        QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),
            QString::fromStdString(loadTranslation(m_lang,"PleaseinputPasswdCycle")));
        m_passwdCycleEdit->setFocus();
        return;
    } else {
        QRegularExpression re("^\\d+$");
        if (!re.match(m_passwdCycleEdit->text()).hasMatch()) {
            QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),
                "密码更新周期 需要输入纯数字，请修改");
            m_passwdCycleEdit->setFocus();
            return;
        }
    }
    if (m_userNameEdit->text().isEmpty()){
        QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),
            QString::fromStdString(loadTranslation(m_lang,"PleaseinputUserName")));
        m_userNameEdit->setFocus();
        return;
    }
    if (m_jobEdit->text().isEmpty()){
        QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),
            QString::fromStdString(loadTranslation(m_lang,"PleaseinputUserJob")));
        m_jobEdit->setFocus();
        return;
    }
    if (m_departmentEdit->text().isEmpty()){
        QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),
            QString::fromStdString(loadTranslation(m_lang,"PleaseinputDepartment")));
        m_departmentEdit->setFocus();
        return;
    }

    if (m_accountManageCombo->currentText().isEmpty()){
        QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),
            QString::fromStdString(loadTranslation(m_lang,"PleaseinputAccountManager")));
        m_accountManageCombo->setFocus();
        return;
    }
    if (m_userAccountEdit->text().isEmpty()){
        QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),
            QString::fromStdString(loadTranslation(m_lang,"PleaseinputAccountName")));
        m_userAccountEdit->setFocus();
        return; 
    }
    if (m_passwdInputLimitCountEdit->text().isEmpty()){
        QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),
            QString::fromStdString(loadTranslation(m_lang,"PleaseinputPasswdLimitCount")));
        m_passwdInputLimitCountEdit->setFocus();
        return;
    } else {
        QRegularExpression re("^\\d+$");
        if (!re.match(m_passwdInputLimitCountEdit->text()).hasMatch()) {
            QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),
                QString::fromStdString(loadTranslation(m_lang,"PleaseinputPasswdLimitCount"))+" 需要输入纯数字，请修改");
            m_passwdInputLimitCountEdit->setFocus();
            return;
        }
    }
    userInfoMap["UserNo"]=m_userNoEdit->text().toStdString();
    userInfoMap["UserAccount"]=m_userAccountEdit->text().toStdString();
    userInfoMap["Password"]=m_passwdEdit->text().toStdString();
    userInfoMap["PasswdInputLimitCount"]=m_passwdInputLimitCountEdit->text().toStdString();
    if (m_type == USER_CREATE) userInfoMap["PasswdCycle"]=m_passwdCycleEdit->text().toStdString();
    else if (m_type == USER_RECTIFY){
        std::vector<std::string> strs = splitStr(m_info.at("PasswdCycle"),';');
        if (strs.size() >1){
            userInfoMap["PasswdCycle"]=m_passwdCycleEdit->text().toStdString()+";"+ strs[1];
        } else {
            userInfoMap["PasswdCycle"]=m_passwdCycleEdit->text().toStdString();
        }
    }
    userInfoMap["UserName"]=m_userNameEdit->text().toStdString();
    userInfoMap["UserJob"]=m_jobEdit->text().toStdString();
    userInfoMap["Department"]=m_departmentEdit->text().toStdString();
    if (m_type==USER_CREATE) userInfoMap["Authority"]="";
    else if (m_type==USER_RECTIFY) userInfoMap["Authority"]=m_info.at("Authority");
    // userInfoMap["Authority"]=findTranslationKey(m_lang,m_authorityCombo->currentText().toStdString());
    userInfoMap["AccountManagement"]=findTranslationKey(m_lang,m_accountManageCombo->currentText().toStdString());
    userInfoMap["UserDescription"]=m_userDescriptionEdit->toPlainText().toStdString();
    // if (findTranslationKey(m_lang, m_markLabel1->text().toStdString()) == "weak"){
    //     QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),
    //         "强制使用中强密码");
    //     m_passwdEdit->setFocus();
    //     return;
    // }
    switch (m_type){
        case USER_CREATE:
        {
            if (m_markLabel7->text() != "✔ *")
            {
                m_userAccountEdit->setFocus();
                return;
            }
            userInfoMap["Creator"]=GlobalSingleton::instance().getSystemInfo("loginName");
            userInfoMap["CreateTime"]=getStandardCurTime();
            userInfoMap["LastModifier"]="";
            userInfoMap["LastModifyTime"]="";
            userInfoMap["LoginTime"]="";
            userInfoMap["QuitTime"]="";
            break;
        }
        case USER_RECTIFY:{
            userInfoMap["LastModifier"]=GlobalSingleton::instance().getSystemInfo("loginName");
            userInfoMap["LastModifyTime"]=getStandardCurTime();
            userInfoMap["Creator"]=m_info["Creator"];
            userInfoMap["CreateTime"]=m_info["CreateTime"];
            userInfoMap["LoginTime"]=m_info["LoginTime"];
            userInfoMap["QuitTime"]=m_info["QuitTime"];
            break;
        }
        default:break;
    }
    
    if (m_type == USER_CREATE){
        RWDb::writeAuditTrailLog("创建用户"+userInfoMap["UserAccount"]+"成功");
    } else if (m_type == USER_RECTIFY){
        std::ostringstream ss;
        ss << "修改用户" << userInfoMap["UserAccount"] << " 修改信息：";
        if (userInfoMap["UserAccount"] != m_info["UserAccount"]) ss << m_userAccountLabel->text().toStdString() << ", from " << m_info["UserAccount"]<<"->"<<userInfoMap["UserAccount"];
        if (userInfoMap["Password"] != m_info["Password"]) {
            ss << m_passwdLabel->text().toStdString() << ", from " << m_info["Password"]<<"->"<<userInfoMap["Password"];
            userInfoMap["CreateTime"]=m_info["CreateTime"]+";"+getStandardCurTime();
        }
        if (userInfoMap["PasswdInputLimitCount"] != m_info["PasswdInputLimitCount"]) ss << m_passwdInputLimitCountLabel->text().toStdString() << ", from " << m_info["PasswdInputLimitCount"]<<"->"<<userInfoMap["PasswdInputLimitCount"];
        if (userInfoMap["PasswdCycle"] != m_info["PasswdCycle"]) ss << m_passwdCycleLabel->text().toStdString() << ", from " << splitStr(m_info["PasswdCycle"],';')[0] <<"->"<< splitStr(userInfoMap["PasswdCycle"],';')[0];
        if (userInfoMap["UserName"] != m_info["UserName"]) ss << m_userNameLabel->text().toStdString() << ", from " << m_info["UserName"]<<"->"<<userInfoMap["UserName"];
        if (userInfoMap["UserJob"] != m_info["UserJob"]) ss << m_jobLabel->text().toStdString() << ", from " << m_info["UserJob"]<<"->"<<userInfoMap["UserJob"];
        if (userInfoMap["Department"] != m_info["Department"]) ss << m_departmentLabel->text().toStdString() << ", from " << m_info["Department"]<<"->"<<userInfoMap["Department"];
        // if (userInfoMap["Authority"] != m_info["Authority"]) ss << m_authorityLabel->text().toStdString() << ", from " << m_info["Authority"]<<"->"<<userInfoMap["Authority"];
        if (userInfoMap["AccountManagement"] != m_info["AccountManagement"]) ss << m_accountManageLabel->text().toStdString() << ", from " << m_info["AccountManagement"]<<"->"<<userInfoMap["AccountManagement"];
        if (userInfoMap["UserDescription"] != m_info["UserDescription"]) ss << m_userDescriptionLabel->text().toStdString() << ", from " << m_info["UserDescription"]<<"->"<<userInfoMap["UserDescription"];
        
        if (userInfoMap["UserAccount"]==GlobalSingleton::instance().getSystemInfo("loginName") &&
            GlobalSingleton::instance().getSystemInfo("免密登录")!="true"){
            GlobalSingleton::instance().setSystemInfo("authority",userInfoMap["Authority"]);
            emit updateAuthority();
        }
        RWDb::writeAuditTrailLog(ss.str());
    }
    emit signalUserInfo(userInfoMap);
}
void UserInfoEditWidget::clickBack()
{
    emit signalBack();
}
void UserInfoEditWidget::onAccountComboBoxChanged(int index){

}
