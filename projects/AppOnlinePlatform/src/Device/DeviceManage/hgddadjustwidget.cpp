#include "hgddadjustwidget.h"
#include "common.h"

HGDDAdjustWidget::HGDDAdjustWidget(std::string lang,QWidget *parent) : QWidget(parent),
m_lang(lang)
{

    this->setWindowFlags(Qt::FramelessWindowHint);
    QPalette palette;
    palette.setColor(QPalette::Window,Qt::white);
    setPalette(palette);
    setAutoFillBackground(true);

    m_layout=new QGridLayout();
    this->setLayout(m_layout);

    m_nameLabel=new QLabel("滴定管校正");
    m_adjustNameLabel=new QLabel("校正方法");
    m_adjustPhaseLabel=new QLabel("校正段数");
    m_cubeRangeLabel=new QLabel("体积范围");
    m_standardLabel=new QLabel("标准值");
    m_unitLabel=new QLabel("单位");
    m_adjustTempLabel=new QLabel("校正温度");
    m_adjustTimeLabel=new QLabel("校正时间");
    m_adjustCircleLabel=new QLabel("校正周期");
    m_adjustNameCombo=new QComboBox();
    m_adjustNameCombo->addItems({"体积校正多段"});
    m_adjustPhaseCombo=new QComboBox();
    m_adjustPhaseCombo->addItems({"第一段"});
    m_cubeRangeCombo=new QComboBox();
    m_cubeRangeCombo->addItems({"2ml"});
    m_unitCombo=new QComboBox();
    m_unitCombo->addItems({"ml"});
    m_adjustTempEdit=new QLineEdit();
    m_adjustTempEdit->setPlaceholderText(QString::fromStdString(loadTranslation(m_lang,"Input")));

    m_adjustTimeEdit=new QLineEdit();
    m_adjustCircleEdit=new QLineEdit();
    m_standardEdit=new QLineEdit();

    m_readBtn=new QPushButton("读取");
    m_okBtn=new QPushButton("确定");
    m_adjustAndSaveToDBBtn=new QPushButton("校正，存入数据库");
    m_cancelABtn=new QPushButton("返回");
    m_adjustBtn=new QPushButton("校正");
    m_cancelBtn=new QPushButton("取消");
    m_applyBtn=new QPushButton("应用");

    connect(m_readBtn,SIGNAL(clicked()),this,SLOT(slotReadDB()));
    connect(m_okBtn,SIGNAL(clicked()),this,SLOT(slotOK()));
    connect(m_adjustAndSaveToDBBtn,SIGNAL(clicked()),this,SLOT(slotAjustAndSave()));
    connect(m_cancelABtn,SIGNAL(clicked()),this,SLOT(slotBack()));
    connect(m_applyBtn,SIGNAL(clicked()),this,SLOT(slotApply()));
    connect(m_cancelBtn,SIGNAL(clicked()),this,SLOT(slotCancel()));
    connect(m_adjustBtn,SIGNAL(clicked()),this,SLOT(slotAdjust()));

    m_line=new QFrame();
    m_line->setFrameShape(QFrame::HLine);
    m_line->setFrameShadow(QFrame::Sunken);

    m_layout->addWidget(m_nameLabel,0,0);
    m_layout->addWidget(m_adjustNameLabel,2,4);
    m_layout->addWidget(m_adjustNameCombo,2,5);
    m_layout->addWidget(m_adjustPhaseLabel,3,4);
    m_layout->addWidget(m_adjustPhaseCombo,3,5);
    m_layout->addWidget(m_cubeRangeLabel,4,4);
    m_layout->addWidget(m_cubeRangeCombo,4,5);
    m_layout->addWidget(m_adjustBtn,4,6);
    m_layout->addWidget(m_standardLabel,5,4);
    m_layout->addWidget(m_standardEdit,5,5);
    m_layout->addWidget(m_unitLabel,6,4);
    m_layout->addWidget(m_unitCombo,6,5);
    m_layout->addWidget(m_okBtn,6,6);
    m_layout->addWidget(m_cancelBtn,6,7);

    m_layout->addWidget(m_adjustTempLabel,7,4);
    m_layout->addWidget(m_adjustTempEdit,7,5);
    m_layout->addWidget(m_adjustTimeLabel,8,4);
    m_layout->addWidget(m_adjustTimeEdit,8,5);
    m_layout->addWidget(m_readBtn,8,6);
    m_layout->addWidget(m_adjustCircleLabel,9,4);
    m_layout->addWidget(m_adjustCircleEdit,9,5);
    m_layout->addWidget(m_adjustAndSaveToDBBtn,10,4,1,1);

    m_layout->addWidget(m_line,11,0,1,8);

    m_layout->addWidget(m_cancelABtn,12,6);
    m_layout->addWidget(m_applyBtn,12,7);

    
}
bool HGDDAdjustWidget::closeWindow(){
    return true;
}
void HGDDAdjustWidget::slotReadDB(){

}
void HGDDAdjustWidget::slotOK(){

}
void HGDDAdjustWidget::slotAjustAndSave(){

}
void HGDDAdjustWidget::slotBack(){
    this->close();
}
void HGDDAdjustWidget::slotApply(){

}
void HGDDAdjustWidget::slotCancel(){
    
}
void HGDDAdjustWidget::slotAdjust                                                                                                                                          (){

}