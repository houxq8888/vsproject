#ifndef HGSWIRLSTEPWIDGET_H
#define HGSWIRLSTEPWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include "hgmanipulatewidget.h"
#include "common.h"

class HGSwirlStepWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGSwirlStepWidget(std::string,std::string name,QWidget *parent = nullptr);
    bool closeWindow();
    std::string getParamStr();
    void setParamStr(std::string);

signals:
    void mainpuateObject(std::string,int);

private:
    QLabel* m_swirlSpeedLabel,*m_actionTimeLabel; 
    QLineEdit*m_actionTimeEdit;
    QLineEdit* m_swirlSpeedEdit;
    QLabel*m_actionPortLabel; 
    QComboBox* m_actionPortCombo;
    QGridLayout *m_layout;
    HGManipulateWidget* m_manipulateW;
    std::string m_lang;

};

#endif // HGSWIRLWIDGET_H
