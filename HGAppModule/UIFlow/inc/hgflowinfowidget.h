#ifndef HGFLOWINFOWIDGET_H
#define HGFLOWINFOWIDGET_H

#include <QWidget>
#include <QToolBox>
#include <QGridLayout>
#include "hgblankwidget.h"
#include "hgdetectwidget.h"
#include "hggetsamplewidget.h"
#include "hgholdsamplewidget.h"
#include "hginjectreagentwidget.h"
#include "hgwashwidget.h"
#include "hgloopwidget.h"
#include "hgwaitwidget.h"
#include "hgjudgewidget.h"
#include "hgaddmedicationwidget.h"
#include "hgbalancewidget.h"
#include "hgswirlstepwidget.h"
#include "hginjectsamplewidget.h"
#include <QEvent>
#include "common.h"


class HGFlowInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGFlowInfoWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();
    void createModule(std::string name,std::string param="");
    std::vector<StepOfFlow> getStepOfFlow();
    void showFlow(FlowOfTask);

signals:

private slots:
    void togglePage(int index);
    void slotManipulateObject(std::string,int);

private:
    void swapItem(int preIndex,int curIndex);
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QToolBox* m_toolBox;
    QGridLayout* m_layout;
    std::map<int,HGblankWidget*> m_blankW;
    std::map<int,HGDetectWidget*> m_detectW;
    std::map<int,HGGetSampleWidget*> m_getsampleW;
    std::map<int,HGHoldSampleWidget*> m_holdSampleW;
    std::map<int,HGinjectReagentWidget*> m_injectReagentW;
    std::map<int,HGWashWidget*> m_washW;
    std::map<int,HGAddMedicationWidget*> m_addMedicationW;
    std::map<int,HGJudgeWidget*> m_judgeW;
    std::map<int,HGLoopWidget*> m_loopW;
    std::map<int,HGWaitWidget*> m_waitW;
    std::map<int,HGBalanceWidget*> m_balanceW;
    std::map<int,HGInjectSampleWidget*> m_injectSampleW;
    std::map<int,HGSwirlStepWidget*> m_swirlW;

    std::vector<StepOfFlow> m_steps;
    int m_lastIndex;
    HGManipulateWidget* m_manipulateW;
    std::string m_lang;

};

#endif // HGFLOWINFOWIDGET_H

