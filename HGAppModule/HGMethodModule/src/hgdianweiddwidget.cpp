#include "hgdianweiddwidget.h"
#include "common.h"

HGDianWeiDDWidget::HGDianWeiDDWidget(std::string lang,QWidget *parent) : QWidget(parent),
m_lang(lang)
{
    m_layout=new QGridLayout();
    this->setLayout(m_layout);
   
    m_methodLabel=new QLabel("方法名称");
    m_methodEdit=new QLineEdit();
    m_dianjiTypeLabel=new QLabel("电极种类");
    m_dianjiTypeCombo=new QComboBox();
    m_dianjiTypeCombo->addItems({"pH复合电极"});
    m_actionLiquidLabel=new QLabel("反应液名称");
    m_actionLiquidCombo=new QComboBox();
    m_actionLiquidCombo->addItems({"缓冲溶液 PNA202409"});
    m_targetElementLabel=new QLabel("目标成分");
    m_targetElementEdit=new QLineEdit();
    m_ddjiLabel=new QLabel("滴定剂名称");
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
    m_formulateEdit=new QTextEdit();
    m_displayUnitLabel=new QLabel("显示单位");
    m_displayUnitCombo=new QComboBox();
    m_displayUnitCombo->addItems({"g/ml"});

    m_hor1=new QFrame;
    m_hor1->setFrameShape(QFrame::HLine);
    m_hor1->setFrameShadow(QFrame::Sunken);
    m_hor2=new QFrame;
    m_hor2->setFrameShape(QFrame::HLine);
    m_hor2->setFrameShadow(QFrame::Sunken);

    m_layout->addWidget(m_methodLabel,0,0);
    m_layout->addWidget(m_methodEdit,0,1);
    m_layout->addWidget(m_dianjiTypeLabel,1,0);
    m_layout->addWidget(m_dianjiTypeCombo,1,1);
    m_layout->addWidget(m_actionLiquidLabel,2,0);
    m_layout->addWidget(m_actionLiquidCombo,2,1);
    m_layout->addWidget(m_targetElementLabel,3,0);
    m_layout->addWidget(m_targetElementEdit,3,1);
    m_layout->addWidget(m_ddjiLabel,4,0);
    m_layout->addWidget(m_ddjiCombo,4,1);
    m_layout->addWidget(m_ddduLabel,5,0);
    m_layout->addWidget(m_ddduEdit,5,1);
    m_layout->addWidget(m_hor1,6,0,1,2);
    m_layout->addWidget(m_targetJudgeLabel,7,0);
    m_layout->addWidget(m_targetJudgeCombo,7,1);
    m_layout->addWidget(m_weifenThresholdLabel,8,0);
    m_layout->addWidget(m_weifenThresholdEdit,8,1);
    m_layout->addWidget(m_targetBeforeCubeLabel,9,0);
    m_layout->addWidget(m_targetBeforeCubeEdit,9,1);
    m_layout->addWidget(m_targetAfterCubeLabel,10,0);
    m_layout->addWidget(m_targetAfterCubeEdit,10,1);
    m_layout->addWidget(m_hor2,11,0,1,2);
    m_layout->addWidget(m_calculateFormulatLabel,12,0);
    m_layout->addWidget(m_calculateFormulatCombo,12,1);
    m_layout->addWidget(m_formulateEdit,13,0,1,2);
    m_layout->addWidget(m_displayUnitLabel,14,0);
    m_layout->addWidget(m_displayUnitCombo,14,1);
}
bool HGDianWeiDDWidget::closeWindow()
{
    return true;
}
void HGDianWeiDDWidget::setParam(std::string param){
    setControlText(m_layout,param);
}
std::string HGDianWeiDDWidget::getParamStr(){
    std::ostringstream param;
    param<<m_methodLabel->text().toStdString()<<":" \
      <<m_methodEdit->text().toStdString()<<"," \
      <<m_dianjiTypeLabel->text().toStdString()<<":" \
      <<m_dianjiTypeCombo->currentText().toStdString()<<"," \
      <<m_actionLiquidLabel->text().toStdString()<<":" \
      <<m_actionLiquidCombo->currentText().toStdString()<<"," \
      <<m_targetElementLabel->text().toStdString()<<":" \
      <<m_targetElementEdit->text().toStdString()<<"," \
      <<m_ddjiLabel->text().toStdString()<<":" \
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
      <<m_calculateFormulatCombo->currentText().toStdString()<<"|" \
      <<m_formulateEdit->toPlainText().toStdString()<<"," \
      <<m_displayUnitLabel->text().toStdString()<<":" \
      <<m_displayUnitCombo->currentText().toStdString();
    return param.str();
}