#ifndef HGDETECTWIDGET_H
#define HGDETECTWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include "hgmanipulatewidget.h"
#include "common.h"

class HGDetectWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGDetectWidget(std::string,std::string name,QWidget *parent = nullptr);
    bool closeWindow();
    std::string getParamStr();
    void setParamStr(std::string);

signals:
    void mainpuateObject(std::string,int);

private:
    QLabel* m_minDDCubeLabel,*m_maxDDCubeLabel,*m_stopTimePerDiLabel,*m_preDDLabel,*m_preAfterStopLabel,*m_swirlSpeedLabel;
    QComboBox* m_swirlSpeedCombo;
    QLineEdit* m_minDDCubeEdit,*m_maxDDCubeEdit,*m_stopTimePerDiEdit,*m_preDDEdit,*m_preAfterStopEdit;
    QGridLayout *m_layout;
    QLabel*m_actionPortLabel,*m_testMethodLabel,*m_actionTimeLabel; 
    QLineEdit*m_actionTimeEdit; 
    QComboBox* m_actionPortCombo,*m_testMethodCombo;
    HGManipulateWidget* m_manipulateW;
    std::string m_lang;
};


#endif // HGDETECTWIDGET_H
