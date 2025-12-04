#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>
#include <string>
#include <QPluginLoader>

using namespace std;

class PluginInterface;

struct InterfaceInfo {
    std::string pluginName;//插件名称
    std::string pluginPath;//插件路径
    PluginInterface* interfacePtr;//保存接口指针
    std::map<string,std::vector<string>> moduleNodes; // 接口输入输出
    bool flag;
    InterfaceInfo() {
        flag=false;
    }
    //QStringList interfaceList;//保存运行结果
};


class PluginManager : public QObject
{
    Q_OBJECT
public:
    static PluginManager *instance()
    {
        if (m_instance==nullptr)
            m_instance=new PluginManager();
        return m_instance;
    }
    std::vector<InterfaceInfo> loadAllPlugins();
    InterfaceInfo loadPlugin(const QString& filepath);
    void unloadPlugin(const QString& filepath);
    void unloadAllPlugins();
    QPluginLoader* getPlugin(const QString &name);
    QVariant getPluginName(QPluginLoader *loader);

public slots:
    void recMsgfromPlugin(QString,QString,QString/*PluginMetaData metadata*/);

private:
    explicit PluginManager(QObject *parent = nullptr);
    ~PluginManager();

    QHash<QString,QPluginLoader*> m_loaders;//插件路径-QPluginLoader实例
    QHash<QString,QString> m_names;//插件路径--插件名称

    static PluginManager *m_instance;
    class GarbageCollector
    {
        ~GarbageCollector()
        {
            if (PluginManager::instance())
            {
                delete PluginManager::instance();
                PluginManager::m_instance=nullptr;
            }
        }
    };
    static GarbageCollector gc;
};

#endif // PLUGINMANAGER_H
