#ifndef WARNINGWIDGET_H
#define WARNINGWIDGET_H

#include <QWidget>
#include <QLabel>
#include "labelwithimg.h"
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include <QSlider>
#include <QTimer>
#include "hgqlabel.h"
#include "basewidget.h"

class WarningWidget : public BaseWidget
{
    Q_OBJECT
public:
    explicit WarningWidget(std::string,QWidget *parent = nullptr);
    ~WarningWidget();
    bool closeWindow();
    void setWarnSoundVolumn(int);
    BaseWidget& operator=(const BaseWidget&) override;
    void setLanguage(std::string lang) override;


signals:
private slots:
    void clickEnableSoundWarn();
    void clickEnableLightWarn();
    void slotSetSoundWarnImg(int);

private:
    void fnReadDB();
    void fnWriteDB();
    void setControlStatus();
    void fnCopy(const WarningWidget&);

private:
    bool m_isEnableSoundWarn,m_isEnableLightWarn;
    QLabel* m_soundWarnLabel, *m_lightWarnLabel;
    LabelWithImg* m_soundWarnImg,*m_lightWarnImg;
    HGQLabel* m_soundWarnText;
    QSlider* m_soundSlider;
    QGridLayout* m_layout;
    bool m_setSoundValueFromMain;
    std::string m_lang;
};

#endif // WARNINGWIDGET_H
