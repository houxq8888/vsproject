#ifndef HGJIAREBANWIDGET_H
#define HGJIAREBANWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include "hgdjadjustwidget.h"

class HGJiarebanWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGJiarebanWidget(std::string,std::string,QWidget *parent = nullptr);
    bool closeWindow();
    std::string getName() {return m_name;}
    void setParamStr(std::string);
    std::string getParamStr();

signals:

private slots:
    void slotStartTempClick();
    void slotStopTempClick();
    void slotOnClick();
    void slotOffClick();
    void slotUpdateDataInfo();

private:
    QLabel* m_nameLabel;
    QGroupBox* m_tempInfoGroup, *m_fanInfoGroup;
    QGridLayout* m_tempInfoLayout, *m_fanInfoLayout, *m_layout;

    // QLabel* m_serialNumberLabel, *m_calibrateRangeLabel,*m_tempRangeLabel,*m_adjustTimeLabel,*m_adjustCircleLabel;
    // QLineEdit* m_serialNumberEdit, *m_calibrateRangeEdit,*m_tempRangeEdit,*m_adjustTimeEdit,*m_adjustCircleEdit;
    QLabel* m_dataInfoLabel;
    QPushButton* m_startBtn, *m_stopBtn, *m_onBtn, *m_offBtn;
    std::string m_name;

    // HGDJAdjustWidget* m_djAdjustW;
    std::string m_lang;
    QTimer *m_timer;
};

#endif // HGJIAREBANWIDGET_H
