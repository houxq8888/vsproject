#ifndef HGFLOWMODULELISTWIDGET_H
#define HGFLOWMODULELISTWIDGET_H

#include <QWidget>
#include "labelwithimg.h"
#include <QVBoxLayout>
#include <QScrollArea>

class HGFlowModuleListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGFlowModuleListWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();

signals:
    void createModule(std::string);

private slots:
    void slotClickGetSample();
    void slotClickQuantity();
    void slotClickDilution();
    void slotClickWash();
    void slotClickBlank();
    void slotClickWait();
    void slotClickOffset();
    void slotClickInjectReagent();
    void slotClickDetect();
    void slotClickRejectLiquid();
    void slotClickHoldSample();
    void slotClickJudge();
    void slotClickAddMedication();
    void slotClickLoop();
    void slotCreateModule(std::string);
    void slotClickBalance();
    void slotClickInjectSample();
    void slotClickSwirl();

private:
    void setAllFalse();
    
private:
    LabelWithImg* m_getSampleLabel,*m_quantityLabel,*m_dilutionLabel;
    LabelWithImg* m_washLabel,*m_blankLabel,*m_waitLabel;
    LabelWithImg* m_offsetLabel,*m_injectReagentLabel, *m_detectLabel;
    LabelWithImg* m_rejectLiquidLabel, *m_holdSampleLabel;
    LabelWithImg* m_judgeLabel, *m_addMedicationLabel,*m_loopLabel;
    LabelWithImg* m_balanceLabel, *m_injectSampleLabel,*m_swirlLabel;
    QVBoxLayout* m_layout, *m_mainLayout;
    QWidget *m_contentWidget;
    QScrollArea* m_scrollArea;
    std::string m_lang;
};

#endif // HGFLOWMODULELISTWIDGET_H
