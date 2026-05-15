#include "hgflowmodulelistwidget.h"
#include "common.h"

HGFlowModuleListWidget::HGFlowModuleListWidget(std::string lang,QWidget *parent) : QWidget(parent),
m_lang(lang)
{          
    m_getSampleLabel=new LabelWithImg(IMGRIGHTBOTTOM,12,getPath("/resources/V1/@1xiconPark-add 1.png"),"取样");
    m_quantityLabel=new LabelWithImg(IMGRIGHTBOTTOM,12,getPath("/resources/V1/@1xiconPark-division 1.png"),"定量");
    m_dilutionLabel=new LabelWithImg(IMGRIGHTBOTTOM,12,getPath("/resources/V1/@1xiconPark-more-three 1.png"),"稀释");
    m_washLabel=new LabelWithImg(IMGRIGHTBOTTOM,12,getPath("/resources/V1/@1xarcoDesign-at 1.png"),"清洗");
    m_blankLabel=new LabelWithImg(IMGRIGHTBOTTOM,12,getPath("/resources/V1/@1xarcoDesign-stop 1.png"),"打空白");
    m_waitLabel=new LabelWithImg(IMGRIGHTBOTTOM,12,getPath("/resources/V1/@1xriLine-rocket-2-line 1.png"),"等待");
    m_offsetLabel=new LabelWithImg(IMGRIGHTBOTTOM,12,getPath("/resources/V1/@1xarcoDesign-code 1.png"),"漂移值");
    m_injectReagentLabel=new LabelWithImg(IMGRIGHTBOTTOM,12,getPath("/resources/V1/@1xze-add-o 1.png"),"注入试剂");
    m_detectLabel=new LabelWithImg(IMGRIGHTBOTTOM,12,getPath("/resources/V1/@1xiconPark-acoustic 1.png"),"测定");
    m_rejectLiquidLabel=new LabelWithImg(IMGRIGHTBOTTOM,12,getPath("/resources/V1/@1xiconPark-return 1.png"),"排液");
    m_holdSampleLabel=new LabelWithImg(IMGRIGHTBOTTOM,12,getPath("/resources/V1/@1xantOutline-info-circle 1.png"),"留样");
    m_judgeLabel=new LabelWithImg(IMGRIGHTBOTTOM,12,getPath("/resources/V1/@1xarcoDesign-code 1.png"),"判定");
    m_addMedicationLabel=new LabelWithImg(IMGRIGHTBOTTOM,12,getPath("/resources/V1/@1xantOutline-info-circle 1.png"),"加药");
    m_loopLabel=new LabelWithImg(IMGRIGHTBOTTOM,12,getPath("/resources/V1/@1xarcoDesign-code 1.png"),"循环");
    m_balanceLabel=new LabelWithImg(IMGRIGHTBOTTOM,12,getPath("/resources/V1/@1xarcoDesign-code 1.png"),"平衡");
    m_injectSampleLabel=new LabelWithImg(IMGRIGHTBOTTOM,12,getPath("/resources/V1/@1xarcoDesign-code 1.png"),"进样");
    m_swirlLabel=new LabelWithImg(IMGRIGHTBOTTOM,12,getPath("/resources/V1/@1xarcoDesign-code 1.png"),"搅拌");

    connect(m_getSampleLabel,SIGNAL(clickLeftButton()),this,SLOT(slotClickGetSample()));
    connect(m_quantityLabel,SIGNAL(clickLeftButton()),this,SLOT(slotClickQuantity()));
    connect(m_dilutionLabel,SIGNAL(clickLeftButton()),this,SLOT(slotClickDilution()));
    connect(m_washLabel,SIGNAL(clickLeftButton()),this,SLOT(slotClickWash()));
    connect(m_blankLabel,SIGNAL(clickLeftButton()),this,SLOT(slotClickBlank()));
    connect(m_waitLabel,SIGNAL(clickLeftButton()),this,SLOT(slotClickWait()));
    connect(m_offsetLabel,SIGNAL(clickLeftButton()),this,SLOT(slotClickOffset()));
    connect(m_injectReagentLabel,SIGNAL(clickLeftButton()),this,SLOT(slotClickInjectReagent()));
    connect(m_detectLabel,SIGNAL(clickLeftButton()),this,SLOT(slotClickDetect()));
    connect(m_rejectLiquidLabel,SIGNAL(clickLeftButton()),this,SLOT(slotClickRejectLiquid()));
    connect(m_holdSampleLabel,SIGNAL(clickLeftButton()),this,SLOT(slotClickHoldSample()));
    connect(m_judgeLabel,SIGNAL(clickLeftButton()),this,SLOT(slotClickJudge()));
    connect(m_addMedicationLabel,SIGNAL(clickLeftButton()),this,SLOT(slotClickAddMedication()));
    connect(m_loopLabel,SIGNAL(clickLeftButton()),this,SLOT(slotClickLoop()));
    connect(m_balanceLabel,SIGNAL(clickLeftButton()),this,SLOT(slotClickBalance()));
    connect(m_injectSampleLabel,SIGNAL(clickLeftButton()),this,SLOT(slotClickInjectSample()));
    connect(m_swirlLabel,SIGNAL(clickLeftButton()),this,SLOT(slotClickSwirl()));

    connect(m_getSampleLabel,SIGNAL(createModule(std::string)),this,SLOT(slotCreateModule(std::string)));
    connect(m_quantityLabel,SIGNAL(createModule(std::string)),this,SLOT(slotCreateModule(std::string)));
    connect(m_dilutionLabel,SIGNAL(createModule(std::string)),this,SLOT(slotCreateModule(std::string)));
    connect(m_washLabel,SIGNAL(createModule(std::string)),this,SLOT(slotCreateModule(std::string)));
    connect(m_blankLabel,SIGNAL(createModule(std::string)),this,SLOT(slotCreateModule(std::string)));
    connect(m_waitLabel,SIGNAL(createModule(std::string)),this,SLOT(slotCreateModule(std::string)));
    connect(m_offsetLabel,SIGNAL(createModule(std::string)),this,SLOT(slotCreateModule(std::string)));
    connect(m_injectReagentLabel,SIGNAL(createModule(std::string)),this,SLOT(slotCreateModule(std::string)));
    connect(m_detectLabel,SIGNAL(createModule(std::string)),this,SLOT(slotCreateModule(std::string)));
    connect(m_rejectLiquidLabel,SIGNAL(createModule(std::string)),this,SLOT(slotCreateModule(std::string)));
    connect(m_holdSampleLabel,SIGNAL(createModule(std::string)),this,SLOT(slotCreateModule(std::string)));
    connect(m_judgeLabel,SIGNAL(createModule(std::string)),this,SLOT(slotCreateModule(std::string)));
    connect(m_addMedicationLabel,SIGNAL(createModule(std::string)),this,SLOT(slotCreateModule(std::string)));
    connect(m_loopLabel,SIGNAL(createModule(std::string)),this,SLOT(slotCreateModule(std::string)));
    connect(m_balanceLabel,SIGNAL(createModule(std::string)),this,SLOT(slotCreateModule(std::string)));
    connect(m_injectSampleLabel,SIGNAL(createModule(std::string)),this,SLOT(slotCreateModule(std::string)));
    connect(m_swirlLabel,SIGNAL(createModule(std::string)),this,SLOT(slotCreateModule(std::string)));

    m_getSampleLabel->setRectPainter(true);
    m_quantityLabel->setRectPainter(true);
    m_dilutionLabel->setRectPainter(true);
    m_washLabel->setRectPainter(true);
    m_blankLabel->setRectPainter(true);
    m_waitLabel->setRectPainter(true);
    m_offsetLabel->setRectPainter(true);
    m_injectReagentLabel->setRectPainter(true);
    m_detectLabel->setRectPainter(true);
    m_rejectLiquidLabel->setRectPainter(true);
    m_holdSampleLabel->setRectPainter(true);
    m_loopLabel->setRectPainter(true);
    m_addMedicationLabel->setRectPainter(true);
    m_judgeLabel->setRectPainter(true);
    m_balanceLabel->setRectPainter(true);
    m_injectSampleLabel->setRectPainter(true);
    m_swirlLabel->setRectPainter(true);

    m_contentWidget= new QWidget();
    // m_contentWidget->setFixedWidth(this->width()/4);
    m_layout=new QVBoxLayout();
    m_contentWidget->setLayout(m_layout);

    m_layout->addWidget(m_getSampleLabel);
    m_layout->addWidget(m_quantityLabel);
    m_layout->addWidget(m_dilutionLabel);
    m_layout->addWidget(m_washLabel);
    m_layout->addWidget(m_blankLabel);
    m_layout->addWidget(m_waitLabel);
    m_layout->addWidget(m_offsetLabel);
    m_layout->addWidget(m_injectReagentLabel);
    m_layout->addWidget(m_detectLabel);
    m_layout->addWidget(m_rejectLiquidLabel);
    m_layout->addWidget(m_holdSampleLabel);
    m_layout->addWidget(m_loopLabel);
    m_layout->addWidget(m_addMedicationLabel);
    m_layout->addWidget(m_judgeLabel);
    m_layout->addWidget(m_balanceLabel);
    m_layout->addWidget(m_injectSampleLabel);
    m_layout->addWidget(m_swirlLabel);

    m_scrollArea = new QScrollArea();
    m_scrollArea->setWidgetResizable(true); // 自动调整内容大小
    m_scrollArea->setWidget(m_contentWidget); // 设置滚动区域的内容

    m_mainLayout=new QVBoxLayout();
    m_mainLayout->addWidget(m_scrollArea);
    this->setLayout(m_mainLayout);
}
void HGFlowModuleListWidget::slotCreateModule(std::string name){
    emit createModule(name);
}
bool HGFlowModuleListWidget::closeWindow(){
    return true;
}
void HGFlowModuleListWidget::setAllFalse(){
    m_getSampleLabel->setFillFlag(false);
    m_quantityLabel->setFillFlag(false);
    m_dilutionLabel->setFillFlag(false);
    m_washLabel->setFillFlag(false);
    m_blankLabel->setFillFlag(false);
    m_waitLabel->setFillFlag(false);
    m_offsetLabel->setFillFlag(false);
    m_injectReagentLabel->setFillFlag(false);
    m_detectLabel->setFillFlag(false);
    m_rejectLiquidLabel->setFillFlag(false);
    m_holdSampleLabel->setFillFlag(false);
    m_loopLabel->setFillFlag(false);
    m_addMedicationLabel->setFillFlag(false);
    m_judgeLabel->setFillFlag(false);
    m_balanceLabel->setFillFlag(false);
    m_injectSampleLabel->setFillFlag(false);
    m_swirlLabel->setFillFlag(false);
}
void HGFlowModuleListWidget::slotClickGetSample(){
    setAllFalse();
    m_getSampleLabel->setFillFlag(true);
}
void HGFlowModuleListWidget::slotClickQuantity(){
    setAllFalse();
    m_quantityLabel->setFillFlag(true);

}
void HGFlowModuleListWidget::slotClickDilution(){
    setAllFalse();
    m_dilutionLabel->setFillFlag(true);

}
void HGFlowModuleListWidget::slotClickWash(){
    setAllFalse();
    m_washLabel->setFillFlag(true);

}
void HGFlowModuleListWidget::slotClickBlank(){
    setAllFalse();
    m_blankLabel->setFillFlag(true);

}
void HGFlowModuleListWidget::slotClickWait(){
    setAllFalse();
    m_waitLabel->setFillFlag(true);

}
void HGFlowModuleListWidget::slotClickOffset(){
    setAllFalse();
    m_offsetLabel->setFillFlag(true);

}
void HGFlowModuleListWidget::slotClickInjectReagent(){
    setAllFalse();
    m_injectReagentLabel->setFillFlag(true);

}
void HGFlowModuleListWidget::slotClickDetect(){
    setAllFalse();
    m_detectLabel->setFillFlag(true);

}
void HGFlowModuleListWidget::slotClickRejectLiquid(){
    setAllFalse();
    m_rejectLiquidLabel->setFillFlag(true);

}
void HGFlowModuleListWidget::slotClickHoldSample(){
    setAllFalse();
    m_holdSampleLabel->setFillFlag(true);

}
void HGFlowModuleListWidget::slotClickBalance(){
    setAllFalse();
    m_balanceLabel->setFillFlag(true);
}
void HGFlowModuleListWidget::slotClickInjectSample(){
    setAllFalse();
    m_injectSampleLabel->setFillFlag(true);
}
void HGFlowModuleListWidget::slotClickJudge(){
    setAllFalse();
    m_judgeLabel->setFillFlag(true);
}
void HGFlowModuleListWidget::slotClickAddMedication(){
    setAllFalse();
    m_addMedicationLabel->setFillFlag(true);
}
void HGFlowModuleListWidget::slotClickLoop(){
    setAllFalse();
    m_loopLabel->setFillFlag(true);
}
void HGFlowModuleListWidget::slotClickSwirl(){
    setAllFalse();
    m_swirlLabel->setFillFlag(true);
}