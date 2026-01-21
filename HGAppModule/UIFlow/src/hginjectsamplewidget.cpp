#include "hginjectsamplewidget.h"
#include <sstream>
#include "common.h"

HGInjectSampleWidget::HGInjectSampleWidget(std::string lang,std::string param,std::string name,QWidget *parent) : QWidget(parent),
m_lang(lang)
{
    m_param=param;
    m_manipulateW=NULL;
    m_manipulateW=new HGManipulateWidget(MANIPULATE_ALL,name);
    connect(m_manipulateW,&HGManipulateWidget::manipulateObject,
        [=](std::string name,int type){
            emit mainpuateObject(name,type);
    });

    m_quyangmoshiLabel=new QLabel("取样模式");
    m_quyangmoshiCombo=new QComboBox();
    // for (auto mode:getSampleMode){
    //     m_quyangmoshiCombo->addItem(QString::fromStdString(mode.second));
    // }
    m_quyangsulvLabel=new QLabel("取样速率");
    m_quyangsulvEdit=new QLineEdit();
    m_quyangtimeLabel=new QLabel("取样时间");
    m_quyangtimeEdit=new QLineEdit();
    m_layout=new QGridLayout();
    this->setLayout(m_layout);
    m_actionPortLabel=new QLabel("执行端口");
    m_actionPortCombo=new QComboBox();
    std::vector<std::string> lowerPCPortNames; //=HGOnlineRWDB::getDevicePortInfos();
    for (int i=0;i<int(lowerPCPortNames.size());i++){
        m_actionPortCombo->addItem(QString::fromStdString(lowerPCPortNames[i]));
    }

    std::map<std::string,std::string> wparam=getParamMap(m_param);
    m_quyangmoshiCombo->setCurrentText(QString::fromStdString(wparam["取样模式"]));
    setMode();
    connect(m_quyangmoshiCombo,&QComboBox::currentTextChanged,[=](QString str){
        setMode();
    });
    m_actionTimeLabel=new QLabel("时长");
    m_actionTimeEdit=new QLineEdit();
    m_layout->addWidget(m_manipulateW,0,4);
    m_layout->addWidget(m_quyangmoshiLabel,0,0);
    m_layout->addWidget(m_quyangmoshiCombo,0,1);
    m_layout->addWidget(m_quyangsulvLabel,1,0);
    m_layout->addWidget(m_quyangsulvEdit,1,1);
    m_layout->addWidget(m_quyangtimeLabel,2,0);
    m_layout->addWidget(m_quyangtimeEdit,2,1);
    m_layout->addWidget(m_actionPortLabel,3,0);
    m_layout->addWidget(m_actionPortCombo,3,1);
    m_layout->addWidget(m_actionTimeLabel,4,0);
    m_layout->addWidget(m_actionTimeEdit,4,1);
}
void HGInjectSampleWidget::setMode(){
    std::string str=m_quyangmoshiCombo->currentText().toStdString();
    if (str=="体积模式"){
        m_quyangtimeLabel->setText("取样体积");
    } else if (str=="计时模式"){
        m_quyangtimeLabel->setText("取样时间");
    }
}
bool HGInjectSampleWidget::closeWindow(){
    if (m_manipulateW){
        if (m_manipulateW->closeWindow()){
            SAFE_DELETE(m_manipulateW);
        }
    }
    return true;
}
std::string HGInjectSampleWidget::getParamStr(){
    std::ostringstream ss;
    ss<<m_quyangmoshiLabel->text().toStdString()<<":" \
      <<m_quyangmoshiCombo->currentText().toStdString()<<"," \
      <<m_quyangsulvLabel->text().toStdString()<<":" \
      <<m_quyangsulvEdit->text().toStdString()<<"," \
      <<m_quyangtimeLabel->text().toStdString()<<":" \
      <<m_quyangtimeEdit->text().toStdString()<<"," \
      <<m_actionPortLabel->text().toStdString()<<":" \
      <<m_actionPortCombo->currentText().toStdString()<<"," \
      <<m_actionTimeLabel->text().toStdString()<<":" \
      <<m_actionTimeEdit->text().toStdString();
    return ss.str();
}

void HGInjectSampleWidget::setParamStr(std::string param){
    setControlText(m_layout,param);
}