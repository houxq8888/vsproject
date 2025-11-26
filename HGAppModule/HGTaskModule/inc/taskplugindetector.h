#ifndef TASKPLUGINDETECTOR_H
#define TASKPLUGINDETECTOR_H


#include <QObject>
#include "plugininterface.h"
#include <QWidget>
#include "hgtaskmanagewidget.h"
#include <QDebug>

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#else
#pragma execution_character_set("utf-8")
#endif

class TaskPluginDetector: public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.csm.embed.plugin.pluginInterface.1.0")


public:
    explicit TaskPluginDetector(QObject* parent=nullptr);
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
    HGTaskManageWidget* taskManageW;

private:

};

#endif // TASKPLUGINDETECTOR_H