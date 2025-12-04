#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <QWidget>
#include <QLabel>
#include "labelwithimg.h"
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>

class BaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BaseWidget(QWidget *parent = nullptr);
    ~BaseWidget();
    virtual BaseWidget& operator=(const BaseWidget&);
    virtual void setLanguage(std::string lang);

signals:
    void signalSoundVolumn(int);
    void updateLanguage(std::string);
    
private slots:


private:
    void fnCopy(const BaseWidget&);
private:
    
};

#endif // BASEWIDGET_H
