#ifndef DATEANDTIMEWIDGET_H
#define DATEANDTIMEWIDGET_H

#include <QWidget>
#include <QLabel>
#include "labelwithimg.h"
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include "basewidget.h"
#include "mykeyboard.h"

class DateAndTimeWidget : public BaseWidget
{
    Q_OBJECT
public:
    explicit DateAndTimeWidget(std::string,QWidget *parent = nullptr);
    ~DateAndTimeWidget();
    bool closeWindow();
    void setLanguage(std::string lang) override;

signals:
private slots:
    void clickEnableAutoSetRegion();
    void clickEnableAutoSetTime();
    void slotRegionChanged(int);
    void clickManualSetTime();
    void fnSetSystemTime(const std::string &time);

private:
    void fnReadDB();
    void fnWriteDB();
    void setControlStatus();

private:
    QLabel* m_autoSetRegionLabel, *m_regionLabel, *m_autoSetTimeLabel, *m_manualSetLabel;
    LabelWithImg* m_autoSetRegionImg,*m_autoSetTimeImg;
    QComboBox* m_regionComboBox;
    QPushButton* m_manualSetBtn;
    QGridLayout* m_layout;
    bool m_isEnableAutoSetRegion, m_isEnableAutoSetTime;
    std::string m_manualSetTimeStr;
    KBTimeWithHourEdit* m_systemTimeCtrl;
    std::string m_lang;
};

#endif // DATEANDTIMEWIDGET_H
