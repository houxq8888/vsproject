/*
**
**	版权:	
**
**	名称:	plugininterface.h
**
**	作者:	侯 晓 琴
**
**	日期:	2023/08/23
**
**	描述:
**
**	功能:	插件接口类
**
**	备注:
**
*/
#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QString>
#include <QtPlugin>
#include <QJsonObject>


/*struct PluginMetaData
{
    QString from;//消息来源
    QString dest;//消息目的地
    QString msg;

    QObject* object=nullptr;
    QJsonObject info=QJsonObject();
};
Q_DECLARE_METATYPE(PluginMetaData);//确保类型可以通过信号槽传递
*/
// 定义一个接口集（只有纯虚函数的类）
class PluginInterface : public QObject
{
    Q_OBJECT
public:
    virtual ~PluginInterface() {} // 避免 编译器抱怨
    virtual std::map<std::string,std::vector<std::string>> moduleNodes()=0; // 给出输入输出
    virtual std::string pluginName()=0; // 插件名称
    virtual QWidget* getWidget()=0; // 获取UI
    virtual void recMsgFromManager(QString from,QString dest,QString msg/*PluginMetaData*/)=0;//接收到来自创建管理器的消息
    virtual void sendMsg2Manager(QString,QString,QString/*PluginMetaData*/)=0;//给插件管理器发消息
public:
    QString _Plugin_Name;
};

// #define PluginInterface_iid "org.csm.embed.plugin.pluginInterface.1.0"
// Q_DECLARE_INTERFACE(PluginInterface, PluginInterface_iid)


#endif // PLUGININTERFACE_H
