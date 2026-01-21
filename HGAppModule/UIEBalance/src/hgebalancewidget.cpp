#include "hgebalancewidget.h"


HGEBalanceWidget::HGEBalanceWidget(std::string name,QWidget *parent)
    : QWidget(parent)
{
    m_lang = name; 
    m_layout=new QGridLayout();
    this->setLayout(m_layout);
    m_setLayout=new QGridLayout();
    m_paramLayout=new QGridLayout();
    m_setGroup=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"Set")));
    m_paramGroup=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"Param")));
    m_setGroup->setLayout(m_setLayout);
    m_paramGroup->setLayout(m_paramLayout);
    m_typeLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"Brand")));
    m_typeComboBox=new QComboBox();
    m_typeComboBox->addItems({"Mettler"});
    m_interfaceLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"Interface")));
    m_interfaceComboBox=new QComboBox();
    m_interfaceComboBox->addItems({"COM","USB","LAN"});
    m_interfaceComboBox->setCurrentIndex(0);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    connect(m_interfaceComboBox,&QComboBox::currentIndexChanged,this,&HGEBalanceWidget::fnChangeParam);
#else
    connect(m_interfaceComboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this, &HGEBalanceWidget::fnChangeParam);
#endif
    fnChangeParam();

    m_setLayout->addWidget(m_typeLabel,0,0);
    m_setLayout->addWidget(m_typeComboBox,0,1);
    m_setLayout->addWidget(m_interfaceLabel,1,0);
    m_setLayout->addWidget(m_interfaceComboBox,1,1);
    m_layout->addWidget(m_setGroup,0,0);
    m_layout->addWidget(m_paramGroup,1,0);
}
HGEBalanceWidget::~HGEBalanceWidget(){

}
bool HGEBalanceWidget::closeWindow(){
    return true;
}
void HGEBalanceWidget::fnChangeParam(){
    removeWidgetsFromLayout(m_paramLayout);
    switch (m_interfaceComboBox->currentIndex()){
        case 0:{
            HGUartWidget *uartWidget = new HGUartWidget(m_lang);
            m_paramLayout->addWidget(uartWidget,0,0);
            break;
        }
    }
}
bool HGEBalanceWidget::eventFilter(QObject* obj,QEvent* event){
    return QObject::eventFilter(obj,event);
}