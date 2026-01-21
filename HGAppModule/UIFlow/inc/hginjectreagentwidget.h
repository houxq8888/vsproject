#ifndef HGINJECTREAGENTWIDGET_H
#define HGINJECTREAGENTWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include "hgmanipulatewidget.h"
#include "common.h"

class HGinjectReagentWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGinjectReagentWidget(std::string,std::string param,std::string name,QWidget *parent = nullptr);
    bool closeWindow();
    std::string getParamStr();
    void setParamStr(std::string);

signals:
    void mainpuateObject(std::string,int);
private:
    void setMode();
private:
    QLabel* m_zhurumoshiLabel,*m_zhurushijiLabel,*m_zhurutijiLabel,*m_zhurusulvLabel;
    QComboBox* m_zhurumoshiCombo,*m_zhurushijiCombo;
    QLineEdit* m_zhurutijiEdit,*m_zhurusulvEdit;
    QGridLayout *m_layout;
    HGManipulateWidget* m_manipulateW;
    std::string m_param;
    std::string m_lang;
};

#endif // HGINJECTREAGENTWIDGET_H
