#include "pluginmanager.h"
#include <QPluginLoader>
#include <QApplication>
#include <QDir>
#include <QDebug>
#include "plugininterface.h"
// 避免byte类型定义冲突 - 重命名Windows byte类型
#define byte unsigned_char_byte
#include "hgcommonutility.h"
#undef byte

PluginManager* PluginManager::m_instance;
using namespace HGMACHINE;

PluginManager::PluginManager(QObject *parent) : QObject(parent)
{

}
PluginManager::~PluginManager()
{
    unloadAllPlugins();
}
//加载所有插件
std::vector<InterfaceInfo> PluginManager::loadAllPlugins()
{
    std::vector<InterfaceInfo> pluginInfos;
    QDir plugindir(qApp->applicationDirPath());
    if (!isFileExist((plugindir.path()+"/plugins").toStdString())) return pluginInfos;
    plugindir.cd("plugins");
    QFileInfoList pluginsInfo=plugindir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    //加载插件
    for (QFileInfo fileinfo : pluginsInfo)
    {
        QString file=fileinfo.absoluteFilePath();
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        if (!(fileinfo.suffix()=="dll"||fileinfo.suffix()=="DLL"||fileinfo.suffix()=="Dll")) continue;
#else
#endif
        InterfaceInfo pluginInfo=loadPlugin(file);
        if (pluginInfo.flag)  pluginInfos.push_back(pluginInfo);
    }
    return pluginInfos;
}
//加载其中某个插件
InterfaceInfo PluginManager::loadPlugin(const QString &filepath)
{
    InterfaceInfo pluginInfo;
    if (!QLibrary::isLibrary(filepath))
        return pluginInfo;
    //加载插件
    QPluginLoader *loader=new QPluginLoader(filepath);
    if (loader->load())
    {
        PluginInterface* plugin=qobject_cast<PluginInterface*>(loader->instance());
        if (plugin)
        {
            // 此处是插件类名，而不是接口类名称
            std::string plugName=qobject_cast<PluginInterface*>(plugin)->pluginName();
            std::map<string,std::vector<string>> moduleNodes=(qobject_cast<PluginInterface*>(plugin))->moduleNodes();
            pluginInfo.pluginName=plugName;
            pluginInfo.pluginPath=filepath.toStdString();
            pluginInfo.moduleNodes=moduleNodes;
            pluginInfo.flag=true;
            m_loaders.insert(filepath,loader);
            m_names.insert(filepath,QString::fromStdString(plugName));
           // qDebug()<<"插件名称:"<<plugin->get_name()<<"插件信息："<<plugin->show_text();
            connect(loader->instance(),SIGNAL(sendMsg2Manager(QString,QString,QString)),this,SLOT(recMsgfromPlugin(QString,QString,QString/*PluginMetaData*/)));
        } else {
            delete loader;
            loader=nullptr;
        }
    } else {
        qDebug()<<"loadPlugin:"<<filepath<<loader->errorString();
    }
    return pluginInfo;
}
//卸载所有插件
void PluginManager::unloadAllPlugins()
{
    for (QString filepath : m_loaders.keys())
        unloadPlugin(filepath);
}
void PluginManager::unloadPlugin(const QString &filepath)
{
    QPluginLoader *loader=m_loaders.value(filepath);
    //卸载插件，并从内部数据结构中移除
    if (loader->unload())
    {
        m_loaders.remove(filepath);
        delete loader;
        loader=nullptr;
    }
}
//获取某个插件名称
QVariant PluginManager::getPluginName(QPluginLoader *loader)
{
    if (loader)
        return m_names.value(m_loaders.key(loader));
    else
        return "";
}
//根据名称获得插件
QPluginLoader *PluginManager::getPlugin(const QString &name)
{
    QString pluginName;
    QHash<QString,QString>::const_iterator it=m_names.constBegin();
    while (it!=m_names.constEnd()) {
        if (name.contains(it.value())) {
            pluginName=it.value();
            break;
        }
        ++it;
    }
    return m_loaders.value(m_names.key(pluginName));
}
void PluginManager::recMsgfromPlugin(QString from,QString dest,QString msg/*PluginMetaData metadata*/)
{
    qDebug()<<"recMsgfromPlugin from "<<from<<",msg="<<msg;
    dest=from;
    msg="change value";
    auto loader=getPlugin(dest);//目标插件
    if (loader)
    {
        auto plugininterface=qobject_cast<PluginInterface*>(loader->instance());
        if (plugininterface)
        {
            plugininterface->recMsgFromManager(from,dest,msg);//转发给对应插件
        }
    }
}
