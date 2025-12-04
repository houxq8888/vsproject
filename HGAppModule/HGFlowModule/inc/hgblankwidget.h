#ifndef HGBLANKWIDGET_H
#define HGBLANKWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include "hgmanipulatewidget.h"

class HGblankWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGblankWidget(std::string,std::string name,QWidget *parent = nullptr);
    bool closeWindow();
    std::string getParamStr();
    void setParamStr(std::string);

signals:
    void mainpuateObject(std::string,int);

private:
    QLabel* m_ddjiLabel,*m_gudingshichangeLabel,*m_outputLabel;
    QComboBox* m_ddjiCombo;
    QLineEdit* m_gudingshichangeEdit,*m_outputEdit;
    QGridLayout *m_layout;
    HGManipulateWidget* m_manipulateW;
    std::string m_lang;
};

#endif // HGBLANKWIDGET_H
