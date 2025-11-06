#ifndef USERINFOEDITWIDGET_H
#define USERINFOEDITWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QGroupBox>
#include <QTextEdit>
#include <QComboBox>
#include <QTableWidget>
#include "hgqlabel.h"

enum{
    USER_CREATE,
    USER_RECTIFY,
    USER_SCAN
};

class UserInfoEditWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UserInfoEditWidget(int type,std::string,const std::map<std::string,std::string>&,QWidget *parent = nullptr);
    bool closeWindow();
    void hasUserNo(bool);
    void passwdStrength(const std::string &level);

signals:
    void signalUserNo(std::string);
    void signalBack();
    void signalUserInfo(const std::map<std::string,std::string>&);
    void updateAuthority();

public slots:
    void clickBack();
    void slotOk();
    void onAccountComboBoxChanged(int);
    void onUserNoEditChanged();
    void onPasswdEditChanged();
    void slotEyeClicked();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
private:
    void fnInit();
    void fnInitScan();

private:
    QGroupBox* m_groupBox;
    QGridLayout* m_layout, *m_userInfoLayout;
    QLabel* m_jobLabel;
    QLineEdit *m_jobEdit;
    QLabel* m_userNoLabel,*m_passwdLabel,*m_passwdCycleLabel,*m_userNameLabel,*m_departmentLabel,*m_authorityLabel,*m_accountManageLabel,*m_userDescriptionLabel;
    QLineEdit* m_userNoEdit,*m_passwdEdit,*m_passwdCycleEdit,*m_userNameEdit,*m_departmentEdit;

    QLabel* m_userAccountLabel, *m_passwdInputLimitCountLabel;
    QLineEdit* m_userAccountEdit, *m_passwdInputLimitCountEdit;
    HGQLabel* m_eyeLabel;
    QComboBox *m_accountManageCombo;
    QTextEdit *m_userDescriptionEdit;
    QPushButton* m_backBtn, *m_okBtn;
    QLineEdit* m_authorityEdit;
    std::string m_lang;
    QLabel* m_markLabel,*m_markLabel1,*m_markLabel2,*m_markLabel3,*m_markLabel4,*m_markLabel5,*m_markLabel6;
    QLabel* m_markLabel7,*m_markLabel8;
    std::map<std::string,std::string> m_info;

    QLabel* m_passwdStandardLabel;

    QLabel* m_accountStateLabel, *m_accountCreaterLabel,*m_accountCreateTimeLabel;
    QLabel* m_lastRectifyLabel, *m_lastRectifyTimeLabel;
    QLabel* m_loginLogLabel;
    QTableWidget *m_loginLogTableW;
    std::vector<std::string> permissionInfo;
    int m_type;
};

#endif