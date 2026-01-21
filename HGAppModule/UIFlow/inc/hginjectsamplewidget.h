#ifndef HGINJECTSAMPLEWIDGET_H
#define HGINJECTSAMPLEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QGridLayout>
#include <QLineEdit>
#include "hgmanipulatewidget.h"
#include "common.h"

class HGInjectSampleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGInjectSampleWidget(std::string,std::string param,std::string name,QWidget *parent = nullptr);
    bool closeWindow();
    std::string getParamStr();
    void setParamStr(std::string);

signals:
    void mainpuateObject(std::string,int);
   
private:
    void setMode();
private:
    QLabel* m_quyangmoshiLabel,*m_quyangsulvLabel,*m_quyangtimeLabel;
    QComboBox* m_quyangmoshiCombo;
    QLineEdit* m_quyangsulvEdit,*m_quyangtimeEdit;
    QLabel*m_actionPortLabel,*m_actionTimeLabel; 
    QLineEdit*m_actionTimeEdit;
    QComboBox* m_actionPortCombo;

    QGridLayout*m_layout;
    std::string m_param;
    HGManipulateWidget* m_manipulateW;
    std::string m_lang;
};

#endif // HGINJECTSAMPLEWIDGET_H
