#ifndef USERREGISTERWIDGET_H
#define USERREGISTERWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QPushButton>
#include <QComboBox>
#include "labelwithimg.h"


class UserRegisterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UserRegisterWidget(std::string,bool isNoPwdLogin, QWidget *parent = nullptr);
    bool closeWindow();
signals:
    void registerUserManage();
    
private slots:
    void clickRegister();
    void slotGetUserName(QString);
    void slotGetUserPwd(QString);
    void slotGetDepart(QString);

private:
    void fnInit();

private:
    QGridLayout *m_layout, *m_widgetLayout;
    QGroupBox* m_groupBox;
    QLabel* m_userLabel;
    QLineEdit* m_userEdit;
    QLabel* m_passwdLabel;
    QLineEdit* m_passwdEdit;
    QPushButton* m_registerBtn;

    bool m_isNoPwdLogin;
    std::string m_lang;

};

#endif // USERREGISTERWIDGET_H
