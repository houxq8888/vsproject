#ifndef HGCOLORDDWIDGET_H
#define HGCOLORDDWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QFrame>
#include "hgqlabel.h"
#include <QTextEdit>


class HGColorDDWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGColorDDWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();
    void setParam(std::string param);
    std::string getParamStr();

signals:

private:
    QGridLayout* m_layout;
    QLabel* m_methodLabel,*m_detectMLabel,*m_actionLiquidLabel,*m_zhishijiLabel;
    QLabel*m_targetElementLabel,*m_ddjiLabel,*m_ddduLabel;
    QLabel*m_targetJudgeLabel,*m_weifenThresholdLabel,*m_targetBeforeCubeLabel;
    QLabel*m_targetAfterCubeLabel,*m_calculateFormulatLabel,*m_displayUnitLabel;
    HGQLabel*m_loadCurveLabel;

    QLineEdit*m_methodEdit,*m_zhishijiEdit,*m_targetElementEdit,*m_ddduEdit;
    QLineEdit*m_weifenThresholdEdit,*m_targetBeforeCubeEdit,*m_targetAfterCubeEdit;
    QComboBox*m_detectCombo,*m_actionLiquidCombo,*m_ddjiCombo;
    QComboBox*m_targetJudgeCombo,*m_calculateFormulatCombo,*m_loadCurveCombo;

    QComboBox* m_displayUnitCombo;

    QTextEdit* m_formulateEdit;
    QFrame* m_hor1,*m_hor2;
    std::string m_lang;
};

#endif // HGCOLORDDWIDGET_H
