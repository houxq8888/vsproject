#include "hgdetectwidget.h"
#include "common.h"
#include <sstream>

HGDetectWidget::HGDetectWidget(std::string lang,std::string name,QWidget *parent) : QWidget(parent),
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

    m_minDDCubeLabel=new QLabel("最小滴定体积(ul)");
    m_minDDCubeEdit=new QLineEdit();
    m_maxDDCubeLabel=new QLabel("最大滴定体积(ul)");
    m_maxDDCubeEdit=new QLineEdit();
    m_stopTimePerDiLabel=new QLabel("每滴暂停(ms)");
    m_stopTimePerDiEdit=new QLineEdit();
    m_preDDLabel=new QLabel("预滴定(ml)");
    m_preDDEdit=new QLineEdit();
    m_preAfterStopLabel=new QLabel("预滴后停(s)");
    m_preAfterStopEdit=new QLineEdit();
    m_swirlSpeedLabel=new QLabel("搅拌速度");
    m_swirlSpeedCombo=new QComboBox();
    m_swirlSpeedCombo->addItems({"5","10","15","20","25","30"});

    m_actionPortLabel=new QLabel("执行端口");
    m_actionPortCombo=new QComboBox();
    std::vector<std::string> lowerPCPortNames;//=HGOnlineRWDB::getDevicePortInfos();
    for (int i=0;i<int(lowerPCPortNames.size());i++){
        m_actionPortCombo->addItem(QString::fromStdString(lowerPCPortNames[i]));
    }

    m_testMethodLabel=new QLabel("测试方法");
    m_testMethodCombo=new QComboBox();
    m_actionTimeLabel=new QLabel("时长");
    m_actionTimeEdit=new QLineEdit();
    std::vector<std::string> names =RWDb::getMethodNames();
    for (int i=0;i<int(names.size());i++){
        m_testMethodCombo->addItem(QString::fromStdString(names[i]));
    }

    m_layout->addWidget(m_manipulateW,0,4);
    m_layout->addWidget(m_minDDCubeLabel,0,0);
    m_layout->addWidget(m_minDDCubeEdit,0,1);
    m_layout->addWidget(m_maxDDCubeLabel,1,0);
    m_layout->addWidget(m_maxDDCubeEdit,1,1);
    m_layout->addWidget(m_stopTimePerDiLabel,2,0);
    m_layout->addWidget(m_stopTimePerDiEdit,2,1);
    m_layout->addWidget(m_preDDLabel,3,0);
    m_layout->addWidget(m_preDDEdit,3,1);
    m_layout->addWidget(m_preAfterStopLabel,4,0);
    m_layout->addWidget(m_preAfterStopEdit,4,1);
    m_layout->addWidget(m_swirlSpeedLabel,5,0);
    m_layout->addWidget(m_swirlSpeedCombo,5,1);
    m_layout->addWidget(m_actionPortLabel,6,0);
    m_layout->addWidget(m_actionPortCombo,6,1);
    m_layout->addWidget(m_testMethodLabel,7,0);
    m_layout->addWidget(m_testMethodCombo,7,1);
    m_layout->addWidget(m_actionTimeLabel,8,0);
    m_layout->addWidget(m_actionTimeEdit,8,1);
}

bool HGDetectWidget::closeWindow(){
    if (m_manipulateW){
        if (m_manipulateW->closeWindow()){
            SAFE_DELETE(m_manipulateW);
        }
    }
    return true;
}
std::string HGDetectWidget::getParamStr(){
    std::ostringstream ss;
    ss<<m_minDDCubeLabel->text().toStdString()<<":" \
      <<m_minDDCubeEdit->text().toStdString()<<"," \
      <<m_maxDDCubeLabel->text().toStdString()<<":" \
      <<m_maxDDCubeEdit->text().toStdString()<<"," \
      <<m_stopTimePerDiLabel->text().toStdString()<<":" \
      <<m_stopTimePerDiEdit->text().toStdString()<<"," \
      <<m_preDDLabel->text().toStdString()<<":" \
      <<m_preDDEdit->text().toStdString()<<"," \
      <<m_preAfterStopLabel->text().toStdString()<<":" \
      <<m_preAfterStopEdit->text().toStdString()<<"," \
      <<m_swirlSpeedLabel->text().toStdString()<<":" \
      <<m_swirlSpeedCombo->currentText().toStdString()<<"," \
      <<m_actionPortLabel->text().toStdString()<<":" \
      <<m_actionPortCombo->currentText().toStdString()<<"," \
      <<m_testMethodLabel->text().toStdString()<<":" \
      <<m_testMethodCombo->currentText().toStdString()<<"," \
      <<m_actionTimeLabel->text().toStdString()<<":" \
      <<m_actionTimeEdit->text().toStdString();
    return ss.str();
}
void HGDetectWidget::setParamStr(std::string param){
    setControlText(m_layout,param);
}