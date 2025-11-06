#include "hgdeviceinfowidget.h"
#include "common.h"


HGDeviceInfoWidget::HGDeviceInfoWidget(std::string lang,QWidget *parent) : QWidget(parent),
    m_deviceNumberCtrl(nullptr),
    m_lang(lang)
{
    m_machineTimeCtrl=NULL;
    fnInit();
    fnReadDB();
}
//
void HGDeviceInfoWidget::fnInit()
{
    m_layout=new QGridLayout();
    m_widgetLayout=new QGridLayout();
    this->setLayout(m_widgetLayout);
    /*"设备信息"*/
    m_groupBox=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"DeviceInfo")),this);
    m_groupBox->setStyleSheet("QGroupBox { font-size: 12pt; font-weight:bold;}");
    m_groupBox->setLayout(m_layout);
    m_deviceNameQLabel=new QLabel();
    m_deviceNameQLabel->setText(QString::fromStdString(loadTranslation(m_lang,"DeviceName"))/*"设备名称"*/);
    m_deviceNameEdit=new QLineEdit();
    // m_deviceNameEdit->setPlaceholderText("请输入");
    m_deviceNameEdit->setReadOnly(true);
    connect(m_deviceNameEdit,SIGNAL(textChanged(QString)),this,SLOT(slotGetDeviceName(QString)));

    m_deviceTypeQLabel=new QLabel();
    m_deviceTypeQLabel->setText(QString::fromStdString(loadTranslation(m_lang,"DeviceType")));//"设备型号");
    m_deviceTypeEdit=new QLineEdit();
    // m_deviceTypeEdit->setPlaceholderText("请输入");
    m_deviceTypeEdit->setReadOnly(true);
    connect(m_deviceTypeEdit,SIGNAL(textChanged(QString)),this,SLOT(slotGetDeviceType(QString)));


    m_deviceNumberQLabel=new QLabel();
    m_deviceNumberQLabel->setText(QString::fromStdString(loadTranslation(m_lang,"DeviceNumber")));//"设备编号");
    m_deviceNumberEdit=new QLineEdit();
    m_deviceNumberEdit->setPlaceholderText(QString::fromStdString(loadTranslation(m_lang,"Input")));//"请输入");
    m_deviceNumberEdit->installEventFilter(this);
    connect(m_deviceNumberEdit,SIGNAL(textChanged(QString)),this,SLOT(slotGetDeviceNumber(QString)));


    m_curVersionQLabel=new QLabel();
    m_curVersionQLabel->setText(QString::fromStdString(loadTranslation(m_lang,"CurVersion")));//"当前版本");
    m_curVersionEdit=new QLineEdit();
    // m_curVersionEdit->setPlaceholderText("请输入");
    m_curVersionEdit->setReadOnly(true);
    connect(m_curVersionEdit,SIGNAL(textChanged(QString)),this,SLOT(slotGetCurVersion(QString)));


    m_initDateQLabel=new QLabel();
    m_initDateQLabel->setText(QString::fromStdString(loadTranslation(m_lang,"FactoryDate")));//"出厂日期");
    m_initDateEdit=new QLineEdit();
    m_initDateEdit->setPlaceholderText(QString::fromStdString(loadTranslation(m_lang,"Input")));//"请输入");
    m_initDateEdit->installEventFilter(this);
    connect(m_initDateEdit,SIGNAL(textChanged(QString)),this,SLOT(slotGetInitDate(QString)));

    m_firmwareLabel=new QLabel();
    m_firmwareLabel->setText(QString::fromStdString(loadTranslation(m_lang,"FirmwareVersion")));
    m_firmwareEdit=new QLineEdit();
    m_firmwareEdit->setPlaceholderText(QString::fromStdString(loadTranslation(m_lang,"Input")));//"请输入");
    m_firmwareEdit->installEventFilter(this);
    m_updateVersionBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"UpdateFirmware")));
    connect(m_updateVersionBtn,SIGNAL(clicked()),this,SLOT(slotUpgradeFirmware()));

    m_layout->addWidget(m_deviceNameQLabel,0,0);
    m_layout->addWidget(m_deviceNameEdit,0,1,1,3);
    m_layout->addWidget(m_deviceTypeQLabel,1,0);
    m_layout->addWidget(m_deviceTypeEdit,1,1);
    m_layout->addWidget(m_deviceNumberQLabel,1,2);
    m_layout->addWidget(m_deviceNumberEdit,1,3);
    m_layout->addWidget(m_curVersionQLabel,2,0);
    m_layout->addWidget(m_curVersionEdit,2,1);
    m_layout->addWidget(m_initDateQLabel,2,2);
    m_layout->addWidget(m_initDateEdit,2,3);
    m_layout->addWidget(m_firmwareLabel,3,0);
    m_layout->addWidget(m_firmwareEdit,3,1);
    m_layout->addWidget(m_updateVersionBtn,3,2);

    m_widgetLayout->addWidget(m_groupBox);

}
bool HGDeviceInfoWidget::closeWindow()
{
    fnWriteDB();
    return true;
}
bool HGDeviceInfoWidget::eventFilter(QObject* obj,QEvent* event)
{
    if (event->type()==QEvent::MouseButtonPress){
        QMouseEvent* mouseEvent=static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton){
            if (obj==m_initDateEdit){
                m_machineTimeCtrl=new KBTimeEdit(false);
                int passPosX = m_initDateEdit->mapToGlobal(QPoint(0,0)).x();
                int passPosY = m_initDateEdit->mapToGlobal(QPoint(0,0)).y() + m_initDateEdit->height();
                // if (passPosX+m_authorityDeadLineCtrl->width()>m_width) passPosX=m_width-m_authorityDeadLineCtrl->width();
                // if (passPosY+m_authorityDeadLineCtrl->height()>m_height) passPosY=m_height-m_authorityDeadLineCtrl->height();
                m_machineTimeCtrl->move(passPosX,passPosY);
                m_machineTimeCtrl->show();
                connect(m_machineTimeCtrl,SIGNAL(MykeyboardSignal(QString)),
                        this,SLOT(slotMachineDate(QString)));
            } else if (obj==m_deviceNumberEdit){
                m_deviceNumberCtrl=new KBCharEdit();
                int passPosX = m_deviceNumberEdit->mapToGlobal(QPoint(0,0)).x();
                int passPosY = m_deviceNumberEdit->mapToGlobal(QPoint(0,0)).y() + m_deviceNumberEdit->height();
                // if (passPosX+m_authorityDeadLineCtrl->width()>m_width) passPosX=m_width-m_authorityDeadLineCtrl->width();
                // if (passPosY+m_authorityDeadLineCtrl->height()>m_height) passPosY=m_height-m_authorityDeadLineCtrl->height();
                m_deviceNumberCtrl->move(passPosX,passPosY);
                m_deviceNumberCtrl->show();
                connect(m_deviceNumberCtrl,SIGNAL(MykeyboardSignal(QString)),
                        this,SLOT(slotDeviceNumber(QString)));
            } else if (obj==m_firmwareEdit){
                m_firmwareCtrl=new KBCharEdit();
                int passPosX = m_firmwareEdit->mapToGlobal(QPoint(0,0)).x();
                int passPosY = m_firmwareEdit->mapToGlobal(QPoint(0,0)).y() + m_firmwareEdit->height();
                // if (passPosX+m_authorityDeadLineCtrl->width()>m_width) passPosX=m_width-m_authorityDeadLineCtrl->width();
                // if (passPosY+m_authorityDeadLineCtrl->height()>m_height) passPosY=m_height-m_authorityDeadLineCtrl->height();
                m_firmwareCtrl->move(passPosX,passPosY);
                m_firmwareCtrl->show();
                connect(m_firmwareCtrl,SIGNAL(MykeyboardSignal(QString)),
                        this,SLOT(slotFirmwareVersion(QString)));
            } 
        }
    }
    return QWidget::eventFilter(obj,event);
}
void HGDeviceInfoWidget::slotMachineDate(QString text){
    std::ostringstream ss;
    ss<<m_initDateQLabel->text().toStdString()<<":["<<m_initDateEdit->text().toStdString()<<"]->"<<text.toStdString();
    RWDb::writeAuditTrailLog(ss.str());
    m_initDateEdit->setText(text);
}
void HGDeviceInfoWidget::slotDeviceNumber(QString text){
    std::ostringstream ss;
    ss<<m_deviceNumberQLabel->text().toStdString()<<":["<<m_deviceNumberEdit->text().toStdString()<<"]->"<<text.toStdString();
    RWDb::writeAuditTrailLog(ss.str());
    m_deviceNumberEdit->setText(text);
}
void HGDeviceInfoWidget::slotFirmwareVersion(QString text){
    m_firmwareEdit->setText(text);
}
void HGDeviceInfoWidget::slotUpgradeFirmware(){
    std::ostringstream ss;
    ss<<m_firmwareLabel->text().toStdString()<<":["<<m_firmwareEdit->text().toStdString()<<"] upgrade";
    RWDb::writeAuditTrailLog(ss.str());
}
void HGDeviceInfoWidget::slotGetDeviceName(QString text){
    // fnWriteDB();
}

