#include "hgshuifenyiwidget.h"
#include "common.h"
#include <sstream>

HGShuifenyiWidget::HGShuifenyiWidget(std::string lang,std::string name,QWidget *parent) : QWidget(parent),
m_lang(lang),
m_name(name)
{
    m_layout=new QGridLayout();
    this->setLayout(m_layout);
    m_nameLabel=new QLabel(QString::fromStdString(name));
    QFont font=m_nameLabel->font();
    font.setPointSize(18);
    m_nameLabel->setFont(font);
      
    m_sampleNameLabel=new QLabel("样品名称");
    m_sampleNameEdit=new QLineEdit();
    m_sampleNoLabel=new QLabel("样品编号");
    m_sampleNoEdit=new QLineEdit();
    m_methodLabel=new QLabel("方法");
    m_methodCombo=new QComboBox();

    m_needleCheck=new QCheckBox("取样针");
    connect(m_needleCheck,SIGNAL(clicked()),this,SLOT(slotNeedleCheck()));

    m_releaseNeedleBtn=new QPushButton("抬针");
    m_punctureBtn=new QPushButton("穿刺");
    m_stopNeedleBtn=new QPushButton("停针");

    m_captureWeightBtn=new QPushButton("采集重量");
    m_weightSampleRadio=new QRadioButton("重量进样");
    m_cubeSampleRadio=new QRadioButton("体积进样");
    m_weightSampleEdit=new QLineEdit();
    m_cubeSampleEdit=new QLineEdit();
    m_weightSampleUnitLabel=new QLabel("g");
    m_cubeSampleUnitLabel=new QLabel("mL");
    m_densityCheck=new QCheckBox("密度");
    m_densityEdit=new QLineEdit();
    m_densityUnitLabel=new QLabel("g/mL");

    m_manualDetectSampleBtn=new QPushButton("手动测样");
    connect(m_manualDetectSampleBtn,SIGNAL(clicked()),this,SLOT(slotManualDetectSample()));

    m_layout->addWidget(m_nameLabel,0,0);
    m_layout->addWidget(m_sampleNameLabel,m_layout->rowCount(),0);
    m_layout->addWidget(m_sampleNameEdit,m_layout->rowCount()-1,1);
    m_layout->addWidget(m_sampleNoLabel,m_layout->rowCount(),0);
    m_layout->addWidget(m_sampleNoEdit,m_layout->rowCount()-1,1);
    m_layout->addWidget(m_methodLabel,m_layout->rowCount(),0);
    m_layout->addWidget(m_methodCombo,m_layout->rowCount()-1,1);
    m_layout->addWidget(m_needleCheck,m_layout->rowCount(),0);
    m_layout->addWidget(m_releaseNeedleBtn,m_layout->rowCount()-1,1);
    m_layout->addWidget(m_punctureBtn,m_layout->rowCount()-1,2);
    m_layout->addWidget(m_stopNeedleBtn,m_layout->rowCount()-1,3);
    m_layout->addWidget(m_captureWeightBtn,m_layout->rowCount(),1);
    m_layout->addWidget(m_weightSampleRadio,m_layout->rowCount(),0);
    m_layout->addWidget(m_weightSampleEdit,m_layout->rowCount()-1,1);
    m_layout->addWidget(m_weightSampleUnitLabel,m_layout->rowCount()-1,2);
    m_layout->addWidget(m_cubeSampleRadio,m_layout->rowCount(),0);
    m_layout->addWidget(m_cubeSampleEdit,m_layout->rowCount()-1,1);
    m_layout->addWidget(m_cubeSampleUnitLabel,m_layout->rowCount()-1,2);
    m_layout->addWidget(m_densityCheck,m_layout->rowCount(),0);
    m_layout->addWidget(m_densityEdit,m_layout->rowCount()-1,1);
    m_layout->addWidget(m_densityUnitLabel,m_layout->rowCount()-1,2);
    m_layout->addWidget(m_manualDetectSampleBtn,m_layout->rowCount(),2);

    m_needleCheck->setChecked(false);
    slotNeedleCheck();
    // fnDisplayBaseInfo();
    // m_timer=new QTimer(this);
    // connect(m_timer,SIGNAL(timeout()),this,SLOT(slotUpdateSerialPort()));
    // m_timer->start(1000);

    // m_deviceComCombo->setCurrentIndex(-1);
}
bool HGShuifenyiWidget::closeWindow(){
    return true;
}
std::string HGShuifenyiWidget::getParamStr(){
    std::ostringstream ss;
    // ss<<m_maxSpeedLabel->text().toStdString()<<":" \
    //   <<m_maxSpeedEdit->text().toStdString()<<"," \
    //   <<m_distanceLabel->text().toStdString()<<":" \
    //   <<m_distanceEdit->text().toStdString()<<"," \
    //   <<m_portLabel->text().toStdString()<<":" \
    //   <<m_portEdit->text().toStdString()<<"," \
    //   <<m_dataLabel->text().toStdString()<<":" \
    //   <<m_dataEdit->text().toStdString()<<"," \
    //   <<m_adjustLabel->text().toStdString()<<":" \
    //   <<m_adjustEdit->text().toStdString();
    return ss.str();
}

void HGShuifenyiWidget::setParamStr(std::string param){
    // setControlText(m_baseInfoLayout,param);
    // setControlText(m_advancedInfoLayout,param);
}
void HGShuifenyiWidget::slotManualDetectSample(){

}
void HGShuifenyiWidget::slotNeedleCheck(){
    if(m_needleCheck->isChecked()){
        m_releaseNeedleBtn->setEnabled(true);
        m_punctureBtn->setEnabled(true);
        m_stopNeedleBtn->setEnabled(true);
        m_releaseNeedleBtn->setVisible(true);
        m_punctureBtn->setVisible(true);
        m_stopNeedleBtn->setVisible(true);
    }
    else{
        m_releaseNeedleBtn->setEnabled(false);
        m_punctureBtn->setEnabled(false);
        m_stopNeedleBtn->setEnabled(false);
        m_releaseNeedleBtn->setVisible(false);
        m_punctureBtn->setVisible(false);
        m_stopNeedleBtn->setVisible(false);
    }
}