#ifndef HGAUXPUMPWIDGET_H
#define HGAUXPUMPWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include "hgmanipulatewidget.h"

class HGAuxPumpWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGAuxPumpWidget(std::string,std::string name,QWidget *parent = nullptr);
    bool closeWindow();
    void setParamStr(std::string);
    std::string getParamStr();
    void setReadOnly(bool);

signals:
    void mainpuateObject(std::string,int);

private:
    
    QGridLayout* m_layout;
    QLabel* m_choiceLabel,*m_functionLabel, *m_rotateSpeedLabel;
    QComboBox* m_choiceCombo,*m_functionCombo;
    QLineEdit* m_rotateSpeedEdit;
    HGManipulateWidget* m_manipulateW;
    std::string m_lang;

};

#endif // HGAUXPUMPWIDGET_H
