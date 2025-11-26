#ifndef SCANNERWIDGETPLUGIN_H
#define SCANNERWIDGETPLUGIN_H

#include <QtUiPlugin/customwidget.h>

QT_BEGIN_NAMESPACE

class ScannerWidgetPlugin : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetInterface" FILE "scannerwidgetplugin.json")
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    explicit ScannerWidgetPlugin(QObject *parent = nullptr);

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

#endif // SCANNERWIDGETPLUGIN_H