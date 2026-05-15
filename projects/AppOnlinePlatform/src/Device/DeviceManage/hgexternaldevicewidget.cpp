#include "hgexternaldevicewidget.h"
#include "common.h"
#include <sstream>
#include "HGOnlinePlatformInterface.h"


HGExternalDeviceWidget::HGExternalDeviceWidget(std::string lang,std::string name,QWidget *parent) : QWidget(parent),
m_lang(lang)
{
    m_layout=new QGridLayout();
    this->setLayout(m_layout);
    m_name=name;
    m_nameLabel=new QLabel(QString::fromStdString(name));
    QFont font=m_nameLabel->font();
    font.setPointSize(18);
    m_nameLabel->setFont(font);
      
    m_baseInfoGroup=new QGroupBox("基本信息");
    m_advancedInfoGroup=new QGroupBox("高级管理");
    m_baseInfoLayout=new QGridLayout();
    m_advancedInfoLayout=new QGridLayout();
    m_baseInfoGroup->setLayout(m_baseInfoLayout);
    m_advancedInfoGroup->setLayout(m_advancedInfoLayout);
     
    m_deviceNameLabel=new QLabel("设备名称");
    m_deviceTypeLabel=new QLabel("设备型号");
    m_maxLabel=new QLabel("最大量程");
    m_accuracyLabel=new QLabel("称量精度");
    m_deviceInterfaceLabel=new QLabel("设备接口");
    m_selectComPortLabel=new QLabel("选择串口");
    m_selectComPortComboBox=new QComboBox();
    slotUpdateSerialPort();
    m_portLabel=new QLabel("波特率");
    m_dataLabel=new QLabel("数据位");
    m_adjustLabel=new QLabel("校验位");
    m_stopLabel=new QLabel("停止位");
    
    m_deviceNameEdit=new QLineEdit();
    m_deviceTypeEdit=new QLineEdit();
    m_maxEdit=new QLineEdit();
    m_accuracyEdit=new QLineEdit();
    m_deviceInterfaceComboBox=new QComboBox();
    m_deviceInterfaceComboBox->addItems({"串口"/*,"CAN","USB"*/});
    connect(m_deviceInterfaceComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(slotDeviceInterfaceChanged(int)));

    m_portEdit=new QComboBox();
    m_portEdit->addItems({"110","300","600","1200","2400","4800","9600","14400","19200","38400","56000","57600","115200","128000","230400","256000","460800","921600","1000000","2000000","Customize"});
    m_dataEdit=new QComboBox();
    m_dataEdit->addItems({"8","7","6","5"});
    m_adjustEdit=new QComboBox();
    m_adjustEdit->addItems({"none","odd","even","mark","space"});
    m_stopEdit=new QComboBox();
    m_stopEdit->addItems({"1","1.5","2"});
    
    m_textEdit=new QTextEdit();
    m_captureCodeLabel=new QLabel("采集命令：");
    m_captureCodeEdit=new QLineEdit();
    m_enterCheck=new QCheckBox("回车");
    m_returnCheck=new QCheckBox("换行");
    m_sendCodeBtn=new QPushButton("发送采集命令");

    m_okBtn=new QPushButton("确定");
    m_cancelBtn=new QPushButton("取消");
    m_setBtn=new QPushButton("打开串口");
    connect(m_okBtn,SIGNAL(clicked()),this,SLOT(slotOKClick()));
    connect(m_cancelBtn,SIGNAL(clicked()),this,SLOT(slotCancelClick()));
    connect(m_setBtn,SIGNAL(clicked()),this,SLOT(slotSetClick()));

    m_setDeviceParamLabel=new LabelWithImg(IMGRIGHT,12,getPath("/resources/V1/@1xze-add-o 1.png"),"自定义设备参数");

    m_baseInfoLayout->addWidget(m_deviceNameLabel,0,0);
    m_baseInfoLayout->addWidget(m_deviceNameEdit,0,1);
    m_baseInfoLayout->addWidget(m_deviceTypeLabel,1,0);
    m_baseInfoLayout->addWidget(m_deviceTypeEdit,1,1);
    m_baseInfoLayout->addWidget(m_setDeviceParamLabel,2,0);
    m_baseInfoLayout->addWidget(m_maxLabel,3,0);
    m_baseInfoLayout->addWidget(m_maxEdit,3,1);
    m_baseInfoLayout->addWidget(m_accuracyLabel,4,0);
    m_baseInfoLayout->addWidget(m_accuracyEdit,4,1);

    m_advancedInfoLayout->addWidget(m_deviceInterfaceLabel,0,0);
    m_advancedInfoLayout->addWidget(m_deviceInterfaceComboBox,0,1);
    m_advancedInfoLayout->addWidget(m_selectComPortLabel,m_advancedInfoLayout->rowCount(),0);
    m_advancedInfoLayout->addWidget(m_selectComPortComboBox,m_advancedInfoLayout->rowCount()-1,1,1,1);
    m_advancedInfoLayout->addWidget(m_portLabel,m_advancedInfoLayout->rowCount(),0,1,1);
    m_advancedInfoLayout->addWidget(m_portEdit,m_advancedInfoLayout->rowCount()-1,1,1,1);
    m_advancedInfoLayout->addWidget(m_dataLabel,m_advancedInfoLayout->rowCount(),0,1,1);
    m_advancedInfoLayout->addWidget(m_dataEdit,m_advancedInfoLayout->rowCount()-1,1,1,1);
    m_advancedInfoLayout->addWidget(m_adjustLabel,m_advancedInfoLayout->rowCount(),0,1,1);
    m_advancedInfoLayout->addWidget(m_adjustEdit,m_advancedInfoLayout->rowCount()-1,1,1,1);
    m_advancedInfoLayout->addWidget(m_stopLabel,m_advancedInfoLayout->rowCount(),0,1,1);
    m_advancedInfoLayout->addWidget(m_stopEdit,m_advancedInfoLayout->rowCount()-1,1,1,1);
    m_advancedInfoLayout->addWidget(m_captureCodeLabel,m_advancedInfoLayout->rowCount()-1,2,1,1);
    m_advancedInfoLayout->addWidget(m_captureCodeEdit,m_advancedInfoLayout->rowCount()-1,3,1,1);
    m_advancedInfoLayout->addWidget(m_enterCheck,m_advancedInfoLayout->rowCount()-1,4,1,1);
    m_advancedInfoLayout->addWidget(m_returnCheck,m_advancedInfoLayout->rowCount()-1,5,1,1);
    m_advancedInfoLayout->addWidget(m_setBtn,m_advancedInfoLayout->rowCount(),1);
    m_advancedInfoLayout->addWidget(m_sendCodeBtn,m_advancedInfoLayout->rowCount()-1,2);
    m_advancedInfoLayout->addWidget(m_textEdit,1,2,4,4);

    // m_advancedInfoLayout->addWidget(m_cancelBtn,m_advancedInfoLayout->rowCount(),2);
    // m_advancedInfoLayout->addWidget(m_okBtn,m_advancedInfoLayout->rowCount()-1,3);


    m_layout->addWidget(m_nameLabel,0,0);
    m_layout->addWidget(m_baseInfoGroup,1,0);
    m_layout->addWidget(m_advancedInfoGroup,2,0);

    m_timer=new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(slotUpdateSerialPort()));
    m_timer->start(10000);
    m_selectComPortComboBox->setCurrentIndex(-1);
}

