#include "hglightcalibmethodwidget.h"
#include "common.h"

HGLightCalibMethodWidget::HGLightCalibMethodWidget(std::string lang,QWidget *parent) : QWidget(parent),
m_lang(lang)
{
    m_layout=new QGridLayout();
    this->setLayout(m_layout);

    m_methodLabel=new QLabel("方法名称");
    m_methodEdit=new QLineEdit();
    m_detectMLabel=new QLabel("检测器");
    m_detectCombo=new QComboBox();
    m_detectCombo->addItems({"光度计"});
    m_zhishiWaveLabel=new QLabel("指示波长");
    m_zhishiWaveCombo=new QComboBox();
    m_zhishiWaveCombo->addItems({""});
    m_fuzhuWaveLabel=new QLabel("辅助波长");
    m_fuzhuWaveCombo=new QComboBox();
    m_fuzhuWaveCombo->addItems({""});
    m_canbiWaveLabel=new QLabel("参比波长");
    m_canbiWaveCombo=new QComboBox();
    m_actionLiquidLabel=new QLabel("反应液名称");
    m_actionLiquidCombo=new QComboBox();
    m_actionLiquidCombo->addItems({"纯水 PNA202410"});
    m_zhishijiLabel=new QLabel("指示剂");
    m_zhishijiEdit=new QLineEdit();
    m_targetElementLabel=new QLabel("目标成分");
    m_targetElementEdit=new QLineEdit();
    m_ddjiNameLabel=new QLabel("滴定剂名称");
    m_ddjiCombo=new QComboBox();
    m_ddjiCombo->addItems({"NaOH PN2024110"});
    m_ddduLabel=new QLabel("滴定度");
    m_ddduEdit=new QLineEdit();
    m_targetJudgeLabel=new QLabel("终点判断");
    m_targetJudgeCombo=new QComboBox();
    m_targetJudgeCombo->addItems({"微分阈值"});
    m_weifenThresholdLabel=new QLabel("微分阈值");
    m_weifenThresholdEdit=new QLineEdit();
    m_targetBeforeCubeLabel=new QLabel("终点前体积");
    m_targetBeforeCubeEdit=new QLineEdit();
    m_targetAfterCubeLabel=new QLabel("终点后体积");
    m_targetAfterCubeEdit=new QLineEdit();
    m_calculateFormulatLabel=new QLabel("计算公式");
    m_calculateFormulatCombo=new QComboBox();
    m_calculateFormulatCombo->addItems({"公式一"});
     
    m_hor1=new QFrame;
    m_hor1->setFrameShape(QFrame::HLine);
    m_hor1->setFrameShadow(QFrame::Sunken);
    m_hor2=new QFrame;
    m_hor2->setFrameShape(QFrame::HLine);
    m_hor2->setFrameShadow(QFrame::Sunken);
    m_hor3=new QFrame;
    m_hor3->setFrameShape(QFrame::HLine);
    m_hor3->setFrameShadow(QFrame::Sunken);

    m_layout->addWidget(m_methodLabel,0,0);
    m_layout->addWidget(m_methodEdit,0,1);
    m_layout->addWidget(m_detectMLabel,1,0);
    m_layout->addWidget(m_detectCombo,1,1);
    m_layout->addWidget(m_zhishiWaveLabel,2,0);
    m_layout->addWidget(m_zhishiWaveCombo,2,1);
    m_layout->addWidget(m_fuzhuWaveLabel,3,0);
    m_layout->addWidget(m_fuzhuWaveCombo,3,1);
    m_layout->addWidget(m_canbiWaveLabel,4,0);
    m_layout->addWidget(m_canbiWaveCombo,4,1);
    m_layout->addWidget(m_hor1,5,0,1,2);
    m_layout->addWidget(m_actionLiquidLabel,6,0);
    m_layout->addWidget(m_actionLiquidCombo,6,1);
    m_layout->addWidget(m_zhishijiLabel,7,0);
    m_layout->addWidget(m_zhishijiEdit,7,1);
    m_layout->addWidget(m_targetElementLabel,8,0);
    m_layout->addWidget(m_targetElementEdit,8,1);
    m_layout->addWidget(m_ddjiNameLabel,9,0);
    m_layout->addWidget(m_ddjiCombo,9,1);
    m_layout->addWidget(m_ddduLabel,10,0);
    m_layout->addWidget(m_ddduEdit,10,1);
    m_layout->addWidget(m_hor2,11,0,1,2);
    m_layout->addWidget(m_targetJudgeLabel,12,0);
    m_layout->addWidget(m_targetJudgeCombo,12,1);
    m_layout->addWidget(m_weifenThresholdLabel,13,0);
    m_layout->addWidget(m_weifenThresholdEdit,13,1);
    m_layout->addWidget(m_targetBeforeCubeLabel,14,0);
    m_layout->addWidget(m_targetBeforeCubeEdit,14,1);
    m_layout->addWidget(m_targetAfterCubeLabel,15,0);
    m_layout->addWidget(m_targetAfterCubeEdit,15,1);
    m_layout->addWidget(m_hor3,16,0,1,2);
    m_layout->addWidget(m_calculateFormulatLabel,17,0);
    m_layout->addWidget(m_calculateFormulatCombo,17,1);


}
bool HGLightCalibMethodWidget::closeWindow()
{
    return true;
}
void HGLightCalibMethodWidget::setParam(std::string param){
    setControlText(m_layout,param);
}
std::string HGLightCalibMethodWidget::getParamStr(){
    std::ostringstream param;
    param<<m_methodLabel->text().toStdString()<<":" \
      <<m_methodEdit->text().toStdString()<<"," \
      <<m_detectMLabel->text().toStdString()<<":" \
      <<m_detectCombo->currentText().toStdString()<<"," \
      <<m_zhishiWaveLabel->text().toStdString()<<":" \
      <<m_zhishiWaveCombo->currentText().toStdString()<<"," \
      <<m_fuzhuWaveLabel->text().toStdString()<<":" \
      <<m_fuzhuWaveCombo->currentText().toStdString()<<"," \
      <<m_canbiWaveLabel->text().toStdString()<<":" \
      <<m_canbiWaveCombo->currentText().toStdString()<<"," \
      <<m_actionLiquidLabel->text().toStdString()<<":" \
      <<m_actionLiquidCombo->currentText().toStdString()<<"," \
      <<m_zhishijiLabel->text().toStdString()<<":" \
      <<m_zhishijiEdit->text().toStdString()<<"," \
      <<m_targetElementLabel->text().toStdString()<<":" \
      <<m_targetElementEdit->text().toStdString()<<"," \
      <<m_ddjiNameLabel->text().toStdString()<<":" \
      <<m_ddjiCombo->currentText().toStdString()<<"," \
      <<m_ddduLabel->text().toStdString()<<":" \
      <<m_ddduEdit->text().toStdString()<<"," \
      <<m_targetJudgeLabel->text().toStdString()<<":" \
      <<m_targetJudgeCombo->currentText().toStdString()<<"," \
      <<m_weifenThresholdLabel->text().toStdString()<<":" \
      <<m_weifenThresholdEdit->text().toStdString()<<"," \
      <<m_targetBeforeCubeLabel->text().toStdString()<<":" \
      <<m_targetBeforeCubeEdit->text().toStdString()<<"," \
      <<m_targetAfterCubeLabel->text().toStdString()<<":" \
      <<m_targetAfterCubeEdit->text().toStdString()<<"," \
      <<m_calculateFormulatLabel->text().toStdString()<<":" \
      <<m_calculateFormulatCombo->currentText().toStdString();
    return param.str();
}