#include "hgmodulelistwidget.h"

HGModuleListWidget::HGModuleListWidget(std::string lang,QWidget *parent) : QWidget(parent),
    m_lang(lang)
{
    std::unordered_map<std::string,std::vector<std::string>> moduleNames;
    std::list<std::string> insertionOrder;
    moduleNames["检测器"].push_back("pH电极");
    moduleNames["检测器"].push_back("颜色传感器");
    moduleNames["检测器"].push_back("光度计");
    moduleNames["检测器"].push_back("指标电极(容量法)");
    insertionOrder.push_back("检测器");
    moduleNames["馈液单元"].push_back("滴定组件(20ml)");
    moduleNames["馈液单元"].push_back("滴定组件(50ml)");
    insertionOrder.push_back("馈液单元");
    moduleNames["辅助泵"].push_back("辅助泵");
    insertionOrder.push_back("辅助泵");
    moduleNames["搅拌器"].push_back("搅拌器(无加热)");
    moduleNames["搅拌器"].push_back("搅拌器(带加热)");
    insertionOrder.push_back("搅拌器");
    moduleNames["外接设备"].push_back("电子天平");
    insertionOrder.push_back("外接设备");
    // moduleNames["扩展接口"].push_back("ModBus");
    // insertionOrder.push_back("扩展接口");

    for (std::string key: insertionOrder){
        m_modules[key].groupBox=new QGroupBox(QString::fromStdString(key));
        m_modules[key].layout=new QVBoxLayout();
        m_modules[key].groupBox->setLayout(m_modules[key].layout);
    }
    
    for (std::string key: insertionOrder){
        for (int i=0;i<int(moduleNames[key].size());i++){
            m_modules[key].labels.push_back(new LabelWithImg(NOIMG,12,"",moduleNames[key][i]));
            m_modules[key].labels[m_modules[key].labels.size()-1]->setStyleSheet("border: 2px solid black;padding:10px;");
            m_modules[key].layout->addWidget(m_modules[key].labels[m_modules[key].labels.size()-1]);
            connect(m_modules[key].labels[m_modules[key].labels.size()-1],
                SIGNAL(createModule(std::string)),this,SLOT(slotCreateModule(std::string)));
            connect(m_modules[key].labels[m_modules[key].labels.size()-1],
                SIGNAL(clickLeftName(std::string)),this,SLOT(slotLeftClickLabel(std::string)));
            m_modules[key].labels[m_modules[key].labels.size()-1]->setRectPainter(true);
        }
    }

    
    m_contentWidget= new QWidget();
    // m_contentWidget->setFixedWidth(this->width()/4);
    m_layout=new QVBoxLayout();
    m_contentWidget->setLayout(m_layout);

    for (std::string key: insertionOrder){
        m_layout->addWidget(m_modules[key].groupBox);
    }


    m_scrollArea = new QScrollArea();
    m_scrollArea->setWidgetResizable(true); // 自动调整内容大小
    m_scrollArea->setWidget(m_contentWidget); // 设置滚动区域的内容

    m_mainLayout=new QVBoxLayout();
    m_mainLayout->addWidget(m_scrollArea);
    this->setLayout(m_mainLayout);
}
bool HGModuleListWidget::closeWindow()
{
    return true;
}
void HGModuleListWidget::slotCreateModule(std::string name){
    emit createModule(name);
}
void HGModuleListWidget::setAllFalse(){
    for (auto module:m_modules){
        std::string rootName=module.first;
        HGModules hgmodules=module.second;
        int index=0;
        for (auto childmodule : hgmodules.labels){
            m_modules[rootName].labels[index]->setFillFlag(false);
            index++;
        }
    }
}
void HGModuleListWidget::slotLeftClickLabel(std::string name){
    setAllFalse();
    for (auto module:m_modules){
        std::string rootName=module.first;
        HGModules hgmodules=module.second;
        int index=0;
        for (auto childmodule : hgmodules.labels){
            if (childmodule->getTextLabel()->text().toStdString()==name){
                m_modules[rootName].labels[index]->setFillFlag(true);
            }
            index++;
        }
    }
}