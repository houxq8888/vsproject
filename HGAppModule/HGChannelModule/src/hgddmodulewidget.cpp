#include "hgddmodulewidget.h"
#include "common.h"

HGDDModuleWidget::HGDDModuleWidget(std::string lang,std::string type,std::string name,QWidget *parent) : QWidget(parent),
m_lang(lang)
{
    m_manipulateW=NULL;
    m_manipulateW=new HGManipulateWidget(MANIPULATE_CLOSE,name);
    connect(m_manipulateW,&HGManipulateWidget::manipulateObject,
        [=](std::string name,int type){
            emit mainpuateObject(name,type);
    });

    m_type=type;
    m_layout=new QGridLayout();
    this->setLayout(m_layout);
    m_ddTypeLabel=new QLabel("滴定管");
    m_ddPumpLabel=new QLabel("滴定泵");
    m_congWavlveLabel=new QLabel("从阀");
    m_inputLiquidSpeedLabel=new QLabel("吸液速率");
    m_backLiquidSpeedLabel=new QLabel("回液速率");;
    m_ddPumpCombo=new QComboBox();
    m_inputLiquidSpeedCombo=new QComboBox();
    m_backLiquidCombo=new QComboBox();
    m_ddTypeEdit=new QLineEdit();
    m_ddTypeEdit->setText(QString::fromStdString(m_type));
    m_congWavlveEdit=new QLineEdit();

    m_layout->addWidget(m_manipulateW,0,4);
    m_layout->addWidget(m_ddTypeLabel,0,0);
    m_layout->addWidget(m_ddTypeEdit,0,1);
    m_layout->addWidget(m_ddPumpLabel,1,0);
    m_layout->addWidget(m_ddPumpCombo,1,1);
    m_layout->addWidget(m_congWavlveLabel,2,0);
    m_layout->addWidget(m_congWavlveEdit,2,1);
    m_layout->addWidget(m_inputLiquidSpeedLabel,3,0);
    m_layout->addWidget(m_inputLiquidSpeedCombo,3,1);
    m_layout->addWidget(m_backLiquidSpeedLabel,4,0);
    m_layout->addWidget(m_backLiquidCombo,4,1);
}
bool HGDDModuleWidget::closeWindow(){
    if (m_manipulateW){
        if (m_manipulateW->closeWindow()){
            SAFE_DELETE(m_manipulateW);
        }
    }
    return true;
}

std::string HGDDModuleWidget::getParamStr(){
    std::ostringstream ss;
    ss<<m_ddTypeLabel->text().toStdString()<<":" \
      <<m_ddTypeEdit->text().toStdString()<<"," \
      <<m_ddPumpLabel->text().toStdString()<<":" \
      <<m_ddPumpCombo->currentText().toStdString()<<"," \
      <<m_congWavlveLabel->text().toStdString()<<":" \
      <<m_congWavlveEdit->text().toStdString()<<"," \
      <<m_inputLiquidSpeedLabel->text().toStdString()<<":" \
      <<m_inputLiquidSpeedCombo->currentText().toStdString()<<"," \
      <<m_backLiquidSpeedLabel->text().toStdString()<<":" \
      <<m_backLiquidCombo->currentText().toStdString();
    return ss.str();
}
void HGDDModuleWidget::setParamStr(std::string param){
    setControlText(m_layout,param);
}
void HGDDModuleWidget::setReadOnly(bool flag){
    m_backLiquidCombo->setEnabled(!flag);
    m_inputLiquidSpeedCombo->setEnabled(!flag);
    m_ddPumpCombo->setEnabled(!flag);
    m_ddTypeEdit->setReadOnly(flag);
    m_congWavlveEdit->setReadOnly(flag);
}