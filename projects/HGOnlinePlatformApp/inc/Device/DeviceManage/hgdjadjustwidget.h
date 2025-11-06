#ifndef HGDJADJUSTWIDGET_H
#define HGDJADJUSTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QFrame>


class HGDJAdjustWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGDJAdjustWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();

signals:

private slots:
    void slotReadDB();
    void slotOK();
    void slotAjustAndSave();
    void slotBack();
    void slotApply();
    
private:
    QLabel *m_nameLabel,*m_curTempLabel,*m_ddAjustLabel,*m_adjustDSLabel;
    QLabel *m_voltageLabel,*m_standardLabel,*m_adjustTimeLabel;
    QLineEdit *m_voltageUnitEdit,*m_standardUnitEdit;

    QLineEdit *m_curTempEdit,*m_ddAdjustEdit,*m_adjustDSEdit,*m_voltageEdit;
    QLineEdit *m_standardEdit,*m_adjustTimeEdit;

    QPushButton *m_readBtn,*m_okBtn,*m_adjustAndSaveToDBBtn,*m_backBtn,*m_applyBtn;

    QFrame *m_line,*m_line1;
    QGridLayout *m_layout;
    std::string m_lang;
};

#endif // HGDJADJUSTWIDGET_H
