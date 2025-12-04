// mainwindow.cpp
#include "mainwindow.h"
#include <QPluginLoader>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QWidget>
#include <QFileDialog>
#include "common.h"
#include "plugininterface.h"

MainWindow::MainWindow(const std::string& lang,QWidget *parent)
    : QWidget(parent),
    m_lang(lang)
{
    setWindowTitle("插件加载示例");
    m_pluginManager = PluginManager::instance();
    std::vector<InterfaceInfo> pluginInfos = m_pluginManager->loadAllPlugins();

    __qtreewidget = new QTreeWidget();
    __qtreewidgetitem = new QTreeWidgetItem();
    __qtreewidgetitem->setText(0, QString::fromUtf8("模块组件"));
    __qtreewidget->setHeaderItem(__qtreewidgetitem);
    
    QVBoxLayout *layout = new QVBoxLayout(this);
    m_pluginArea=new QWidget();
    m_pluginlayout=new QGridLayout(m_pluginArea);

    // 插件显示区域
    layout->addWidget(__qtreewidget);
    layout->addWidget(m_pluginArea);
    // 加载插件按钮
    QPushButton *loadPluginButton = new QPushButton("加载插件", this);
    layout->addWidget(loadPluginButton);
    connect(__qtreewidget, &QTreeWidget::itemClicked, this, &MainWindow::onItemClicked);
    
    for (const InterfaceInfo& info : pluginInfos) {
        QTreeWidgetItem* item = new QTreeWidgetItem(__qtreewidget);
        std::string pluginName = loadTranslation(m_lang,info.pluginName);
        item->setText(0, QString::fromStdString(pluginName));
    }
}

MainWindow::~MainWindow()
{
}

void MainWindow::onItemClicked(QTreeWidgetItem *item, int column)
{
    // 获取被点击项的名称（文本）
    QString itemName = item->text(0);
    loadPlugin(itemName);
}

void MainWindow::loadPlugin(const QString& name)
{
    removeWidgetsFromLayout(m_pluginlayout);
    QString pluginName = QString::fromStdString(findTranslationKey(m_lang,name.toStdString()));
    auto loader = m_pluginManager->getPlugin(pluginName);
    if (loader){
        QObject* plugin = loader->instance();
        if (plugin){
            QWidget* widget=(qobject_cast<PluginInterface*>(plugin)->getWidget());
            if (widget==NULL) qDebug()<<"plugin widget is null";
            else {
                widget->setObjectName(pluginName);
                m_pluginlayout->addWidget(widget);
            }
        }
    }
}
