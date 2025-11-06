#include "hginjectreagentwidget.h"
#include "common.h"
#include <sstream>

HGinjectReagentWidget::HGinjectReagentWidget(std::string lang,std::string param,std::string name,QWidget *parent) : QWidget(parent),
m_lang(lang)
{
    m_param=param;
    m_manipulateW=NULL;
    m_manipulateW=new HGManipulateWidget(MANIPULATE_ALL,name);
    connect(m_manipulateW,&HGManipulateWidget::manipulateObject,
        [=](std::string name,int type){
            emit mainpuateObject(name,type);
    });

    m_layout=new QGridLayout();
    this->setLayout(m_layout);

    m_zhurushijiLabel=new QLabel("注入试剂");
    m_zhurushijiCombo=new QComboBox();
    m_zhurushijiCombo->addItems({"反应液"});
    m_zhurumoshiLabel=new QLabel("注入模式");
    m_zhurumoshiCombo=new QComboBox();
    // for (auto mode:injectReagentMode){
    //     m_zhurumoshiCombo->addItem(QString::fromStdString(mode.second));
    // }
    std::map<std::string,std::string> wparam=getParamMap(m_param);
    m_zhurumoshiCombo->setCurrentText(QString::fromStdString(wparam["注入模式"]));
    setMode();

    m_zhurusulvLabel=new QLabel("注入速率");
    m_zhurusulvEdit=new QLineEdit();
    m_zhurutijiLabel=new QLabel("注入体积");
    m_zhurutijiEdit=new QLineEdit();
    connect(m_zhurumoshiCombo,&QComboBox::currentTextChanged,[=](QString str){
        setMode();
    });
     
    m_layout->addWidget(m_manipulateW,0,4);
    m_layout->addWidget(m_zhurushijiLabel,0,0);
    m_layout->addWidget(m_zhurushijiCombo,0,1);
    m_layout->addWidget(m_zhurumoshiLabel,1,0);
    m_layout->addWidget(m_zhurumoshiCombo,1,1);
    m_layout->addWidget(m_zhurusulvLabel,2,0);
    m_layout->addWidget(m_zhurusulvEdit,2,1);
    m_layout->addWidget(m_zhurutijiLabel,3,0);
    m_layout->addWidget(m_zhurutijiEdit,3,1);
}

bool HGinjectReagentWidget::closeWindow(){
    if (m_manipulateW){
        if (m_manipulateW->closeWindow()){
            SAFE_DELETE(m_manipulateW);
        }
    }
    return true;
}
std::string HGinjectReagentWidget::getParamStr(){
    std::ostringstream ss;
    ss<<m_zhurushijiLabel->text().toStdString()<<":" \
      <<m_zhurushijiCombo->currentText().toStdString()<<"," \
      <<m_zhurumoshiLabel->text().toStdString()<<":" \
      <<m_zhurumoshiCombo->currentText().toStdString()<<"," \
      <<m_zhurusulvLabel->text().toStdString()<<":" \
      <<m_zhurusulvEdit->text().toStdString()<<"," \
      <<m_zhurutijiLabel->text().toStdString()<<":" \
      <<m_zhurutijiEdit->text().toStdString();
    return ss.str();
}
void HGinjectReagentWidget::setParamStr(std::string param){
    setControlText(m_layout,param);
}
void HGinjectReagentWidget::setMode(){
    std::string str=m_zhurumoshiCombo->currentText().toStdString();
    if (str=="体积注入"){
        m_zhurutijiLabel->setText("注入体积");
    } else if (str=="计时注入"){
        m_zhurutijiLabel->setText("注入时间");
    }
}