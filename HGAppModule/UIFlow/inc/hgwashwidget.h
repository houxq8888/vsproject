#ifndef HGWASHWIDGET_H
#define HGWASHWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include "hgmanipulatewidget.h"
#include "common.h"

class HGWashWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGWashWidget(std::string,std::string name,QWidget *parent = nullptr);
    bool closeWindow();
    std::string getParamStr();
    void setParamStr(std::string);

signals:
    void mainpuateObject(std::string,int);

private:
    QLabel* m_paiyemoshiLabel,*m_zhurushijiLabel,*m_zhurutijiLabel,*m_loopCntLabel;
    QComboBox* m_paiyemoshiCombo,*m_zhurushijiCombo,*m_loopCntCombo;
    QLineEdit* m_zhurutijiEdit;
    QGridLayout *m_layout;
    HGManipulateWidget* m_manipulateW;
    std::string m_lang;
};

#endif // HGWASHWIDGET_H
