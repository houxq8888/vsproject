#ifndef HGSWIRLWIDGET_H
#define HGSWIRLWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include "hgmanipulatewidget.h"

class HGSwirlWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGSwirlWidget(std::string,bool isHot,std::string name,QWidget *parent = nullptr);
    bool closeWindow();
    void setParamStr(std::string);
    std::string getParamStr();
    void setReadOnly(bool);

signals:
    void mainpuateObject(std::string,int);

private:
    bool m_isHot;
    QGridLayout* m_layout;
    QLabel* m_choiceLabel,*m_swirlSpeedLabel, *m_hotModuleLabel,*m_temperatureSetLabel;
    QComboBox* m_choiceCombo,*m_hotModuleCombo;
    QLineEdit* m_swirlSpeedEdit,*m_temperatureSetEdit;
    HGManipulateWidget* m_manipulateW;
    std::string m_lang;
};

#endif // HGSWIRLWIDGET_H
