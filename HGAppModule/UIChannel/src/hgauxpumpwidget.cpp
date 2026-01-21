#include "hgauxpumpwidget.h"
#include "common.h"

HGAuxPumpWidget::HGAuxPumpWidget(std::string lang,std::string name,QWidget *parent) : QWidget(parent),
m_lang(lang)
{
    m_manipulateW=NULL;
    m_manipulateW=new HGManipulateWidget(MANIPULATE_CLOSE,name);
    connect(m_manipulateW,&HGManipulateWidget::manipulateObject,
        [=](std::string name,int type){
            emit mainpuateObject(name,type);
    });

    m_layout=new QGridLayout();
    this->setLayout(m_layout);
    m_choiceLabel=new QLabel("选用");
    m_functionLabel=new QLabel("功能");
    m_rotateSpeedLabel=new QLabel("转动速率");
    m_choiceCombo=new QComboBox();
    m_functionCombo=new QComboBox();
    m_functionCombo->addItems({"清洗泵"});
    m_rotateSpeedEdit=new QLineEdit();

    m_layout->addWidget(m_manipulateW,0,4);
    m_layout->addWidget(m_choiceLabel,0,0);
    m_layout->addWidget(m_choiceCombo,0,1);
    m_layout->addWidget(m_functionLabel,1,0);
    m_layout->addWidget(m_functionCombo,1,1);
    m_layout->addWidget(m_rotateSpeedLabel,2,0);
    m_layout->addWidget(m_rotateSpeedEdit,2,1);
}
bool HGAuxPumpWidget::closeWindow(){
    if (m_manipulateW){
        if (m_manipulateW->closeWindow()){
            SAFE_DELETE(m_manipulateW);
        }
    }
    return true;
}
std::string HGAuxPumpWidget::getParamStr(){
    std::ostringstream ss;
    ss<<m_choiceLabel->text().toStdString()<<":" \
      <<m_choiceCombo->currentText().toStdString()<<"," \
      <<m_functionLabel->text().toStdString()<<":" \
      <<m_functionCombo->currentText().toStdString()<<"," \
      <<m_rotateSpeedLabel->text().toStdString()<<":" \
      <<m_rotateSpeedEdit->text().toStdString();
    return ss.str();
}

void HGAuxPumpWidget::setParamStr(std::string param){
    setControlText(m_layout,param);
}

void HGAuxPumpWidget::setReadOnly(bool flag){
    m_choiceCombo->setEnabled(!flag);
    m_functionCombo->setEnabled(!flag);
    m_rotateSpeedEdit->setReadOnly(flag);
}