#ifndef HGDDJICALIBRATIONWIDGET_H
#define HGDDJICALIBRATIONWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include "hgqlabel.h"
#include <QFrame>
#include <QComboBox>

class HGDDjiCalibrationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGDDjiCalibrationWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();

signals:

private slots:
    void slotInject();
    void slotBack();
    void slotInput();
    void slotInjectWash();
    void slotBackWash();
    void slotSaveResult();
    void clickBackWindow();
    void clickOpenMethod();

private:
    QLabel *m_nameLabel,*m_prepareLabel,*m_methodLabel,*m_standardLabel;
    QLabel *m_otherLabel,*m_cubeLabel,*m_nongduLabel;
    QLabel* m_unitLabel, *m_ddMethodLabel;

    QComboBox* m_methodCombo, *m_standardCombo, *m_unitCombo;
    QLineEdit *m_otherEdit,*m_cubeEdit;
    QLineEdit *m_nongduEdit,*m_ddMethodEdit;

    QPushButton *m_saveResultBtn;
    QPushButton *m_injectBtn,*m_backBtn,*m_inputBtn,*m_injectWashBtn,*m_backWashBtn;
    HGQLabel* m_backLabel,*m_openLabel;
    QGridLayout *m_layout;
    std::string m_lang;
};

#endif // HGDDJICALIBRATIONWIDGET_H
