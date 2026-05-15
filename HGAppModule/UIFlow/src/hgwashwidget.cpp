#include "hgwashwidget.h"
#include "common.h"
#include <sstream>

HGWashWidget::HGWashWidget(std::string lang,std::string name,QWidget *parent) : QWidget(parent),
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
        
      
    m_paiyemoshiLabel=new QLabel("排液模式");
    m_paiyemoshiCombo=new QComboBox();
    // for (auto mode:washPaiYeMode){
    //     m_paiyemoshiCombo->addItem(QString::fromStdString(mode.second));
    // }
    m_zhurushijiLabel=new QLabel("注入试剂");
    m_zhurushijiCombo=new QComboBox();
    m_zhurushijiCombo->addItems({"乙醇 PN2024112"});
    m_zhurutijiLabel=new QLabel("注入体积");
    m_zhurutijiEdit=new QLineEdit();
    m_loopCntLabel=new QLabel("循环次数");
    m_loopCntCombo=new QComboBox();
    m_loopCntCombo->addItems({"2次"});

    m_layout->addWidget(m_manipulateW,0,4);
    m_layout->addWidget(m_paiyemoshiLabel,0,0);
    m_layout->addWidget(m_paiyemoshiCombo,0,1);
    m_layout->addWidget(m_zhurushijiLabel,1,0);
    m_layout->addWidget(m_zhurushijiCombo,1,1);
    m_layout->addWidget(m_zhurutijiLabel,2,0);
    m_layout->addWidget(m_zhurutijiEdit,2,1);
    m_layout->addWidget(m_loopCntLabel,3,0);
    m_layout->addWidget(m_loopCntCombo,3,1);
}

bool HGWashWidget::closeWindow(){
    if (m_manipulateW){
        if (m_manipulateW->closeWindow()){
            SAFE_DELETE(m_manipulateW);
        }
    }
    return true;
}
std::string HGWashWidget::getParamStr(){
    std::ostringstream ss;
    ss<<m_paiyemoshiLabel->text().toStdString()<<":" \
      <<m_paiyemoshiCombo->currentText().toStdString()<<"," \
      <<m_zhurushijiLabel->text().toStdString()<<":" \
      <<m_zhurushijiCombo->currentText().toStdString()<<"," \
      <<m_zhurutijiLabel->text().toStdString()<<":" \
      <<m_zhurutijiEdit->text().toStdString()<<"," \
      <<m_loopCntLabel->text().toStdString()<<":" \
      <<m_loopCntCombo->currentText().toStdString();
    return ss.str();
}
void HGWashWidget::setParamStr(std::string param){
    setControlText(m_layout,param);
}