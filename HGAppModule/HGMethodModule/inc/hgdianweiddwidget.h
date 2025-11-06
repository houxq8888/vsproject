#ifndef HGDIANWEIDDWIDGET_H
#define HGDIANWEIDDWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QFrame>
#include <QTextEdit>


class HGDianWeiDDWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGDianWeiDDWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();
    void setParam(std::string param);
    std::string getParamStr();

signals:
private:
    QGridLayout* m_layout;
    QLabel* m_methodLabel,*m_dianjiTypeLabel,*m_actionLiquidLabel;
    QLabel*m_targetElementLabel,*m_ddjiLabel,*m_ddduLabel;
    QLabel*m_targetJudgeLabel,*m_weifenThresholdLabel,*m_targetBeforeCubeLabel;
    QLabel*m_targetAfterCubeLabel,*m_calculateFormulatLabel,*m_displayUnitLabel;
    // HGQLabel*m_loadCurveLabel;

    QLineEdit*m_methodEdit,*m_targetElementEdit,*m_ddduEdit;
    QLineEdit*m_weifenThresholdEdit,*m_targetBeforeCubeEdit,*m_targetAfterCubeEdit;
    QComboBox*m_dianjiTypeCombo,*m_actionLiquidCombo,*m_ddjiCombo;
    QComboBox*m_targetJudgeCombo,*m_calculateFormulatCombo;
    QComboBox*m_displayUnitCombo;
    // *m_loadCurveCombo;

    QTextEdit* m_formulateEdit;
    QFrame* m_hor1,*m_hor2;
    std::string m_lang;
};

#endif // HGDIANWEIDDWIDGET_H
