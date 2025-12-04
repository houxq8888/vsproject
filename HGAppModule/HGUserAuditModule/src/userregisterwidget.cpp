#include "userregisterwidget.h"
#include "common.h"
#include <QShortcut>
#include <QMessageBox>
#include "globalsingleton.h"


UserRegisterWidget::UserRegisterWidget(std::string lang,bool isNoPwdLogin,QWidget *parent) : QWidget(parent),
m_lang(lang)
{
    RWDb::writeAuditTrailLog(loadTranslation(m_lang,"Enter")+loadTranslation(m_lang,"Date from"));
    m_isNoPwdLogin=isNoPwdLogin;
    fnInit();
}
void UserRegisterWidget::fnInit()
{
    m_layout=new QGridLayout();
    m_widgetLayout=new QGridLayout();
    this->setLayout(m_widgetLayout);
    m_groupBox=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"UserManage")),this);
    m_groupBox->setStyleSheet("QGroupBox { font-size: 12pt; font-weight:bold;}");

    m_groupBox->setLayout(m_layout);

    m_userLabel=new QLabel();
    m_userLabel->setText(QString::fromStdString(loadTranslation(m_lang,"UserAccount")));
    m_userEdit=new QLineEdit();
    m_userEdit->setPlaceholderText(QString::fromStdString(loadTranslation(m_lang,"Input")));
    connect(m_userEdit,SIGNAL(textChanged(QString)),this,SLOT(slotGetUserName(QString)));


    m_passwdLabel=new QLabel();
    m_passwdLabel->setText(QString::fromStdString(loadTranslation(m_lang,"Password")));//"密码");
    m_passwdEdit=new QLineEdit();
    m_passwdEdit->setPlaceholderText(QString::fromStdString(loadTranslation(m_lang,"Input")));
    m_passwdEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    connect(m_passwdEdit,SIGNAL(textChanged(QString)),this,SLOT(slotGetUserPwd(QString)));

    m_registerBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Login")));
    QShortcut *shortcut=new QShortcut(QKeySequence(Qt::Key_Return),this);
    connect(shortcut,&QShortcut::activated,m_registerBtn,&QPushButton::click);
    connect(m_registerBtn,        SIGNAL(clicked()),this,SLOT(clickRegister()));


    m_layout->setSpacing(10);
    m_layout->addWidget(m_userLabel,0,4,1,1);
    m_layout->addWidget(m_userEdit,0,5,1,1);
    m_layout->addWidget(m_passwdLabel,1,4,1,1);
    m_layout->addWidget(m_passwdEdit,1,5,1,1);
    m_layout->addWidget(m_registerBtn,2,7);

    m_widgetLayout->addWidget(m_groupBox,1,0,1,7);
    setUniformSizePolicy(m_layout,QSizePolicy::Expanding,QSizePolicy::Expanding);
}
bool UserRegisterWidget::closeWindow()
{
    GlobalSingleton::instance().saveUsersInfo();
    return true;
}
void UserRegisterWidget::clickRegister()
{
    std::vector<std::string> authorities = splitStr(GlobalSingleton::instance().getSystemInfo("authority"),';');
    if(GlobalSingleton::instance().getSystemInfo("免密登录")=="true")
    { 
        emit registerUserManage();
        return;
    }
    std::string userName=m_userEdit->text().toStdString();
    std::string userPwd=m_passwdEdit->text().toStdString();
    if (std::find(authorities.begin(),authorities.end(),"Manager")==authorities.end()
            && std::find(authorities.begin(),authorities.end(),"SystemManager")==authorities.end()
            && std::find(authorities.begin(),authorities.end(),"所有权限")==authorities.end())
    {
        if (userName=="" || userPwd==""){
            QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),"系统登录账户没有权限访问，请在此页面输入超级管理员/管理员/厂家账号，才可进入！");
            return;
        } else if (userName!="") {
            std::string authority=GlobalSingleton::instance().getUserAuthority(userName);
            if (authority.find("Manager")==std::string::npos &&
                authority.find("SystemManager")==std::string::npos &&
                authority.find("所有权限")==std::string::npos)
            {
                QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),
                    "输入账户["+QString::fromStdString(userName)+"]没有权限访问，请在此页面输入超级管理员/管理员/厂家账号，才可进入！");
                RWDb::writeAuditTrailLog("输入账户["+userName+"]没有权限访问，请在此页面输入超级管理员/管理员/厂家账号，才可进入！");
                return;
            } else {
                GlobalSingleton::instance().setSystemInfo("enterUsersManageName", userName);
                GlobalSingleton::instance().setSystemInfo("enterUsersManageAuthority", authority);
            }
        }
    } else {
        RWDb::writeAuditTrailLog("系统登录账户有用户组管理权限，可进入");
        GlobalSingleton::instance().setSystemInfo("enterUsersManageName", GlobalSingleton::instance().getSystemInfo("loginName"));
        GlobalSingleton::instance().setSystemInfo("enterUsersManageAuthority", GlobalSingleton::instance().getSystemInfo("authority"));

        emit registerUserManage();
        return;
    }

    bool found=false;
    bool passwdFlag=false;
    std::vector<std::map<std::string,std::string>> userInfos=GlobalSingleton::instance().getUsersInfo();
    std::string userNo,userAuthority,maxWrongPasswdCnt;
    int index=-1;

    for (int i=0;i<int(userInfos.size());i++){
        if (userInfos[i]["UserAccount"] == userName)
        {
            found=true;
            if (userInfos[i].at("AccountManagement") != "Enable")
            {
                if (userInfos[i]["AccountManagement"] == "Disable")
                {
                    QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), QString::fromStdString(userName)+"该用户已禁用！");
                    RWDb::writeAuditTrailLog(userName+ "已禁用！");
                    return;
                }
                else
                {
                    std::string management = userInfos[i]["AccountManagement"];
                    size_t start = management.find("[");
                    size_t end = management.find("]");
                    std::string timeStr;
                    if (start != std::string::npos && end != std::string::npos && end > start)
                    {
                        timeStr = management.substr(start + 1, end - start - 1);
                        HGExactTime time=HGExactTime::currentTime();
                        int countsecond = time.fasterThanThirtyMimutes(timeStr);
                        int minutes = int(countsecond / 60.0 + 0.5);
                        if (minutes < 30){
                            QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "该用户已被锁，剩余时间：" + QString::number(30-minutes) + "分钟！");
                            RWDb::writeAuditTrailLog(userName+ "该用户已被锁，剩余时间：" + std::to_string(30-minutes) + "分钟！");
                            return;
                        } else {
                            QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "该用户已解锁");
                            RWDb::writeAuditTrailLog(userName+ "该用户已解锁");
                        }
                    }        
                }
            }
            maxWrongPasswdCnt=userInfos[i]["PasswdInputLimitCount"];
            index=i;
            GlobalSingleton::instance().setUserField(index,"AccountManagement","Enable");

            if (userPwd == "")
            {
                QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "请输入密码！");
                RWDb::writeAuditTrailLog("请输入密码！");
                return;
            }
            if (userInfos[i]["Password"] == userPwd)
            {
                passwdFlag = true;
                userNo = userInfos[i]["UserNo"];
                userAuthority = userInfos[i]["Authority"];

                break;
            }
        }
        if (found) break;
    }
    static int wrongPasswdCnt=0;
    if (!found)
    {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "用户不存在！");
        RWDb::writeAuditTrailLog(userName+"用户不存在！");
        return;
    }
    else
    {
        if (!passwdFlag)
        {
            wrongPasswdCnt++;
            if (wrongPasswdCnt > std::atoi(maxWrongPasswdCnt.c_str()))
            {
                QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "密码输入错误次数已达上限[" + QString::number(wrongPasswdCnt) + "],账户已锁定，请30分钟后再试!");
                RWDb::writeAuditTrailLog(userName+ "密码输入错误次数已达上限[" + std::to_string(wrongPasswdCnt) + "],账户已锁定，请30分钟后再试!");
                GlobalSingleton::instance().setUserField(index,"AccountManagement","Locked");
                HGExactTime curTime=HGExactTime::currentTime();
                GlobalSingleton::instance().addUserField(index,"AccountManagement", "["+curTime.toStringFromYearToSec()+"]");
                wrongPasswdCnt = 0;
                return;
            }else {
                QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "密码错误！");
                RWDb::writeAuditTrailLog(userName+ "密码错误！");
                return;
            }
        }
        else
        {
            wrongPasswdCnt=0;
        }
    }

    RWDb::writeAuditTrailLog(userName+" 进入用户管理页面");
    emit registerUserManage();
}

void UserRegisterWidget::slotGetUserName(QString text){

}
void UserRegisterWidget::slotGetUserPwd(QString text){

}
void UserRegisterWidget::slotGetDepart(QString text){

}