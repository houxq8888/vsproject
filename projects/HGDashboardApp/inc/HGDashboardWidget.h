#ifndef HGDASHBOARDWIDGET_H
#define HGDASHBOARDWIDGET_H


#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QMouseEvent>
#include <QGridLayout>
#include <QGroupBox>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QWidget>
#include <QFrame>
#include <QTableWidget>
#include "hgqlabel.h"
#include "QRoundProgressBar.h"
#include "HGCurDataWidget.h"
#include "hgcomwithmqtt.h"
#include "hgcomwithsocket.h"
#include "tcpvideoreceiver.h"

using namespace HGMACHINE;

typedef struct tagDataInfo{
    std::string value;
    std::string status;
    std::string ts;
    std::string iconpath;
    QColor color;
} DashboardDataInfo;

typedef struct tagRange{
    float min;
    float max;
} Range;

enum {
    TUZHUANG=0,
    DIDING=1,
    SHUIFEN=2
};
enum {
    REAL_DATA=0,
    USER_DATA=1,
    MAINTENANCE=2,
    MONITOR=3
};

#define HGDASHBOARDNAME "[HGDashboard]"

#define SAVE_IMG_PATH(base) base+"/img"

class HGDashboardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HGDashboardWidget(QWidget *parent = nullptr);
    virtual ~HGDashboardWidget();

protected:
    void closeEvent(QCloseEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

signals:
    void signalNewFrame(uint8_t* data,int width,int height);
    void signalNewFrame(const QImage& img,int);
private slots:
    void fnSlotListenTimerOut();
    void fnSlotListenWarnTimerOut();
    void fnSlotListenSystemUsageTimerOut();
public slots:
    void onNewFrame(const QImage &img,int);

private:
    bool fnInitCurlWeather();
    void setTableData(const int& type,const int &row, const DashboardDataInfo &dataInfo);
    void fillTableData(int type,const std::vector<std::string> &firstColumnName,
        const std::vector<std::string> &secondColumnNames);
    void displayRealData();
    void deThread();

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    int m_width;
    int m_height;
    int m_dataType;
    int m_funcType, m_lastFuncType;

    QLabel* m_bkLabel;
    QGridLayout *m_gridLayout;
    QRoundProgressBar *m_ElectricalConductivity1Bar, *m_ElectricalConductivity2Bar;

    HGCurDataWidget *m_Alkalinity1Curve, *m_Alkalinity2Curve, *m_FreeAcidityCurve, *m_TotalAcidityCurve;
    HGCurDataWidget *m_didingChannel1Curve, *m_didingChannel2Curve,*m_didingChannel3Curve, *m_didingChannel4Curve;
    HGCurDataWidget *m_shuifenChannel1Curve, *m_shuifenChannel2Curve;

    QWidget* m_mainContent;
    QLabel* m_titleLabel,*m_timeLabel, *m_rightTimeLabel;
    QLabel* m_deviceStatusLabel[3], *m_errStatusLabel[3];
    QTimer *m_timer, *m_warnTimer;
    QTimer* m_systemusageTimer;
    HgComWithLibMQTT* m_mqttclient;
    HgComWithSocket* m_socketServer;
    HgComWithSocket* m_socketServerDiding;
    HgComWithSocket* m_socketServerShuifen;

    QFrame* m_hor1,*m_hor2;
    QTableWidget* m_tableW[3];
    QLabel* m_maintenanceLabel, *m_mapLabel, *m_monitorLabel, *m_realDataLabel;
    QLabel* m_tuzhuangLabel, *m_didingLabel, *m_shuifenLabel;

    QFont font;
    Range m_Alkalinity1Range, m_Alkalinity2Range, m_FreeAcidityRange, m_TotalAcidityRange, m_ElectricalConductivity1Range, m_ElectricalConductivity2Range;
    Range m_didingChannel1Range, m_didingChannel2Range,m_didingChannel3Range,m_didingChannel4Range;
    Range m_shuifenChannel1Range, m_shuifenChannel2Range;
    Range m_vlRange;
    std::string m_basePath;
    std::string m_temperature, m_weatherStatus;

    bool m_playVideoFlag;

#define VIDEO_THREAD_NUM 1
    std::atomic<bool> m_videoRunning[VIDEO_THREAD_NUM]; // 控制循环
    TCPVideoReceiver* m_receiver[VIDEO_THREAD_NUM];
    std::thread m_videoThread[VIDEO_THREAD_NUM];
    QLabel* m_watchLabel[VIDEO_THREAD_NUM];
    std::atomic<int> frameCounter{0};
};

#endif // HGDASHBOARDWIDGET_H
