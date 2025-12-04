#ifndef HGRONGLIANGMETHODWIDGET_H
#define HGRONGLIANGMETHODWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QFrame>
#include "hgqlabel.h"
#include "common.h"


class HGRongliangMethodWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGRongliangMethodWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();
    void setParam(std::string param);
    std::string getParamStr();

signals:
private slots:
    void slotLabelClicked(std::string name);

private:
    void setDriftParam();
private:
    QGridLayout* m_layout;
    QLabel* m_methodLabel,*m_detectMLabel;
    QLabel*m_targetElementLabel,*m_ddjiLabel,*m_ddduLabel,*m_ddModeLabel;
    QLabel*m_ddTimeLabel;
    QLabel*m_targetDWLabel,*m_targetMethodLabel,*m_targetYSLabel;
    QLabel*m_dynamicRangeLabel,*m_limitConditionLabel,*m_cubeLabel;
    // HGQLabel*m_loadCurveLabel;
    QGroupBox* m_dataProcessBox;
    QGridLayout* m_dataProcessLayout;
    LabelWithImg* m_driftLabel;
    LabelWithImg* m_autoDriftLabel,*m_humanDriftLabel;
    QLineEdit* m_humanDriftEdit;
    QLabel* m_blankLabel;
    QLineEdit* m_blankEdit;
    QLineEdit*m_methodEdit,*m_targetElementEdit,*m_ddduEdit;
    QLineEdit*m_targetDWEdit,*m_targetYSEdit;
    QComboBox*m_detectCombo,*m_ddjiCombo,*m_ddModeCombo;
    QComboBox*m_limitConditionCombo,*m_cubeCombo,*m_loadCurveCombo;
    QComboBox*m_targetMethodCombo;
    QLineEdit*m_ddTimeEdit,*m_dynimicRangeEdit;

    QFrame* m_hor1,*m_hor2,*m_hor3;
    bool m_driftflag;
    int m_driftAutoFlag;
    std::string m_lang;
};

#endif // HGRONGLIANGMETHODWIDGET_H
