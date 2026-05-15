#include "hgjiarebanwidget.h"
#include "common.h"
#include <sstream>
#include "HGOnlinePlatformInterface.h"


HGJiarebanWidget::HGJiarebanWidget(std::string lang,std::string name,QWidget *parent) : QWidget(parent),
m_lang(lang)
{
    m_layout=new QGridLayout();
    this->setLayout(m_layout);
    m_name=name;
    m_nameLabel=new QLabel(QString::fromStdString(name));
    QFont font=m_nameLabel->font();
    font.setPointSize(18);
    m_nameLabel->setFont(font);
      
    double currentTemp = 32.9;
    int heatingTemp = 160;
    QString fanStatus = "关";
    QString strstatus = QString("当前温度：%1 °C\n"
        "加热温度：%2 °C\n"
        "风扇状态：%3\n")
        .arg(currentTemp, 0, 'f', 1)  // 保留1位小数
        .arg(heatingTemp)
        .arg(fanStatus);
    m_dataInfoLabel=new QLabel(strstatus);

    m_tempInfoGroup=new QGroupBox("温度");
    m_fanInfoGroup=new QGroupBox("风扇");
    m_tempInfoLayout=new QGridLayout();
    m_fanInfoLayout=new QGridLayout();
    m_tempInfoGroup->setLayout(m_tempInfoLayout);
    m_fanInfoGroup->setLayout(m_fanInfoLayout);
       
    m_startBtn=new QPushButton("开始加热");
    m_stopBtn=new QPushButton("停止加热");
    m_onBtn=new QPushButton("开");
    m_offBtn=new QPushButton("关");

    connect(m_startBtn,SIGNAL(clicked()),this,SLOT(slotStartTempClick()));
    connect(m_stopBtn,SIGNAL(clicked()),this,SLOT(slotStopTempClick()));
    connect(m_onBtn,SIGNAL(clicked()),this,SLOT(slotOnClick()));
    connect(m_offBtn,SIGNAL(clicked()),this,SLOT(slotOffClick()));

    m_tempInfoLayout->addWidget(m_startBtn,0,0);
    m_tempInfoLayout->addWidget(m_stopBtn,0,1);

    m_fanInfoLayout->addWidget(m_onBtn,0,0);
    m_fanInfoLayout->addWidget(m_offBtn,0,1);

    m_layout->addWidget(m_nameLabel,0,0);
    m_layout->addWidget(m_tempInfoGroup,1,0);
    m_layout->addWidget(m_fanInfoGroup,2,0);
    m_layout->addWidget(m_dataInfoLabel,3,0);

    m_timer=new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(slotUpdateDataInfo()));
    m_timer->start(100);
}
bool HGJiarebanWidget::closeWindow(){
    return true;
}

std::string HGJiarebanWidget::getParamStr(){
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

void HGJiarebanWidget::setParamStr(std::string param){
    // setControlText(m_tempInfoLayout,param);
    // setControlText(m_fanInfoLayout,param);
}
void HGJiarebanWidget::slotStartTempClick(){
    setHeatOn();
}
void HGJiarebanWidget::slotStopTempClick(){
    setHeatOff();
}
void HGJiarebanWidget::slotOnClick(){
    setFanOn();
}
void HGJiarebanWidget::slotOffClick(){
    setFanOff();
}
void HGJiarebanWidget::slotUpdateDataInfo(){
    setRequestData();
    std::map<std::string,std::string> values = getCAS6RawValues();
    if (values.size() < 3) return;
    double currentTemp = values.find("当前温度")!=values.end()?std::atof(values["当前温度"].c_str()):0.0;
    double heatingTemp = values.find("加热温度")!=values.end()?std::atof(values["加热温度"].c_str()):0.0;
    std::istringstream iss(values.find("风扇状态")!=values.end()?values["风扇状态"]:"0 0");
    std::string first="", second="";
    iss >> first >> second;

    QString fanStatus = (first == "1" ? "开" : "关");
    QString strstatus = QString("当前温度：%1 °C\n"
        "加热温度：%2 °C\n"
        "风扇状态：%3\n")
        .arg(currentTemp, 0, 'f', 1)  // 保留1位小数
        .arg(heatingTemp)
        .arg(fanStatus);
    m_dataInfoLabel->setText(strstatus);
}