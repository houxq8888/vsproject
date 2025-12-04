#ifndef HGBACKUNITWIDGET_H
#define HGBACKUNITWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include "labelwithimg.h"
#include <QSlider>
#include <QTableWidget>
#include "hgddadjustwidget.h"

class HGBackUnitWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGBackUnitWidget(std::string,std::string name,QWidget *parent = nullptr);
    std::string getName() {return m_name;}
    void setParamStr(std::string);
    std::string getParamStr();
    bool closeWindow();

signals:

private slots:
    void clickEnablePump();
    void slotAdjustClick();

private:
    QLabel* m_nameLabel;
    QGroupBox* m_baseInfoGroup, *m_advancedInfoGroup;
    QGridLayout* m_baseInfoLayout, *m_advancedInfoLayout, *m_layout;

    QLabel* m_serialNumberLabel, *m_ddSpecificationLabel,*m_minDDCubeLabel,*m_ddAccuracyLabel,*m_valveTypeLabel,*m_adjustTimeLabel,*m_adjustCircleLabel;
    QLineEdit* m_serialNumberEdit, *m_ddSpecificationEdit,*m_minDDCubeEdit,*m_ddAccuracyEdit,*m_valveTypeEdit,*m_adjustTimeEdit,*m_adjustCircleEdit;
    LabelWithImg* m_enablePumpLabel;
    QLabel* m_pumpLifeCircleLabel;
    QLineEdit *m_pumpLifeCircleEdit;
    QLabel* m_cumulativeLifeSpanLabel;
    QSlider* m_cumulativeSlider;
    QPushButton* m_adjustBtn;

    bool m_isEnablePump;
    std::string m_name;

    HGDDAdjustWidget *m_ddAdjustW;
    std::string m_lang;
};

#endif // HGBACKUNITWIDGET_H
