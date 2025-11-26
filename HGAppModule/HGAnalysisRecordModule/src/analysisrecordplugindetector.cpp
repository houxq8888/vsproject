#include "analysisrecordplugindetector.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

AnalysisRecordPluginDetector::AnalysisRecordPluginDetector(QObject* parent)
{
    _Plugin_Name="AnalysisRecord";
}
std::string AnalysisRecordPluginDetector::pluginName()
{
    std::string name=_Plugin_Name.toStdString();
    return name;
}
QWidget* AnalysisRecordPluginDetector::getWidget()
{
    analysisRecordW=new HGAnalysisRecordWidget("zh");
    // connect(pluginDetector_widget,SIGNAL(sendMsg2Manager(int)),this,SLOT(recvMsgFromUI(int)));

    QWidget* _widget=analysisRecordW;
    return _widget;
}
void AnalysisRecordPluginDetector::recvMsgFromUI(int value)
{
    emit sendMsg2Manager(_Plugin_Name,"main",QString::number(value));
}
std::map<std::string,std::vector<std::string>> AnalysisRecordPluginDetector::moduleNodes()
{
    std::map<std::string,std::vector<std::string>> nodes;
    nodes["input"].push_back("Trigger");
    nodes["input"].push_back("UCHAR");
    nodes["output"].push_back("Trigger");
    nodes["output"].push_back("UCHAR");
    return nodes;
}

