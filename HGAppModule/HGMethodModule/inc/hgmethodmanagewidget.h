#ifndef HGMETHODMANAGEWIDGET_H
#define HGMETHODMANAGEWIDGET_H

#include <QWidget>
#include "hgqlabel.h"
#include "hgmethodeditwidget.h"
#include "hgmethodinfowidget.h"
#include "hgmethodlistwidget.h"
#include <QSplitter>

class HGMethodManageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGMethodManageWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();
    ~HGMethodManageWidget();

signals:

private slots:
    void slotCreateModule(std::string name);
    void slotGetMethodList();
    void slotMethodInfoWShow(std::string,std::string);
    void slotSaveMethod();

private:
    HGMethodEditWidget* m_methodEditW;
    HGMethodListWidget* m_methodlistW;
    HGMethodInfoWidget* m_methodInfoW;
    QGridLayout* m_layout;
    QSplitter *m_versplitter1,*m_versplitter;
    std::string m_lang;
};

#endif // HGMETHODMANAGEWIDGET_H
