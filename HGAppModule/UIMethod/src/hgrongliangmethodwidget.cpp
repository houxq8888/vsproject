#include "hgrongliangmethodwidget.h"
#include "common.h"


HGRongliangMethodWidget::HGRongliangMethodWidget(std::string lang,QWidget *parent) : QWidget(parent),
m_lang(lang)
{
    m_driftflag=false;
    m_driftAutoFlag=DRIFT_OFF;
    m_layout=new QGridLayout();
    this->setLayout(m_layout);
            
    m_methodLabel=new QLabel("方法名称");
    m_methodEdit=new QLineEdit();
    m_detectMLabel=new QLabel("检测器");
    m_detectCombo=new QComboBox();
    m_detectCombo->addItems({"指示电极"});
    m_targetElementLabel=new QLabel("目标成分");
    m_targetElementEdit=new QLineEdit();
    m_ddjiLabel=new QLabel("滴定剂名称");
    m_ddjiCombo=new QComboBox();
    m_ddjiCombo->addItems({"卡费 PN2024110"});
    m_ddduLabel=new QLabel("滴定度");
    m_ddduEdit=new QLineEdit();
    m_ddModeLabel=new QLabel("滴定方式");
    m_ddModeCombo=new QComboBox();
    m_ddModeCombo->addItems({"最短时长"});
    m_ddTimeLabel=new QLabel("时长");
    m_ddTimeEdit=new QLineEdit();
    m_targetDWLabel=new QLabel("终点电位");
    m_targetDWEdit=new QLineEdit();
    m_targetMethodLabel=new QLabel("终点方式");
    m_targetMethodCombo=new QComboBox();
    m_targetMethodCombo->addItems({"延时终止"});
    m_targetYSLabel=new QLabel("终点延时");
    m_targetYSEdit=new QLineEdit();
    m_dynamicRangeLabel=new QLabel("动态范围");
    m_dynimicRangeEdit=new QLineEdit();
    m_limitConditionLabel=new QLabel("限制条件");
    m_limitConditionCombo=new QComboBox();
    m_limitConditionCombo->addItems({"最大体积"});
    m_cubeLabel=new QLabel("体积");
    m_cubeCombo=new QComboBox();
    m_cubeCombo->addItems({"30 ml"});
    // m_formulateEdit=new QTextEdit();
    // m_displayUnitLabel=new QLabel("显示单位");
    // m_displayUnitCombo=new QComboBox();
    // m_displayUnitCombo->addItems({"g/ml"});
    m_dataProcessBox=new QGroupBox("数据处理");
    m_dataProcessLayout=new QGridLayout();
    m_dataProcessBox->setLayout(m_dataProcessLayout);
    m_driftLabel=new LabelWithImg(IMGRIGHT,12,getPath("/resources/V1/@1xIOS开关.png"),"漂移扣除");
    m_autoDriftLabel=new LabelWithImg(IMGLEFT,12,getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"),"自动漂移扣除");
    m_humanDriftLabel=new LabelWithImg(IMGLEFT,12,getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"),"手动漂移扣除");
    m_humanDriftEdit=new QLineEdit();
    m_blankLabel=new QLabel("空白值");
    m_blankEdit=new QLineEdit();
 
    connect(m_driftLabel,SIGNAL(clickLeftName(std::string)),this,SLOT(slotLabelClicked(std::string)));
    connect(m_autoDriftLabel,SIGNAL(clickLeftName(std::string)),this,SLOT(slotLabelClicked(std::string)));
    connect(m_humanDriftLabel,SIGNAL(clickLeftName(std::string)),this,SLOT(slotLabelClicked(std::string)));
    m_dataProcessLayout->addWidget(m_driftLabel,0,0);
    m_dataProcessLayout->addWidget(m_autoDriftLabel,1,0);
    m_dataProcessLayout->addWidget(m_humanDriftLabel,2,0);
    m_dataProcessLayout->addWidget(m_humanDriftEdit,2,1);
    m_dataProcessLayout->addWidget(m_blankLabel,3,0);
    m_dataProcessLayout->addWidget(m_blankEdit,3,1);
    
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
    m_layout->addWidget(m_targetElementLabel,2,0);
    m_layout->addWidget(m_targetElementEdit,2,1);
    m_layout->addWidget(m_ddjiLabel,3,0);
    m_layout->addWidget(m_ddjiCombo,3,1);
    m_layout->addWidget(m_ddduLabel,4,0);
    m_layout->addWidget(m_ddduEdit,4,1);
    m_layout->addWidget(m_hor1,5,0,1,2);
    m_layout->addWidget(m_ddModeLabel,6,0);
    m_layout->addWidget(m_ddModeCombo,6,1);
    m_layout->addWidget(m_ddTimeLabel,7,0);
    m_layout->addWidget(m_ddTimeEdit,7,1);
    m_layout->addWidget(m_hor2,8,0,1,2);
    m_layout->addWidget(m_targetDWLabel,9,0);
    m_layout->addWidget(m_targetDWEdit,9,1);
    m_layout->addWidget(m_targetMethodLabel,10,0);
    m_layout->addWidget(m_targetMethodCombo,10,1);
    m_layout->addWidget(m_targetYSLabel,11,0);
    m_layout->addWidget(m_targetYSEdit,11,1);
    m_layout->addWidget(m_dynamicRangeLabel,12,0);
    m_layout->addWidget(m_dynimicRangeEdit,12,1);
    m_layout->addWidget(m_limitConditionLabel,13,0);
    m_layout->addWidget(m_limitConditionCombo,13,1);
    m_layout->addWidget(m_cubeLabel,14,0);
    m_layout->addWidget(m_cubeCombo,14,1);
    m_layout->addWidget(m_hor3,15,0,1,2);
    m_layout->addWidget(m_dataProcessBox,16,0,1,2);
}
bool HGRongliangMethodWidget::closeWindow()
{
    return true;
}
void HGRongliangMethodWidget::slotLabelClicked(std::string name){
    if (name=="漂移扣除"){
        m_driftflag=!m_driftflag;
        if (!m_driftflag) {
            m_driftLabel->setImg(getPath("/resources/V1/@1xIOS开关.png"));
            m_driftAutoFlag=DRIFT_OFF;
        }
        else {
            m_driftLabel->setImg(getPath("/resources/V1/@1xIOS开关_enable.png"));
            m_driftAutoFlag=DRIFT_AUTO;
        }
    } else if (name=="自动漂移扣除"){
        m_driftAutoFlag=DRIFT_AUTO;
    } else if (name=="手动漂移扣除"){
        m_driftAutoFlag=DRIFT_MANUAL;
    }
    setDriftParam();
}
void HGRongliangMethodWidget::setDriftParam()
{
    switch (m_driftAutoFlag) {
        case DRIFT_OFF:
        m_autoDriftLabel->setEnabled(false);
        m_humanDriftLabel->setEnabled(false);
        m_humanDriftEdit->setEnabled(false);
        m_blankLabel->setEnabled(false);
        m_blankEdit->setEnabled(false);
        break;
        case DRIFT_AUTO:
        m_autoDriftLabel->setImg(getPath("/resources/V1/@1xze-certificate 1.png"));
        m_humanDriftLabel->setImg(getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"));
        m_autoDriftLabel->setEnabled(true);
        m_humanDriftLabel->setEnabled(true);
        m_humanDriftEdit->setEnabled(false);
        m_blankLabel->setEnabled(false);
        m_blankEdit->setEnabled(false);
        break;
        case DRIFT_MANUAL:
        m_autoDriftLabel->setImg(getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"));
        m_humanDriftLabel->setImg(getPath("/resources/V1/@1xze-certificate 1.png"));
        m_autoDriftLabel->setEnabled(true);
        m_humanDriftLabel->setEnabled(true);
        m_humanDriftEdit->setEnabled(true);
        m_blankLabel->setEnabled(true);
        m_blankEdit->setEnabled(true);
        break;
    }
}
void HGRongliangMethodWidget::setParam(std::string param){
    setControlText(m_layout,param);
    std::map<std::string,std::string> wparam=getParamMap(param);
    if (wparam["漂移扣除"]=="true"){
        m_driftflag=true;
        m_driftLabel->setImg(getPath("/resources/V1/@1xIOS开关_enable.png"));
    } else {
        m_driftflag=false;
        m_driftLabel->setImg(getPath("/resources/V1/@1xIOS开关.png"));
    }

    if (wparam["自动漂移扣除"]=="auto"){
        m_driftAutoFlag=DRIFT_AUTO;
    } else if (wparam["自动漂移扣除"]=="manual"){
        m_driftAutoFlag=DRIFT_MANUAL;
    } else if (wparam["自动漂移扣除"]=="off"){
        m_driftAutoFlag=DRIFT_OFF;
    }
    setDriftParam();
    m_humanDriftEdit->setText(QString::fromStdString(wparam["手动漂移扣除"]));
    m_blankEdit->setText(QString::fromStdString(wparam["空白值"]));
}
std::string HGRongliangMethodWidget::getParamStr(){
    std::string driftStr="";
    if (m_driftflag) driftStr="true";
    else driftStr="false";
    std::string autoDriftStr="";
    switch (m_driftAutoFlag){
        case DRIFT_AUTO:
        autoDriftStr="auto";
        break;
        case DRIFT_MANUAL:
        autoDriftStr="manual";
        break;
        case DRIFT_OFF:
        autoDriftStr="off";
        break;
    }
    std::ostringstream param;
    param<<m_methodLabel->text().toStdString()<<":" \
      <<m_methodEdit->text().toStdString()<<"," \
      <<m_detectMLabel->text().toStdString()<<":" \
      <<m_detectCombo->currentText().toStdString()<<"," \
      <<m_targetElementLabel->text().toStdString()<<":" \
      <<m_targetElementEdit->text().toStdString()<<"," \
      <<m_ddjiLabel->text().toStdString()<<":" \
      <<m_ddjiCombo->currentText().toStdString()<<"," \
      <<m_ddduLabel->text().toStdString()<<":" \
      <<m_ddduEdit->text().toStdString()<<"," \
      <<m_ddModeLabel->text().toStdString()<<":" \
      <<m_ddModeCombo->currentText().toStdString()<<"," \
      <<m_ddTimeLabel->text().toStdString()<<":" \
      <<m_ddTimeEdit->text().toStdString()<<"," \
      <<m_targetDWLabel->text().toStdString()<<":" \
      <<m_targetDWEdit->text().toStdString()<<"," \
      <<m_targetMethodLabel->text().toStdString()<<":" \
      <<m_targetMethodCombo->currentText().toStdString()<<"," \
      <<m_targetYSLabel->text().toStdString()<<":" \
      <<m_targetYSEdit->text().toStdString()<<"," \
      <<m_dynamicRangeLabel->text().toStdString()<<":" \
      <<m_dynimicRangeEdit->text().toStdString()<<"," \
      <<m_limitConditionLabel->text().toStdString()<<":" \
      <<m_limitConditionCombo->currentText().toStdString()<<"," \
      <<m_cubeLabel->text().toStdString()<<":" \
      <<m_cubeCombo->currentText().toStdString()<<"," \
      <<m_driftLabel->getTextLabel()->text().toStdString()<<":" \
      <<driftStr<<"," \
      <<m_autoDriftLabel->getTextLabel()->text().toStdString()<<":" \
      <<autoDriftStr<<"," \
      <<m_humanDriftLabel->getTextLabel()->text().toStdString()<<":" \
      <<m_humanDriftEdit->text().toStdString()<<"," \
      <<m_blankLabel->text().toStdString()<<":" \
      <<m_blankEdit->text().toStdString();
    return param.str();
}