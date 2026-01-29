#include "hgloginwidget.h"
#include <QDebug>
#include <QMessageBox>
#include <QShortcut>
#include "common.h"
#include "SvcFactory.h"
#include "loginterface.h"
#include "UserAuditManager.h"
#include "SystemDataManager.h"

using namespace HGMACHINE;


HGLoginWidget::HGLoginWidget(std::string lang,QWidget *parent) : QWidget(parent),
    m_lang(lang)
{     
    LOG_IF.writeAuditTrailLog(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Enter")+SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"User Login"));

    if (SystemDataManager::instance().get().getSystemInfo("loginNames")=="") {
        SystemDataManager::instance().get().setSystemInfo("loginNames", "admin:");
    }
    std::vector<std::string> logins = SvcFactory::CreateCommonService()->SplitString(SystemDataManager::instance().get().getSystemInfo("loginNames"),':');
    
    m_imgLabel=new HGQLabel(true,getPath("/resources/login.png"));
    m_imgLabel1=new HGQLabel(false,getPath("/resources/login.png"));

    m_softwareNameLabel=new QLabel(QString::fromStdString(SvcFactory::CreateFrameService()->getAppName()));
    QColor color(127,131,247);
    m_softwareNameLabel->setStyleSheet(QString("color:%1;").arg(color.name()));
    QFont font=m_softwareNameLabel->font();
    font.setPointSize(64);
    m_softwareNameLabel->setFont(font);
    m_softwareNameLabel->setAlignment(Qt::AlignCenter);
    
    m_userLabel=new QLabel(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"User Login")));
    color=QColor(147,210,243);
    m_userLabel->setStyleSheet(QString("color:%1;").arg(color.name()));
    font=m_userLabel->font();
    font.setPointSize(32);
    m_userLabel->setFont(font);
    m_userLabel->setAlignment(Qt::AlignCenter);

    m_userNameLabel=new QLabel(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"User")));//"用户名");
    m_userNameLabel->setStyleSheet(QString("color:%1;").arg(color.name()));
    font=m_userNameLabel->font();
    font.setPointSize(20);
    m_userNameLabel->setFont(font);

    m_userpwdLabel=new QLabel(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Password")));
    m_userpwdLabel->setStyleSheet(QString("color:%1;").arg(color.name()));
    font=m_userpwdLabel->font();
    font.setPointSize(20);
    m_userpwdLabel->setFont(font);
    

    m_userNameCombo=new QComboBox();
    m_userNameCombo->setEditable(true);
    for (int i=0;i<int(logins.size());i++){
        if (logins[i]=="") continue;
        m_userNameCombo->addItem(QString::fromStdString(logins[i]));
    }
    m_userpwdEdit=new QLineEdit();
    m_userpwdEdit->setPlaceholderText(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Input")));
    m_userpwdEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);

    m_resetBtn=new QPushButton(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Reset")));//"重置");
    m_loginBtn=new QPushButton(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"Login")));
    m_loginBtn->setShortcut(Qt::Key_Enter);
    QShortcut *shortcut=new QShortcut(QKeySequence(Qt::Key_Return),this);
    connect(shortcut,&QShortcut::activated,m_loginBtn,&QPushButton::click);
    connect(m_resetBtn,SIGNAL(clicked()),this,SLOT(clickReset()));
    connect(m_loginBtn,SIGNAL(clicked()),this,SLOT(clickLoginOn()));

    m_layout=new QGridLayout();
    this->setLayout(m_layout);

    m_userGroup=new QGroupBox();
    m_userLayout=new QGridLayout();
    m_userGroup->setLayout(m_userLayout);

    m_userLayout->addWidget(m_userLabel,0,2,1,1);
    m_userLayout->addWidget(m_userNameLabel,1,0,2,1);
    m_userLayout->addWidget(m_userNameCombo,1,2,2,2);
    m_userLayout->addWidget(m_userpwdLabel,3,0,2,1);
    m_userLayout->addWidget(m_userpwdEdit,3,2,2,2);
    m_userLayout->addWidget(m_resetBtn,5,2,2,1);
    m_userLayout->addWidget(m_loginBtn,5,3,2,1);

    m_layout->addWidget(m_imgLabel,0,0,1,4);
    m_layout->addWidget(m_softwareNameLabel,1,0,1,4);
    m_layout->addWidget(m_userGroup,2,1,1,2);
    m_layout->addWidget(m_imgLabel1,4,0,1,4);
    m_layout->setRowStretch(0,2);
    m_layout->setRowStretch(1,3);
    m_layout->setRowStretch(2,8);
    m_layout->setRowStretch(3,2);
    m_layout->setRowStretch(4,2);

    if (SystemDataManager::instance().get().getSystemInfo("免密登录")=="true"){
        if (m_userNameCombo->findText("admin")==-1){
            m_userNameCombo->addItem("admin");
        }
        m_userNameCombo->setCurrentText("admin");
        m_userpwdEdit->setText("admin");
    }
}
bool HGLoginWidget::closeWindow()
{
    SystemDataManager::instance().get().saveSystemInfo();
    UserAuditManager::instance().get().saveUsersInfo();
    return true;
}
void HGLoginWidget::clickReset()
{
    std::string userName = m_userNameCombo->currentText().toStdString();
    bool found=false;
    std::vector<std::map<std::string,std::string>> np=UserAuditManager::instance().get().getUsersInfo();
    int index=-1;

    if (userName == ""){
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "请输入账号！");
        return;
    }
    for (int i=0;i<int(np.size());i++){
        if (np[i].at("UserAccount")==userName){
            found=true;
            index=i;
            break;
        }
    }
    if (!found){
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "用户不存在！");
        return;
    }
    QDialog dialog(this);
    dialog.setWindowTitle(QString::fromStdString(SvcFactory::CreateConfigService()->LoadTranslation(m_lang,"修改密码")));
    dialog.setWindowModality(Qt::ApplicationModal);
    QLabel* originPasswd=new QLabel("原始密码");
    QLineEdit* originPasswdEdit = new QLineEdit();
    originPasswdEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    QLabel* newPasswd=new QLabel("新密码");
    QLineEdit* newPasswdEdit = new QLineEdit();
    newPasswdEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    QLabel* confirmPasswd=new QLabel("确认密码");
    QLineEdit* confirmPasswdEdit = new QLineEdit();
    confirmPasswdEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    QPushButton* okBtn=new QPushButton("确定");
    QPushButton* cancelBtn=new QPushButton("取消");
    connect(okBtn,&QPushButton::clicked,[&](){
        std::string userPass = originPasswdEdit->text().toStdString();
        if (UserAuditManager::instance().get().getUserField(index,"Password")!=userPass){
            QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "原始密码错误！");
            return;
        }
        if (newPasswdEdit->text().toStdString()!=confirmPasswdEdit->text().toStdString()){
            QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "新密码不一致！");
            return;
        } else {
            UserAuditManager::instance().get().setUserField(index,"Password",confirmPasswdEdit->text().toStdString());
            UserAuditManager::instance().get().setUserField(index,"AccountManagement","Enable");
            UserAuditManager::instance().get().addUserField(index,"CreateTime",";"+SvcFactory::CreateCommonService()->GetStandardCurTime());
            UserAuditManager::instance().get().setUserField(index,"LastModifyTime",SvcFactory::CreateCommonService()->GetStandardCurTime());
            QMessageBox::information(this, QString::fromStdString(HG_DEVICE_NAME), "修改成功！");
        }
        dialog.close();
    });
    connect(cancelBtn,&QPushButton::clicked,[&](){
        dialog.close();
    });

    QGridLayout* layout=new QGridLayout(&dialog);
    layout->addWidget(originPasswd,0,0,1,1);
    layout->addWidget(originPasswdEdit,0,1,1,2);
    layout->addWidget(newPasswd,1,0,1,1);
    layout->addWidget(newPasswdEdit,1,1,1,2);
    layout->addWidget(confirmPasswd,2,0,1,1);
    layout->addWidget(confirmPasswdEdit,2,1,1,2);
    layout->addWidget(okBtn,3,1,1,1);
    layout->addWidget(cancelBtn,3,2,1,1);

    dialog.exec();
}
void HGLoginWidget::clickLoginOn()
{
    if (SystemDataManager::instance().get().getSystemInfo("免密登录")=="true"){
        // HGOnlineRWDB::writeLoginInfo("admin","");
        SystemDataManager::instance().get().setSystemInfo("loginName", "admin");
        SystemDataManager::instance().get().setSystemInfo("authority","");
        LOG_IF.writeAuditTrailLog("admin 免密登录");
        if (SystemDataManager::instance().get().getSystemInfo("loginNames").find("admin")==std::string::npos){
            std::string loginNames = SystemDataManager::instance().get().getSystemInfo("loginNames");
            if (loginNames[loginNames.length()-1]!=':') SystemDataManager::instance().get().addSystemInfo("loginNames", ":admin:");
            else SystemDataManager::instance().get().addSystemInfo("loginNames", "admin:");
        }
        
        LOG_IF.writeAuditTrailLog("免密登录 success");
        m_userInfoStruct = ServiceInterfaces::UserInfoS();
        emit loginOn(m_userInfoStruct);
        return;
    }
    std::string userName=m_userNameCombo->currentText().toStdString();
    std::string userPwd=m_userpwdEdit->text().toStdString();
    if (userName=="") {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "请输入用户名和密码！");
        return;
    }

    std::string authorityType;
    bool accountflag=false, passwdFlag=false;
    std::vector<std::map<std::string,std::string>> userInfos=UserAuditManager::instance().get().getUsersInfo();
    std::string userNo,userAuthority,maxWrongPasswdCnt;
    int index=-1;
    for (int i=0;i<int(userInfos.size());i++){
        if (userInfos[i]["UserAccount"] == userName)
        {
            accountflag = true;
            if (userInfos[i].at("AccountManagement").find("Enable")==std::string::npos)
            {
                if (userInfos[i]["AccountManagement"] == "Disable")
                {
                    QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "该用户已禁用！");
                    LOG_IF.writeAuditTrailLog("用户 " + userName + " 登录失败，用户已禁用");
                    return;
                } else if (userInfos[i]["AccountManagement"] == "LockedForever"){
                    QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "该用户已永久被锁！");
                    LOG_IF.writeAuditTrailLog("用户 " + userName + " 登录失败，用户已永久被锁");
                    return;
                } else {
                    std::string management = userInfos[i]["AccountManagement"];
                    size_t start = management.find("[");
                    size_t end = management.find("]");
                    std::string timeStr;
                    int lockDuration=0;
                    if (start != std::string::npos && end != std::string::npos && end > start)
                    {
                        std::string result = management.substr(start+1, end - start - 1);
                        std::vector<std::string> splitStrs = SvcFactory::CreateCommonService()->SplitString(result,':');
                        if (splitStrs.size() >0) timeStr=splitStrs[0];
                        if (splitStrs.size() >1) lockDuration=std::atoi(splitStrs[1].c_str());
                        // lockDuration=0;// debug
                        int countsecond = SvcFactory::CreateTimeService()->GetTimeDifferenceInSeconds(timeStr);
                        if (countsecond < lockDuration){
                            QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "该用户已被锁，剩余时间：" + QString::number(lockDuration-countsecond) + "秒!");
                            LOG_IF.writeAuditTrailLog("该用户已被锁，剩余时间："+std::to_string(lockDuration-countsecond)+"秒");
                            return;
                        } 
                    }        
                }
            }
            maxWrongPasswdCnt=userInfos[i]["PasswdInputLimitCount"];
            index=i;
            if (userPwd == "")
            {
                QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "请输入密码！");
                LOG_IF.writeAuditTrailLog("用户 " + userName + " 登录失败，请输入密码");
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
        if (accountflag) break;
    }
    if (!accountflag)
    {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "用户不存在！");
        LOG_IF.writeAuditTrailLog("用户 " + userName + " 登录失败，用户不存在");
        return;
    }
    else
    {
        std::string loginAuthority=UserAuditManager::instance().get().getUserField(index,"Authority");
        if (!passwdFlag)
        {
            std::string accountManage=UserAuditManager::instance().get().getUserField(index,"AccountManagement");
            if (accountManage.find("Locked")!=std::string::npos){
                int wrongPasswdCnt=0;
                size_t start = accountManage.find("[");
                size_t end = accountManage.find("]");
                if (start != std::string::npos && end != std::string::npos && end > start){
                    std::string result = accountManage.substr(start+1, end - start - 1);
                    std::vector<std::string> splitStrs = SvcFactory::CreateCommonService()->SplitString(result,':');
                    if (splitStrs.size() >2) wrongPasswdCnt=std::atoi(splitStrs[2].c_str());
                }
                wrongPasswdCnt++;
                LOG_IF.writeAuditTrailLog("用户 " + userName + " 曾经锁定，还未真正解锁");
                if (wrongPasswdCnt<=std::atoi(maxWrongPasswdCnt.c_str())+3){
                    QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "密码错误!");
                    UserAuditManager::instance().get().setUserField(index,"AccountManagement","Locked");
                    UserAuditManager::instance().get().addUserField(index,"AccountManagement","["+SvcFactory::CreateTimeService()->GetCurrentTimeFromYearToSec()+":0:"+std::to_string(wrongPasswdCnt)+"]");
                    LOG_IF.writeAuditTrailLog(userName + " 登录密码错误["+std::to_string(wrongPasswdCnt)+"次!");
                    return;
                } else if (wrongPasswdCnt<=std::atoi(maxWrongPasswdCnt.c_str())+8){
                    // 渐近式延迟，每次错误后等待时间延长，如5秒-15秒-30秒-60秒-2分钟
                    std::vector<int> waitTime={5,15,30,60,120};
                    QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "30分钟到期后，密码再次输出错，请"+QString::number(waitTime.at(wrongPasswdCnt-std::atoi(maxWrongPasswdCnt.c_str())-4))+"秒后，请重新输入密码！");
                    LOG_IF.writeAuditTrailLog(userName+" 登录密码错误["+std::to_string(wrongPasswdCnt)+"次!,请"+std::to_string(waitTime.at(wrongPasswdCnt-std::atoi(maxWrongPasswdCnt.c_str())-4))+"秒后，请重新输入密码！");

                    UserAuditManager::instance().get().setUserField(index,"AccountManagement","Locked");
                    UserAuditManager::instance().get().addUserField(index,"AccountManagement","["+SvcFactory::CreateTimeService()->GetCurrentTimeFromYearToSec()+":"+
                        std::to_string(waitTime.at(wrongPasswdCnt-std::atoi(maxWrongPasswdCnt.c_str())-4))+":"+std::to_string(wrongPasswdCnt)+"]");
                    return;
                } else if (wrongPasswdCnt<=std::atoi(maxWrongPasswdCnt.c_str())+11){
                    QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "密码再次输出错，请15分钟后，请重新输入密码！");
                    LOG_IF.writeAuditTrailLog(userName+" 登录密码错误["+std::to_string(wrongPasswdCnt)+"次!,请15分钟后，请重新输入密码！");

                    UserAuditManager::instance().get().setUserField(index,"AccountManagement","Locked");
                    UserAuditManager::instance().get().addUserField(index,"AccountManagement","["+SvcFactory::CreateTimeService()->GetCurrentTimeFromYearToSec()+":900:"+
                        std::to_string(wrongPasswdCnt)+"]");
                    return;
                } else {
                    QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "密码输错已超过12次，已永久锁定，找管理解锁！");
                    LOG_IF.writeAuditTrailLog(userName+" 密码输错已超过12次，已永久锁定，找管理解锁！");
                    UserAuditManager::instance().get().setUserField(index,"AccountManagement","LockedForever");
                    return;
                }
            } else {
                size_t start = accountManage.find("[");
                size_t end = accountManage.find("]");
                std::string timeStr;
                int wrongPasswdCnt=0;
                if (start != std::string::npos && end != std::string::npos && end > start){
                    std::string result = accountManage.substr(start+1, end - start - 1);
                    wrongPasswdCnt=std::atoi(result.c_str());
                }
                wrongPasswdCnt++;
                // first locked
                if (loginAuthority.find("SystemManager")==std::string::npos&&
                    wrongPasswdCnt>=std::atoi(maxWrongPasswdCnt.c_str())){
                    QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "密码输入错误次数已达上限[" + QString::fromStdString(maxWrongPasswdCnt) + "],账户已锁定，请30分钟后再试!");
                    LOG_IF.writeAuditTrailLog(userName+" 密码输入错误次数已达上限["+maxWrongPasswdCnt+"次,账户已锁定，请30分钟后再试!");
                    UserAuditManager::instance().get().setUserField(index,"AccountManagement","Locked");
                    UserAuditManager::instance().get().addUserField(index,"AccountManagement","["+SvcFactory::CreateTimeService()->GetCurrentTimeFromYearToSec()+":"+std::to_string(30*60)+":"+std::to_string(wrongPasswdCnt)+"]");
                    return;
                } else {
                    QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "密码错误！");
                    UserAuditManager::instance().get().setUserField(index,"AccountManagement","Enable["+std::to_string(wrongPasswdCnt)+"]");
                    LOG_IF.writeAuditTrailLog(userName+" 密码错误["+std::to_string(wrongPasswdCnt)+"次!");
                    return;
                }
            }
        }
        else
        {
            UserAuditManager::instance().get().setUserField(index,"AccountManagement","Enable");
            std::string createTimeStr = UserAuditManager::instance().get().getUserField(index,"CreateTime");
            TimeInfo createTime = SvcFactory::CreateTimeService()->GetCurrentTime();
            if (createTimeStr.size() >= 8) {
                createTime.year = atoi(createTimeStr.substr(0, 4).c_str());
                createTime.month = atoi(createTimeStr.substr(4, 2).c_str());
                createTime.day = atoi(createTimeStr.substr(6, 2).c_str());
            }
            int passwdCycle = std::atoi(UserAuditManager::instance().get().getUserField(index,"PasswdCycle").c_str());

            TimeInfo currentTime = SvcFactory::CreateTimeService()->GetCurrentTime();
            int passDay = currentTime - createTime;

            if (loginAuthority.find("SystemManager") == std::string::npos)
            {
                if (UserAuditManager::instance().get().getUserField(index, "PasswdCycle").find(';') != std::string::npos)
                {
                    std::vector<std::string> strs = SvcFactory::CreateCommonService()->SplitString(UserAuditManager::instance().get().getUserField(index, "PasswdCycle"), ';');
                    if (std::atoi(strs[0].c_str()) < std::atoi(strs[1].c_str()))
                    {
                        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "该账号密码已过期，请联系管理员！");
                        UserAuditManager::instance().get().setUserField(index, "PasswdCycle", std::to_string(passwdCycle) + ";" + std::to_string(passDay));
                        return;
                    }
                    else
                    {
                        if (std::atoi(strs[1].c_str()) >= (passwdCycle - 35))
                        {
                            createTime += passwdCycle;
                            std::ostringstream ss;
                            ss << "该账号密码将于" << createTime.year << "年" << createTime.month << "月" << createTime.day << "日" << "失效，请及时修改密码!";
                            QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), QString::fromStdString(ss.str()));
                            LOG_IF.writeAuditTrailLog(userName + ss.str());
                            UserAuditManager::instance().get().setUserField(index, "PasswdCycle", std::to_string(passwdCycle) + ";" + std::to_string(passDay));
                        }
                    }
                }
                else
                {
                    if (passDay > passwdCycle)
                    {
                        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "该账号密码已过期，请联系管理员！");

                        UserAuditManager::instance().get().addUserField(index, "PasswdCycle", ";" + std::to_string(passDay));
                        return;
                    }
                    else if (passDay >= (passwdCycle - 35))
                    {
                        createTime += passwdCycle;
                        std::ostringstream ss;
                        ss << "该账号密码将于" << createTime.year << "年" << createTime.month << "月" << createTime.day << "日" << "失效，请及时修改密码!";
                        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), QString::fromStdString(ss.str()));
                        UserAuditManager::instance().get().addUserField(index, "PasswdCycle", ";" + std::to_string(passDay));
                    }
                    else
                    {
                        UserAuditManager::instance().get().addUserField(index, "PasswdCycle", ";" + std::to_string(passDay));
                    }
                }
            }
            m_userInfoStruct = ServiceInterfaces::UserInfoS(UserAuditManager::instance().get().getUserField(index,"UserName"),
                                         UserAuditManager::instance().get().getUserField(index,"Password"),
                                         UserAuditManager::instance().get().getUserField(index,"Authority"),
                                         UserAuditManager::instance().get().getUserField(index,"Department"),
                                         UserAuditManager::instance().get().getUserField(index,"UserNo"));

            SystemDataManager::instance().get().setSystemInfo("loginName", userName);
            SystemDataManager::instance().get().setSystemInfo("authority",userAuthority);
            LOG_IF.writeAuditTrailLog(UserAuditManager::instance().get().getUserField(index,"UserAccount") + " logined");
            std::vector<std::string> loginTimeCnt=SvcFactory::CreateCommonService()->SplitString(UserAuditManager::instance().get().getUserField(index,"LoginTime"),';');
            if (loginTimeCnt.size() >= 10){
                std::ostringstream ss;
                for (int cntIndex=std::max(0,int(loginTimeCnt.size())-10);
                    cntIndex<int(loginTimeCnt.size())-1;cntIndex++)
                {
                    ss << loginTimeCnt[cntIndex] << ';';
                }
                ss<< SvcFactory::CreateCommonService()->GetStandardCurTime()<<';';
                UserAuditManager::instance().get().setUserField(index, "LoginTime", ss.str());
            } else {
                UserAuditManager::instance().get().addUserField(index, "LoginTime", SvcFactory::CreateCommonService()->GetStandardCurTime()+';');
            }
            if (SystemDataManager::instance().get().getSystemInfo("loginNames").find(userName)==std::string::npos){
                std::string loginNames = SystemDataManager::instance().get().getSystemInfo("loginNames");
                if (loginNames[loginNames.length()-1]!=':') SystemDataManager::instance().get().addSystemInfo("loginNames", ":"+userName+":");
                else SystemDataManager::instance().get().addSystemInfo("loginNames", userName+":");
            }

            emit loginOn(m_userInfoStruct);
        }
    }
}