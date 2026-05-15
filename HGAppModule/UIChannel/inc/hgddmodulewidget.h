#ifndef HGDDMODULEWIDGET_H
#define HGDDMODULEWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include "hgmanipulatewidget.h"

class HGDDModuleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGDDModuleWidget(std::string,std::string type,std::string name,QWidget *parent = nullptr);
    bool closeWindow();
    void setParamStr(std::string);
    std::string getParamStr();
    void setReadOnly(bool);

signals:
    void mainpuateObject(std::string,int);

private:
    std::string m_type;
    QGridLayout* m_layout;
    QLabel* m_ddTypeLabel,*m_ddPumpLabel, *m_congWavlveLabel,*m_inputLiquidSpeedLabel,*m_backLiquidSpeedLabel;
    QComboBox* m_ddPumpCombo,*m_inputLiquidSpeedCombo,*m_backLiquidCombo;
    QLineEdit* m_ddTypeEdit,*m_congWavlveEdit;
    HGManipulateWidget* m_manipulateW;
    std::string m_lang;
};

#endif // HGDDMODULEWIDGET_H
