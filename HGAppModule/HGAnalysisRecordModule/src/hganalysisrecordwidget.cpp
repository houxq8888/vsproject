#include "hganalysisrecordwidget.h"
#include "mypixmapitem.h"
#include "common.h"
#include <QPrinter>
#include <QPrintDialog>
#include <QTextDocument>
#include <QMessageBox>
#include <QGraphicsOpacityEffect>


HGAnalysisRecordWidget::HGAnalysisRecordWidget(std::string lang,QWidget *parent) : QWidget(parent),
    m_lang(lang),
    m_printW(nullptr)
{
    RWDb::writeAuditTrailLog(loadTranslation(m_lang,"Enter")+loadTranslation(m_lang,"AnalysisRecord"));
    m_displayRecordW=NULL;
    m_displayChartsW=NULL;
    m_displayRecordInfoW=NULL;
    m_displayRecordW=new HGDisplayRecordWidget(m_lang);
    std::vector<std::string> chartNames={
        "时间~体积",
        "时间~电压"
    };
    m_displayChartsW=new HGDisplayChartWidget(m_lang,chartNames);
    m_displayRecordInfoW=new HGDisplayRecordInfoWidget(m_lang,loadTranslation(m_lang,"TestInfo"));//"测试信息");

    connect(m_displayRecordW,&HGDisplayRecordWidget::signalShowDataInfo,this,&HGAnalysisRecordWidget::slotShowDataInfo);
    connect(m_displayRecordW,&HGDisplayRecordWidget::showPrinter,this,&HGAnalysisRecordWidget::slotShowPrinter);

    m_versplitter=new QSplitter();
    m_versplitter->setOrientation(Qt::Vertical);
    m_versplitter->setStyleSheet("QSplitter::handle{background-color:lightgray;}");

    m_horsplitter=new QSplitter();
    m_horsplitter->setOrientation(Qt::Horizontal);
    m_horsplitter->setStyleSheet("QSplitter::handle{background-color:lightgray;}");

    m_rightBox=new QGroupBox();
    m_verlayout=new QVBoxLayout();
    m_rightBox->setLayout(m_verlayout);
    m_versplitter->addWidget(m_displayChartsW);
    m_versplitter->addWidget(m_displayRecordInfoW);
    m_verlayout->addWidget(m_versplitter);

    m_horlayout=new QHBoxLayout();
    m_horsplitter->addWidget(m_displayRecordW);
    m_horsplitter->addWidget(m_rightBox);
    m_horlayout->addWidget(m_horsplitter);
    this->setLayout(m_horlayout);

    m_timer=new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(onTimeout()));
    m_timer->start(1000);
}
void HGAnalysisRecordWidget::onTimeout(){
    fnUpdateDataInfo();
}
void HGAnalysisRecordWidget::fnUpdateDataInfo(){
    std::map<std::string,std::string> dataInfo; //=getDataInfo();
    // m_displayRecordW->showDataInfo(dataInfo);
    m_displayChartsW->showDataInfo(dataInfo);
    // m_displayRecordInfoW->showTestInfo(dataInfo);
}
bool HGAnalysisRecordWidget::closeWindow()
{
    if (m_displayRecordW){
        if (m_displayRecordW->closeWindow()){
            SAFE_DELETE(m_displayRecordW);
        }
    }
    if (m_displayChartsW){
        if (m_displayChartsW->closeWindow()){
            SAFE_DELETE(m_displayChartsW);
        }
    }
    if (m_displayRecordInfoW){
        if (m_displayRecordInfoW->closeWindow()){
            SAFE_DELETE(m_displayRecordInfoW);
        }
    }
    return true;
}
HGAnalysisRecordWidget::~HGAnalysisRecordWidget()
{
    
}
void HGAnalysisRecordWidget::slotShowDataInfo(std::map<std::string,std::string> info){
    m_displayRecordInfoW->showTestInfo(info);
    m_displayChartsW->showTestInfo(info["DBName"]);
}

void HGAnalysisRecordWidget::slotShowPrinter(){
    std::vector<std::string> printNames={"PrintName","PrintData","PrintTime","PrintUser","PrintIP","PrintMac","PrintModel",\
            "PrintSerialNumber","PrintSoftwareVersion","PrintHardwareVersion","PrintStatus","PrintResult","PrintRemark","PrintSignature",\
            "PrintSignatureTime","PrintSignatureUser","PrintSignatureIP","PrintSignatureMac","PrintSignatureModel","PrintSignatureSerialNumber",\
            "PrintSignatureSoftwareVersion","PrintHardwareVersion","PrintSignatureHardwareVersion","PrintSignatureStatus"};
    
    m_printW=new HGPrintWidget(m_lang,"Report",printNames);
    std::vector<InPrintOption> printDatasList;
    printDatasList.resize(2);
    for (int m = 0; m < int(printDatasList.size()); m++)
    {
        for (int i = 0; i < int(printNames.size()); i++)
        {
            printDatasList[m].printTitle = "TestReport" + std::to_string(m);
            printDatasList[m].inPrintElements.push_back(InPrintElement(printNames[i], ""));
        }
    }
    m_printW->setPrintData(printDatasList);

    connect(m_printW,&HGPrintWidget::signalBackClicked,[=](){
        m_horsplitter->replaceWidget(m_horsplitter->indexOf(m_printW), m_displayRecordW);
    });

    m_horsplitter->replaceWidget(m_horsplitter->indexOf(m_displayRecordW), m_printW);

}