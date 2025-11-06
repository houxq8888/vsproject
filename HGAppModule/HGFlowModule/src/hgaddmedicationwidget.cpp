#include "hgaddmedicationwidget.h"
#include <sstream>
#include "common.h"

HGAddMedicationWidget::HGAddMedicationWidget(std::string lang,std::string name,QWidget *parent) : QWidget(parent),
m_lang(lang)
{
    m_manipulateW=NULL;
    m_manipulateW=new HGManipulateWidget(MANIPULATE_ALL,name);
    connect(m_manipulateW,&HGManipulateWidget::manipulateObject,
        [=](std::string name,int type){
            emit mainpuateObject(name,type);
    });

    m_layout=new QGridLayout();
    this->setLayout(m_layout);
    m_addReagentLabel=new QLabel("加药试剂");
    m_addSpeedLabel=new QLabel("加药速率(rps)");
    m_addValueLabel=new QLabel("加药数值");
    m_unitLabel=new QLabel("单位");
    m_addReagentCombo=new QComboBox();
    m_addReagentCombo->addItems({"Hcl PN2024115"});
    m_unitCombo=new QComboBox();
    m_unitCombo->addItems({"s"});
    m_addSpeedEdit=new QLineEdit();
    m_addValueEdit=new QLineEdit();
    
    m_layout->addWidget(m_manipulateW,0,4);
    m_layout->addWidget(m_addReagentLabel,1,0);
    m_layout->addWidget(m_addReagentCombo,1,1);
    m_layout->addWidget(m_addSpeedLabel,2,0);
    m_layout->addWidget(m_addSpeedEdit,2,1);
    m_layout->addWidget(m_addValueLabel,3,0);
    m_layout->addWidget(m_addValueEdit,3,1);
    m_layout->addWidget(m_unitLabel,4,0);
    m_layout->addWidget(m_unitCombo,4,1);
}


bool HGAddMedicationWidget::closeWindow(){
    if (m_manipulateW){
        if (m_manipulateW->closeWindow()){
            SAFE_DELETE(m_manipulateW);
        }
    }
    return true;
}
std::string HGAddMedicationWidget::getParamStr(){
    std::ostringstream ss;
    ss<<m_addReagentLabel->text().toStdString()<<":" \
      <<m_addReagentCombo->currentText().toStdString()<<"," \
      <<m_addSpeedLabel->text().toStdString()<<":" \
      <<m_addSpeedEdit->text().toStdString()<<"," \
      <<m_addValueLabel->text().toStdString()<<":" \
      <<m_addValueEdit->text().toStdString()<<"," \
      <<m_unitLabel->text().toStdString()<<":" \
      <<m_unitCombo->currentText().toStdString();
    return ss.str();
}

void HGAddMedicationWidget::setParamStr(std::string param){
    setControlText(m_layout,param);
}