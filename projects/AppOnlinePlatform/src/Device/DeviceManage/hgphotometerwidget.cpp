#include "hgphotometerwidget.h"
#include "common.h"
#include <sstream>

HGPhotometerWidget::HGPhotometerWidget(std::string lang,std::string name,QWidget *parent) : QWidget(parent),
m_lang(lang)
{     
    m_layout=new QGridLayout();
    this->setLayout(m_layout);
       
    m_name=name;
    m_nameLabel=new QLabel("光度计");
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
    m_waveLengthLabel=new QLabel("波长范围");
    m_tempRangeLabel=new QLabel("温度范围");
    
    m_serialNumberEdit=new QLineEdit();
    m_waveLengthEdit=new QLineEdit();
    m_tempRangeEdit=new QLineEdit();
    
    m_wlAdjustLabel=new QLabel("波长校正");
    font=m_wlAdjustLabel->font();
    font.setPointSize(18);
    m_wlAdjustLabel->setFont(font);
    m_wlAdjustAttentionLabel=new QLabel("将校正光源放置在正确位置");

    m_adjustBtn=new QPushButton("自动波长校正");

    m_baseInfoLayout->addWidget(m_serialNumberLabel,0,0);
    m_baseInfoLayout->addWidget(m_serialNumberEdit,0,1);
    m_baseInfoLayout->addWidget(m_waveLengthLabel,1,0);
    m_baseInfoLayout->addWidget(m_waveLengthEdit,1,1);
    m_baseInfoLayout->addWidget(m_tempRangeLabel,2,0);
    m_baseInfoLayout->addWidget(m_tempRangeEdit,2,1);

    m_advancedInfoLayout->addWidget(m_wlAdjustLabel,0,0);
    m_advancedInfoLayout->addWidget(m_wlAdjustAttentionLabel,1,0);
    m_advancedInfoLayout->addWidget(m_adjustBtn,1,1);

    m_layout->addWidget(m_nameLabel,0,0);
    m_layout->addWidget(m_baseInfoGroup,1,0);
    m_layout->addWidget(m_advancedInfoGroup,2,0);
}
bool HGPhotometerWidget::closeWindow(){
    return true;
}
std::string HGPhotometerWidget::getParamStr(){
    std::ostringstream ss;
    ss<<m_serialNumberLabel->text().toStdString()<<":" \
      <<m_serialNumberEdit->text().toStdString()<<"," \
      <<m_waveLengthLabel->text().toStdString()<<":" \
      <<m_waveLengthEdit->text().toStdString()<<"," \
      <<m_tempRangeLabel->text().toStdString()<<":" \
      <<m_tempRangeEdit->text().toStdString();
    return ss.str();
}

void HGPhotometerWidget::setParamStr(std::string param){
    setControlText(m_baseInfoLayout,param);
    setControlText(m_advancedInfoLayout,param);
}