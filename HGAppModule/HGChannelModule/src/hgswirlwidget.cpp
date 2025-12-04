#include "hgswirlwidget.h"
#include "common.h"

HGSwirlWidget::HGSwirlWidget(std::string lang,bool isHot,std::string name,QWidget *parent) : QWidget(parent),
m_lang(lang)
{
    m_manipulateW=NULL;
    m_manipulateW=new HGManipulateWidget(MANIPULATE_CLOSE,name);
    connect(m_manipulateW,&HGManipulateWidget::manipulateObject,
        [=](std::string name,int type){
            emit mainpuateObject(name,type);
    });

    m_isHot=isHot;
    m_layout=new QGridLayout();
    this->setLayout(m_layout);

    m_choiceLabel=new QLabel("选用");
    m_swirlSpeedLabel=new QLabel("搅拌速度");
    m_choiceCombo=new QComboBox();
    m_choiceCombo->addItems({"1#"});
    m_swirlSpeedEdit=new QLineEdit();
    m_layout->addWidget(m_manipulateW,0,4);
    m_layout->addWidget(m_choiceLabel,0,0);
    m_layout->addWidget(m_choiceCombo,0,1);
    m_layout->addWidget(m_swirlSpeedLabel,1,0);
    m_layout->addWidget(m_swirlSpeedEdit,1,1);
    if (m_isHot)
    {
        m_hotModuleLabel = new QLabel("加热模块");
        m_temperatureSetLabel = new QLabel("温度设定");
        m_hotModuleCombo = new QComboBox();
        m_hotModuleCombo->addItems({"1#"});
        m_temperatureSetEdit = new QLineEdit();
        m_layout->addWidget(m_hotModuleLabel,2,0);
        m_layout->addWidget(m_hotModuleCombo,2,1);
        m_layout->addWidget(m_temperatureSetLabel,3,0);
        m_layout->addWidget(m_temperatureSetEdit,3,1);
    }
}
bool HGSwirlWidget::closeWindow(){
    if (m_manipulateW){
        if (m_manipulateW->closeWindow()){
            SAFE_DELETE(m_manipulateW);
        }
    }
    return true;
}

std::string HGSwirlWidget::getParamStr(){
    std::ostringstream ss;
    ss<<m_choiceLabel->text().toStdString()<<":" \
      <<m_choiceCombo->currentText().toStdString()<<"," \
      <<m_swirlSpeedLabel->text().toStdString()<<":" \
      <<m_swirlSpeedEdit->text().toStdString();
    if (m_isHot){
        ss<<",";
        ss<<m_hotModuleLabel->text().toStdString()<<":" \
            <<m_hotModuleCombo->currentText().toStdString()<<"," \
            <<m_temperatureSetLabel->text().toStdString()<<":" \
            <<m_temperatureSetEdit->text().toStdString();
    }
    return ss.str();
}
void HGSwirlWidget::setParamStr(std::string param){
    setControlText(m_layout,param);
}
void HGSwirlWidget::setReadOnly(bool flag){
    m_choiceCombo->setEnabled(!flag);
    m_swirlSpeedEdit->setReadOnly(flag);
    if (m_isHot){
        m_hotModuleCombo->setEnabled(!flag);
        m_temperatureSetEdit->setReadOnly(flag);
    }
}