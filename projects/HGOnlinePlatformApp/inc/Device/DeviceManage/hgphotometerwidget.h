#ifndef HGPHOTOMETERWIDGET_H
#define HGPHOTOMETERWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include "labelwithimg.h"
#include <QTableWidget>

class HGPhotometerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGPhotometerWidget(std::string,std::string,QWidget *parent = nullptr);
    bool closeWindow();
    std::string getName() {return m_name;}
    void setParamStr(std::string);
    std::string getParamStr();

signals:

private:
    QLabel* m_nameLabel;
    QGroupBox* m_baseInfoGroup, *m_advancedInfoGroup;
    QGridLayout* m_baseInfoLayout, *m_advancedInfoLayout, *m_layout;

    QLabel* m_serialNumberLabel, *m_waveLengthLabel,*m_tempRangeLabel;
    QLineEdit* m_serialNumberEdit, *m_waveLengthEdit,*m_tempRangeEdit;

    QLabel* m_wlAdjustLabel;
    QLabel* m_wlAdjustAttentionLabel;
    QPushButton* m_adjustBtn;
    std::string m_name;
    std::string m_lang;


};

#endif // HGPHOTOMETERWIDGET_H
