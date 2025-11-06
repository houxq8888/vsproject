#ifndef HGDEVICEMANAGEWIDGET_H
#define HGDEVICEMANAGEWIDGET_H

#include <QWidget>
#include "hgqlabel.h"
#include <QFrame>
#include <QGridLayout>
#include <QLineEdit>
#include "labelwithimg.h"
#include "flowtreewidget.h"
#include "hgphdjwidget.h"
#include "hgcolorsensor.h"
#include "hgphotometerwidget.h"
#include "hgsignaldjwidget.h"
#include "hgbackunitwidget.h"
#include "hgauxiliarypumpwidget.h"
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include "hgrs485widget.h"
#include "hgexternaldevicewidget.h"
#include "hgjiarebanwidget.h"
#include "hgshuifenyiwidget.h"
#include "hgzidongjinyangqiwidget.h"
#include "common.h"
#include "HGOnlinePlatformInterface.h"



class HGDeviceManageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGDeviceManageWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();
    ~HGDeviceManageWidget();

signals:

private slots:
    void slotGetDeviceName(QTreeWidgetItem *item,int column);
    void slotCheckItemStatus(QTreeWidgetItem*,int);
    void slotLeftClickSearchLabel();
    void showContextMenu(const QPoint &pos);

private:
    void fnInitDeviceStatus();
    void fnInitTreeWidget(const std::vector<std::string> &existDevices);
    QList<QTreeWidgetItem*> findItemsInAllColumns(QTreeWidget* treeWidget, const QString& text, Qt::CaseSensitivity flags);
    void findItemsInAllColumnsRecursively(QTreeWidgetItem* parentItem, const QString& text, Qt::CaseSensitivity flags, QList<QTreeWidgetItem*>& foundItems);
    int fnIsInDevices(std::string name);
    void setCheckStateRecursive(QTreeWidgetItem* item,Qt::CheckState state);
    void updateSingleFirmware(QTreeWidgetItem *item);

private:
    QFrame* m_vline;
    QLineEdit* m_inputEdit;
    HGQLabel* m_searchLabel;
    QGridLayout* m_layout;

    QTreeWidget *m_treeWidget;

    HGPHdjWidget *m_hgphDJW;
    HGColorSensor *m_colorsensorW;
    HGPhotometerWidget *m_photometerW;
    HGSignalDJWidget* m_signalDJW;
    HGBackUnitWidget* m_backUnitW;
    HGAuxiliaryPumpWidget* m_auxiliaryPumpW;
    HGRs485Widget* m_rs485W;
    HGExternalDeviceWidget* m_externalDeviceW;
    HGJiarebanWidget* m_jiarebanW;
    HGShuifenyiWidget* m_shuifenyiW;
    HGZidongjinyangqiWidget* m_zidongjinyangqiW;

    std::vector<DeviceInterface> m_deviceInterfaces;
    int m_linkStatus;
    std::string m_lang;
};
#endif // HGDEVICEMANAGEWIDGET_H
