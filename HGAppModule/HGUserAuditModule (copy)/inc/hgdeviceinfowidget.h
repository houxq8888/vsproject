#ifndef HGDEVICEINFOWIDGET_H
#define HGDEVICEINFOWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include "mykeyboard.h"

class HGDeviceInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGDeviceInfoWidget(std::string,QWidget *parent = nullptr);
    ~HGDeviceInfoWidget();
    bool closeWindow();
    
signals:

private slots:
    void slotGetDeviceName(QString);
    void slotGetDeviceType(QString);
    void slotGetDeviceNumber(QString);
    void slotGetCurVersion(QString);
    void slotGetInitDate(QString);
    void slotMachineDate(QString);
    void slotDeviceNumber(QString);
    void slotFirmwareVersion(QString);
    void slotUpgradeFirmware();

private:
    void fnInit();
    void fnWriteDB();
    void fnReadDB();
    bool eventFilter(QObject* obj,QEvent* event);

private:
    QGridLayout *m_layout, *m_widgetLayout;
    QGroupBox* m_groupBox;
    QLabel* m_deviceNameQLabel;
    QLineEdit* m_deviceNameEdit;
    QLabel* m_deviceTypeQLabel;
    QLineEdit* m_deviceTypeEdit;
    QLabel* m_deviceNumberQLabel;
    QLineEdit* m_deviceNumberEdit;
    QLabel* m_curVersionQLabel;
    QLineEdit* m_curVersionEdit;
    QLabel* m_initDateQLabel;
    QLineEdit* m_initDateEdit;
    QLabel* m_firmwareLabel;
    QLineEdit* m_firmwareEdit;
    QPushButton* m_updateVersionBtn;

    KBTimeEdit* m_machineTimeCtrl;
    KBCharEdit* m_deviceNumberCtrl, *m_firmwareCtrl;
    std::string m_lang;
};

#endif // HGDEVICEINFOWIDGET_H
