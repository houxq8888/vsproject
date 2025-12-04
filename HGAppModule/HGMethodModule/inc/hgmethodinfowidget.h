#ifndef HGMETHODINFOWIDGET_H
#define HGMETHODINFOWIDGET_H

#include <QWidget>
#include <QToolBox>
#include <QGridLayout>
#include <QToolBox>
#include "hglightcalibmethodwidget.h"
#include "hgcolorddwidget.h"
#include "hgdianweiddwidget.h"
#include "hgkulunwaterwidget.h"
#include "hgrongliangmethodwidget.h"

class HGMethodInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGMethodInfoWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();
    void createModule(std::string name,std::string param="");
    void methodShow(std::string,std::string);
    std::string getParam();

signals:

private slots:
    // void togglePage(int index);

private:
    QGridLayout* m_layout;
    // QToolBox* m_toolBox;
    HGLightCalibMethodWidget* m_lightCalibMW;
    HGColorDDWidget* m_colorDDMW;
    HGDianWeiDDWidget* m_dianweiDDMW;
    HGKulunWaterWidget* m_kulunWaterMW;
    HGRongliangMethodWidget* m_rongliangMW;
    std::string m_name;
    std::string m_lang;
};

#endif // HGMETHODINFOWIDGET_H
