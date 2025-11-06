#include "hgmethodmanagewidget.h"
HGMethodManageWidget::HGMethodManageWidget(std::string lang,QWidget *parent) : QWidget(parent),
m_lang(lang)
{
    RWDb::writeAuditTrailLog(loadTranslation(m_lang,"Enter")+loadTranslation(m_lang,"Method"));
    m_methodlistW=NULL;
    m_methodlistW=new HGMethodListWidget(m_lang);

    m_methodEditW=NULL;
    m_methodEditW=new HGMethodEditWidget(m_lang,m_methodlistW->getMethods());

    m_methodInfoW=NULL;
    m_methodInfoW=new HGMethodInfoWidget(m_lang);
    connect(m_methodlistW,SIGNAL(createModule(std::string)),this,SLOT(slotCreateModule(std::string)));
    connect(m_methodEditW,SIGNAL(getMethodLists()),this,SLOT(slotGetMethodList()));
    connect(m_methodEditW,SIGNAL(methodInfoWShow(std::string,std::string)),this,SLOT(slotMethodInfoWShow(std::string,std::string)));
    connect(m_methodEditW,SIGNAL(saveSignal()),this,SLOT(slotSaveMethod()));


    m_versplitter=new QSplitter();
    m_versplitter->setOrientation(Qt::Horizontal);
    m_versplitter->setStyleSheet("QSplitter::handle{background-color:lightgray;}");


    m_versplitter->addWidget(m_methodEditW);
    m_versplitter->addWidget(m_methodlistW);
    m_versplitter->addWidget(m_methodInfoW);

    m_versplitter->setStretchFactor(0, 3);
    m_versplitter->setStretchFactor(1, 1);
    m_versplitter->setStretchFactor(2, 5);
    m_layout=new QGridLayout();
    this->setLayout(m_layout);

    m_layout->addWidget(m_versplitter);
    // m_layout->addWidget(m_vline,0,3,1,1);
    // m_layout->addWidget(m_methodlistW,0,4,1,1);
    // m_layout->addWidget(m_vline1,0,5,1,1);
    // m_layout->addWidget(m_methodInfoW,0,6,1,5);
}

bool HGMethodManageWidget::closeWindow()
{
    if (m_methodEditW){
        if (m_methodEditW->closeWindow()){
            SAFE_DELETE(m_methodEditW);
        }
    }
    if (m_methodlistW){
        if (m_methodlistW->closeWindow()){
            SAFE_DELETE(m_methodlistW);
        }
    }
    if (m_methodInfoW){
        if (m_methodInfoW->closeWindow()){
            SAFE_DELETE(m_methodInfoW);
        }
    }
    return true;
}
HGMethodManageWidget::~HGMethodManageWidget()
{
    
}
void HGMethodManageWidget::slotCreateModule(std::string name){
    m_methodInfoW->createModule(name);
}
void HGMethodManageWidget::slotGetMethodList(){
    std::vector<std::string> methods=m_methodlistW->getMethods();
    m_methodEditW->addMethodType(methods);
}
void HGMethodManageWidget::slotMethodInfoWShow(std::string type,std::string param){
    m_methodInfoW->methodShow(type,param);
    m_methodlistW->lightMethod(type);
}
void HGMethodManageWidget::slotSaveMethod(){
    std::string param=m_methodInfoW->getParam();
    std::ostringstream ss;
    ss<<"param:"<<param;
    HGLog4Cplus::getLogInstance(LOG_PATH)->logout(ss.str(),LOGINFO);
    printf("param:%s\n",param.c_str());
    m_methodEditW->saveParam(param);
}