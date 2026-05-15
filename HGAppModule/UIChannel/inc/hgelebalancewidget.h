#ifndef HGELEBALANCEWIDGET_H
#define HGELEBALANCEWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include "hgmanipulatewidget.h"

class HGEleBalanceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGEleBalanceWidget(std::string,std::string name,QWidget *parent = nullptr);
    bool closeWindow();
    void setParamStr(std::string);
    std::string getParamStr();
    void setReadOnly(bool);

signals:
    void mainpuateObject(std::string,int);

private:
    QGridLayout* m_layout;
    QLabel* m_inputWeigthLabel,*m_unitLabel;
    QComboBox* m_unitCombo;
    QLineEdit* m_inputWeightEdit;
    HGManipulateWidget* m_manipulateW;
    std::string m_lang;
};

#endif // HGELEBALANCEWIDGET_H
