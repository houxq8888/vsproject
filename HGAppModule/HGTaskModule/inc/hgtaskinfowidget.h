#ifndef HGTASKINFOWIDGET_H
#define HGTASKINFOWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QFrame>
#include <QGridLayout>
#include <QScrollArea>
#include <QGroupBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QSpinBox>
#include "labelwithimg.h"
#include "hgaddfactorwidget.h"

class HGTaskInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGTaskInfoWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();
    void displayTaskInfo(Task info);

signals:
    void updateTaskInfo(int,Task);

private slots:
    void clickCreateTask();
    void clickRectifyTask();
    void clickSaveTask();
    void clickAddFactor();

private: 
    void fninitTaskData();
    void fnSetReadOnly(bool);
    void fnUpdateTaskInfo();
    void fnDisplayDB();
    void fnSetTaskData();

private:
    int m_editType;
    QLabel* m_sampleNameLabel;
    QLineEdit* m_sampleNameEdit;
    QGridLayout* m_layout;
    QPushButton* m_rectifyBtn,*m_saveBtn,*m_createBtn;
    QLabel *m_testFlowLabel;
    QComboBox *m_testFlowCombo;
    QLabel*m_testChannelLabel;
    QSpinBox*m_testChannelEdit;

    #ifdef ENABLE_CAS6
    QCheckBox *m_blankBox;
    QLabel* m_batchNoLabel, *m_sampleNoLabel, *m_weightLabel, *m_statusLabel;
    QLineEdit* m_batchNoEdit,*m_sampleNoEdit,*m_weightEdit;
    QLabel* m_weightUnitLabel;
    QPushButton* m_getWeightBtn;
    QRadioButton* m_statusReadyRadio, *m_statusStopDetectRadio;
    QLabel* m_circleNoLabel;
    QSpinBox* m_circleNoEdit;
    QLabel* m_whichBalanceLabel;
    QRadioButton* m_detectOverBalanceRadio,*m_backToZeroBalanceRadio,*m_directNextRadio;
    #else
    QLabel* m_sampleInputLabel,*m_getSampleMethodLabel;
    QLabel*m_testMethodLabel,*m_workModeLabel,*m_intervalLabel;
    QLineEdit* m_sampleInputEdit;
    QComboBox* m_getSampleMethodCombo;
    
   
    
    QComboBox*m_testMethodCombo,*m_workModeCombo,*m_intervalCombo;

    QLabel* m_getSamplePumpLabel;
    QComboBox* m_getSamplePumpCombo;
    
    QLabel*m_actionLabel;
    

    std::vector<HGAddFactorWidget *> m_addFactorWidgets;
    QScrollArea* m_scrollArea;
    QFrame *m_horline, *m_horline1,*m_horline2;
    QGroupBox* m_targetSampleGroup;
    QGridLayout *m_targetSampleLayout;
    LabelWithImg* m_addFactorLabel;
    #endif

    Task m_taskS;
    std::string m_lang;
};

#endif // HGTASKINFOWIDGET_H
