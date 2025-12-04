#ifndef HGEXCEPTIONHANDLEWIDGET_H
#define HGEXCEPTIONHANDLEWIDGET_H

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
#include "mychartview.h"
#include <QChart>
#include <QPieSeries>
#include "hginputsearchconditionwidget.h"

class HGExceptionHandleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGExceptionHandleWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();
    ~HGExceptionHandleWidget();

private slots:
    void togglePage(int index);
    void slotKeyWord(QString);
    void slotTimeFrom(QString);
    void slotTimeTo(QString);
    void slotSearch();
    void onSliceHovered(bool state, QPieSlice *slice, QColor originalColor);

signals:

private:
    void fnInitMaintainence();
    void fnReadDB();

private:
    HGQLabel *m_saveLabel, *m_exportLabel,*m_deleteLabel;
    QGroupBox *m_manipulateGroup, *m_upperGroup1, *m_maintenanceGroup;
    QGridLayout *m_manipulateLayout, *m_layout, *m_upperLayout1, *m_maintenanceLayout;
    QPushButton* m_errJudgeBtn;

    QGroupBox* m_noProcesGroup, *m_jiaozhengGroup, *m_shoudongGroup,*m_chooseGroup,*m_shoudongmanipulateGroup;
    QGridLayout* m_noProcessLayout, *m_jiaozhengLayout, *m_shoudongLayout,*m_chooseLayout,*m_shoudongmanipulateLayout;

    QLabel* m_functionMLabel, *m_numberLabel;
    QComboBox* m_functionMCombo, *m_numberCombo;
    QComboBox* m_jiaozhengTypeCombo,*m_jiaozhengDeviceCombo;
    QPushButton* m_startJiaozheng;
    HGInputSearchConditionWidget* m_inputsearchConditionW;
    QTableWidget* m_tableW;

    QSplitter* m_versplitter, *m_horsplitter;

    QLabel* m_exceptionLabel;
    QComboBox* m_exceptionCombo;
    QPushButton* m_detailBtn;
    QToolBox* m_toolBox;
    std::string m_lang;
    QTableWidget* m_statisticsResultTableW;

    QGroupBox* m_statisticsResultGroup;
    QGridLayout* m_statisticsLayout;

    MyChartView* m_chartView;
    QChart* m_chart;
    std::map<std::string,QColor> m_originalColors;
};

#endif // HGEXCEPTIONHANDLEWIDGET_H
