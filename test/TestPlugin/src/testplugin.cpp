#include "testplugin.h"
#include "testwidget.h"
#include <QtCore/qplugin.h>
#include <QDebug>
#include <QCoreApplication>
#include <QLibraryInfo>

QT_BEGIN_NAMESPACE

TestPlugin::TestPlugin(QObject *parent)
    : QObject(parent), initialized(false)
{
    qDebug() << "TestPlugin constructor called.";
    qDebug() << "Looking for plugin at:" << QCoreApplication::applicationDirPath();
    qDebug() << "Plugin path:" << QLibraryInfo::location(QLibraryInfo::PluginsPath);
    qDebug() << "QT_PLUGIN_PATH:" << qgetenv("QT_PLUGIN_PATH");

}
bool TestPlugin::isContainer() const
{
    qDebug() << "isContainer called";
    return false;
}
bool TestPlugin::isInitialized() const
{
    qDebug() << "isInitialized called";
    return initialized;
}
QWidget *TestPlugin::createWidget(QWidget *parent)
{
    qDebug() << "createWidget called";
    return new TestWidget(parent);
}
QString TestPlugin::name() const
{
    qDebug() << "name called";
    return QStringLiteral("TestWidget");
}
QString TestPlugin::group() const
{
    qDebug() << "group called";
    return QStringLiteral("Defined Widgets");
}
QString TestPlugin::toolTip() const
{
    qDebug() << "toolTip called";
    return QStringLiteral("Test Widget");
}
QString TestPlugin::whatsThis() const
{
    qDebug() << "whatsThis called";
    return QStringLiteral("Test Widget");
}
QString TestPlugin::includeFile() const
{
    qDebug() << "includeFile called";
    return QStringLiteral("testwidget.h");
}
QIcon TestPlugin::icon() const
{
    qDebug() << "icon called";
    return QIcon("./scanner.png");
}

void TestPlugin::initialize(QDesignerFormEditorInterface *core)
{
    if (initialized)
        return;

    if (!core) {
        qDebug() << "Error: core is null.";
        return;
    }

    initialized = true;
    qDebug() << "TestPlugin initialized.";
}
QString TestPlugin::domXml() const
{
    return QStringLiteral(R"(
<ui language="c++">
    <widget class="TestWidget" name="testWidget">
        <property name="url">
            <url>
                <string>about:blank</string>
            </url>
        </property>
        <property name="geometry">
            <rect>
                <x>0</x>
                <y>0</y>
                <width>300</width>
                <height>200</height>
            </rect>
        </property>
    </widget>
</ui>
)");
}

QT_END_NAMESPACE