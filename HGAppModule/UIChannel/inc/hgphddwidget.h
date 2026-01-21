#ifndef HGPHDDWIDGET_H
#define HGPHDDWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include "hgmanipulatewidget.h"

class HGphDDWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGphDDWidget(std::string,std::string name,QWidget *parent = nullptr);
    bool closeWindow();
    void setParamStr(std::string);
    std::string getParamStr();
    void setReadOnly(bool);

signals:
    void mainpuateObject(std::string,int);

private:
    QGridLayout* m_layout;
    QLabel* m_detectNameLabel;
    QComboBox* m_detectCombo;
    HGManipulateWidget* m_manipulateW;
    std::string m_lang;
};

#endif // HGPHDDWIDGET_H
