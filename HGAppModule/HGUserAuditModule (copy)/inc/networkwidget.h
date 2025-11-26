#ifndef NETWORKWIDGET_H
#define NETWORKWIDGET_H

#include <QWidget>
#include <QLabel>
#include "labelwithimg.h"
#include "hgqlabel.h"
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include <QSlider>
#include "basewidget.h"
#include <QListWidget>
#include "wifiwidget.h"

typedef struct tagWifiWidgetInfo{
    WifiWidget* wifiW;
    QListWidgetItem* wifiItem;
} WifiWidgetInfo;

class NetworkWidget : public BaseWidget
{
    Q_OBJECT
public:
    explicit NetworkWidget(std::string,QWidget *parent = nullptr);
    ~NetworkWidget();
    bool closeWindow();
    void setLanguage(std::string lang) override;

signals:

private slots:
    void clickEnableWifi();

public slots:
    void clickListItem(QListWidgetItem* item);

private:
    void fnReadDB();
    void fnWriteDB();
    void setControlStatus();
    void fnInitWifiInfo();
    void fnDestroyWifiWidget();
    
private:
    LabelWithImg* m_wifiImg;
    QGridLayout* m_layout;
    HGQLabel* m_wifiLabel;
    bool m_isEnableWifi;

    QListWidget* m_listWidget;
    std::map<std::string,WifiWidgetInfo> m_wifiWs;
    std::string m_lang;
};

#endif // NETWORKWIDGET_H
