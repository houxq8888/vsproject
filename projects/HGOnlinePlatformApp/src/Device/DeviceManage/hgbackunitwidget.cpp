#include "hgbackunitwidget.h"
#include "common.h"
#include <sstream>
#include <QToolTip>

HGBackUnitWidget::HGBackUnitWidget(std::string lang,std::string name,QWidget *parent) : QWidget(parent),
m_lang(lang)
{
    m_isEnablePump=true;
    m_layout=new QGridLayout();
    this->setLayout(m_layout);
    m_ddAdjustW=NULL;
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

    m_serialNumberLabel=new QLabel("泵序列号");
    m_ddSpecificationLabel=new QLabel("滴定管规格");
    m_minDDCubeLabel=new QLabel("最小滴定体积");
    m_ddAccuracyLabel=new QLabel("滴定管精度");
    m_valveTypeLabel=new QLabel("从阀类型");
    m_adjustTimeLabel=new QLabel("校正时间");
    m_adjustCircleLabel=new QLabel("校正周期");
    
    m_serialNumberEdit=new QLineEdit();
    m_ddSpecificationEdit=new QLineEdit();
    m_minDDCubeEdit=new QLineEdit();
    m_ddAccuracyEdit=new QLineEdit();
    m_valveTypeEdit=new QLineEdit();
    m_adjustTimeEdit=new QLineEdit();
    m_adjustCircleEdit=new QLineEdit();
       
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

    m_adjustBtn=new QPushButton("滴定管校正");
    connect(m_adjustBtn,SIGNAL(clicked()),this,SLOT(slotAdjustClick()));


    m_baseInfoLayout->addWidget(m_serialNumberLabel,0,0);
    m_baseInfoLayout->addWidget(m_serialNumberEdit,0,1);
    m_baseInfoLayout->addWidget(m_ddSpecificationLabel,1,0);
    m_baseInfoLayout->addWidget(m_ddSpecificationEdit,1,1);
    m_baseInfoLayout->addWidget(m_minDDCubeLabel,2,0);
    m_baseInfoLayout->addWidget(m_minDDCubeEdit,2,1);
    m_baseInfoLayout->addWidget(m_ddAccuracyLabel,3,0);
    m_baseInfoLayout->addWidget(m_ddAccuracyEdit,3,1);
    m_baseInfoLayout->addWidget(m_valveTypeLabel,4,0);
    m_baseInfoLayout->addWidget(m_valveTypeEdit,4,1);
    m_baseInfoLayout->addWidget(m_adjustTimeLabel,5,0);
    m_baseInfoLayout->addWidget(m_adjustTimeEdit,5,1);
    m_baseInfoLayout->addWidget(m_adjustCircleLabel,6,0);
    m_baseInfoLayout->addWidget(m_adjustCircleEdit,6,1);

    m_advancedInfoLayout->addWidget(m_pumpLifeCircleLabel,0,0);
    m_advancedInfoLayout->addWidget(m_pumpLifeCircleEdit,0,1);
    m_advancedInfoLayout->addWidget(m_enablePumpLabel,1,0);
    m_advancedInfoLayout->addWidget(m_cumulativeSlider,1,1);
    m_advancedInfoLayout->addWidget(m_cumulativeLifeSpanLabel,1,2);
    m_advancedInfoLayout->addWidget(m_adjustBtn,2,0);

    m_layout->addWidget(m_nameLabel,0,0);
    m_layout->addWidget(m_baseInfoGroup,1,0);
    m_layout->addWidget(m_advancedInfoGroup,2,0);
}
bool HGBackUnitWidget::closeWindow(){
    if (m_ddAdjustW){
        if (m_ddAdjustW->closeWindow()){
            SAFE_DELETE(m_ddAdjustW);
        }
    }
    return true;
}
void HGBackUnitWidget::clickEnablePump()
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
std::string HGBackUnitWidget::getParamStr(){
    std::ostringstream ss;
    ss<<m_serialNumberLabel->text().toStdString()<<":" \
      <<m_serialNumberEdit->text().toStdString()<<"," \
      <<m_ddSpecificationLabel->text().toStdString()<<":" \
      <<m_ddSpecificationEdit->text().toStdString()<<"," \
      <<m_minDDCubeLabel->text().toStdString()<<":" \
      <<m_minDDCubeEdit->text().toStdString()<<"," \
      <<m_ddAccuracyLabel->text().toStdString()<<":" \
      <<m_ddAccuracyEdit->text().toStdString()<<"," \
      <<m_valveTypeLabel->text().toStdString()<<":" \
      <<m_valveTypeEdit->text().toStdString()<<"," \
      <<m_adjustTimeLabel->text().toStdString()<<":" \
      <<m_adjustTimeEdit->text().toStdString()<<"," \
      <<m_adjustCircleLabel->text().toStdString()<<":" \
      <<m_adjustCircleEdit->text().toStdString()<<"," \
      <<m_pumpLifeCircleLabel->text().toStdString()<<":" \
      <<m_pumpLifeCircleEdit->text().toStdString()<<"," \
      <<m_enablePumpLabel->getTextLabel()->text().toStdString()<<":" \
      <<m_enablePumpLabel->getImgPath();
    // printf("backUnit:%s\n",ss.str().c_str());
    return ss.str();
}

void HGBackUnitWidget::setParamStr(std::string param){
    setControlText(m_baseInfoLayout,param);
    setControlText(m_advancedInfoLayout,param);
}
void HGBackUnitWidget::slotAdjustClick(){
    m_ddAdjustW=new HGDDAdjustWidget(m_lang,this);
    int posy=m_nameLabel->pos().y()+m_nameLabel->height();
    m_ddAdjustW->move(m_nameLabel->pos().x(),posy);
    m_ddAdjustW->resize(this->width(),this->height()-posy);
    
    m_ddAdjustW->show();
}