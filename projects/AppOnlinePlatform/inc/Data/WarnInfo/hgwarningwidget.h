#ifndef HGWARNINGWIDGET_H
#define HGWARNINGWIDGET_H

#include <QObject>
#include <QWidget>
#include "hgqlabel.h"

class HGWarningWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGWarningWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();
    ~HGWarningWidget();
    
signals:

private:
    HGQLabel *m_label;
    std::string m_lang;
};

#endif // HGWARNINGWIDGET_H
