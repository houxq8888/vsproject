#ifndef HGCHANNELEDITWIDGET_H
#define HGCHANNELEDITWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QTableWidget>
#include "labelwithimg.h"
#include "hgqlabel.h"
#include <QGridLayout>
#include "common.h"



class HGChannelEditWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGChannelEditWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();
    void updateChannelInfo(Channel,bool coverflag=false);

signals:
    void modulesShow(Channel);
    void editFlag(bool);

private slots:
    void slotLeftClickClose();
    void slotLeftClickEdit();
    void slotLeftClickOk();
    void onCellClicked(int,int);

private:
    void writeDB();
    void fnDisplayChannelInfo(Channel info);

private:
    QGroupBox* m_editGroupBox;
    QGridLayout* m_editlayout, *m_layout;
    HGQLabel* m_closeLabel, *m_editLabel, *m_okLabel;
    QTableWidget* m_tableW;

    std::vector<Channel> m_channels;

    int m_curRow;
    std::string m_lang;
    
};

#endif // HGCHANNELEDITWIDGET_H
