#include "hgmethodinfowidget.h"

HGMethodInfoWidget::HGMethodInfoWidget(std::string lang,QWidget *parent) : QWidget(parent),
m_lang(lang)
{
    m_lightCalibMW=NULL;
    m_dianweiDDMW=NULL;
    m_colorDDMW=NULL;
    m_kulunWaterMW=NULL;
    m_rongliangMW=NULL;
    
    m_layout=new QGridLayout();
    this->setLayout(m_layout);

    // m_toolBox=new QToolBox();
    // connect(m_toolBox,SIGNAL(currentChanged(int)),this,SLOT(togglePage(int)));

    // m_layout->addWidget(m_toolBox,0,0,1,3);
}
bool HGMethodInfoWidget::closeWindow(){
    if (m_lightCalibMW){
        if (m_lightCalibMW->closeWindow()){
            SAFE_DELETE(m_lightCalibMW);
        }
    } 
    if (m_dianweiDDMW){
        if (m_dianweiDDMW->closeWindow()){
            SAFE_DELETE(m_dianweiDDMW);
        }
    } 
    if (m_colorDDMW){
        if (m_colorDDMW->closeWindow()){
            SAFE_DELETE(m_colorDDMW);
        }
    } 
    if (m_kulunWaterMW){
        if (m_kulunWaterMW->closeWindow()){
            SAFE_DELETE(m_kulunWaterMW);
        }
    } 
    if (m_rongliangMW){
        if (m_rongliangMW->closeWindow()){
            SAFE_DELETE(m_rongliangMW);
        }
    }
    return true;
}
// void HGMethodInfoWidget::togglePage(int index){
//     int cnt=m_toolBox->count();
//     for (int i=0;i<cnt;i++){
//         if (i==index) m_toolBox->setItemIcon(i,QIcon("./resources/V1/@1xze-arrow-down 1.png"));
//         else m_toolBox->setItemIcon(i,QIcon("./resources/V1/@1xze-arrow 1.png"));
//     }
// }

void HGMethodInfoWidget::createModule(std::string name,std::string param){
    closeWindow();
    if ("电位滴定"==name){
        m_dianweiDDMW=new HGDianWeiDDWidget(m_lang);
        m_layout->addWidget(m_dianweiDDMW,0,0);
        m_dianweiDDMW->setParam(param);
        // m_toolBox->addItem(m_dianweiDDMW,QIcon("./resources/V1/@1xze-arrow 1.png"),QString::fromStdString(name));
    } else if ("光度法"==name){
        m_lightCalibMW=new HGLightCalibMethodWidget(m_lang);
        m_layout->addWidget(m_lightCalibMW,0,0);
        m_lightCalibMW->setParam(param);
        // m_toolBox->addItem(m_lightCalibMW,QIcon("./resources/V1/@1xze-arrow 1.png"),QString::fromStdString(name));
    } else if ("颜色滴定"==name){
        m_colorDDMW=new HGColorDDWidget(m_lang);
        m_layout->addWidget(m_colorDDMW,0,0);
        m_colorDDMW->setParam(param);
        // m_toolBox->addItem(m_colorDDMW,QIcon("./resources/V1/@1xze-arrow 1.png"),QString::fromStdString(name));
    } else if ("库伦法(水分)"==name){
        m_kulunWaterMW=new HGKulunWaterWidget(m_lang);
        m_layout->addWidget(m_kulunWaterMW,0,0);
        m_kulunWaterMW->setParam(param);
        // m_toolBox->addItem(m_kulunWaterMW,QIcon("./resources/V1/@1xze-arrow 1.png"),QString::fromStdString(name));
    } else if ("容量法(水分)"==name){
        m_rongliangMW=new HGRongliangMethodWidget(m_lang);
        m_layout->addWidget(m_rongliangMW,0,0);
        m_rongliangMW->setParam(param);
        // m_toolBox->addItem(m_rongliangMW,QIcon("./resources/V1/@1xze-arrow 1.png"),QString::fromStdString(name));
    } else if ("温度滴定"==name){
        // m_washW=new HGWashWidget();
        // m_toolBox->addItem(m_washW,QIcon("./resources/V1/@1xze-arrow 1.png"),QString::fromStdString(name));
    }
}
void HGMethodInfoWidget::methodShow(std::string name,std::string param){
    createModule(name,param);
    m_name=name;
}
std::string HGMethodInfoWidget::getParam(){
    std::string name=m_name;
    std::string param;
    if ("电位滴定"==name){
        param=m_dianweiDDMW->getParamStr();
    } else if ("光度法"==name){
        param=m_lightCalibMW->getParamStr();
    } else if ("颜色滴定"==name){
        param=m_colorDDMW->getParamStr();
    } else if ("库伦法(水分)"==name){
        param=m_kulunWaterMW->getParamStr();
    } else if ("容量法(水分)"==name){
        param=m_rongliangMW->getParamStr();
    } else if ("温度滴定"==name){
    }
    return param;
}