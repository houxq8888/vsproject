#ifndef HGLOGINWIDGET_H
#define HGLOGINWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "hgqlabel.h"
#include <QGridLayout>
#include <QGroupBox>
#include <QComboBox>
#include "common.h"


class HGLoginWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGLoginWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();

signals:
    void loginOn(UserInfoS);

private slots:
    void clickLoginOn();
    void clickReset();
    
private:
    HGQLabel* m_imgLabel,*m_imgLabel1;
    QLabel* m_softwareNameLabel,*m_userLabel,*m_userNameLabel,*m_userpwdLabel;
    QLineEdit *m_userpwdEdit;
    QComboBox* m_userNameCombo;

    QPushButton* m_resetBtn,*m_loginBtn;
    QGridLayout* m_layout, *m_userLayout;
    QGroupBox* m_userGroup;

    UserInfoS m_userInfoStruct;
    std::string m_lang;

};

#endif // HGLOGINWIDGET_H
