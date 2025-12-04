#include "hgelebalancewidget.h"
#include "common.h"

HGEleBalanceWidget::HGEleBalanceWidget(std::string lang,std::string name,QWidget *parent) : QWidget(parent),
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
    m_inputWeigthLabel=new QLabel("输入(重量)");
    m_unitLabel=new QLabel("单位");
    m_unitCombo=new QComboBox();
    m_unitCombo->addItems({"g"});
    m_inputWeightEdit=new QLineEdit();

    m_layout->addWidget(m_manipulateW,0,4);
    m_layout->addWidget(m_inputWeigthLabel,0,0);
    m_layout->addWidget(m_inputWeightEdit,0,1);
    m_layout->addWidget(m_unitLabel,1,0);
    m_layout->addWidget(m_unitCombo,1,1);
}
bool HGEleBalanceWidget::closeWindow(){
    return true;
} 

std::string HGEleBalanceWidget::getParamStr(){
    std::ostringstream ss;
    ss<<m_inputWeigthLabel->text().toStdString()<<":" \
      <<m_inputWeightEdit->text().toStdString()<<"," \
      <<m_unitLabel->text().toStdString()<<":" \
      <<m_unitCombo->currentText().toStdString();
    return ss.str();
}
void HGEleBalanceWidget::setParamStr(std::string param){
    setControlText(m_layout,param);
}
void HGEleBalanceWidget::setReadOnly(bool flag){
    m_unitCombo->setEnabled(!flag);
    m_inputWeightEdit->setReadOnly(flag);
}