void HGDeviceInfoWidget::slotGetDeviceType(QString text){
    // fnWriteDB();
}
void HGDeviceInfoWidget::slotGetDeviceNumber(QString text){
    // fnWriteDB();
}
void HGDeviceInfoWidget::slotGetCurVersion(QString text){
    // fnWriteDB();
}
void HGDeviceInfoWidget::slotGetInitDate(QString text){
    // fnWriteDB();
}

void HGDeviceInfoWidget::fnWriteDB(){
    GlobalSingleton::instance().setSystemInfo("设备名称", m_deviceNameEdit->text().toStdString());
    GlobalSingleton::instance().setSystemInfo("设备型号", m_deviceTypeEdit->text().toStdString());
    GlobalSingleton::instance().setSystemInfo("设备编号", m_deviceNumberEdit->text().toStdString());
    GlobalSingleton::instance().setSystemInfo("当前版本", m_curVersionEdit->text().toStdString());
    GlobalSingleton::instance().setSystemInfo("出厂日期", m_initDateEdit->text().toStdString());

    GlobalSingleton::instance().saveSystemInfo();
}
void HGDeviceInfoWidget::fnReadDB(){
    m_deviceTypeEdit->setText(HG_DEVICE_MODEL);
    m_deviceNumberEdit->setText(QString::fromStdString(GlobalSingleton::instance().getSystemInfo("设备编号")));
    // m_curVersionEdit->setText(HG_SOFTWARE_VERSION);
    // m_deviceNameEdit->setText(HG_DEVICE_NAME);
    m_initDateEdit->setText(QString::fromStdString(GlobalSingleton::instance().getSystemInfo("出厂日期")));
}