#include "hgjudgewidget.h"
#include <sstream>
#include "common.h"
HGJudgeWidget::HGJudgeWidget(std::string lang,std::string name,QWidget *parent) : QWidget(parent),
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
    m_compareSignalLabel=new QLabel("对比信号值");
    m_unitLabel=new QLabel("单位");
    m_compareValueLabel=new QLabel("比较值");
    m_trackLabel=new QLabel("跟随操作");
    m_compareSignalCombo=new QComboBox();
    m_compareSignalCombo->addItems({"pH电极"});
    m_unitCombo=new QComboBox();
    m_unitCombo->addItems({"pH"});
    m_compareValueCombo=new QComboBox();
    m_compareValueCombo->addItems({"< 7"});
    m_trackCombo=new QComboBox();
    m_trackCombo->addItems({"2 step"});

    m_layout->addWidget(m_manipulateW,0,4);
    m_layout->addWidget(m_compareSignalLabel,0,0);
    m_layout->addWidget(m_compareSignalCombo,0,1);
    m_layout->addWidget(m_unitLabel,1,0);
    m_layout->addWidget(m_unitCombo,1,1);
    m_layout->addWidget(m_compareValueLabel,2,0);
    m_layout->addWidget(m_compareValueCombo,2,1);
    m_layout->addWidget(m_trackLabel,3,0);
    m_layout->addWidget(m_trackCombo,3,1);
}

bool HGJudgeWidget::closeWindow(){
    if (m_manipulateW){
        if (m_manipulateW->closeWindow()){
            SAFE_DELETE(m_manipulateW);
        }
    }
    return true;
}
std::string HGJudgeWidget::getParamStr(){
    std::ostringstream ss;
    ss<<m_compareSignalLabel->text().toStdString()<<":" \
      <<m_compareSignalCombo->currentText().toStdString()<<"," \
      <<m_unitLabel->text().toStdString()<<":" \
      <<m_unitCombo->currentText().toStdString()<<"," \
      <<m_compareValueLabel->text().toStdString()<<":" \
      <<m_compareValueCombo->currentText().toStdString()<<"," \
      <<m_trackLabel->text().toStdString()<<":" \
      <<m_trackCombo->currentText().toStdString();
    return ss.str();
}
void HGJudgeWidget::setParamStr(std::string param){
    setControlText(m_layout,param);
}