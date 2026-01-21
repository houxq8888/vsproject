#include "hgdisplayrecordwidget.h"
#include <QHeaderView>
#include "common.h"
#include <QToolTip>

HGDisplayRecordWidget::HGDisplayRecordWidget(std::string lang,QWidget *parent) : QWidget(parent),
    m_lang(lang)
{
    m_layout=new QGridLayout();
    this->setLayout(m_layout);
    m_inputsearchConditionW=NULL;
    m_inputsearchConditionW=new HGInputSearchConditionWidget(HG_MAX_SEARCH_RANGE,m_lang);

    m_manipulateGroup=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"manipulate")));//"操作");
    m_manipulateGroup->setStyleSheet("QGroupBox { font-size: 12pt; font-weight:bold;}");
    m_manipulateLayout=new QGridLayout();

    m_exportLabel=new HGQLabel(false,getPath("/resources/V1/@1xiconPark-export 1.png"));
    m_deleteLabel=new HGQLabel(false,getPath("/resources/V1/@1xif-bin 1.png"));
    m_saveLabel=new HGQLabel(false,getPath("/resources/V1/@1xmb-save 1.png"));
 
    m_multipleChooseBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"MultiChoose")));//"多选");
    m_displayBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Scan")));//"查看");
    m_staticBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Statistics")));//"统计");
    m_printBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Print")));//"打印");

    
    QStringList headers;
    #ifdef ENABLE_CAS6
    headers=QStringList{"序号", "时间", "用户", "样品名", "编号", "样品总量", "水质量", "结果"};
    #else
    headers=QStringList{"时间","通道","样品池","目标成分","测试含量","操作员"/*,"分析时长","水质量","测量电压","电解速率","漂移值"*/};
    #endif
    m_tableW=new QTableWidget(0,headers.size());  
    m_tableW->setHorizontalHeaderLabels(headers);
    m_tableW->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableW->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_tableW->verticalHeader()->setVisible(false);
    m_tableW->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableW->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_tableW->resizeRowsToContents();
    m_tableW->setMouseTracking(true);

    connect(m_tableW, SIGNAL(cellClicked(int, int)), this, SLOT(onCellClicked(int, int)));
    connect(m_multipleChooseBtn,SIGNAL(clicked()),this,SLOT(slotMultipleChoose()));
    connect(m_displayBtn,SIGNAL(clicked()),this,SLOT(slotScan()));
    connect(m_staticBtn,SIGNAL(clicked()),this,SLOT(slotStatistics()));
    connect(m_printBtn,SIGNAL(clicked()),this,SLOT(slotPrint()));

    connect(m_tableW, &QTableWidget::cellEntered, [&](int row, int column)
    {
        QTableWidgetItem *item = m_tableW->item(row, column);
        if (item) {
            QToolTip::showText(QCursor::pos(), item->text(), m_tableW);// 动态设置 ToolTip
        }
    });

    m_timer=new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(onTimerTimeout()));
    // m_timer->start(1000);

    m_manipulateLayout->addWidget(m_exportLabel,0,1);
    m_manipulateLayout->addWidget(m_saveLabel,0,2);
    m_manipulateLayout->addWidget(m_deleteLabel,0,3);
    m_manipulateLayout->addWidget(m_multipleChooseBtn,0,6);
    m_manipulateLayout->addWidget(m_displayBtn,0,7);
    m_manipulateLayout->addWidget(m_staticBtn,0,8);
    m_manipulateLayout->addWidget(m_printBtn,0,9);
    m_manipulateLayout->addWidget(m_tableW,1,0,1,10);
    m_manipulateGroup->setLayout(m_manipulateLayout);

    m_layout->addWidget(m_inputsearchConditionW,0,1);
    m_layout->addWidget(m_manipulateGroup,1,1);

    onTimerTimeout();
}
void HGDisplayRecordWidget::onTimerTimeout(){
    std::vector<std::map<std::string,std::string>> taskInfos=RWDb::getTaskRunInfo();
    m_taskInfos.clear();
    m_taskInfos=taskInfos;
    if (int(taskInfos.size()) <= 0) return;
    for (int index=0;index<int(taskInfos.size());index++){
        fnDisplayDataInfo(index,taskInfos[index]);
    }
}
void HGDisplayRecordWidget::fnDisplayDataInfo(int count,std::map<std::string,std::string> fillContent){
    if (count >= m_tableW->rowCount()) m_tableW->insertRow(m_tableW->rowCount());
    m_tableW->setEditTriggers(QAbstractItemView::AllEditTriggers);
    m_tableW->setItem(count, 0, new QTableWidgetItem(QString::fromStdString(std::to_string(count))));

    for (auto content : fillContent)
    {
        int nameColIndex = getColumnIndexByName(m_tableW, QString::fromStdString(content.first));
        if (nameColIndex < 0 || nameColIndex >= m_tableW->columnCount())
            continue;
        m_tableW->setItem(count, nameColIndex, new QTableWidgetItem(QString::fromStdString(content.second)));
    }
    // std::vector<std::string> samplePools={"样品池"/*,"测试含量","操作员"*/};
    // std::vector<std::string> targetSample={"样品来源"/*,"样品总量"*/};
    // int index=0;
    // for (auto content : samplePools)
    // {
    //     int nameColIndex = getColumnIndexByName(m_tableW, QString::fromStdString(content));
    //     if (nameColIndex < 0 || nameColIndex >= m_tableW->columnCount())
    //         continue;
    //     std::string contentName = targetSample[index++];
    //     m_tableW->setItem(count, nameColIndex, new QTableWidgetItem(QString::fromStdString(fillContent[contentName])));
    // }

    m_tableW->setEditTriggers(QAbstractItemView::NoEditTriggers);
}
void HGDisplayRecordWidget::showDataInfo(const std::map<std::string,std::string> &info){
    if (info.size()<=0) return;
    int index=m_tableW->rowCount();
    fnDisplayDataInfo(index,info);
}
bool HGDisplayRecordWidget::closeWindow()
{
    if (m_inputsearchConditionW){
        if (m_inputsearchConditionW->closeWindow()){
            SAFE_DELETE(m_inputsearchConditionW);
        }
    }
    return true;
}
HGDisplayRecordWidget::~HGDisplayRecordWidget()
{
    
}
void HGDisplayRecordWidget::onCellClicked(int row,int column){
    if (row<0||row>=m_tableW->rowCount()) return;
    m_tableW->selectRow(row);
    std::map<std::string,std::string> info=m_taskInfos[row];
    emit signalShowDataInfo(info);
}
void HGDisplayRecordWidget::slotMultipleChoose(){

}
void HGDisplayRecordWidget::slotScan(){
    
}
void HGDisplayRecordWidget::slotStatistics(){
    
}
void HGDisplayRecordWidget::slotPrint(){
    emit showPrinter();
}