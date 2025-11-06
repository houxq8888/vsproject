#include "hgcolorsensor.h"
#include <QHeaderView>
#include "common.h"
#include <sstream>
HGColorSensor::HGColorSensor(std::string lang,std::string name,QWidget *parent) : QWidget(parent),
m_lang(lang)
{
    m_isEnableAutoGain=true;
    m_layout=new QGridLayout();
    this->setLayout(m_layout);
       
    m_name=name;
    m_nameLabel=new QLabel("颜色传感器");
    QFont font=m_nameLabel->font();
    font.setPointSize(18);
    m_nameLabel->setFont(font);

    m_baseInfoGroup=new QGroupBox("基本信息");
    m_advancedInfoGroup=new QGroupBox("高级管理");
    m_baseInfoLayout=new QGridLayout();
    m_advancedInfoLayout=new QGridLayout();
    m_baseInfoGroup->setLayout(m_baseInfoLayout);
    m_advancedInfoGroup->setLayout(m_advancedInfoLayout);

    m_serialNumberLabel=new QLabel("序列号");
    m_resolutionLabel=new QLabel("分辨率");
    m_frameRateLabel=new QLabel("帧率");
    m_sensorLabel=new QLabel("感光部件");
    
    m_serialNumberEdit=new QLineEdit();
    m_resolutionEdit=new QLineEdit();
    m_frameRateEdit=new QLineEdit();
    m_sensorEdit=new QLineEdit();
    
    m_autoGainLabel=new LabelWithImg(IMGRIGHT,12,getPath("/resources/V1/@1xIOS开关_enable.png"),"自动增益");
    
    m_wbLabel=new QLabel("白平衡校正");
    m_adjustBtn=new QPushButton("白平衡校正");
    m_tableW=new QTableWidget(3,2);
    m_tableW->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableW->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableW->verticalHeader()->setVisible(false);
    m_tableW->horizontalHeader()->setVisible(false);

    QString data[3][2]={
        {"Red Gain","1"},
        {"Blue Gain","1"},
        {"Green Gain","1"},
    };
    for (int row=0;row<3;++row){
        for (int col=0;col<2;++col){
            QTableWidgetItem* item=new QTableWidgetItem(data[row][col]);
            m_tableW->setItem(row,col,item);
            if (col==0) item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        }
    }

    connect(m_autoGainLabel,SIGNAL(clickImgLabel()),this,SLOT(clickEnableAutoGain()));

    m_baseInfoLayout->addWidget(m_serialNumberLabel,0,0);
    m_baseInfoLayout->addWidget(m_serialNumberEdit,0,1);
    m_baseInfoLayout->addWidget(m_resolutionLabel,1,0);
    m_baseInfoLayout->addWidget(m_resolutionEdit,1,1);
    m_baseInfoLayout->addWidget(m_frameRateLabel,2,0);
    m_baseInfoLayout->addWidget(m_frameRateEdit,2,1);
    m_baseInfoLayout->addWidget(m_sensorLabel,3,0);
    m_baseInfoLayout->addWidget(m_sensorEdit,3,1);

    m_advancedInfoLayout->addWidget(m_autoGainLabel,0,0,1,3);
    m_advancedInfoLayout->addWidget(m_wbLabel,1,0);
    m_advancedInfoLayout->addWidget(m_tableW,1,1);
    m_advancedInfoLayout->addWidget(m_adjustBtn,1,2);

    m_layout->addWidget(m_nameLabel,0,0);
    m_layout->addWidget(m_baseInfoGroup,1,0);
    m_layout->addWidget(m_advancedInfoGroup,2,0);
}
bool HGColorSensor::closeWindow(){
    return true;
}
void HGColorSensor::clickEnableAutoGain()
{
    m_isEnableAutoGain=!m_isEnableAutoGain;
    if (!m_isEnableAutoGain){
        m_autoGainLabel->setImg(getPath("/resources/V1/@1xIOS开关.png"));
    } else {
        m_autoGainLabel->setImg(getPath("/resources/V1/@1xIOS开关_enable.png"));

    }
}
std::string HGColorSensor::getParamStr(){
    std::ostringstream ss;
    ss<<m_serialNumberLabel->text().toStdString()<<":" \
      <<m_serialNumberEdit->text().toStdString()<<"," \
      <<m_resolutionLabel->text().toStdString()<<":" \
      <<m_resolutionEdit->text().toStdString()<<"," \
      <<m_frameRateLabel->text().toStdString()<<":" \
      <<m_frameRateEdit->text().toStdString()<<"," \
      <<m_sensorLabel->text().toStdString()<<":" \
      <<m_sensorEdit->text().toStdString();
    return ss.str();
}

void HGColorSensor::setParamStr(std::string param){
    setControlText(m_baseInfoLayout,param);
    setControlText(m_advancedInfoLayout,param);
}