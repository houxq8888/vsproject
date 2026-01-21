#ifndef HGJUDGEWIDGET_H
#define HGJUDGEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QGridLayout>
#include "hgmanipulatewidget.h"

class HGJudgeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGJudgeWidget(std::string,std::string name,QWidget *parent = nullptr);
    bool closeWindow();
    std::string getParamStr();
    void setParamStr(std::string);

signals:
    void mainpuateObject(std::string,int);

private:
    QGridLayout* m_layout;
    QLabel* m_compareSignalLabel,*m_unitLabel,*m_compareValueLabel,*m_trackLabel;
    QComboBox*m_compareSignalCombo,*m_unitCombo,*m_compareValueCombo,*m_trackCombo;
    HGManipulateWidget* m_manipulateW;
    std::string m_lang;
};

#endif // HGJUDGEWIDGET_H
