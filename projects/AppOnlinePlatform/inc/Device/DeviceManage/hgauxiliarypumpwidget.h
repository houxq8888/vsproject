#ifndef HGAUXILIARYPUMPWIDGET_H
#define HGAUXILIARYPUMPWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include "labelwithimg.h"
#include <QSlider>
#include <QTableWidget>

class HGAuxiliaryPumpWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGAuxiliaryPumpWidget(std::string,std::string name,QWidget *parent = nullptr);
    bool closeWindow();
    std::string getName() {return m_name;}
    void setParamStr(std::string);
    std::string getParamStr();

signals:

private slots:
    void clickEnablePump();

private:
    QLabel* m_nameLabel;
    QGroupBox* m_baseInfoGroup, *m_advancedInfoGroup;
    QGridLayout* m_baseInfoLayout, *m_advancedInfoLayout, *m_layout;

    QLabel* m_serialNumberLabel, *m_ratedSpeedLabel,*m_maxSpeedLabel;
    QLineEdit* m_serialNumberEdit, *m_ratedSpeedEdit,*m_maxSpeedEdit;
    
    LabelWithImg* m_enablePumpLabel;
    QLabel* m_pumpLifeCircleLabel;
    QLineEdit *m_pumpLifeCircleEdit;
    QLabel* m_cumulativeLifeSpanLabel;
    QSlider* m_cumulativeSlider;
    QPushButton* m_adjustBtn;
    std::string m_name;

    bool m_isEnablePump;
    std::string m_lang;
};

#endif // HGAUXILIARYPUMPWIDGET_H
