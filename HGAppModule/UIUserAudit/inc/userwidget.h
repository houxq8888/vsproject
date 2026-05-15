#ifndef USERWIDGET_H
#define USERWIDGET_H

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
#include "userinfoeditwidget.h"
#include <QHeaderView>
#include <QTextCharFormat>



class UserWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UserWidget(std::string,QWidget *parent = nullptr);
    ~UserWidget();
    bool closeWindow();
    void fnReadDB();

signals:
    void signalBack();
    void updateAuthority();
    
private slots:
    void newUser();
    void deleteUser();
    void editUser();
    void scanUser();
    void slotUserInfo(const std::map<std::string,std::string>&);
    void onUserManageClicked(int,int);
    void slotHasUserNo(std::string);
    void returnToList();
    void slotUpdateAuthority();
    void onSearchTextChanged(const QString &text);
    void highlightSearchResults(const QString &text);

protected:
    // 重载closeEvent函数
    void closeEvent(QCloseEvent *event);
private:
    void fnAddUserListW();
    void fnWriteDB();
    void fnInit();
    void fnFillUserList();
    int getColumnIndexByName(QTableWidget* table,const QString &columnName);
    void findNextRowAndCol(QTableWidget* tableW,std::string key,int& row,int& col);
    int  findUserAuthorityIndex(const std::string &userNo);
    void fnRemoveUsrsGroupUser(const std::string& authorityName, const std::string &text);
    void fnAddUsrsGroupUser(const std::string& authorityName, const std::string &text);
    void fnSetupSearchWidget();
    void clearHighlights();
    void filterTable(const QString &text);

private:
    QGridLayout *m_userLayout;

    //

    // 
    std::vector<std::string> permissionInfo;
    QLabel* m_userManageLabel;
    QTableWidget* m_userManageTableW;
    HGQLabel* m_newUserLabel, *m_deleteUserLabel, *m_editUserLabel, *m_scanUserLabel;

    std::string m_lang;

    UserInfoEditWidget* m_userInfoEditWidget;
    
    // Search functionality
    QLineEdit* m_searchLineEdit;
    QLabel* m_searchLabel;
    QWidget* m_searchWidget;
    QGridLayout* m_searchLayout;
    QString m_currentSearchText;
    QList<QColor> m_highlightColors;

};

#endif // USERWIDGET_H
