#include "userinfoeditwidget.h"
#include <QHeaderView>
#include <QMessageBox>
#include "common.h"
#include <QToolTip>
#include "loginterface.h"
#include "SvcFactory.h"
#include "UserAuditManager.h"
#include "SystemDataManager.h"

using namespace HGMACHINE;

UserInfoEditWidget::UserInfoEditWidget(int type,std::string lang,const std::map<std::string,std::string>& info,QWidget *parent) : QWidget(parent),
    m_lang(lang),
    m_info(info),
    m_type(type)
{
    std::string enterUsersManageName=SystemDataManager::instance().get().getSystemInfo("enterUsersManageName");
    std::string authority=UserAuditManager::instance().get().getUserAuthority(enterUsersManageName);
    permissionInfo = UserAuditManager::instance().get().getAuthorityDetail(authority);

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
                m_passwdCycleEdit->setText(QString::fromStdString(SvcFactory::CreateCommonService()->SplitString(m_info.at("PasswdCycle"),';')[0]));
                m_userNameEdit->setText(QString::fromStdString(m_info.at("UserName")));
                m_jobEdit->setText(QString::fromStdString(m_info.find("UserJob")!=m_info.end()?m_info.at("UserJob"):""));
                m_departmentEdit->setText(QString::fromStdString(m_info.at("Department")));
                std::string authorityTemp="";
                std::vector<std::string> authoritys=SvcFactory::CreateCommonService()->SplitString(m_info.at("Authority"),';');
                for(int i=0;i<int(authoritys.size());i++){
                    if (authoritys[i]=="") continue;
                    authorityTemp+=SvcFactory::CreateConfigService()->LoadTranslation(m_lang,authoritys[i])+';';
                }
                m_authorityEdit->setText(QString::fromStdString(authorityTemp));
                // m_authorityCombo->setCurrentText(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang, m_info.at("Authority"))));
                m_accountManageCombo->setCurrentText(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang, m_info.at("AccountManagement"))));
                m_userDescriptionEdit->setPlainText(QString::fromStdString(m_info.at("UserDescription")));
            } else {
                std::string nocount = UserAuditManager::instance().get().getUserInfoLastCount();
                m_userNoEdit->setText(QString::fromStdString(nocount));
            }
            m_userNoEdit->setEnabled(false);
            break;
        }
        case USER_SCAN:{
            fnInitScan();
            std::vector<std::string> resLogin=SvcFactory::CreateCommonService()->SplitString(m_info["LoginTime"], ';');
            std::vector<std::string> resQuit=SvcFactory::CreateCommonService()->SplitString(m_info["QuitTime"], ';');
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
    m_groupBox=new QGroupBox(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"ScanUserInfo")),this);
    m_groupBox->setStyleSheet("QGroupBox { font-size: 12pt; font-weight:bold;}");
    m_groupBox->setLayout(m_userInfoLayout);

    m_backBtn=new QPushButton(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Back")));
    connect(m_backBtn,SIGNAL(clicked()),this,SLOT(clickBack()));

    m_accountStateLabel=new QLabel(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang, "AccountManagement")
        +":"+SvcFactory::CreateConfigService()->LoadTranslation(m_lang,m_info.at("AccountManagement"))));
    m_accountCreaterLabel=new QLabel(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang, "Creator")
        +":"+SvcFactory::CreateConfigService()->LoadTranslation(m_lang,m_info.at("Creator"))));
    m_accountCreateTimeLabel=new QLabel(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang, "CreateTime")
        +":"+SvcFactory::CreateConfigService()->LoadTranslation(m_lang,m_info.at("CreateTime"))));
    m_lastRectifyLabel=new QLabel(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang, "LastModifier")
        +":"+SvcFactory::CreateConfigService()->LoadTranslation(m_lang,m_info.at("LastModifier"))));
    m_lastRectifyTimeLabel=new QLabel(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang, "LastModifyTime")
        +":"+SvcFactory::CreateConfigService()->LoadTranslation(m_lang,m_info.at("LastModifyTime"))));
    m_loginLogLabel=new QLabel(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang, "ScanLoginRecord")));
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
            QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Index")),
            QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"LoginTime")),
            QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"QuitTime")),
            QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Index")),
            QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"LoginTime")),
            QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"QuitTime")),
            QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Index")),
            QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"LoginTime")),
            QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"QuitTime"))
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
    m_groupBox=new QGroupBox(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"NewRectifyUserInfo")),this);
    m_groupBox->setStyleSheet("QGroupBox { font-size: 12pt; font-weight:bold;}");
    m_groupBox->setLayout(m_userInfoLayout);
    
    m_backBtn=new QPushButton(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Back")));
    connect(m_backBtn,SIGNAL(clicked()),this,SLOT(clickBack()));
           
    m_userAccountLabel=new QLabel(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"UserAccount")));
    m_passwdInputLimitCountLabel=new QLabel(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"PasswdInputLimitCount")));
    m_userNoLabel=new QLabel(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"UserNo")));
    m_passwdLabel=new QLabel(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Password")));
    m_eyeLabel=new HGQLabel(false,getPath("/resources/V1/@1xze-eye-o 1.png"));
    connect(m_eyeLabel,SIGNAL(SvcFactory::CreateConfigService()->LeftClicked()),this,SLOT(slotEyeClicked()));
    
    m_passwdCycleLabel=new QLabel(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"PasswdCycle")));
    m_userNameLabel=new QLabel(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"UserName")));
    m_jobLabel=new QLabel(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"UserJob")));
    m_departmentLabel=new QLabel(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Department")));
    m_authorityLabel=new QLabel(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Authority")));
    m_accountManageLabel=new QLabel(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"AccountManagement")));
    m_userDescriptionLabel=new QLabel(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"UserDescription")));
    m_markLabel=new QLabel("*");
    m_markLabel->setStyleSheet("color:red");
    m_markLabel1=new QLabel("*");
    m_markLabel1->setStyleSheet("color:red");
    m_markLabel2=new QLabel(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Day"))+"*");
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

    QIntValidator *intValidator = new QIntValidator(1, 999, this);
    m_userAccountEdit=new QLineEdit();
    m_userAccountEdit->setPlaceholderText(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Input")));
    connect(m_userAccountEdit,&QLineEdit::editingFinished,this,&UserInfoEditWidget::onUserNoEditChanged);
    m_passwdInputLimitCountEdit=new QLineEdit();
    m_passwdInputLimitCountEdit->setValidator(intValidator);
    m_passwdInputLimitCountEdit->setPlaceholderText(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Input")));
    m_userNoEdit=new QLineEdit();
    m_userNoEdit->installEventFilter(this);
    m_userNoEdit->setPlaceholderText(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Input")));
    // connect(m_userNoEdit,&QLineEdit::editingFinished,this,&UserInfoEditWidget::onUserNoEditChanged);
    m_passwdEdit=new QLineEdit();
    m_passwdEdit->installEventFilter(this);
    m_passwdEdit->setPlaceholderText(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Input")));
    connect(m_passwdEdit,&QLineEdit::editingFinished,this,&UserInfoEditWidget::onPasswdEditChanged);
    m_passwdEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    m_passwdCycleEdit=new QLineEdit();
    m_passwdCycleEdit->setValidator(intValidator);
    m_passwdCycleEdit->setPlaceholderText(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Input")));
    m_userNameEdit=new QLineEdit();
    m_userNameEdit->setPlaceholderText(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Input")));
    m_jobEdit=new QLineEdit();
    m_jobEdit->setPlaceholderText(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Input")));
    m_departmentEdit=new QLineEdit();
    m_departmentEdit->setPlaceholderText(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Input")));

    m_userDescriptionEdit=new QTextEdit();
    m_userDescriptionEdit->setPlaceholderText(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Input")));

    m_passwdStandardLabel=new QLabel();
    std::ostringstream ss;
    ss << "密码规则：\n"
       << "强：长度>=12且字符类型==4 (包含大写、小写、数字、特殊字符)\n"
       << "中：长度>=8且字符类型>=2 (至少两种字符类型）\n"
       << "弱：其他情况";
    m_passwdStandardLabel->setText(QString::fromStdString(ss.str()));

    // m_authorityCombo=new QComboBox();

    // if (SystemDataManager::instance().get().getSystemInfo("userGroupName")==""){
    //     SystemDataManager::instance().get().setSystemInfo("userGroupName", "SystemManager:Manager:Operator:MaintenanceStaff:");
    // }

    // std::vector<std::string> usersGroupName=splitStr(SystemDataManager::instance().get().getSystemInfo("userGroupName"),':');
    // usersGroupName.erase(usersGroupName.begin()+usersGroupName.size()-1);
    // for (int i=0;i<int(usersGroupName.size());i++){
    //     m_authorityCombo->addItem(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,usersGroupName[i])));
    // }

          
    m_authorityEdit=new QLineEdit();
    m_accountManageCombo=new QComboBox();
    m_accountManageCombo->addItems({
        QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Enable")),
        QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Disable")),
        QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Locked"))
    });
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    connect(m_accountManageCombo,&QComboBox::currentIndexChanged,this,
        &UserInfoEditWidget::onAccountComboBoxChanged);
#else
    connect(m_accountManageCombo, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this, &UserInfoEditWidget::onAccountComboBoxChanged);
#endif
    m_okBtn=new QPushButton(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Ok")));
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
    if (m_layout);{
        delete m_layout;
        m_layout=nullptr;
    }
    if (m_userInfoLayout);{
        delete m_userInfoLayout;
        m_userInfoLayout=nullptr;
    }
    if (m_groupBox);{
        delete m_groupBox;
        m_groupBox=nullptr;
    }
    if (m_backBtn);{
        delete m_backBtn;
        m_backBtn=nullptr;
    }
    if (m_accountStateLabel);{
        delete m_accountStateLabel;
        m_accountStateLabel=nullptr;
    }
    if (m_accountCreaterLabel);{
        delete m_accountCreaterLabel;
        m_accountCreaterLabel=nullptr;
    }
    if (m_accountCreateTimeLabel);{
        delete m_accountCreateTimeLabel;
        m_accountCreateTimeLabel=nullptr;
    }
    if (m_lastRectifyLabel);{
        delete m_lastRectifyLabel;
        m_lastRectifyLabel=nullptr;
    }
    if (m_lastRectifyTimeLabel);{
        delete m_lastRectifyTimeLabel;
        m_lastRectifyTimeLabel=nullptr;
    }
    if (m_loginLogLabel);{
        delete m_loginLogLabel;
        m_loginLogLabel=nullptr;
    }
    if (m_loginLogTableW);{
        delete m_loginLogTableW;
        m_loginLogTableW=nullptr;
    }
    if (m_userAccountLabel);{
        delete m_userAccountLabel;
        m_userAccountLabel=nullptr;
    }
    if (m_passwdInputLimitCountLabel);{
        delete m_passwdInputLimitCountLabel;
        m_passwdInputLimitCountLabel=nullptr;
    }
    if (m_userNoLabel);{
        delete m_userNoLabel;
        m_userNoLabel=nullptr;
    }
    if (m_passwdLabel);{
        delete m_passwdLabel;
        m_passwdLabel=nullptr;
    }
    if (m_eyeLabel);{
        delete m_eyeLabel;
        m_eyeLabel=nullptr;
    }
    if (m_passwdCycleLabel);{
        delete m_passwdCycleLabel;
        m_passwdCycleLabel=nullptr;
    }
    if (m_userNameLabel);{
        delete m_userNameLabel;
        m_userNameLabel=nullptr;
    }
    if (m_jobLabel);{
        delete m_jobLabel;
        m_jobLabel=nullptr;
    }
    if (m_departmentLabel);{
        delete m_departmentLabel;
        m_departmentLabel=nullptr;
    }
    if (m_authorityLabel);{
        delete m_authorityLabel;
        m_authorityLabel=nullptr;
    }
    if (m_accountManageLabel);{
        delete m_accountManageLabel;
        m_accountManageLabel=nullptr;
    }
    if (m_userDescriptionLabel);{
        delete m_userDescriptionLabel;
        m_userDescriptionLabel=nullptr;
    }
    if (m_markLabel);{
        delete m_markLabel;
        m_markLabel=nullptr;
    }
    if (m_markLabel1);{
        delete m_markLabel1;
        m_markLabel1=nullptr;
    }
    if (m_markLabel2);{
        delete m_markLabel2;
        m_markLabel2=nullptr;
    }
    if (m_markLabel3);{
        delete m_markLabel3;
        m_markLabel3=nullptr;
    }
    if (m_markLabel4);{
        delete m_markLabel4;
        m_markLabel4=nullptr;
    }
    if (m_markLabel5);{
        delete m_markLabel5;
        m_markLabel5=nullptr;
    }
    if (m_markLabel6);{
        delete m_markLabel6;
        m_markLabel6=nullptr;
    }
    if (m_markLabel7);{
        delete m_markLabel7;
        m_markLabel7=nullptr;
    }
    if (m_markLabel8);{
        delete m_markLabel8;
        m_markLabel8=nullptr;
    }
    if (m_userAccountEdit);{
        delete m_userAccountEdit;
        m_userAccountEdit=nullptr;
    }
    if (m_passwdInputLimitCountEdit);{
        delete m_passwdInputLimitCountEdit;
        m_passwdInputLimitCountEdit=nullptr;
    }
    if (m_userNoEdit);{
        delete m_userNoEdit;
        m_userNoEdit=nullptr;
    }
    if (m_passwdEdit);{
        delete m_passwdEdit;
        m_passwdEdit=nullptr;
    }
    if (m_passwdCycleEdit);{
        delete m_passwdCycleEdit;
        m_passwdCycleEdit=nullptr;
    }
    if (m_userNameEdit);{
        delete m_userNameEdit;
        m_userNameEdit=nullptr;
    }
    if (m_jobEdit);{
        delete m_jobEdit;
        m_jobEdit=nullptr;
    }
    if (m_departmentEdit);{
        delete m_departmentEdit;
        m_departmentEdit=nullptr;
    }
    if (m_userDescriptionEdit);{
        delete m_userDescriptionEdit;
        m_userDescriptionEdit=nullptr;
    }
    if (m_passwdStandardLabel);{
        delete m_passwdStandardLabel;
        m_passwdStandardLabel=nullptr;
    }
    if (m_authorityEdit);{
        delete m_authorityEdit;
        m_authorityEdit=nullptr;
    }
    if (m_accountManageCombo);{
        delete m_accountManageCombo;
        m_accountManageCombo=nullptr;
    }
    if (m_okBtn);{
        delete m_okBtn;
        m_okBtn=nullptr;
    }
}
bool UserInfoEditWidget::closeWindow()
{
    SystemDataManager::instance().get().saveSystemInfo();
    UserAuditManager::instance().get().saveUsersInfo();
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
        m_markLabel7->setText(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"AlreadyExist")));
    } else {
        m_markLabel7->setText("✔ *");
    }
}
void UserInfoEditWidget::passwdStrength(const std::string &passwd){
    std::string level = SvcFactory::CreateCommonService()->CheckPasswordStrengthToString(passwd);
    m_markLabel1->setText(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,level)));
}
void UserInfoEditWidget::slotOk()
{
    std::map<std::string,std::string> userInfoMap;
    // HGOnlineRWDB::deleteDB(USERINFODBNAME);
    if (m_userNoEdit->text().isEmpty()){
        QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),
            QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"PleaseinputUserNo")));
        m_userNoEdit->setFocus();
        return;
    }
    if (m_passwdEdit->text().isEmpty()){
        QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),
            QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"PleaseinputPassword")));
        m_passwdEdit->setFocus();
        return;
    }
    if (m_passwdCycleEdit->text().isEmpty()){
        QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),
            QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"PleaseinputPasswdCycle")));
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
            QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"PleaseinputUserName")));
        m_userNameEdit->setFocus();
        return;
    }
    if (m_jobEdit->text().isEmpty()){
        QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),
            QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"PleaseinputUserJob")));
        m_jobEdit->setFocus();
        return;
    }
    if (m_departmentEdit->text().isEmpty()){
        QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),
            QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"PleaseinputDepartment")));
        m_departmentEdit->setFocus();
        return;
    }

    if (m_accountManageCombo->currentText().isEmpty()){
        QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),
            QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"PleaseinputAccountManager")));
        m_accountManageCombo->setFocus();
        return;
    }
    if (m_userAccountEdit->text().isEmpty()){
        QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),
            QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"PleaseinputAccountName")));
        m_userAccountEdit->setFocus();
        return; 
    }
    if (m_passwdInputLimitCountEdit->text().isEmpty()){
        QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),
            QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"PleaseinputPasswdLimitCount")));
        m_passwdInputLimitCountEdit->setFocus();
        return;
    } else {
        QRegularExpression re("^\\d+$");
        if (!re.match(m_passwdInputLimitCountEdit->text()).hasMatch()) {
            QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),
                QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"PleaseinputPasswdLimitCount"))+" 需要输入纯数字，请修改");
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
        std::vector<std::string> strs = SvcFactory::CreateCommonService()->SplitString(m_info.at("PasswdCycle"),';');
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
    userInfoMap["AccountManagement"]=SvcFactory::CreateConfigService()->FindTranslationKey(m_lang,m_accountManageCombo->currentText().toStdString());
    userInfoMap["UserDescription"]=m_userDescriptionEdit->toPlainText().toStdString();
    if (SvcFactory::CreateConfigService()->FindTranslationKey(m_lang, m_markLabel1->text().toStdString()) == "weak"){
        QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),
            "强制使用中强密码");
        m_passwdEdit->setFocus();
        return;
    }
    switch (m_type){
        case USER_CREATE:
        {
            if (m_markLabel7->text() != "✔ *")
            {
                m_userAccountEdit->setFocus();
                return;
            }
            userInfoMap["Creator"]=SystemDataManager::instance().get().getSystemInfo("loginName");
            userInfoMap["CreateTime"]=SvcFactory::CreateCommonService()->GetStandardCurTime();
            userInfoMap["LastModifier"]="";
            userInfoMap["LastModifyTime"]="";
            userInfoMap["LoginTime"]="";
            userInfoMap["QuitTime"]="";
            break;
        }
        case USER_RECTIFY:{
            userInfoMap["LastModifier"]=SystemDataManager::instance().get().getSystemInfo("loginName");
            userInfoMap["LastModifyTime"]=SvcFactory::CreateCommonService()->GetStandardCurTime();
            userInfoMap["Creator"]=m_info["Creator"];
            userInfoMap["CreateTime"]=m_info["CreateTime"];
            userInfoMap["LoginTime"]=m_info["LoginTime"];
            userInfoMap["QuitTime"]=m_info["QuitTime"];
            break;
        }
        default:break;
    }
    
    if (m_type == USER_CREATE){
        LOG_IF.writeAuditTrailLog("创建用户"+userInfoMap["UserAccount"]+"成功");
    } else if (m_type == USER_RECTIFY){
        std::ostringstream ss;
        ss << "修改用户" << userInfoMap["UserAccount"] << " 修改信息：";
        if (userInfoMap["UserAccount"] != m_info["UserAccount"]) ss << m_userAccountLabel->text().toStdString() << ", from " << m_info["UserAccount"]<<"->"<<userInfoMap["UserAccount"];
        if (userInfoMap["Password"] != m_info["Password"]) {
            ss << m_passwdLabel->text().toStdString() << ", from " << m_info["Password"]<<"->"<<userInfoMap["Password"];
            userInfoMap["CreateTime"]=m_info["CreateTime"]+";"+SvcFactory::CreateCommonService()->GetStandardCurTime();
        }
        if (userInfoMap["PasswdInputLimitCount"] != m_info["PasswdInputLimitCount"]) ss << m_passwdInputLimitCountLabel->text().toStdString() << ", from " << m_info["PasswdInputLimitCount"]<<"->"<<userInfoMap["PasswdInputLimitCount"];
        if (userInfoMap["PasswdCycle"] != m_info["PasswdCycle"]) ss << m_passwdCycleLabel->text().toStdString() << ", from " << SvcFactory::CreateCommonService()->SplitString(m_info["PasswdCycle"],';')[0] <<"->"<< SvcFactory::CreateCommonService()->SplitString(userInfoMap["PasswdCycle"],';')[0];
        if (userInfoMap["UserName"] != m_info["UserName"]) ss << m_userNameLabel->text().toStdString() << ", from " << m_info["UserName"]<<"->"<<userInfoMap["UserName"];
        if (userInfoMap["UserJob"] != m_info["UserJob"]) ss << m_jobLabel->text().toStdString() << ", from " << m_info["UserJob"]<<"->"<<userInfoMap["UserJob"];
        if (userInfoMap["Department"] != m_info["Department"]) ss << m_departmentLabel->text().toStdString() << ", from " << m_info["Department"]<<"->"<<userInfoMap["Department"];
        // if (userInfoMap["Authority"] != m_info["Authority"]) ss << m_authorityLabel->text().toStdString() << ", from " << m_info["Authority"]<<"->"<<userInfoMap["Authority"];
        if (userInfoMap["AccountManagement"] != m_info["AccountManagement"]) ss << m_accountManageLabel->text().toStdString() << ", from " << m_info["AccountManagement"]<<"->"<<userInfoMap["AccountManagement"];
        if (userInfoMap["UserDescription"] != m_info["UserDescription"]) ss << m_userDescriptionLabel->text().toStdString() << ", from " << m_info["UserDescription"]<<"->"<<userInfoMap["UserDescription"];
        
        if (userInfoMap["UserAccount"]==SystemDataManager::instance().get().getSystemInfo("loginName") &&
            SystemDataManager::instance().get().getSystemInfo("免密登录")!="true"){
            SystemDataManager::instance().get().setSystemInfo("authority",userInfoMap["Authority"]);
            emit updateAuthority();
        }
        LOG_IF.writeAuditTrailLog(ss.str());
    }
    emit signalUserInfo(userInfoMap);
}
void UserInfoEditWidget::clickBack()
{
    emit signalBack();
}
void UserInfoEditWidget::onAccountComboBoxChanged(int index){

}
