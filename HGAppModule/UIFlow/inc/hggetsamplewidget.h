#ifndef HGGETSAMPLEWIDGET_H
#define HGGETSAMPLEWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include "hgmanipulatewidget.h"
#include "common.h"

class HGGetSampleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGGetSampleWidget(std::string,std::string param,std::string name,QWidget *parent = nullptr);
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
    QLabel*m_actionPortLabel,*m_actionTimeLabel; 
    QLineEdit*m_actionTimeEdit;
    QComboBox* m_actionPortCombo;
    QLineEdit* m_quyangsulvEdit,*m_quyangtimeEdit;
    QGridLayout *m_layout;
    HGManipulateWidget* m_manipulateW;
    std::string m_param;
    std::string m_lang;
};

#endif // HGGETSAMPLEWIDGET_H
