#ifndef HGCHANNELMODULEEDITWIDGET_H
#define HGCHANNELMODULEEDITWIDGET_H

#include <QWidget>
#include <QToolBox>
#include <QPushButton>
#include <QGridLayout>
#include <QComboBox>
#include "hgddmodulewidget.h"
#include "hgphddwidget.h"
#include "hgswirlwidget.h"
#include "hgauxpumpwidget.h"
#include "hgelebalancewidget.h"
#include "common.h"


class HGChannelModuleEditWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGChannelModuleEditWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();
    void createModule(std::string name,std::string param="");
    void showChannel(Channel info);
    void setEditFlag(bool);
    Channel getChannel();

signals:
    void updateChannelInfo(Channel);

private slots:
    void togglePage(int index);
    void slotOk();
    void slotCancel();
    void slotApply();
    void slotManipulateObject(std::string name,int type);

private:
    void fnDisplayDB();
    void fninitChannelData(bool);
    void fnSetChannelData();
    void fnUpdateChannelInfo();

private:
    QToolBox* m_toolBox;
    QPushButton* m_cancelBtn, *m_okBtn,*m_applyBtn;
    QGridLayout* m_layout;
    QComboBox* m_channelCombo;
    std::map<int,HGDDModuleWidget*> m_ddModuleWs;
    std::map<int,HGphDDWidget*> m_phDDWs;
    std::map<int,HGSwirlWidget*> m_swirlWs;
    std::map<int,HGAuxPumpWidget*> m_auxPumpWs;
    std::map<int,HGEleBalanceWidget*> m_eleBalanceWs;

    Channel m_channel;
    std::vector<ModuleOfChannel> m_modules;

    bool m_displayFlag;
    std::string m_lang;
};

#endif // HGCHANNELMODULEEDITWIDGET_H
