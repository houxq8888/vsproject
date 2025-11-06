#ifndef USERLISTWIDGET_H
#define USERLISTWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QPushButton>
#include "labelwithimg.h"
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QComboBox>
#include <QTabWidget>
#include "userwidget.h"
#include "authoritywidget.h"



class UserListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UserListWidget(std::string,const std::vector<std::string> &authority,QWidget *parent = nullptr);
    ~UserListWidget();
    bool closeWindow();

signals:
    void signalBack();
    void updateAuthority();
    
private slots:
    void clickBack();
    void slotUpdateAuthority();

private:
    void fnInit();

private:
    std::vector<std::string> m_whole_authority;
    QGridLayout *m_layout;

    QPushButton *m_backBtn;

    std::string m_lang;

    UserWidget* m_userW;
    AuthorityWidget* m_authorityW;

    QTabWidget* m_tabWidget;

    int m_curPhase;
};

#endif // USERLISTWIDGET_H
