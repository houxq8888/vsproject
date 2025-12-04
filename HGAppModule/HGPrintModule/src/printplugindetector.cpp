#include "printplugindetector.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

#define PluginInterface_iid "org.csm.embed.plugin.pluginInterface.1.0"
Q_DECLARE_INTERFACE(PluginInterface, PluginInterface_iid)

    Q_INTERFACES(PluginInterface)
    
PrintPluginDetector::PrintPluginDetector(QObject* parent)
{
    _Plugin_Name="Print";
}
std::string PrintPluginDetector::pluginName()
{
    std::string name=_Plugin_Name.toStdString();
    return name;
}
QWidget* PrintPluginDetector::getWidget()
{
    std::vector<std::string> printNames={"PrintName","PrintData","PrintTime","PrintUser","PrintIP","PrintMac","PrintModel",\
            "PrintSerialNumber","PrintSoftwareVersion","PrintHardwareVersion","PrintStatus","PrintResult","PrintRemark","PrintSignature",\
            "PrintSignatureTime","PrintSignatureUser","PrintSignatureIP","PrintSignatureMac","PrintSignatureModel","PrintSignatureSerialNumber",\
            "PrintSignatureSoftwareVersion","PrintHardwareVersion","PrintSignatureHardwareVersion","PrintSignatureStatus"};
    
    printW=new HGPrintWidget("zh","Report",printNames);
    // connect(pluginDetector_widget,SIGNAL(sendMsg2Manager(int)),this,SLOT(recvMsgFromUI(int)));

    QWidget* _widget=printW;
    return _widget;
}
void PrintPluginDetector::recvMsgFromUI(int value)
{
    emit sendMsg2Manager(_Plugin_Name,"main",QString::number(value));
}
std::map<std::string,std::vector<std::string>> PrintPluginDetector::moduleNodes()
{
    std::map<std::string,std::vector<std::string>> nodes;
    nodes["input"].push_back("Trigger");
    nodes["input"].push_back("UCHAR");
    nodes["output"].push_back("Trigger");
    nodes["output"].push_back("UCHAR");
    return nodes;
}

