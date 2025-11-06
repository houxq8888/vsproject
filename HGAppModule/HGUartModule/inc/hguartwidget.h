#ifndef HGUARTWIDGET_H
#define HGUARTWIDGET_H

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


class HGUartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGUartWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();
    ~HGUartWidget();

signals:
  

private slots:
   

private:
    bool eventFilter(QObject* obj,QEvent* event);

private:
    QGridLayout* m_layout;
    std::string m_lang;

    QGroupBox* m_baseGroup, *m_advanceGroup;
    QGridLayout* m_baseLayout, *m_advanceLayout;

    QLabel* m_comSelectLabel,*m_baudrateLabel,*m_dataBitsLabel,*m_stopBitsLabel,*m_parityLabel,*m_flowControlLabel;
    QComboBox* m_comSelectComboBox,*m_baudrateComboBox,*m_dataBitsComboBox,*m_stopBitsComboBox,*m_parityComboBox,*m_flowControlComboBox;
    QPushButton* m_openButton,*m_sendButton;

    QTextEdit* m_receiveTextEdit;
    QLabel* m_captureLabel;
    QLineEdit* m_captureLineEdit;
    QCheckBox* m_enterCheck, *m_returnCheck;

    HGSerial* m_serialPort;
};

#endif // HGUARTWIDGET_H
