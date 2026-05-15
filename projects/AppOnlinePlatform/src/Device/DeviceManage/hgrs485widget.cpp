#include "hgrs485widget.h"
#include "common.h"
#include <sstream>
#include "HGOnlinePlatformInterface.h"


HGRs485Widget::HGRs485Widget(std::string lang,std::string name,QWidget *parent) : QWidget(parent),
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
   
    m_typeLabel=new QLabel("类型");
    m_deviceComLabel=new QLabel("串口号");
    m_typeCombo=new QComboBox();
    m_typeCombo->addItems({"RS 232","RS 485"});
    m_typeCombo->setCurrentIndex(1);
    connect(m_typeCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(slotTypeIndexChanged(int)));

    m_deviceComCombo=new QComboBox();
    slotUpdateSerialPort();

    m_maxSpeedLabel=new QLabel("最高传输速率");
    m_distanceLabel=new QLabel("传输距离");
    m_advanceNameLabel=new QLabel("通讯配置");
    m_portLabel=new QLabel("波特率");
    m_dataLabel=new QLabel("数据位");
    m_adjustLabel=new QLabel("校验位");
    
    m_maxSpeedEdit=new QLineEdit();
    m_distanceEdit=new QLineEdit();
    m_maxSpeedEdit->setReadOnly(true);
    m_distanceEdit->setReadOnly(true);
    m_portEdit=new QLineEdit();
    m_dataEdit=new QLineEdit();
    m_adjustEdit=new QLineEdit();
       
    m_okBtn=new QPushButton("确定");
    connect(m_okBtn,SIGNAL(clicked()),this,SLOT(slotOKClick()));

    m_baseInfoLayout->addWidget(m_typeLabel,0,0);
    m_baseInfoLayout->addWidget(m_typeCombo,0,1);
    m_baseInfoLayout->addWidget(m_deviceComLabel,1,0);
    m_baseInfoLayout->addWidget(m_deviceComCombo,1,1);
    m_baseInfoLayout->addWidget(m_maxSpeedLabel,2,0);
    m_baseInfoLayout->addWidget(m_maxSpeedEdit,2,1);
    m_baseInfoLayout->addWidget(m_distanceLabel,3,0);
    m_baseInfoLayout->addWidget(m_distanceEdit,3,1);

    m_advancedInfoLayout->addWidget(m_advanceNameLabel,0,0);
    m_advancedInfoLayout->addWidget(m_portLabel,1,0);
    m_advancedInfoLayout->addWidget(m_portEdit,1,1);
    m_advancedInfoLayout->addWidget(m_dataLabel,2,0);
    m_advancedInfoLayout->addWidget(m_dataEdit,2,1);
    m_advancedInfoLayout->addWidget(m_adjustLabel,3,0);
    m_advancedInfoLayout->addWidget(m_adjustEdit,3,1);
    m_advancedInfoLayout->addWidget(m_okBtn,3,2);

    m_layout->addWidget(m_nameLabel,0,0);
    m_layout->addWidget(m_baseInfoGroup,1,0);
    m_layout->addWidget(m_advancedInfoGroup,2,0);

    fnDisplayBaseInfo();
    m_timer=new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(slotUpdateSerialPort()));
    m_timer->start(10000);

    m_deviceComCombo->setCurrentIndex(-1);
}
bool HGRs485Widget::closeWindow(){
    return true;
}
void HGRs485Widget::slotTypeIndexChanged(int index){
    fnDisplayBaseInfo();
}
void HGRs485Widget::fnDisplayBaseInfo(){
    switch (m_typeCombo->currentIndex()){
        case 0:
        m_maxSpeedEdit->setText("1Mbs");
        m_distanceEdit->setText("15 m");
        break;
        case 1:
        m_maxSpeedEdit->setText("10Mbs");
        m_distanceEdit->setText("3000 m");
        break;
    }
}
std::string HGRs485Widget::getParamStr(){
    std::ostringstream ss;
    ss<<m_maxSpeedLabel->text().toStdString()<<":" \
      <<m_maxSpeedEdit->text().toStdString()<<"," \
      <<m_distanceLabel->text().toStdString()<<":" \
      <<m_distanceEdit->text().toStdString()<<"," \
      <<m_portLabel->text().toStdString()<<":" \
      <<m_portEdit->text().toStdString()<<"," \
      <<m_dataLabel->text().toStdString()<<":" \
      <<m_dataEdit->text().toStdString()<<"," \
      <<m_adjustLabel->text().toStdString()<<":" \
      <<m_adjustEdit->text().toStdString();
    return ss.str();
}

void HGRs485Widget::setParamStr(std::string param){
    setControlText(m_baseInfoLayout,param);
    setControlText(m_advancedInfoLayout,param);
}
void HGRs485Widget::slotOKClick(){

}
void HGRs485Widget::slotUpdateSerialPort(){
    HGOnlineRWDB::writeSerialPortInfo(listAvailableSerialPorts());
    m_deviceComCombo->clear();
    std::vector<std::string> serialPorts=HGOnlineRWDB::readSerialPortInfo();
    for (auto port : serialPorts){
        QString qport=QString::fromStdString(port);
        m_deviceComCombo->addItem(qport);
    }
}