#ifndef HGLIGHTCALIBMETHODWIDGET_H
#define HGLIGHTCALIBMETHODWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QFrame>

class HGLightCalibMethodWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGLightCalibMethodWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();
    void setParam(std::string param);
    std::string getParamStr();

signals:
private:
    QGridLayout* m_layout;
    QLabel* m_methodLabel,*m_detectMLabel,*m_zhishiWaveLabel,*m_fuzhuWaveLabel;
    QLabel*m_canbiWaveLabel,*m_actionLiquidLabel,*m_zhishijiLabel;
    QLabel*m_targetElementLabel,*m_ddjiNameLabel,*m_ddduLabel;
    QLabel*m_targetJudgeLabel,*m_weifenThresholdLabel,*m_targetBeforeCubeLabel;
    QLabel*m_targetAfterCubeLabel,*m_calculateFormulatLabel;
    // QLabel*m_displayUnitLabel;
    // HGQLabel*m_loadCurveLabel;

    QLineEdit*m_methodEdit,*m_zhishijiEdit,*m_targetElementEdit,*m_ddduEdit;
    QLineEdit*m_weifenThresholdEdit,*m_targetBeforeCubeEdit,*m_targetAfterCubeEdit;
    QComboBox*m_detectCombo,*m_actionLiquidCombo,*m_ddjiCombo;
    QComboBox*m_targetJudgeCombo,*m_calculateFormulatCombo;
    // QComboBox*m_loadCurveCombo;
    QComboBox *m_zhishiWaveCombo,*m_fuzhuWaveCombo,*m_canbiWaveCombo;

    QFrame* m_hor1,*m_hor2,*m_hor3;
    std::string m_lang;
};

#endif // HGLIGHTCALIBMETHODWIDGET_H
