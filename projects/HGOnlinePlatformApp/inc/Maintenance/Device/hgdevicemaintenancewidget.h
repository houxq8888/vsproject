#ifndef HGDEVICEMAINTENANCEWIDGET_H
#define HGDEVICEMAINTENANCEWIDGET_H

#include <QWidget>
#include "hgqlabel.h"
#include <QSplitter>
#include <QToolBox>
#include <QPushButton>
#include <QTableWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QComboBox>
#include "hginputsearchconditionwidget.h"

class HGDeviceMaintenanceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGDeviceMaintenanceWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();
    ~HGDeviceMaintenanceWidget();

private slots:
    void togglePage(int index);
    void slotClickOk();

signals:

private:
    void fnInitMaintainence();

private:
  //  HGQLabel *m_saveLabel, *m_exportLabel,*m_deleteLabel;
    QGroupBox *m_warnSetGroup, *m_upperGroup1, *m_maintenanceGroup;
    QGridLayout *m_warnSetLayout, *m_layout, *m_upperLayout1, *m_maintenanceLayout;
    // QPushButton* m_errJudgeBtn;

    QGroupBox* m_definitionGroup, *m_jiaozhengGroup, *m_shoudongGroup,*m_chooseGroup,*m_shoudongmanipulateGroup;
    QGridLayout* m_definitionLayout, *m_jiaozhengLayout, *m_shoudongLayout,*m_chooseLayout,*m_shoudongmanipulateLayout;

    QLabel* m_functionMLabel, *m_numberLabel;
    QComboBox* m_functionMCombo, *m_numberCombo;
    QComboBox* m_jiaozhengTypeCombo,*m_jiaozhengDeviceCombo;
    QPushButton* m_startJiaozheng, *m_okbtn;
    // HGInputSearchConditionWidget* m_inputsearchConditionW;
    QTableWidget* m_tableW;

    QSplitter* m_versplitter, *m_horsplitter;

    QLabel* m_typeLabel,*m_linkedDeviceLabel,*m_errTypeLabel,*m_objectLabel,*m_deviceNoLabel,*m_warnTypeLabel;
    QComboBox* m_typeCombo,*m_linkedDeviceCombo,*m_errTypeCombo,*m_objectCombo,*m_deviceNoCombo,*m_warnTypeCombo;


    QToolBox* m_toolBox;
    std::string m_lang;
};

#endif // HGDEVICEMAINTENANCEWIDGET_H
