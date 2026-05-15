#ifndef HGEXTERNALDEVICEWIDGET_H
#define HGEXTERNALDEVICEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <QTimer>
#include <QTextEdit>
#include "labelwithimg.h"
#include <QSlider>

class HGExternalDeviceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGExternalDeviceWidget(std::string,std::string,QWidget *parent = nullptr);
    bool closeWindow();
    std::string getName() {return m_name;}
    void setParamStr(std::string);
    std::string getParamStr();

signals:

private slots:
    void slotOKClick();
    void slotCancelClick();
    void slotSetClick();
    void slotDeviceInterfaceChanged(int);
    void slotUpdateSerialPort();
    
private:
    std::string m_name;
    QLabel* m_nameLabel;
    QGroupBox* m_baseInfoGroup, *m_advancedInfoGroup;
    QGridLayout* m_baseInfoLayout, *m_advancedInfoLayout, *m_layout;
    QLabel* m_deviceNameLabel,*m_deviceTypeLabel;
    QLabel* m_maxLabel,*m_accuracyLabel;
    QLabel* m_deviceInterfaceLabel,*m_portLabel,*m_dataLabel,*m_adjustLabel,*m_stopLabel;

    QLabel* m_selectComPortLabel;
    QComboBox* m_selectComPortComboBox;
    QLineEdit*m_deviceNameEdit,*m_deviceTypeEdit,*m_maxEdit,*m_accuracyEdit;
    QComboBox*m_portEdit,*m_dataEdit,*m_adjustEdit,*m_stopEdit;
    QComboBox *m_deviceInterfaceComboBox;
    QPushButton *m_okBtn,*m_cancelBtn,*m_setBtn;

    QTextEdit* m_textEdit;
    QLabel* m_captureCodeLabel;
    QLineEdit* m_captureCodeEdit;
    QCheckBox* m_enterCheck, *m_returnCheck;
    QPushButton* m_sendCodeBtn;

    QTimer* m_timer;
    LabelWithImg* m_setDeviceParamLabel;
    std::string m_lang;
};

#endif // HGEXTERNALDEVICEWIDGET_H
