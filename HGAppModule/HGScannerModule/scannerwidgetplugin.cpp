#include "scannerwidgetplugin.h"
#include "scannerwidget.h"
#include <QtCore/qplugin.h>
#include <QDebug>
#include <QCoreApplication>
#include <QLibraryInfo>

QT_BEGIN_NAMESPACE

ScannerWidgetPlugin::ScannerWidgetPlugin(QObject *parent)
    : QObject(parent), initialized(false)
{
    qDebug() << "ScannerWidgetPlugin constructor called.";
    qDebug() << "Looking for plugin at:" << QCoreApplication::applicationDirPath();
    qDebug() << "Plugin path:" << QLibraryInfo::location(QLibraryInfo::PluginsPath);
    qDebug() << "QT_PLUGIN_PATH:" << qgetenv("QT_PLUGIN_PATH");

}
bool ScannerWidgetPlugin::isContainer() const
{
    qDebug() << "isContainer called";
    return false;
}
bool ScannerWidgetPlugin::isInitialized() const
{
    qDebug() << "isInitialized called";
    return initialized;
}
QWidget *ScannerWidgetPlugin::createWidget(QWidget *parent)
{
    qDebug() << "createWidget called";
    return new ScannerWidget("zh",parent);
}
QString ScannerWidgetPlugin::name() const
{
    qDebug() << "name called";
    return QStringLiteral("ScannerWidget");
}
QString ScannerWidgetPlugin::group() const
{
    qDebug() << "group called";
    return QStringLiteral("Defined Widgets");
}
QString ScannerWidgetPlugin::toolTip() const
{
    qDebug() << "toolTip called";
    return QStringLiteral("Scanner Widget");
}
QString ScannerWidgetPlugin::whatsThis() const
{
    qDebug() << "whatsThis called";
    return QStringLiteral("Scanner Widget");
}
QString ScannerWidgetPlugin::includeFile() const
{
    qDebug() << "includeFile called";
    return QStringLiteral("scannerwidget.h");
}
QIcon ScannerWidgetPlugin::icon() const
{
    qDebug() << "icon called";
    return QIcon("./scanner.png");
}

void ScannerWidgetPlugin::initialize(QDesignerFormEditorInterface *core)
{
    if (initialized)
        return;

    if (!core) {
        qDebug() << "Error: core is null.";
        return;
    }

    initialized = true;
    qDebug() << "ScannerWidgetPlugin initialized.";
}

QString ScannerWidgetPlugin::domXml() const
{
    return QStringLiteral(R"(
<ui language="c++">
    <widget class="ScannerWidget" name="scannerWidget">
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