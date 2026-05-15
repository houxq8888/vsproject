#include "hgphddwidget.h"
#include "common.h"
HGphDDWidget::HGphDDWidget(std::string lang,std::string name,QWidget *parent) : QWidget(parent),
m_lang(lang)
{
    m_manipulateW=NULL;
    m_manipulateW=new HGManipulateWidget(MANIPULATE_CLOSE,name);
    connect(m_manipulateW,&HGManipulateWidget::manipulateObject,
        [=](std::string name,int type){
            emit mainpuateObject(name,type);
    });

    m_layout=new QGridLayout();
    m_detectNameLabel=new QLabel("检测方法");
    m_detectCombo=new QComboBox();
    m_detectCombo->addItems({"滴定法"});

    this->setLayout(m_layout);

    m_layout->addWidget(m_manipulateW,0,4);
    m_layout->addWidget(m_detectNameLabel,0,0);
    m_layout->addWidget(m_detectCombo,0,1);
}
bool HGphDDWidget::closeWindow(){
    if (m_manipulateW){
        if (m_manipulateW->closeWindow()){
            SAFE_DELETE(m_manipulateW);
        }
    }
    return true;
}

std::string HGphDDWidget::getParamStr(){
    std::ostringstream ss;
    ss<<m_detectNameLabel->text().toStdString()<<":" \
      <<m_detectCombo->currentText().toStdString();
    return ss.str();
}
void HGphDDWidget::setParamStr(std::string param){
    setControlText(m_layout,param);
}
void HGphDDWidget::setReadOnly(bool flag){
    m_detectCombo->setEnabled(!flag);
}