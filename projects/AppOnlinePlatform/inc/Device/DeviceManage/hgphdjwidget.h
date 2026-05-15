#ifndef HGPHDJWIDGET_H
#define HGPHDJWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include "hgdjadjustwidget.h"

class HGPHdjWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGPHdjWidget(std::string,std::string,QWidget *parent = nullptr);
    bool closeWindow();
    std::string getName() {return m_name;}
    void setParamStr(std::string);
    std::string getParamStr();

signals:

private slots:
    void slotAdjustClick();

private:
    QLabel* m_nameLabel;
    QGroupBox* m_baseInfoGroup, *m_advancedInfoGroup;
    QGridLayout* m_baseInfoLayout, *m_advancedInfoLayout, *m_layout;

    QLabel* m_serialNumberLabel, *m_calibrateRangeLabel,*m_tempRangeLabel,*m_adjustTimeLabel,*m_adjustCircleLabel;
    QLineEdit* m_serialNumberEdit, *m_calibrateRangeEdit,*m_tempRangeEdit,*m_adjustTimeEdit,*m_adjustCircleEdit;
    QLabel* m_elecodeManageLabel;
    QPushButton* m_adjustBtn;
    std::string m_name;

    HGDJAdjustWidget* m_djAdjustW;
    std::string m_lang;
};

#endif // HGPHDJWIDGET_H
