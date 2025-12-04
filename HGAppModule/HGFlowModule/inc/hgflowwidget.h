#ifndef HGFLOWWIDGET_H
#define HGFLOWWIDGET_H

#include <QWidget>
#include "hgfloweditwidget.h"
#include "hgflowinfowidget.h"
#include "hgflowmodulelistwidget.h"
#include "hgqlabel.h"
#include <QSplitter>
#include <QGridLayout>

class HGFlowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGFlowWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();
    ~HGFlowWidget();

signals:

private slots:
    void slotCreateModule(std::string name);
    void slotSaveFlow();
    void slotFlowShow(FlowOfTask);
    
private:
    QGridLayout* m_layout;
    HGFlowEditWidget* m_flowEditW;
    HGFlowInfoWidget* m_flowInfoW;
    HGFlowModuleListWidget* m_flowModuleListW;
    
    QSplitter *m_versplitter1,*m_versplitter;
    std::string m_lang;
};

#endif // HGFLOWWIDGET_H
