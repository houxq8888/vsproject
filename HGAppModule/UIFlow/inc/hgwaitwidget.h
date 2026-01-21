#ifndef HGWAITWIDGET_H
#define HGWAITWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QGridLayout>
#include "hgmanipulatewidget.h"

class HGWaitWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGWaitWidget(std::string,std::string name,QWidget *parent = nullptr);
    bool closeWindow();
    std::string getParamStr();
    void setParamStr(std::string);

signals:
    void mainpuateObject(std::string,int);

private:
    QLabel*m_waitTimeLabel;
    QComboBox*m_waitTimeCombo;
    QGridLayout*m_layout;
    HGManipulateWidget* m_manipulateW;
    std::string m_lang;
};

#endif // HGWAITWIDGET_H
