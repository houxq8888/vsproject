#ifndef HGADDMEDICATIONWIDGET_H
#define HGADDMEDICATIONWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QGridLayout>
#include <QLineEdit>
#include "hgmanipulatewidget.h"

class HGAddMedicationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGAddMedicationWidget(std::string,std::string name,QWidget *parent = nullptr);
    bool closeWindow();
    std::string getParamStr();
    void setParamStr(std::string);

signals:
    void mainpuateObject(std::string,int);
    
private:
    QLabel*m_addReagentLabel,*m_addSpeedLabel,*m_addValueLabel,*m_unitLabel;
    QComboBox*m_addReagentCombo,*m_unitCombo;
    QLineEdit*m_addSpeedEdit,*m_addValueEdit;
    QGridLayout*m_layout;

    HGManipulateWidget* m_manipulateW;
    std::string m_lang;
};

#endif // HGADDMEDICATIONWIDGET_H
