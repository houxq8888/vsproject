#ifndef HGZIDONGJINYANGQIWIDGET_H
#define HGZIDONGJINYANGQIWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QComboBox>
#include <QSpinBox>
#include "hgdjadjustwidget.h"

class HGZidongjinyangqiWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGZidongjinyangqiWidget(std::string,std::string,QWidget *parent = nullptr);
    bool closeWindow();
    std::string getName() {return m_name;}
    void setParamStr(std::string);
    std::string getParamStr();

signals:

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
private slots:
   void slotOnClick();
   void slotOffClick();
   void slotAscendClick();
   void slotDescendClick();
   void slotArrivalClick();
   void slotReleaseClick();
   void slotPunctureClick();
   void slotChuisaoClick();
   void slotCircleExecuteClick();
   void slotUpdateDataInfo();
   void slotUpdateClick();
   void slotStopClick();
   void slotSavePosClick();
   

private:
    QLabel* m_nameLabel;
    QGroupBox* m_pumpInfoGroup, *m_swirlInfoGroup, *m_punctureInfoGroup;
    QGridLayout* m_pumpInfoLayout, *m_swirlInfoLayout, *m_punctureInfoLayout, *m_layout;
    QGroupBox* m_heightGroup, *m_adjustModeGroup, *m_circleGroup;
    QGridLayout* m_heightLayout, *m_adjustModeLayout, *m_circleLayout;
    QRadioButton* m_tallHeightRadio, *m_middleHeightRadio, *m_shortHeightRadio;
    QRadioButton* m_longPressRadio, *m_arriveLimitRadio, *m_stepArriveRadio;
    QSpinBox* m_circleNumberSpin;
    QComboBox* m_stepCombo;

    QPushButton* m_arrivalBtn, *m_releaseBtn, *m_punctureBtn;
    QPushButton* m_chuisaoBtn, *m_circleExecuteBtn;
    // QLabel* m_serialNumberLabel, *m_calibrateRangeLabel,*m_tempRangeLabel,*m_adjustTimeLabel,*m_adjustCircleLabel;
    // QLineEdit* m_serialNumberEdit, *m_calibrateRangeEdit,*m_tempRangeEdit,*m_adjustTimeEdit,*m_adjustCircleEdit;
    QLabel* m_dataInfoLabel;

    QLabel* m_curNeedlePosLabel;
    QPushButton* m_updateBtn, *m_stopBtn, *m_savePosBtn;
    QPushButton* m_onBtn, *m_offBtn, *m_ascendBtn, *m_descendBtn;
    std::string m_name;
    std::string m_lang;

    QTimer *m_timer;
};

#endif // HGZIDONGJINYANGQIWIDGET_H
