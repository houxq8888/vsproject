#ifndef HGONLINEMAINWIDGET_H
#define HGONLINEMAINWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <QTabWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include "hgqlabel.h"
#include "hglogwidget.h"
#include "hgflowwidget.h"
#include "hgloginwidget.h"
#include "hgrunningwidget.h"
#include "hgwarningwidget.h"
#include "hgerrorcodewidget.h"
#include "hgsystemsetwidget.h"
#include "hgchannelsetwidget.h"
#include "hgtaskmanagewidget.h"
#include "hgmethodmanagewidget.h"
#include "hgdevicemanagewidget.h"
#include "hgreagentmanagewidget.h"
#include "hgmaterialmanagewidget.h"
#include "hganalysisrecordwidget.h"
#include "hgexceptionhandlewidget.h"
#include "hgdevicemaintenancewidget.h"

class HGOnlineMainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HGOnlineMainWidget(QWidget *parent = nullptr);
    virtual ~HGOnlineMainWidget();

protected:
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void slotRunningTaskSequence();
    void clickEscape();
    void fnSlotListenTimerOut();
    void slotDeviceLabelleftClicked();
    void slotMainPageLabelleftClicked();
    void slotDataLabelleftClicked();
    void slotMaintenanceLabelleftClicked();
    void slotSystemSetLabelleftClicked();
    void slotDeviceManageLabelleftClicked();
    void slotChannelSetLabelleftClicked();
    void slotReagentManageLabelleftClicked();
    void slotRunningLabelleftClicked();
    void slotTaskManageLabelleftClicked();
    void slotFlowLabelleftClicked();
    void slotMethodManageLabelleftClicked();
    void slotAnalysisRecordLabelleftClicked();
    void slotWarningLabelleftClicked();
    void slotLogLabelleftClicked();
    void slotDeviceMaintenanceLabelleftClicked();
    void slotExceptionHandleLabelleftClicked();
    void slotMaterialManageLabelleftClicked();
    void slotLoginOn(UserInfoS);
    void clickSound();
    void clickRelogin();
    void clickCurlogin();
    void clickDownloadDB();
    void slotUpdateSoundVolumn(int);
    void slotUpdateLanguage(std::string);
    void slotJumpToExceptionPage();

private:
    void fnResize();
    void fnInitPos();
    void fnCloseWindow();
    void fnInitSoftwarePage();

private:
    int m_width;
    int m_height;
    HGQLabel* m_quitLabel,*m_soundLabel,*m_reloginLabel,*m_curloginLabel;
    HGQLabel* m_downloadDBLabel;

    // bool m_valid;
    QTimer *m_listenTimer;
    QLabel *m_titleTimer, *m_statusLabel;
    QLabel *m_topLabel, *m_leftLabel;
    HGQLabel *m_deviceLabel, *m_mainPageLabel, *m_dataLabel, *m_maintenanceLabel;
    HGQLabel *m_systemSetLabel, *m_deviceManageLabel, *m_channelSetLabel, *m_reagentManageLabel;
    HGQLabel *m_runningLabel, *m_taskManageLabel, *m_flowLabel, *m_methodManageLabel;
    HGQLabel *m_analysisRecordLabel, *m_warningLabel, *m_logLabel;
    HGQLabel *m_deviceMaintenanceLabel, *m_materialManageLabel, *m_exceptionHandleLabel;

    HGDeviceManageWidget        *m_deviceManageW;
    HGSystemSetWidget           *m_systemSetW;
    HGChannelSetWidget          *m_channelSetW;
    HGReagentManageWidget       *m_reagentManageW;
    HGRunningWidget             *m_runningW;
    HGTaskManageWidget          *m_taskManageW;
    HGFlowWidget                *m_flowW;
    HGMethodManageWidget        *m_methodManageW;
    HGAnalysisRecordWidget      *m_analysisRecordW;
    HGErrorCodeWidget           *m_warningW;
    HGLogWidget                 *m_logW;
    HGDeviceMaintenanceWidget   *m_deviceMaintenanceW;
    HGMaterialManageWidget      *m_materialManageW;
    HGExceptionHandleWidget     *m_exceptionHandleW;

    HGLoginWidget               *m_loginW;
    UserInfoS                   m_userInfoS;
    int m_leftIndex;
    int m_topIndex;
    bool m_loginFlag;
    bool m_soundFlag;
    int m_loginAuthority;
    std::string m_language;
    bool m_runningTaskSequence;

};
#endif // HGONLINEMAINWIDGET_H
