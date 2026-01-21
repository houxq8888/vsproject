#ifndef HGRS485WIDGET_H
#define HGRS485WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include "labelwithimg.h"
#include <QSlider>
#include <QTableWidget>
#include <QComboBox>
#include "hgddadjustwidget.h"
#include <QTimer>


class HGRs485Widget : public QWidget
{
    Q_OBJECT
public:
    explicit HGRs485Widget(std::string,std::string,QWidget *parent = nullptr);
    bool closeWindow();
    std::string getName() {return m_name;}
    void setParamStr(std::string);
    std::string getParamStr();
signals:

private slots:
    void slotOKClick();
    void slotTypeIndexChanged(int);
    void slotUpdateSerialPort();
    
private:
    void fnDisplayBaseInfo();
    

private:
    std::string m_name;
    QLabel* m_nameLabel;
    QGroupBox* m_baseInfoGroup, *m_advancedInfoGroup;
    QGridLayout* m_baseInfoLayout, *m_advancedInfoLayout, *m_layout;

    QLabel* m_typeLabel,*m_deviceComLabel;
    QComboBox* m_typeCombo, *m_deviceComCombo;
    QLabel* m_maxSpeedLabel,*m_distanceLabel;
    QLabel* m_advanceNameLabel,*m_portLabel,*m_dataLabel,*m_adjustLabel;
    QLineEdit*m_maxSpeedEdit,*m_distanceEdit,*m_portEdit,*m_dataEdit,*m_adjustEdit;

    QPushButton *m_okBtn;
    QTimer* m_timer;
    std::string m_lang;
};

#endif // HGRS485WIDGET_H