bool HGExternalDeviceWidget::closeWindow(){
    return true;
}
void HGExternalDeviceWidget::slotUpdateSerialPort(){
    HGOnlineRWDB::writeSerialPortInfo(listAvailableSerialPorts());
    m_selectComPortComboBox->clear();
    std::vector<std::string> serialPorts=HGOnlineRWDB::readSerialPortInfo();
    for (auto port : serialPorts){
        QString qport=QString::fromStdString(port);
        m_selectComPortComboBox->addItem(qport);
    }
}
void HGExternalDeviceWidget::slotDeviceInterfaceChanged(int index){
    switch (index){
        case 0:
        m_portLabel->setVisible(true);
        m_portEdit->setVisible(true);
        m_dataLabel->setVisible(true);
        m_dataEdit->setVisible(true);
        m_adjustLabel->setVisible(true);
        m_adjustEdit->setVisible(true);
        m_stopLabel->setVisible(true);
        m_stopEdit->setVisible(true);
        m_selectComPortLabel->setVisible(true);
        m_selectComPortComboBox->setVisible(true);
        break;
        default:
        m_portLabel->setVisible(false);
        m_portEdit->setVisible(false);
        m_dataLabel->setVisible(false);
        m_dataEdit->setVisible(false);
        m_adjustLabel->setVisible(false);
        m_adjustEdit->setVisible(false);
        m_stopLabel->setVisible(false);
        m_stopEdit->setVisible(false);
        m_selectComPortLabel->setVisible(false);
        m_selectComPortComboBox->setVisible(false);
        break;
    }
}
std::string HGExternalDeviceWidget::getParamStr(){
    std::ostringstream ss;
    ss<<m_deviceNameLabel->text().toStdString()<<":" \
      <<m_deviceNameEdit->text().toStdString()<<"," \
      <<m_deviceTypeLabel->text().toStdString()<<":" \
      <<m_deviceTypeEdit->text().toStdString()<<"," \
      <<m_maxLabel->text().toStdString()<<":" \
      <<m_maxEdit->text().toStdString()<<"," \
      <<m_accuracyLabel->text().toStdString()<<":" \
      <<m_accuracyEdit->text().toStdString()<<"," \
      <<m_deviceInterfaceLabel->text().toStdString()<<":" \
      <<m_deviceInterfaceComboBox->currentText().toStdString()<<"," \
      <<m_portLabel->text().toStdString()<<":" \
      <<m_portEdit->currentText().toStdString()<<"," \
      <<m_dataLabel->text().toStdString()<<":" \
      <<m_dataEdit->currentText().toStdString()<<"," \
      <<m_adjustLabel->text().toStdString()<<":" \
      <<m_adjustEdit->currentText().toStdString()<<"," \
      <<m_stopLabel->text().toStdString()<<":" \
      <<m_stopEdit->currentText().toStdString();
    return ss.str();
}

void HGExternalDeviceWidget::setParamStr(std::string param){
    setControlText(m_baseInfoLayout,param);
    setControlText(m_advancedInfoLayout,param);
}
void HGExternalDeviceWidget::slotOKClick(){

}
void HGExternalDeviceWidget::slotSetClick(){
    
}
void HGExternalDeviceWidget::slotCancelClick(){
    
}