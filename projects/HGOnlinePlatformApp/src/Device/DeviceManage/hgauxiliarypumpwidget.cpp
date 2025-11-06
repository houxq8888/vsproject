#include "hgauxiliarypumpwidget.h"
#include "common.h"
#include <sstream>
#include <QToolTip>

HGAuxiliaryPumpWidget::HGAuxiliaryPumpWidget(std::string lang,std::string name,QWidget *parent) : QWidget(parent),
m_lang(lang)
{
    m_isEnablePump=true;
    m_layout=new QGridLayout();
    this->setLayout(m_layout);
       
    m_name=name;
    m_nameLabel=new QLabel(QString::fromStdString(name));
    QFont font=m_nameLabel->font();
    font.setPointSize(18);
    m_nameLabel->setFont(font);
      
    m_baseInfoGroup=new QGroupBox("基本信息");
    m_advancedInfoGroup=new QGroupBox("高级管理");
    m_baseInfoLayout=new QGridLayout();
    m_advancedInfoLayout=new QGridLayout();
    m_baseInfoGroup->setLayout(m_baseInfoLayout);
    m_advancedInfoGroup->setLayout(m_advancedInfoLayout);
  
    m_serialNumberLabel=new QLabel("辅助泵序列号");
    m_ratedSpeedLabel=new QLabel("额定转速");
    m_maxSpeedLabel=new QLabel("最大转速");
    
    m_serialNumberEdit=new QLineEdit();
    m_ratedSpeedEdit=new QLineEdit();
    m_maxSpeedEdit=new QLineEdit();
       
    m_pumpLifeCircleLabel=new QLabel("泵使用寿命");
    m_pumpLifeCircleEdit=new QLineEdit();
    m_enablePumpLabel=new LabelWithImg(IMGRIGHT,12,getPath("/resources/V1/@1xIOS开关_enable.png"),"启用泵");
    
    m_cumulativeLifeSpanLabel=new QLabel("累计使用寿命");
    m_cumulativeSlider=new QSlider();
    m_cumulativeSlider->setOrientation(Qt::Horizontal);
    // 连接 QSlider 的 valueChanged 信号，在滑动时显示 QToolTip
    connect(m_cumulativeSlider, &QSlider::sliderMoved, this, [=](int value) {
            // 获取滑块的位置
        QPoint pos = m_cumulativeSlider->mapToGlobal(QPoint(m_cumulativeSlider->width() * value / m_cumulativeSlider->maximum(), 0));
        QToolTip::showText(pos, QString::number(value));
    });

    connect(m_enablePumpLabel,SIGNAL(clickImgLabel()),this,SLOT(clickEnablePump()));

    m_adjustBtn=new QPushButton("流量校正");

    m_baseInfoLayout->addWidget(m_serialNumberLabel,0,0);
    m_baseInfoLayout->addWidget(m_serialNumberEdit,0,1);
    m_baseInfoLayout->addWidget(m_ratedSpeedLabel,1,0);
    m_baseInfoLayout->addWidget(m_ratedSpeedEdit,1,1);
    m_baseInfoLayout->addWidget(m_maxSpeedLabel,2,0);
    m_baseInfoLayout->addWidget(m_maxSpeedEdit,2,1);

    m_advancedInfoLayout->addWidget(m_pumpLifeCircleLabel,0,0);
    m_advancedInfoLayout->addWidget(m_pumpLifeCircleEdit,0,1);
    m_advancedInfoLayout->addWidget(m_enablePumpLabel,1,0);
    m_advancedInfoLayout->addWidget(m_cumulativeSlider,2,1);
    m_advancedInfoLayout->addWidget(m_cumulativeLifeSpanLabel,2,0);
    m_advancedInfoLayout->addWidget(m_adjustBtn,3,0);

    m_layout->addWidget(m_nameLabel,0,0);
    m_layout->addWidget(m_baseInfoGroup,1,0);
    m_layout->addWidget(m_advancedInfoGroup,2,0);
}
bool HGAuxiliaryPumpWidget::closeWindow(){
    return true;
}
void HGAuxiliaryPumpWidget::clickEnablePump()
{
    m_isEnablePump=!m_isEnablePump;
    if (!m_isEnablePump){
        m_enablePumpLabel->setImg(getPath("/resources/V1/@1xIOS开关.png"));
    } else {
        m_enablePumpLabel->setImg(getPath("/resources/V1/@1xIOS开关_enable.png"));

    }

    m_pumpLifeCircleEdit->setEnabled(m_isEnablePump);
    m_cumulativeSlider->setEnabled(m_isEnablePump);
}
std::string HGAuxiliaryPumpWidget::getParamStr(){
    std::ostringstream ss;
    ss<<m_serialNumberLabel->text().toStdString()<<":" \
      <<m_serialNumberEdit->text().toStdString()<<"," \
      <<m_ratedSpeedLabel->text().toStdString()<<":" \
      <<m_ratedSpeedEdit->text().toStdString()<<"," \
      <<m_maxSpeedLabel->text().toStdString()<<":" \
      <<m_maxSpeedEdit->text().toStdString()<<"," \
      <<m_pumpLifeCircleLabel->text().toStdString()<<":" \
      <<m_pumpLifeCircleEdit->text().toStdString()<<"," \
      <<m_enablePumpLabel->getTextLabel()->text().toStdString()<<":" \
      <<m_enablePumpLabel->getImgPath();

    // printf("auxiliaryPump:%s\n",ss.str().c_str());
    return ss.str();
}

void HGAuxiliaryPumpWidget::setParamStr(std::string param){
    setControlText(m_baseInfoLayout,param);
    setControlText(m_advancedInfoLayout,param);
}