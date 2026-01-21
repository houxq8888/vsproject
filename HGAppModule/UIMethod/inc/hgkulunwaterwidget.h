#ifndef HGKULUNWATERWIDGET_H
#define HGKULUNWATERWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QFrame>
#include <QButtonGroup>
#include <QRadioButton>
#include <QCheckBox>
#include <QPushButton>
#include <QTableWidget>
#include "labelwithimg.h"
#include "common.h"


class HGKulunWaterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGKulunWaterWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();
    void setParam(std::string param);
    std::string getParamStr();
    std::map<std::string,std::vector<uint8_t>> getSendMethodInfo();

signals:

private slots:
    void slotLabelClicked(std::string name);

private:
    void setDriftParam();
private:
    QGridLayout* m_layout;
    QLabel* m_methodLabel,*m_detectMLabel,*m_dianjieDJLabel;
    QLabel*m_targetElementLabel,*m_ddjiLabel,*m_ddduLabel;
    QLabel*m_ddModeMinLabel,*m_ddModeMaxLabel,*m_cuiquLabel;
    QLineEdit*m_ddModeMinEdit,*m_ddModeMaxEdit,*m_cuiquEdit;
    QLabel*m_targetDWLabel,*m_targetMethodLabel;
    QLabel*m_targetYSLabel,*m_dynamicRangeLabel,*m_limitConditionLabel;

    QTableWidget* m_tableW;

    QLabel* m_targetDWUnitLabel, *m_targetYSUnitLabel, *m_jiarebanguanLabel;
    QLabel* m_jiarebanguanUnitLabel, *m_jiaozhengyinziLabel;
    QLineEdit* m_jiarebanguanEdit, *m_jiaozhengyinziEdit;

    QLabel*m_cubeLabel;
    QLabel* m_endTimeLabel;
    QLineEdit*m_endTimeEdit;
    QLabel* m_endTimeUnitLabel, *m_ddModeMinUnitLabel, *m_ddModeMaxUnitLabel,*m_cuiquUnitLabel;
    // HGQLabel*m_loadCurveLabel;
    QLabel* m_targetDriftLabel, *m_targetDriftUnitLabel;
    QLabel* m_startDriftLabel, *m_startDriftUnitLabel;
    QLineEdit* m_startDriftEdit, *m_targetDriftEdit;
    QLabel* m_fanLabel;
    QComboBox* m_fanCombo;
    QLabel* m_initTempLabel, *m_initTempUnitLabel;
    QLineEdit* m_initTempEdit;
    QLabel* m_stableTempTimeLabel, *m_stableTempTimeUnitLabel;
    QLineEdit* m_stableTempTimeEdit;

    QGroupBox* m_dataProcessBox;
    QGridLayout* m_dataProcessLayout;
    LabelWithImg* m_driftLabel;
    LabelWithImg* m_autoDriftLabel,*m_humanDriftLabel;
    QLineEdit* m_humanDriftEdit, *m_humanDriftTimeEdit;
    QLabel* m_humanDriftUnitLabel, *m_humanDriftTimeUnitLabel;
    QPushButton *m_humanDriftReadBtn;
    QLabel* m_manualblankUnitLabel;
    QRadioButton* m_manualblankCheckBox, *m_autoBlankCheckBox;
    QLineEdit* m_manualblankEdit;

    QCheckBox* m_programAscendTemperatureCheckBox;

    
    

    QLineEdit*m_methodEdit,*m_targetElementEdit,*m_ddduEdit;
    QLineEdit*m_targetDWEdit,*m_targetYSEdit,*m_dynimicRangeEdit;
    QComboBox*m_detectCombo,*m_dianjieDJCombo,*m_ddjiCombo,*m_cubeCombo;
    QComboBox*m_targetMethodCombo,*m_limitConditionCombo;

    QFrame* m_hor1,*m_hor2,*m_hor3;
    bool m_driftflag;
    int m_driftAutoFlag;
    std::string m_lang;
};

#endif // HGKULUNWATERWIDGET_H
