#include "hgblankwidget.h"
#include <sstream>
#include "common.h"

HGblankWidget::HGblankWidget(std::string lang,std::string name,QWidget *parent) : QWidget(parent),
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

    m_ddjiLabel=new QLabel("滴定剂");
    m_ddjiCombo=new QComboBox();
    m_ddjiCombo->addItems({"NaOH PN 2024113"});

    m_gudingshichangeLabel=new QLabel("固定时长");
    m_gudingshichangeEdit=new QLineEdit();
    m_outputLabel=new QLabel("输出");
    m_outputEdit=new QLineEdit();

    m_layout->addWidget(m_manipulateW,0,4);
    m_layout->addWidget(m_ddjiLabel,0,0);
    m_layout->addWidget(m_ddjiCombo,0,1);
    m_layout->addWidget(m_gudingshichangeLabel,1,0);
    m_layout->addWidget(m_gudingshichangeEdit,1,1);
    m_layout->addWidget(m_outputLabel,2,0);
    m_layout->addWidget(m_outputEdit,2,1);
}
bool HGblankWidget::closeWindow(){
    if (m_manipulateW){
        if (m_manipulateW->closeWindow()){
            SAFE_DELETE(m_manipulateW);
        }
    }
    return true;
}

std::string HGblankWidget::getParamStr(){
    std::ostringstream ss;
    ss<<m_ddjiLabel->text().toStdString()<<":" \
      <<m_ddjiCombo->currentText().toStdString()<<"," \
      <<m_gudingshichangeLabel->text().toStdString()<<":" \
      <<m_gudingshichangeEdit->text().toStdString()<<"," \
      <<m_outputLabel->text().toStdString()<<":" \
      <<m_outputEdit->text().toStdString();
    return ss.str();
}
void HGblankWidget::setParamStr(std::string param){
    setControlText(m_layout,param);
}