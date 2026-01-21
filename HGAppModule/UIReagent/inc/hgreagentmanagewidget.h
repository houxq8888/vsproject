#ifndef HGREAGENTMANAGEWIDGET_H
#define HGREAGENTMANAGEWIDGET_H

#include <QWidget>
#include "hgqlabel.h"
#include <QLabel>
#include <QPushButton>
#include <QTabWidget>
#include <QGridLayout>
#include <QTableWidget>
#include <QSplitter>
#include "hgddjicalibrationwidget.h"
#include "common.h"


enum{
    REAGENT_MANIPULATE,
    DEVICE_MANIPULATE
};

class HGReagentManageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGReagentManageWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();
    ~HGReagentManageWidget();

signals:

private slots:
    void slotBiaodingClicked();
    void slotOpenReagent();
    void slotSaveReagent();
    void slotAddReagent();
    void slotCloseReagent();
    void slotEditReagent();
    void slotOkReagent();
    void onCellClicked(int,int);
    void onCellChanged(int,int);
    void slotClickAdd();
    void slotClickLinkDevice();
    void onDeviceCellChanged(int,int);
    void onDeviceCellClicked(int,int);

private:
    void fnDisplayReagentInfo(int count,std::map<std::string,std::string> fillContent);
    void fnDisplayReagentDeviceInfo(int count,std::map<std::string,std::string> fillContent);
    void fnDeleteReagent();
    void fnDeleteDevice();
    void fnUpdateContent();
    void loadReagent();

private:
    HGQLabel *m_openHGLabel,*m_saveHGLabel,*m_addHGLabel,*m_closeHGLabel,*m_editHGLabel,*m_okHGLabel;
    QLabel* m_didingjiLabel;
    QPushButton* m_biaodingBtn;
    QPushButton* m_addBtn,*m_linkDeviceBtn;
    QGridLayout* m_layout;

    QSplitter* m_horsplitter;
    int m_curReagentRow;
    int m_whichW;
    QTableWidget* m_tableWidget,*m_deviceTableW;

    std::vector<REAGENT> m_reagentS;

    HGDDjiCalibrationWidget* m_ddjiCalibrateW;
    std::string m_lang;
};

#endif // HGREAGENTMANAGEWIDGET_H
