#include "hgphdjwidget.h"
#include "common.h"
#include <sstream>

HGPHdjWidget::HGPHdjWidget(std::string lang,std::string name,QWidget *parent) : QWidget(parent),
m_lang(lang)
{        
    m_layout=new QGridLayout();
    this->setLayout(m_layout);

    m_djAdjustW=NULL;
    m_name=name;
    m_nameLabel=new QLabel("pH电极");
    QFont font=m_nameLabel->font();
    font.setPointSize(18);
    m_nameLabel->setFont(font);

    m_baseInfoGroup=new QGroupBox("基本信息");
    m_advancedInfoGroup=new QGroupBox("高级管理");
    m_baseInfoLayout=new QGridLayout();
    m_advancedInfoLayout=new QGridLayout();
    m_baseInfoGroup->setLayout(m_baseInfoLayout);
    m_advancedInfoGroup->setLayout(m_advancedInfoLayout);

    m_serialNumberLabel=new QLabel("电极序列号");
    m_calibrateRangeLabel=new QLabel("测量范围");
    m_tempRangeLabel=new QLabel("温度范围");
    m_adjustTimeLabel=new QLabel("校正时间");
    m_adjustCircleLabel=new QLabel("校正周期");
    
    m_serialNumberEdit=new QLineEdit();
    m_calibrateRangeEdit=new QLineEdit();
    m_tempRangeEdit=new QLineEdit();
    m_adjustTimeEdit=new QLineEdit();
    m_adjustCircleEdit=new QLineEdit();
    
    m_elecodeManageLabel=new QLabel("电极管理");
    m_adjustBtn=new QPushButton("电极校正");
    connect(m_adjustBtn,SIGNAL(clicked()),this,SLOT(slotAdjustClick()));

    m_baseInfoLayout->addWidget(m_serialNumberLabel,0,0);
    m_baseInfoLayout->addWidget(m_serialNumberEdit,0,1);
    m_baseInfoLayout->addWidget(m_calibrateRangeLabel,1,0);
    m_baseInfoLayout->addWidget(m_calibrateRangeEdit,1,1);
    m_baseInfoLayout->addWidget(m_tempRangeLabel,2,0);
    m_baseInfoLayout->addWidget(m_tempRangeEdit,2,1);
    m_baseInfoLayout->addWidget(m_adjustTimeLabel,3,0);
    m_baseInfoLayout->addWidget(m_adjustTimeEdit,3,1);
    m_baseInfoLayout->addWidget(m_adjustCircleLabel,4,0);
    m_baseInfoLayout->addWidget(m_adjustCircleEdit,4,1);

    m_advancedInfoLayout->addWidget(m_elecodeManageLabel,0,0);
    m_advancedInfoLayout->addWidget(m_adjustBtn,0,1);

    m_layout->addWidget(m_nameLabel,0,0);
    m_layout->addWidget(m_baseInfoGroup,1,0);
    m_layout->addWidget(m_advancedInfoGroup,2,0);
}
bool HGPHdjWidget::closeWindow(){
    if (m_djAdjustW){
        if (m_djAdjustW->closeWindow()){
            SAFE_DELETE(m_djAdjustW);
        }
    }
    return true;
}
std::string HGPHdjWidget::getParamStr(){
    std::ostringstream ss;
    ss<<m_serialNumberLabel->text().toStdString()<<":" \
      <<m_serialNumberEdit->text().toStdString()<<"," \
      <<m_calibrateRangeLabel->text().toStdString()<<":" \
      <<m_calibrateRangeEdit->text().toStdString()<<"," \
      <<m_tempRangeLabel->text().toStdString()<<":" \
      <<m_tempRangeEdit->text().toStdString()<<"," \
      <<m_adjustTimeLabel->text().toStdString()<<":" \
      <<m_adjustTimeEdit->text().toStdString()<<"," \
      <<m_adjustCircleLabel->text().toStdString()<<":" \
      <<m_adjustCircleEdit->text().toStdString();
    return ss.str();
}

void HGPHdjWidget::setParamStr(std::string param){
    setControlText(m_baseInfoLayout,param);
    setControlText(m_advancedInfoLayout,param);
}
void HGPHdjWidget::slotAdjustClick(){
    m_djAdjustW=new HGDJAdjustWidget(m_lang,this);
    int posy=m_nameLabel->pos().y()+m_nameLabel->height();
    m_djAdjustW->move(m_nameLabel->pos().x(),posy);
    m_djAdjustW->resize(this->width(),this->height()-posy);
    
    m_djAdjustW->show();
}