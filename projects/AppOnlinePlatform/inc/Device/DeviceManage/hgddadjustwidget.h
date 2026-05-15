#ifndef HGDDADJUSTWIDGET_H
#define HGDDADJUSTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QFrame>
#include <QComboBox>

class HGDDAdjustWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGDDAdjustWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();

signals:

private slots:
    void slotReadDB();
    void slotOK();
    void slotAjustAndSave();
    void slotBack();
    void slotApply();
    void slotCancel();
    void slotAdjust();
    
private:
    QLabel *m_nameLabel,*m_adjustNameLabel,*m_adjustPhaseLabel,*m_cubeRangeLabel;
    QLabel *m_standardLabel,*m_unitLabel,*m_adjustTempLabel,*m_adjustTimeLabel;
    QLabel *m_adjustCircleLabel;
    QLineEdit *m_voltageUnitEdit;
    
    QComboBox* m_adjustNameCombo,*m_adjustPhaseCombo,*m_cubeRangeCombo;
    QComboBox* m_unitCombo;
    QLineEdit *m_adjustTempEdit,*m_adjustTimeEdit,*m_adjustCircleEdit;
    QLineEdit *m_standardEdit;

    QPushButton *m_adjustBtn,*m_okBtn,*m_cancelBtn,*m_readBtn;
    QPushButton *m_cancelABtn,*m_applyBtn,*m_adjustAndSaveToDBBtn;

    QFrame *m_line;
    QGridLayout *m_layout;
    std::string m_lang;
};

#endif // HGDDADJUSTWIDGET_H
