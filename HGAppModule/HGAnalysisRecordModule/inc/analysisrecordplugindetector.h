#ifndef ANALYSISRECORDPLUGINDETECTOR_H
#define ANALYSISRECORDPLUGINDETECTOR_H


#include <QObject>
#include "plugininterface.h"
#include <QWidget>
#include "hganalysisrecordwidget.h"
#include <QDebug>

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#else
#pragma execution_character_set("utf-8")
#endif

#define PluginInterface_iid "org.csm.embed.plugin.pluginInterface.1.0"
Q_DECLARE_INTERFACE(PluginInterface, PluginInterface_iid)

    Q_INTERFACES(PluginInterface)
    
class AnalysisRecordPluginDetector: public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.csm.embed.plugin.pluginInterface.1.0")
    Q_INTERFACES(PluginInterface)

public:
    explicit AnalysisRecordPluginDetector(QObject* parent=nullptr);
    std::map<std::string,std::vector<std::string>> moduleNodes();
    std::string pluginName();
    QWidget * getWidget();
    virtual void recMsgFromManager(QString from,QString dest,QString msg) Q_DECL_OVERRIDE {
        qDebug()<<"插件接收消息："<<msg;
    }

signals:
    void sendMsg2Manager(QString,QString,QString) Q_DECL_OVERRIDE;

private slots:
    void recvMsgFromUI(int);
private:
    HGAnalysisRecordWidget* analysisRecordW;

private:

};

#endif // ANALYSISRECORDPLUGINDETECTOR_H