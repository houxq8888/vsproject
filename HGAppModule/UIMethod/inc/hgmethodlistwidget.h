#ifndef HGMETHODLISTWIDGET_H
#define HGMETHODLISTWIDGET_H

#include <QWidget>
#include "labelwithimg.h"
#include <QVBoxLayout>
#include <QScrollArea>

class HGMethodListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGMethodListWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();
    std::vector<std::string> getMethods();
    void lightMethod(std::string);
    
signals:
    void createModule(std::string);

private slots:
    void slotClickDianWei();
    void slotClickColorDD();
    void slotClickPhotometer();
    void slotClickRongLiang();
    void slotClickKulun();
    void slotClickTempDD();
    void slotCreateModule(std::string name);

private:
    void setAllFalse();

private:
    LabelWithImg* m_dianweiLabel,*m_colorDDLabel,*m_photometerLabel,*m_rongliangLabel,*m_kulunLabel,*m_tempDDLabel;
    QVBoxLayout* m_layout, *m_mainLayout;
    QWidget *m_contentWidget;
    QScrollArea* m_scrollArea;
    std::string m_lang;
};  

#endif // HGMETHODLISTWIDGET_H
