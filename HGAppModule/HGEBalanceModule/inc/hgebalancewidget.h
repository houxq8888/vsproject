#ifndef HGEBALANCEWIDGET_H
#define HGEBALANCEWIDGET_H

#include <QWidget>
#include "hgqlabel.h"
#include <QSplitter>
#include <QPushButton>
#include <QTableWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QTextEdit>
#include "mykeyboard.h"
#include "common.h"
#include "hguartwidget.h"


class HGEBalanceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGEBalanceWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();
    ~HGEBalanceWidget();

signals:
  

private slots:
   

private:
    bool eventFilter(QObject* obj,QEvent* event);
    void fnChangeParam();
    
private:
    QGridLayout* m_layout;
    std::string m_lang;

    QGroupBox* m_setGroup, *m_paramGroup;
    QGridLayout* m_setLayout, *m_paramLayout;

    QLabel* m_typeLabel,*m_interfaceLabel;
    QComboBox* m_typeComboBox,*m_interfaceComboBox;
};

#endif // HGEBALANCEWIDGET_H
