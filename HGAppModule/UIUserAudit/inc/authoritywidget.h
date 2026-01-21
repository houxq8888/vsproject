#ifndef AUTHORITYWIDGET_H
#define AUTHORITYWIDGET_H

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


enum {
    USER_SCAN_USERS,
    USER_AUTHORITY,
    USER_INIT
};

class AuthorityWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AuthorityWidget(std::string,const std::vector<std::string>&,QWidget *parent = nullptr);
    ~AuthorityWidget();
    bool closeWindow();
    void fnReadDB();

signals:
    void signalBack();
    void updateAuthority();
    
private slots:
    void setAuthority();
    void onUsersAuthorityClicked(int,int);
    void onUsersComboBoxChanged(int);
    void slotAddUser();
    void slotRemoveUser();
    void slotScanAuthorityInfo();
    void clickSelfDefine();
    void clickMoreInfo();
    void clickBack();

protected:
    // 重载 closeEvent 方法
    void closeEvent(QCloseEvent *event) override;
private:
    void fnWriteDB();
    void fnInit();
    void fnFillAuthorityList();
    int getColumnIndexByName(QTableWidget* table,const QString &columnName);
    void findNextRowAndCol(QTableWidget* tableW,std::string key,int& row,int& col);
    int  findUserAuthorityIndex(const std::string &userNo);
    void fnInitAuthorityTable();
    void fnResetAuthorityToEnable();
    void fnFillAuthorityInfo();
    void fnRemoveUsrsGroupUser(const std::string& authorityName, const std::string &text);
    void fnAddUsrsGroupUser(const std::string& authorityName, const std::string &text);
    void fnRemoveAuthorityItem(const std::string& authorityName, const std::string &text);
    void fnAddAuthorityItem(const std::string &authorityName, const std::string &text);
    bool fnHasUsersManageAuthority();
    bool fnHasUserManageAuthority();


private:
    QGridLayout *m_usersLayout;

    QTableWidget* m_usersAuthorityInfoW;
    QPushButton* m_selfDefineBtn, *m_moreInfoBtn;
    //
    QLabel* m_usersLabel;
    QComboBox* m_usersComboBox;
    QLabel* m_userLabel;
    QPushButton* m_addUserBtn, *m_removeUserBtn;
    QTableWidget* m_usersAuthorityTableW, *m_userTableW;
    QPushButton* m_setAuthorityBtn;
    QPushButton* m_backBtn;

    // 
    std::vector<std::string> m_whole_authority;
    std::vector<std::string> permissionInfo;
    QPushButton* m_scanAuthorityInfoBtn;

    std::string m_lang;
    bool m_setAuthorityFlag;
    int m_curPhase;
    
};

#endif // AUTHORITYWIDGET_H
