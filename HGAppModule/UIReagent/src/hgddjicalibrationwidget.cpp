#include "hgddjicalibrationwidget.h"
#include "common.h"

HGDDjiCalibrationWidget::HGDDjiCalibrationWidget(std::string lang,QWidget *parent) : QWidget(parent),
m_lang(lang)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    QPalette palette;
    palette.setColor(QPalette::Window,Qt::white);
    setPalette(palette);
    setAutoFillBackground(true);

    m_layout=new QGridLayout();
    this->setLayout(m_layout);
  
    m_nameLabel=new QLabel("滴定剂标定");
    QFont font=m_nameLabel->font();
    font.setPointSize(18);
    m_nameLabel->setFont(font);
    m_prepareLabel=new QLabel("标定准备");
    m_methodLabel=new QLabel("标定方式");
    m_standardLabel=new QLabel("标准物");
    m_otherLabel=new QLabel("其他物质");
    m_cubeLabel=new QLabel("体积");
    m_nongduLabel=new QLabel("浓度");
    m_unitLabel=new QLabel("单位");
    m_ddMethodLabel=new QLabel("滴定方法");

    m_methodCombo=new QComboBox();
    m_methodCombo->addItems({"主动滴定"});
    m_standardCombo=new QComboBox();
    m_standardCombo->addItems({"其他"});
    m_unitCombo=new QComboBox();
    m_unitCombo->addItems({"1"});

    m_otherEdit=new QLineEdit();
    m_otherEdit->setPlaceholderText(QString::fromStdString(loadTranslation(m_lang,"Input")));//"请输入");
    m_cubeEdit=new QLineEdit();
    m_cubeEdit->setPlaceholderText(QString::fromStdString(loadTranslation(m_lang,"Input")));
    m_nongduEdit=new QLineEdit();
    m_nongduEdit->setPlaceholderText(QString::fromStdString(loadTranslation(m_lang,"Input")));
    m_ddMethodEdit=new QLineEdit();
    m_ddMethodEdit->setPlaceholderText(QString::fromStdString(loadTranslation(m_lang,"Input")));

    m_injectBtn=new QPushButton("注液");
    m_backBtn=new QPushButton("回液");
    m_inputBtn=new QPushButton("吸液");
    m_injectWashBtn=new QPushButton("注液清洗");
    m_backWashBtn=new QPushButton("回液清洗");
    m_saveResultBtn=new QPushButton("保存结果");

    m_backLabel=new HGQLabel(false,getPath("/resources/V1/@1xiconPark-return 1.png"));
    m_openLabel=new HGQLabel(false,getPath("/resources/V1/@1xantOutline-folder-open 1.png"));

    connect(m_backLabel,SIGNAL(leftClicked()),this,SLOT(clickBackWindow()));
    connect(m_openLabel,SIGNAL(leftClicked()),this,SLOT(clickOpenMethod()));

    connect(m_injectBtn,SIGNAL(clicked()),this,SLOT(slotInject()));
    connect(m_backBtn,SIGNAL(clicked()),this,SLOT(slotBack()));
    connect(m_inputBtn,SIGNAL(clicked()),this,SLOT(slotInput()));
    connect(m_injectWashBtn,SIGNAL(clicked()),this,SLOT(slotInjectWash()));
    connect(m_backWashBtn,SIGNAL(clicked()),this,SLOT(slotBackWash()));
    connect(m_saveResultBtn,SIGNAL(clicked()),this,SLOT(slotSaveResult()));

    m_layout->setSpacing(10);
    m_layout->addWidget(m_nameLabel,0,0,1,1);
    m_layout->addWidget(m_backLabel,0,20,1,1);
    m_layout->addWidget(m_prepareLabel,1,1,1,1);
    m_layout->addWidget(m_injectBtn,1,2,1,1);
    m_layout->addWidget(m_backBtn,1,3,1,1);
    m_layout->addWidget(m_inputBtn,1,4,1,1);
    m_layout->addWidget(m_injectWashBtn,1,5,1,1);
    m_layout->addWidget(m_backWashBtn,1,6,1,1);

    m_layout->addWidget(m_methodLabel,2,1,1,1);
    m_layout->addWidget(m_methodCombo,2,2,1,2);

    m_layout->addWidget(m_standardLabel,3,1,1,1);
    m_layout->addWidget(m_standardCombo,3,2,1,2);
    m_layout->addWidget(m_otherLabel,3,4,1,1);
    m_layout->addWidget(m_otherEdit,3,5,1,2);

    m_layout->addWidget(m_cubeLabel,5,1,1,1);
    m_layout->addWidget(m_cubeEdit,5,2,1,1);
    m_layout->addWidget(m_nongduLabel,5,3,1,1);
    m_layout->addWidget(m_nongduEdit,5,4,1,1);
    m_layout->addWidget(m_unitLabel,5,5,1,1);
    m_layout->addWidget(m_unitCombo,5,6,1,1);

    m_layout->addWidget(m_ddMethodLabel,6,1,1,1);
    m_layout->addWidget(m_ddMethodEdit,6,2,1,2);
    m_layout->addWidget(m_openLabel,6,4,1,1);

    m_layout->addWidget(m_saveResultBtn,11,7,1,1);
    setUniformSizePolicy(m_layout,QSizePolicy::Expanding,QSizePolicy::Expanding);
}
bool HGDDjiCalibrationWidget::closeWindow(){
    return true;
}

void HGDDjiCalibrationWidget::slotInject(){

}
void HGDDjiCalibrationWidget::slotBack(){

}
void HGDDjiCalibrationWidget::slotInput(){

}
void HGDDjiCalibrationWidget::slotInjectWash(){

}
void HGDDjiCalibrationWidget::slotBackWash(){

}
void HGDDjiCalibrationWidget::slotSaveResult(){

}
void HGDDjiCalibrationWidget::clickBackWindow(){
    this->close();
}
void HGDDjiCalibrationWidget::clickOpenMethod(){

}