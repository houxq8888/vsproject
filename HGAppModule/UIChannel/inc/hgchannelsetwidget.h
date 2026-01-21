#ifndef HGCHANNELSETWIDGET_H
#define HGCHANNELSETWIDGET_H



#include <QWidget>
#include "hgqlabel.h"
#include <QGroupBox>
#include <QGridLayout>
#include <QFrame>
#include "hgchanneleditwidget.h"
#include "hgchannelmoduleeditwidget.h"
#include "hgmodulelistwidget.h"
#include <QSplitter>

class HGChannelSetWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGChannelSetWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();
    ~HGChannelSetWidget();

signals:

private slots:
    void slotCreateModule(std::string);
    void slotUpdateChannelInfo(Channel);
    void slotModulesShow(Channel);
    void slotEditFlag(bool);

private:
    HGQLabel *m_closelabel,*m_editlabel,*m_oklabel;
    QGroupBox* m_channelGroup;
    QGridLayout* m_channelLayout;
    QSplitter *m_versplitter1,*m_versplitter;
    HGChannelEditWidget* m_channelEditW;
    HGChannelModuleEditWidget* m_channelModuleEditW;
    HGModuleListWidget* m_moduleListW;
    std::string m_lang;


};


#endif // HGCHANNELSETWIDGET_H
