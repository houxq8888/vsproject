#include "hgholdsamplewidget.h"
#include "common.h"
#include <sstream>
HGHoldSampleWidget::HGHoldSampleWidget(std::string lang,std::string name,QWidget *parent) : QWidget(parent),
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
  
    m_jiliangModeLabel=new QLabel("计量方式");
    m_jiliangModeCombo=new QComboBox();
    m_jiliangModeCombo->addItems({"时间"});
    m_liuyeSpeedLabel=new QLabel("留液速度(rps)");
    m_liuyeSpeedEdit=new QLineEdit();
    m_liuyeTimeLabel=new QLabel("留液时间(s)");
    m_liuyeTimeEdit=new QLineEdit();

    m_layout->addWidget(m_manipulateW,0,4);
    m_layout->addWidget(m_jiliangModeLabel,0,0);
    m_layout->addWidget(m_jiliangModeCombo,0,1);
    m_layout->addWidget(m_liuyeSpeedLabel,1,0);
    m_layout->addWidget(m_liuyeSpeedEdit,1,1);
    m_layout->addWidget(m_liuyeTimeLabel,2,0);
    m_layout->addWidget(m_liuyeTimeEdit,2,1);
}

bool HGHoldSampleWidget::closeWindow(){
    if (m_manipulateW){
        if (m_manipulateW->closeWindow()){
            SAFE_DELETE(m_manipulateW);
        }
    }
    return true;
}
std::string HGHoldSampleWidget::getParamStr(){
    std::ostringstream ss;
    ss<<m_jiliangModeLabel->text().toStdString()<<":" \
      <<m_jiliangModeCombo->currentText().toStdString()<<"," \
      <<m_liuyeSpeedLabel->text().toStdString()<<":" \
      <<m_liuyeSpeedEdit->text().toStdString()<<"," \
      <<m_liuyeTimeLabel->text().toStdString()<<":" \
      <<m_liuyeTimeEdit->text().toStdString();
    return ss.str();
}
void HGHoldSampleWidget::setParamStr(std::string param){
    setControlText(m_layout,param);
}