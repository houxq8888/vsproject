#ifndef DISPLAYANDLIGHTWIDGET_H
#define DISPLAYANDLIGHTWIDGET_H

#include <QWidget>
#include <QLabel>
#include "labelwithimg.h"
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include <QSlider>
#include "basewidget.h"

class DisplayAndLightWidget : public BaseWidget
{
    Q_OBJECT
public:
    explicit DisplayAndLightWidget(std::string,QWidget *parent = nullptr);
    ~DisplayAndLightWidget();
    bool closeWindow();
    void setLanguage(std::string lang) override;

signals:

private slots:
    void clickEnableAutoRectifyLight();
    void slotResolutionChanged(int);
    void slotLightChanged(int value);

private:
    void fnReadDB();
    void fnWriteDB();
    void setControlStatus();

private:
    bool m_autoRectifyLightFlag;
    QLabel* m_resolutionLabel, *m_autoRectifyLightLabel, *m_lightLabel;
    LabelWithImg* m_autoRectifyLightImg;
    QComboBox* m_resolutionComboBox;
    QSlider* m_lightSlider;
    QGridLayout* m_layout;
    std::string m_deviceName;
    std::string m_lang;
};

#endif // DISPLAYANDLIGHTWIDGET_H
