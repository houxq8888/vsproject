#include "hgsignaldjwidget.h"
#include "common.h"
#include <sstream>

HGSignalDJWidget::HGSignalDJWidget(std::string lang,std::string name,QWidget *parent) : QWidget(parent),
m_lang(lang)
{
    m_layout=new QGridLayout();
    this->setLayout(m_layout);
       
    m_name=name;
    // m_nameLabel=new QLabel("颜色传感器");
    // QFont font=m_nameLabel->font();
    // font.setPointSize(18);
    // m_nameLabel->setFont(font);
  
    m_baseInfoGroup=new QGroupBox("基本信息");
    // m_advancedInfoGroup=new QGroupBox("高级管理");
    m_baseInfoLayout=new QGridLayout();
    // m_advancedInfoLayout=new QGridLayout();
    m_baseInfoGroup->setLayout(m_baseInfoLayout);
    // m_advancedInfoGroup->setLayout(m_advancedInfoLayout);

    m_serialLabel=new QLabel("电极序列号");
    m_typeLabel=new QLabel("电极类型");
    m_workCurrentLabel=new QLabel("工作电流");
    
    m_typeEdit=new QLineEdit();
    m_serialEdit=new QLineEdit();
    m_workCurrentEdit=new QLineEdit();

    m_baseInfoLayout->addWidget(m_typeLabel,0,0);
    m_baseInfoLayout->addWidget(m_typeEdit,0,1);
    m_baseInfoLayout->addWidget(m_serialLabel,1,0);
    m_baseInfoLayout->addWidget(m_serialEdit,1,1);
    m_baseInfoLayout->addWidget(m_workCurrentLabel,2,0);
    m_baseInfoLayout->addWidget(m_workCurrentEdit,2,1);

    // m_advancedInfoLayout->addWidget(m_autoGainLabel,0,0,1,3);
    // m_advancedInfoLayout->addWidget(m_wbLabel,1,0);
    // m_advancedInfoLayout->addWidget(m_tableW,1,1);
    // m_advancedInfoLayout->addWidget(m_adjustBtn,1,2);

    // m_layout->addWidget(m_nameLabel,0,0);
    m_layout->addWidget(m_baseInfoGroup,0,0);
    // m_layout->addWidget(m_advancedInfoGroup,2,0);
}
bool HGSignalDJWidget::closeWindow(){
    return true;
}
std::string HGSignalDJWidget::getParamStr(){
    std::ostringstream ss;
    ss<<m_typeLabel->text().toStdString()<<":" \
      <<m_typeEdit->text().toStdString()<<"," \
      <<m_serialLabel->text().toStdString()<<":" \
      <<m_serialEdit->text().toStdString()<<"," \
      <<m_workCurrentLabel->text().toStdString()<<":" \
      <<m_workCurrentEdit->text().toStdString();
    return ss.str();
}

void HGSignalDJWidget::setParamStr(std::string param){
    setControlText(m_baseInfoLayout,param);
    // setControlText(m_advancedInfoLayout,param);
}