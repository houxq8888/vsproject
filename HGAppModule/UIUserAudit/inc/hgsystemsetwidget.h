#ifndef HGSYSTEMSETWIDGET_H
#define HGSYSTEMSETWIDGET_H

#include <QWidget>
#include "hgqlabel.h"
#include "hgdeviceinfowidget.h"
#include "hgsoftwareinfowidget.h"
#include "userregisterwidget.h"
#include "dateandtimewidget.h"
#include "displayandlightwidget.h"
#include "languagewidget.h"
#include "warningwidget.h"
#include "networkwidget.h"
#include "hgserviceinfowidget.h"
#include <QFrame>
#include "userlistwidget.h"
#include "labelwithimg.h"
#include "myToolBox.h"


enum {
    SYSTEMSET_DEVICEINFO,
    SYSTEMSET_SYSTEMSET,
    SYSTEMSET_SEARCH,
    SYSTEMSET_SERVICE,
};

class HGSystemSetWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGSystemSetWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();
    ~HGSystemSetWidget();
    bool isToolBoxNull();
    void setWarnSoundVolumnMute(const bool &flag);

signals:
    void signalSoundVolumn(int);
    void updateLanguage(std::string);

private slots:
    void slotClickDeviceInfo();
    void slotClickSystemSet();
    void slotClickSearchBefore();
    void slotClickDServiceInfo();
    void slotEnterUserManage();
    void clickNoWdLogin();
    void clickTrackLog();
    void clickDebugMode();
    void slotBackToSystemSet();


private:
    HGDeviceInfoWidget *m_deviceInfoW;
    HGSoftwareInfoWidget *m_softwareInfoW;
    UserRegisterWidget* m_userRegisterW;
    UserListWidget* m_userListW;
    HGServiceInfoWidget* m_serviceInfoW;
    QLineEdit* m_inputEdit;
    HGQLabel* m_searchLabel;
    QFrame* m_vline,*m_hline,*m_hline1;
    LabelWithImg* m_deviceInfoLabel;
    LabelWithImg* m_systemSetLabel, *m_searchBeforeLabel,*m_serviceInfoLabel;

    QGridLayout* m_layout;
    std::vector<std::string> permissionInfo;
    
    int m_choice;
    QLabel* m_lockLabel;
    LabelWithImg* m_loginStatusLabel,*m_trackLogLabel,*m_debugModelLabel;
    LabelWithImg* m_trackLogWarnLabel, *m_debugModeWarnLabel;
    bool m_isNoPwdLogin,m_trackLogFlag,m_debugModeFlag;

    MyToolBox *m_toolBox;
    int m_loginAuthority;
    std::string m_lang;

};

#endif // HGSYSTEMSETWIDGET_H
