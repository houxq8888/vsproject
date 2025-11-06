#include "hgdjadjustwidget.h"
#include "common.h"

HGDJAdjustWidget::HGDJAdjustWidget(std::string lang,QWidget *parent) : QWidget(parent),
m_lang(lang)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    QPalette palette;
    palette.setColor(QPalette::Window,Qt::white);
    setPalette(palette);
    setAutoFillBackground(true);

    m_layout=new QGridLayout();
    this->setLayout(m_layout);
      
    m_nameLabel=new QLabel("电极校正");
    m_curTempLabel=new QLabel("当前温度");
    m_ddAjustLabel=new QLabel("电极校正");
    m_adjustDSLabel=new QLabel("校正点数");
    m_voltageLabel=new QLabel("电位值");
    m_standardLabel=new QLabel("标准值");
    m_adjustTimeLabel=new QLabel("校正时间");
    m_voltageUnitEdit=new QLineEdit();
    m_standardUnitEdit=new QLineEdit();
    m_curTempEdit=new QLineEdit();
    m_curTempEdit->setPlaceholderText(QString::fromStdString(loadTranslation(m_lang,"Input")));
    m_ddAdjustEdit=new QLineEdit();
    m_adjustDSEdit=new QLineEdit();
    m_voltageEdit=new QLineEdit();
    m_standardEdit=new QLineEdit();
    m_adjustTimeEdit=new QLineEdit();

    m_readBtn=new QPushButton("读取");
    m_okBtn=new QPushButton("确定");
    m_adjustAndSaveToDBBtn=new QPushButton("校正，存入数据库");
    m_backBtn=new QPushButton("返回");
    m_applyBtn=new QPushButton("应用");

    connect(m_readBtn,SIGNAL(clicked()),this,SLOT(slotReadDB()));
    connect(m_okBtn,SIGNAL(clicked()),this,SLOT(slotOK()));
    connect(m_adjustAndSaveToDBBtn,SIGNAL(clicked()),this,SLOT(slotAjustAndSave()));
    connect(m_backBtn,SIGNAL(clicked()),this,SLOT(slotBack()));
    connect(m_applyBtn,SIGNAL(clicked()),this,SLOT(slotApply()));

    m_line=new QFrame();
    m_line->setFrameShape(QFrame::HLine);
    m_line->setFrameShadow(QFrame::Sunken);
    m_line1=new QFrame();
    m_line1->setFrameShape(QFrame::HLine);
    m_line1->setFrameShadow(QFrame::Sunken);

    m_layout->addWidget(m_nameLabel,0,0);
    m_layout->addWidget(m_curTempLabel,2,4);
    m_layout->addWidget(m_curTempEdit,2,5);
    m_layout->addWidget(m_readBtn,2,7);
    m_layout->addWidget(m_line,3,0,1,8);
    m_layout->addWidget(m_ddAjustLabel,4,4);
    m_layout->addWidget(m_ddAdjustEdit,4,5);
    m_layout->addWidget(m_adjustDSLabel,5,4);
    m_layout->addWidget(m_adjustDSEdit,5,5);
    m_layout->addWidget(m_voltageLabel,6,4);
    m_layout->addWidget(m_voltageEdit,6,5);
    m_layout->addWidget(m_voltageUnitEdit,6,6);
    m_layout->addWidget(m_standardLabel,7,4);
    m_layout->addWidget(m_standardEdit,7,5);
    m_layout->addWidget(m_standardUnitEdit,7,6);
    m_layout->addWidget(m_adjustTimeLabel,8,4);
    m_layout->addWidget(m_adjustTimeEdit,8,5);
    m_layout->addWidget(m_okBtn,8,7);
    m_layout->addWidget(m_adjustAndSaveToDBBtn,9,4,1,1);
    m_layout->addWidget(m_line1,10,0,1,8);
    m_layout->addWidget(m_backBtn,11,6);
    m_layout->addWidget(m_applyBtn,11,7);
    
}
bool HGDJAdjustWidget::closeWindow(){
    return true;
}
void HGDJAdjustWidget::slotReadDB(){

}
void HGDJAdjustWidget::slotOK(){

}
void HGDJAdjustWidget::slotAjustAndSave(){

}
void HGDJAdjustWidget::slotBack(){
    this->close();
}
void HGDJAdjustWidget::slotApply(){

}