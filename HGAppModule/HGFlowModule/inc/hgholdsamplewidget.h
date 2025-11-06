#ifndef HGHOLDSAMPLEWIDGET_H
#define HGHOLDSAMPLEWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include "hgmanipulatewidget.h"

class HGHoldSampleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGHoldSampleWidget(std::string,std::string name,QWidget *parent = nullptr);
    bool closeWindow();
    std::string getParamStr();
    void setParamStr(std::string);

signals:
    void mainpuateObject(std::string,int);

private:
    QLabel* m_jiliangModeLabel,*m_liuyeSpeedLabel,*m_liuyeTimeLabel;
    QComboBox* m_jiliangModeCombo;
    QLineEdit* m_liuyeSpeedEdit,*m_liuyeTimeEdit;
    QGridLayout *m_layout;
    HGManipulateWidget* m_manipulateW;
    std::string m_lang;
};

#endif // HGHOLDSAMPLEWIDGET_H
