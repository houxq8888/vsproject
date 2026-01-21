#include "hgchannelsetwidget.h"
#include "common.h"


HGChannelSetWidget::HGChannelSetWidget(std::string lang,QWidget *parent) : QWidget(parent),
m_lang(lang)
{
    RWDb::writeAuditTrailLog(loadTranslation(m_lang,"Enter")+loadTranslation(m_lang,"ChannelSet"));
    m_channelEditW=NULL;
    m_channelModuleEditW=NULL;
    m_moduleListW=NULL;
    m_channelEditW=new HGChannelEditWidget(m_lang);
    m_channelModuleEditW=new HGChannelModuleEditWidget(m_lang);
    m_moduleListW=new HGModuleListWidget(m_lang);
    qRegisterMetaType<Channel>("Channel");
    connect(m_moduleListW,SIGNAL(createModule(std::string)),this,SLOT(slotCreateModule(std::string)));
    connect(m_channelModuleEditW,SIGNAL(updateChannelInfo(Channel)),this,SLOT(slotUpdateChannelInfo(Channel)));
    connect(m_channelEditW,SIGNAL(modulesShow(Channel)),this,SLOT(slotModulesShow(Channel)));
    connect(m_channelEditW,SIGNAL(editFlag(bool)),this,SLOT(slotEditFlag(bool)));

    m_versplitter=new QSplitter();
    m_versplitter->setOrientation(Qt::Horizontal);
    m_versplitter->setStyleSheet("QSplitter::handle{background-color:lightgray;}");


    m_versplitter->addWidget(m_channelEditW);
    m_versplitter->addWidget(m_moduleListW);
    m_versplitter->addWidget(m_channelModuleEditW);

    m_versplitter->setStretchFactor(0, 3);
    m_versplitter->setStretchFactor(1, 1);
    m_versplitter->setStretchFactor(2, 3);

    m_channelLayout=new QGridLayout();
    this->setLayout(m_channelLayout);

    m_channelLayout->addWidget(m_versplitter);
}
bool HGChannelSetWidget::closeWindow()
{
    if (m_channelEditW){
        if (m_channelEditW->closeWindow()){
            SAFE_DELETE(m_channelEditW);
        }
    }
    if (m_moduleListW){
        if (m_moduleListW->closeWindow()){
            SAFE_DELETE(m_moduleListW);
        }
    }
    if (m_channelModuleEditW){
        if (m_channelModuleEditW->closeWindow()){
            SAFE_DELETE(m_channelModuleEditW);
        }
    }
    return true;
}
HGChannelSetWidget::~HGChannelSetWidget()
{
    
}
void HGChannelSetWidget::slotCreateModule(std::string name){
    m_channelModuleEditW->createModule(name);
}
void HGChannelSetWidget::slotUpdateChannelInfo(Channel info){
    m_channelEditW->updateChannelInfo(info);
}
void HGChannelSetWidget::slotModulesShow(Channel info){
    m_channelModuleEditW->showChannel(info);
}
void HGChannelSetWidget::slotEditFlag(bool flag){
    m_channelModuleEditW->setEditFlag(flag);
    if (!flag){
        Channel info =m_channelModuleEditW->getChannel();
        m_channelEditW->updateChannelInfo(info,true);
    }
}