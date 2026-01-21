#include "hgloopwidget.h"
#include <sstream>
#include "common.h"

HGLoopWidget::HGLoopWidget(std::string lang,std::string name,QWidget *parent) : QWidget(parent),
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
// 连续进行
    m_typeLabel=new QLabel("循环模式");
    m_modeLabel=new QLabel("形式");
    m_maxCountLabel=new QLabel("最大次数");
    m_modeCombo=new QComboBox();
    m_modeCombo->addItems({"起点"});
    m_typeEdit=new QLineEdit();
    m_maxCountEdit=new QLineEdit();
    
    m_layout->addWidget(m_manipulateW,0,4);
    m_layout->addWidget(m_modeLabel,0,0);
    m_layout->addWidget(m_modeCombo,0,1);
    m_layout->addWidget(m_typeLabel,1,0);
    m_layout->addWidget(m_typeEdit,1,1);
    m_layout->addWidget(m_maxCountLabel,2,0);
    m_layout->addWidget(m_maxCountEdit,2,1);
}

bool HGLoopWidget::closeWindow(){
    if (m_manipulateW){
        if (m_manipulateW->closeWindow()){
            SAFE_DELETE(m_manipulateW);
        }
    }
    return true;
}
std::string HGLoopWidget::getParamStr(){
    std::ostringstream ss;
    ss<<m_modeLabel->text().toStdString()<<":" \
      <<m_modeCombo->currentText().toStdString()<<"," \
      <<m_typeLabel->text().toStdString()<<":" \
      <<m_typeEdit->text().toStdString()<<"," \
      <<m_maxCountLabel->text().toStdString()<<":" \
      <<m_maxCountEdit->text().toStdString();
    return ss.str();
}
void HGLoopWidget::setParamStr(std::string param){
    setControlText(m_layout,param);
}