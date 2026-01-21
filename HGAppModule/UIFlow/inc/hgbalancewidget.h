#ifndef HGBALANCEWIDGET_H
#define HGBALANCEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QGridLayout>
#include <QLineEdit>
#include "hgmanipulatewidget.h"

class HGBalanceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGBalanceWidget(std::string,std::string name,QWidget *parent = nullptr);
    bool closeWindow();
    std::string getParamStr();
    void setParamStr(std::string);

signals:
    void mainpuateObject(std::string,int);
   
private:
    // QLab*m_addSpeedLabel,*m_addValueLabel,*m_unitLabel;
    // QComboBox*m_addReagentCombo,*m_unitCombo;
    // QLineEdit*m_addSpeedEdit,*m_addValueEdit;
    QLabel*m_actionPortLabel,*m_actionTimeLabel; 
    QLineEdit*m_actionTimeEdit;
    QComboBox* m_actionPortCombo;
    QGridLayout*m_layout;

    HGManipulateWidget* m_manipulateW;
    std::string m_lang;
};

#endif // HGBALANCEWIDGET_H
