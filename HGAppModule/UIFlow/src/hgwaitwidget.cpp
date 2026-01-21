#include "hgwaitwidget.h"
#include <sstream>
#include "common.h"

HGWaitWidget::HGWaitWidget(std::string lang,std::string name,QWidget *parent) : QWidget(parent),
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
    m_waitTimeLabel=new QLabel("等待时间(s)");
    m_waitTimeCombo=new QComboBox();
    m_waitTimeCombo->addItems({"60"});
    m_layout->addWidget(m_manipulateW,0,4);
    m_layout->addWidget(m_waitTimeLabel,0,0);
    m_layout->addWidget(m_waitTimeCombo,0,1);
}

bool HGWaitWidget::closeWindow(){
    if (m_manipulateW){
        if (m_manipulateW->closeWindow()){
            SAFE_DELETE(m_manipulateW);
        }
    }
    return true;
}
std::string HGWaitWidget::getParamStr(){
    std::ostringstream ss;
    ss << m_waitTimeLabel->text().toStdString() << ":"
       << m_waitTimeCombo->currentText().toStdString();
    return ss.str();
}
void HGWaitWidget::setParamStr(std::string param){
    setControlText(m_layout,param);
}