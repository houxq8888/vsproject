#include "hgswirlstepwidget.h"
#include "common.h"
#include <sstream>

HGSwirlStepWidget::HGSwirlStepWidget(std::string lang,std::string name,QWidget *parent) : QWidget(parent),
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


    m_swirlSpeedLabel=new QLabel("搅拌速度");
    m_swirlSpeedEdit=new QLineEdit();

    m_actionPortLabel=new QLabel("执行端口");
    m_actionPortCombo=new QComboBox();
    std::vector<std::string> lowerPCPortNames; //=HGOnlineRWDB::getDevicePortInfos();
    for (int i=0;i<int(lowerPCPortNames.size());i++){
        m_actionPortCombo->addItem(QString::fromStdString(lowerPCPortNames[i]));
    }
    m_actionTimeLabel=new QLabel("时长");
    m_actionTimeEdit=new QLineEdit();
    m_layout->addWidget(m_manipulateW,0,4);
    m_layout->addWidget(m_swirlSpeedLabel,0,0);
    m_layout->addWidget(m_swirlSpeedEdit,0,1);
    m_layout->addWidget(m_actionPortLabel,1,0);
    m_layout->addWidget(m_actionPortCombo,1,1);
    m_layout->addWidget(m_actionTimeLabel,2,0);
    m_layout->addWidget(m_actionTimeEdit,2,1);
}
bool HGSwirlStepWidget::closeWindow(){
    if (m_manipulateW){
        if (m_manipulateW->closeWindow()){
            SAFE_DELETE(m_manipulateW);
        }
    }
    return true;
}
std::string HGSwirlStepWidget::getParamStr(){
    std::ostringstream ss;
    ss<<m_swirlSpeedLabel->text().toStdString()<<":" \
      <<m_swirlSpeedEdit->text().toStdString()<<"," \
      <<m_actionPortLabel->text().toStdString()<<":" \
      <<m_actionPortCombo->currentText().toStdString()<<"," \
      <<m_actionTimeLabel->text().toStdString()<<":" \
      <<m_actionTimeEdit->text().toStdString();
    return ss.str();
}
void HGSwirlStepWidget::setParamStr(std::string param){
    setControlText(m_layout,param);
}
