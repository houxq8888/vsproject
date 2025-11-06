#ifndef HGSHUIFENYIWIDGET_H
#define HGSHUIFENYIWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QRadioButton>
#include <QPushButton>
#include <QCheckBox>
#include "hgdjadjustwidget.h"

class HGShuifenyiWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGShuifenyiWidget(std::string,std::string,QWidget *parent = nullptr);
    bool closeWindow();
    std::string getName() {return m_name;}
    void setParamStr(std::string);
    std::string getParamStr();

signals:

private slots:
    void slotManualDetectSample();
    void slotNeedleCheck();

private:
    QGridLayout* m_layout;
    QLabel* m_sampleNameLabel;
    QLineEdit* m_sampleNameEdit;
    QLabel *m_sampleNoLabel;
    QLineEdit*m_sampleNoEdit;
    QLabel* m_methodLabel;
    QComboBox* m_methodCombo;

    QPushButton* m_captureWeightBtn;
    QRadioButton* m_weightSampleRadio,*m_cubeSampleRadio;
    QLineEdit* m_weightSampleEdit,*m_cubeSampleEdit;
    QLabel* m_weightSampleUnitLabel,*m_cubeSampleUnitLabel;

    QPushButton* m_manualDetectSampleBtn;
    QPushButton* m_releaseNeedleBtn, *m_punctureBtn, *m_stopNeedleBtn;
    QCheckBox* m_needleCheck;
    QCheckBox* m_densityCheck;
    QLineEdit* m_densityEdit;
    QLabel* m_densityUnitLabel;

    QLabel* m_nameLabel;
    std::string m_name;
    std::string m_lang;
};

#endif // HGSHUIFENYIWIDGET_H
