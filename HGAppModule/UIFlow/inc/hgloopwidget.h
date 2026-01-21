#ifndef HGLOOPWIDGET_H
#define HGLOOPWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QGridLayout>
#include "hgmanipulatewidget.h"

class HGLoopWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGLoopWidget(std::string,std::string name,QWidget *parent = nullptr);
    bool closeWindow();
    std::string getParamStr();
    void setParamStr(std::string);

signals:
    void mainpuateObject(std::string,int);

private:
    QGridLayout* m_layout;
    QLabel* m_typeLabel,*m_modeLabel,*m_maxCountLabel;
    QComboBox* m_modeCombo;
    QLineEdit* m_typeEdit,*m_maxCountEdit;

    HGManipulateWidget* m_manipulateW;
    std::string m_lang;

};

#endif // HGLOOPWIDGET_H
