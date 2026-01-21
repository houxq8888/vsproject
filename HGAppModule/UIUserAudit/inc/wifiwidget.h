#ifndef WIFIWIDGET_H
#define WIFIWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QRadioButton>
#include "labelwithimg.h"
#include "common.h"


class WifiWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WifiWidget(std::string,WifiNetwork info,QWidget *parent = nullptr);
    ~WifiWidget();

    bool closeWindow();
    void changeStatus();

signals:
   
private:
   void collapse();
   void expand();

private:
    LabelWithImg* m_label;
    QGridLayout* m_layout;
    QLabel* m_wifi_securityLabel;
    QPushButton* m_connectBtn;
    QRadioButton* m_connectRadio;
    bool m_isExpand;
    std::string m_lang;
};


#endif // WIFIWIDGET_H