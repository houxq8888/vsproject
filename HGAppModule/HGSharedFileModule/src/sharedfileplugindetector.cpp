#include "sharedfileplugindetector.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif
#define PluginInterface_iid "org.csm.embed.plugin.pluginInterface.1.0"
Q_DECLARE_INTERFACE(PluginInterface, PluginInterface_iid)

    Q_INTERFACES(PluginInterface)
SharedFilePluginDetector::SharedFilePluginDetector(QObject* parent)
{
    _Plugin_Name="SharedFile";
}
std::string SharedFilePluginDetector::pluginName()
{
    std::string name=_Plugin_Name.toStdString();
    return name;
}
QWidget* SharedFilePluginDetector::getWidget()
{
    sharedFileW=new HGSharedFileWidget("zh");
    // connect(pluginDetector_widget,SIGNAL(sendMsg2Manager(int)),this,SLOT(recvMsgFromUI(int)));

    QWidget* _widget=sharedFileW;
    return _widget;
}
void SharedFilePluginDetector::recvMsgFromUI(int value)
{
    emit sendMsg2Manager(_Plugin_Name,"main",QString::number(value));
}
std::map<std::string,std::vector<std::string>> SharedFilePluginDetector::moduleNodes()
{
    std::map<std::string,std::vector<std::string>> nodes;
    nodes["input"].push_back("Trigger");
    nodes["input"].push_back("UCHAR");
    nodes["output"].push_back("Trigger");
    nodes["output"].push_back("UCHAR");
    return nodes;
}

