#ifndef HGCOLORSENSOR_H
#define HGCOLORSENSOR_H

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include "labelwithimg.h"
#include <QTableWidget>

class HGColorSensor : public QWidget
{
    Q_OBJECT
public:
    explicit HGColorSensor(std::string,std::string,QWidget *parent = nullptr);

    std::string getName() {return m_name;}
    void setParamStr(std::string);
    std::string getParamStr();
    bool closeWindow();

signals:

private slots:
    void clickEnableAutoGain();
    
private:
    QLabel* m_nameLabel;
    QGroupBox* m_baseInfoGroup, *m_advancedInfoGroup;
    QGridLayout* m_baseInfoLayout, *m_advancedInfoLayout, *m_layout;

    QLabel* m_serialNumberLabel, *m_resolutionLabel,*m_frameRateLabel,*m_sensorLabel;
    QLineEdit* m_serialNumberEdit, *m_resolutionEdit,*m_frameRateEdit,*m_sensorEdit;
    LabelWithImg* m_autoGainLabel;
    QLabel* m_wbLabel;
    QTableWidget* m_tableW;
    QPushButton* m_adjustBtn;
    std::string m_name;

    bool m_isEnableAutoGain;
    std::string m_lang;
};

#endif // HGCOLORSENSOR_H
