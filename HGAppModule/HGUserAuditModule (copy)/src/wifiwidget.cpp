#include "wifiwidget.h"
#include "common.h"
WifiWidget::WifiWidget(std::string lang,WifiNetwork info,QWidget *parent)
    :QWidget(parent),
    m_label(nullptr),
    m_isExpand(true),
    m_lang(lang)
{
    m_layout=new QGridLayout();
    this->setLayout(m_layout);
    m_label=new LabelWithImg(IMGLEFT,12,getPath("/resources/V1/@1xiconPark-wifi 1.png"),info.ssid);

    if (info.security.find("WPA")!=std::string::npos) {
        m_wifi_securityLabel=new QLabel(QString::fromStdString("安全"));
    } else {
        m_wifi_securityLabel=new QLabel(QString::fromStdString("开放"));
    }
    m_wifi_securityLabel->setStyleSheet("QLabel{color:{222,222,222};font-size:8px;}");

    m_connectBtn=new QPushButton("连接");
    m_connectRadio=new QRadioButton("自动连接");

    connect(m_connectBtn,&QPushButton::clicked,[=](){
        if (m_connectBtn->text()=="连接"){
            m_connectBtn->setText("断开连接");
            m_wifi_securityLabel->setText("已连接,"+m_wifi_securityLabel->text());
            m_connectRadio->hide();
        }  else {
            m_connectBtn->setText("连接");
            m_connectRadio->setChecked(false);
            m_connectRadio->show();
            m_wifi_securityLabel->setText(m_wifi_securityLabel->text().section(',',1,1));
        }
    });
    connect(m_connectRadio,&QRadioButton::clicked,[=](){
        if (m_connectRadio->isChecked()){
            m_connectBtn->setText("断开连接");
            m_wifi_securityLabel->setText("已连接,"+m_wifi_securityLabel->text());
            m_connectRadio->hide();
        }
    });
    m_layout->addWidget(m_label,0,0,1,1);
    m_layout->addWidget(m_wifi_securityLabel,1,1,1,1);
    m_layout->addWidget(m_connectRadio,2,1,1,1);
    m_layout->addWidget(m_connectBtn,3,3,1,1);

    if (m_isExpand) expand();
    else collapse();
}
WifiWidget::~WifiWidget(){}

bool WifiWidget::closeWindow()
{
    return true;
}
void WifiWidget::collapse(){
    m_wifi_securityLabel->hide();
    m_connectRadio->hide();
    m_connectBtn->hide();
}
void WifiWidget::expand(){
    m_wifi_securityLabel->show();
    m_connectBtn->show();
    m_connectRadio->show();     
}
void WifiWidget::changeStatus()
{
    m_isExpand=!m_isExpand;
    if (m_isExpand) expand();
    else collapse();
}