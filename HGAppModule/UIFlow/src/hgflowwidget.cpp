#include "hgflowwidget.h"

HGFlowWidget::HGFlowWidget(std::string lang,QWidget *parent) : QWidget(parent),
m_lang(lang)
{
    RWDb::writeAuditTrailLog(loadTranslation(m_lang,"Enter")+loadTranslation(m_lang,"Flow"));
    m_flowEditW=NULL;
    m_flowEditW=new HGFlowEditWidget(m_lang);
    m_flowModuleListW=NULL;
    m_flowModuleListW=new HGFlowModuleListWidget(m_lang);
    m_flowInfoW=NULL;
    m_flowInfoW=new HGFlowInfoWidget(m_lang);
    qRegisterMetaType<FlowOfTask>("FlowOfTask");
    connect(m_flowModuleListW,SIGNAL(createModule(std::string)),this,SLOT(slotCreateModule(std::string)));
    connect(m_flowEditW,SIGNAL(saveSignal()),this,SLOT(slotSaveFlow()));
    connect(m_flowEditW,SIGNAL(flowInfoWShow(FlowOfTask)),this,SLOT(slotFlowShow(FlowOfTask)));

    m_versplitter=new QSplitter();
    m_versplitter->setOrientation(Qt::Horizontal);
    m_versplitter->setStyleSheet("QSplitter::handle{background-color:lightgray;}");

    m_versplitter->addWidget(m_flowEditW);
    m_versplitter->addWidget(m_flowModuleListW);
    m_versplitter->addWidget(m_flowInfoW);

    m_versplitter->setStretchFactor(0, 3);
    m_versplitter->setStretchFactor(1, 1);
    m_versplitter->setStretchFactor(2, 5);
    m_layout=new QGridLayout();
    this->setLayout(m_layout);

    m_layout->addWidget(m_versplitter);
}
bool HGFlowWidget::closeWindow()
{
    if (m_flowEditW){
        if (m_flowEditW->closeWindow()){
            SAFE_DELETE(m_flowEditW);
        }
    }
    if (m_flowModuleListW){
        if (m_flowModuleListW->closeWindow()){
            SAFE_DELETE(m_flowModuleListW);
        }
    }
    if (m_flowInfoW){
        if (m_flowInfoW->closeWindow()){
            SAFE_DELETE(m_flowInfoW);
        }
    }
    return true;
}
HGFlowWidget::~HGFlowWidget()
{
    
}
void HGFlowWidget::slotCreateModule(std::string name){
    m_flowInfoW->createModule(name);
}
void HGFlowWidget::slotSaveFlow(){
    std::vector<StepOfFlow> steps=m_flowInfoW->getStepOfFlow();
    m_flowEditW->saveSteps(steps);
}
void HGFlowWidget::slotFlowShow(FlowOfTask info){
    m_flowInfoW->showFlow(info);
}