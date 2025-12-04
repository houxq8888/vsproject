#ifndef TESTPLUGIN_H
#define TESTPLUGIN_H

#include <QtUiPlugin/customwidget.h>

QT_BEGIN_NAMESPACE

class TestPlugin : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetInterface" FILE "testplugin.json")
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    explicit TestPlugin(QObject *parent = nullptr);

    // 必须实现的接口函数
    QString name() const override;
    QString group() const override;
    QString toolTip() const override;
    QString whatsThis() const override;
    QString includeFile() const override;
    QIcon icon() const;
    bool isContainer() const override;
    QWidget *createWidget(QWidget *parent) override;
    bool isInitialized() const override;
    void initialize(QDesignerFormEditorInterface *core) override;
    QString domXml() const override;
    
private:
    bool initialized;
};

QT_END_NAMESPACE
#endif // TESTPLUGIN_H