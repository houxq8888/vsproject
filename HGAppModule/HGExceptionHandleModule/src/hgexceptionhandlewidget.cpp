#include "hgexceptionhandlewidget.h"
#include <QHeaderView>
#include <QDebug>
#include "common.h"

HGExceptionHandleWidget::HGExceptionHandleWidget(std::string lang,QWidget *parent) : QWidget(parent),
m_lang(lang),
m_chartView(new MyChartView()),
m_chart(new QChart())
{
    RWDb::writeAuditTrailLog(loadTranslation(m_lang,"Enter")+loadTranslation(m_lang,"ExceptionHandle"));
    m_layout=new QGridLayout();
    this->setLayout(m_layout);
    fnInitMaintainence();

    // m_chart->setTitle(QString::fromStdString(loadTranslation(m_lang,"AbnormalDistribution")));

    // 创建视图
    m_chartView->setChart(m_chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);

    m_inputsearchConditionW=NULL;
    m_inputsearchConditionW=new HGInputSearchConditionWidget(HG_MAX_SEARCH_RANGE,m_lang);
    connect(m_inputsearchConditionW,SIGNAL(signalKeyWord(QString)),this,SLOT(slotKeyWord(QString)));
    connect(m_inputsearchConditionW,SIGNAL(signalTimeFrom(QString)),this,SLOT(slotTimeFrom(QString)));
    connect(m_inputsearchConditionW,SIGNAL(signalTimeTo(QString)),this,SLOT(slotTimeTo(QString)));
    connect(m_inputsearchConditionW,SIGNAL(signalSearch()),this,SLOT(slotSearch()));

    m_upperGroup1=new QGroupBox();
    m_upperLayout1=new QGridLayout();
    m_upperGroup1->setLayout(m_upperLayout1);
    m_manipulateGroup=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"manipulate")));//"操作");
    m_manipulateGroup->setStyleSheet("QGroupBox { font-size: 12pt; font-weight:bold;}");
    m_manipulateLayout=new QGridLayout();

    m_statisticsResultGroup=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"StatisticsResult")));
    m_statisticsLayout=new QGridLayout();
    m_statisticsResultGroup->setLayout(m_statisticsLayout);
    m_detailBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Detail")));
    m_statisticsResultTableW=new QTableWidget(2,7);
    QString data[2][7]={
        {
            QString::fromStdString(loadTranslation(m_lang,"ExceptionSource")),
            QString::fromStdString(loadTranslation(m_lang,"Device")),
            QString::fromStdString(loadTranslation(m_lang,"Reagent")),
            QString::fromStdString(loadTranslation(m_lang,"Consumable")),
            QString::fromStdString(loadTranslation(m_lang,"IPC")),
            QString::fromStdString(loadTranslation(m_lang,"SoftwareF")),
            QString::fromStdString(loadTranslation(m_lang,"AppF"))
        },
        {
            QString::fromStdString(loadTranslation(m_lang,"FrequencyStat")),
            "","","","","",""
        },
    };

    m_statisticsResultTableW->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_statisticsResultTableW->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_statisticsResultTableW->verticalHeader()->setVisible(false);
    m_statisticsResultTableW->horizontalHeader()->setVisible(false);
    m_statisticsResultTableW->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for (int row=0;row<2;++row){
        for (int col=0;col<7;++col){
            QTableWidgetItem* item=new QTableWidgetItem(data[row][col]);
            m_statisticsResultTableW->setItem(row,col,item);
            if (col==0) item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        }
    }

    m_statisticsLayout->addWidget(m_chartView,0,0,2,1);
    m_statisticsLayout->addWidget(m_statisticsResultTableW,3,0,1,1);
    m_statisticsLayout->addWidget(m_detailBtn,1,1);

    m_exportLabel=new HGQLabel(false,getPath("/resources/V1/@1xiconPark-export 1.png"));
    m_saveLabel=new HGQLabel(false,getPath("/resources/V1/@1xmb-save 1.png")); 
    m_deleteLabel=new HGQLabel(false,getPath("/resources/V1/@1xif-bin 1.png"));
            
    
    QStringList headers={
        QString::fromStdString(loadTranslation(m_lang,"WarnTime")),
        QString::fromStdString(loadTranslation(m_lang,"Operator")),
        QString::fromStdString(loadTranslation(m_lang,"InstrumentID")),
        QString::fromStdString(loadTranslation(m_lang,"WarnContent")),
        QString::fromStdString(loadTranslation(m_lang,"ErrCode")),
        QString::fromStdString(loadTranslation(m_lang,"WarnCount")),
        QString::fromStdString(loadTranslation(m_lang,"State")),
        QString::fromStdString(loadTranslation(m_lang,"RecommandSolution")),
        QString::fromStdString(loadTranslation(m_lang,"Solution"))};
    m_tableW=new QTableWidget(0,headers.size());
    m_tableW->setHorizontalHeaderLabels(headers);
    m_tableW->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableW->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_tableW->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_tableW->resizeRowsToContents();

    m_errJudgeBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"ErrJudge")));//"故障诊断");

    m_noProcesGroup=new QGroupBox();
    m_noProcessLayout=new QGridLayout();
    m_noProcesGroup->setLayout(m_noProcessLayout);
    m_noProcessLayout->addWidget(m_tableW,1,0,1,10);
    m_noProcessLayout->addWidget(m_errJudgeBtn,0,7,1,1);

    m_versplitter=new QSplitter();
    m_versplitter->setOrientation(Qt::Vertical);
    m_versplitter->setStyleSheet("QSplitter::handle{background-color:lightgray;}");

    m_manipulateLayout->addWidget(m_exportLabel,0,1);
    m_manipulateLayout->addWidget(m_saveLabel,0,2);
    m_manipulateLayout->addWidget(m_deleteLabel,0,3);
    m_manipulateGroup->setLayout(m_manipulateLayout);

    m_exceptionLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"ExceptionStatistics")));
    m_exceptionCombo=new QComboBox();
    m_exceptionCombo->addItems({QString::fromStdString(loadTranslation(m_lang,"All"))});

    m_toolBox=new QToolBox();
    m_toolBox->addItem(m_noProcesGroup,QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))),
        QString::fromStdString(loadTranslation(m_lang,"UntreatedInfo")));//"未处理设备信息");
    // m_toolBox->addItem(m_errJudgeContent,QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))),
    //     QString::fromStdString(loadTranslation(m_lang,"Diagnostic")));//"诊断信息");
    // m_toolBox->addItem(m_maintenanceGroup,QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))),
    //     QString::fromStdString(loadTranslation(m_lang,"MaintainManage")));//"维护管理");
    connect(m_toolBox,SIGNAL(currentChanged(int)),this,SLOT(togglePage(int)));

    m_upperLayout1->addWidget(m_exceptionLabel,0,0);
    m_upperLayout1->addWidget(m_exceptionCombo,0,1);
    m_upperLayout1->addWidget(m_inputsearchConditionW,1,0,1,2);
    m_upperLayout1->addWidget(m_statisticsResultGroup,0,3,2,1);

    m_versplitter->addWidget(m_upperGroup1);
    m_versplitter->addWidget(m_toolBox);

    m_layout->addWidget(m_versplitter);

    fnReadDB();
}
void HGExceptionHandleWidget::fnReadDB()
{
    std::map<std::string,int> exceptionCount;
    exceptionCount["Device"]=15;
    exceptionCount["Reagent"]=25;
    exceptionCount["Consumable"]=30;
    exceptionCount["IPC"]=15;
    exceptionCount["SoftwareF"]=10;
    exceptionCount["AppF"]=5;
    // 创建数据系列
    QPieSeries *series = new QPieSeries();

    for (const auto count : exceptionCount){
        QString name=QString::fromStdString(loadTranslation(m_lang,count.first));
        series->append(name,count.second);

        for (int col = 0; col < m_statisticsResultTableW->columnCount(); ++col)
        {
            QTableWidgetItem *item = m_statisticsResultTableW->item(0, col);
            if (item)
            {
                if (item->text()==name){
                    m_statisticsResultTableW->setItem(1, col, new QTableWidgetItem(QString::number(count.second)));
                    break;
                }
            }
        }
    }

    // 遍历所有切片，计算并设置比例数字
    double total = 0;
    for (QPieSlice *slice : series->slices())
    {
        QObject::connect(slice, &QPieSlice::hovered, [=](bool state) {  
            std::string name=slice->label().toStdString();
            QColor color=m_originalColors.at(name);
            onSliceHovered(state, slice, color); 
        });
        total += slice->value();
        // slice->setLabelPosition(QPieSlice::LabelInsideHorizontal);  // 切片内，水平居中（默认）
        // slice->setLabelPosition(QPieSlice::LabelInsideNormal);       // 切片内，垂直方向排列
        // slice->setLabelPosition(QPieSlice::LabelOutside);  
    }

    for (QPieSlice *slice : series->slices())
    {
        double percentage = (slice->value() / total) * 100;
        slice->setLabel(QString("%1 (%2%)").arg(slice->label()).arg(percentage, 0, 'f', 1));
        slice->setLabelVisible(true); // 确保标签可见
    }

    // 创建图表
    m_chart->addSeries(series);
    // 获取图例并设置位置
    m_chart->legend()->setVisible(true);             // 显示图例
    m_chart->legend()->setAlignment(Qt::AlignRight); // 设置图例在右侧

    for (QPieSlice *slice : series->slices()){
        m_originalColors[slice->label().toStdString()]=slice->brush().color();
    }
}
// 处理鼠标悬停事件的槽函数
void HGExceptionHandleWidget::onSliceHovered(bool state, QPieSlice *slice, QColor originalColor) {
    if (state) {
        slice->setExploded(true);   // 让切片突出
        slice->setLabelVisible(true); // 显示标签
        slice->setBrush(Qt::red); // 改变颜色
    } else {
        slice->setExploded(false);
        slice->setBrush(originalColor); // 还原颜色
    }
}
bool HGExceptionHandleWidget::closeWindow()
{
    if (m_inputsearchConditionW){
        if (m_inputsearchConditionW->closeWindow()){
            SAFE_DELETE(m_inputsearchConditionW);
        }
    }
    return true;
}
HGExceptionHandleWidget::~HGExceptionHandleWidget()
{

}
void HGExceptionHandleWidget::fnInitMaintainence()
{
    m_horsplitter=new QSplitter();
    m_horsplitter->setOrientation(Qt::Horizontal);
    m_horsplitter->setStyleSheet("QSplitter::handle{background-color:lightgray;}");

    m_jiaozhengTypeCombo=new QComboBox();
    m_jiaozhengDeviceCombo=new QComboBox();
    m_startJiaozheng=new QPushButton();
    m_jiaozhengTypeCombo->addItems({"电极校正","滴定管校正", "滴定剂校正", "辅助泵校正", "波长校正"});
    m_startJiaozheng->setText(QString::fromStdString(loadTranslation(m_lang,"start calibrating")));//"开始校正");
   
    m_functionMLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"FunctionModule")));//"功能模块");
    m_numberLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"No.")));//"编号");
    m_functionMCombo=new QComboBox();
    m_numberCombo=new QComboBox();

    m_jiaozhengGroup=new QGroupBox();
    m_jiaozhengLayout=new QGridLayout();
    m_jiaozhengGroup->setLayout(m_jiaozhengLayout);

    m_shoudongGroup=new QGroupBox();
    m_chooseGroup=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"choose")));//"选择对象");
    m_shoudongmanipulateGroup=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"manual")));//"手动操作");
    m_shoudongLayout=new QGridLayout();
    m_chooseLayout=new QGridLayout();
    m_shoudongmanipulateLayout=new QGridLayout();
    m_shoudongGroup->setLayout(m_shoudongLayout);
    m_chooseGroup->setLayout(m_chooseLayout);
    m_shoudongmanipulateGroup->setLayout(m_shoudongmanipulateLayout);

    m_chooseLayout->addWidget(m_functionMLabel,0,0);
    m_chooseLayout->addWidget(m_functionMCombo,0,1);
    m_chooseLayout->addWidget(m_numberLabel,1,0);
    m_chooseLayout->addWidget(m_numberCombo,1,1);

    m_shoudongLayout->addWidget(m_chooseGroup,0,0);
    m_shoudongLayout->addWidget(m_shoudongmanipulateGroup,1,0);

    m_jiaozhengLayout->addWidget(m_jiaozhengTypeCombo,0,0);
    m_jiaozhengLayout->addWidget(m_jiaozhengDeviceCombo,0,1);
    m_jiaozhengLayout->addWidget(m_startJiaozheng);

    m_maintenanceGroup=new QGroupBox();
    m_maintenanceLayout=new QGridLayout();
    m_maintenanceGroup->setLayout(m_maintenanceLayout);

    m_horsplitter->addWidget(m_jiaozhengGroup);
    m_horsplitter->addWidget(m_shoudongGroup);
    m_maintenanceLayout->addWidget(m_horsplitter);
}
void HGExceptionHandleWidget::togglePage(int index){
    int cnt=m_toolBox->count();
    for (int i=0;i<cnt;i++){
        if (i==index) m_toolBox->setItemIcon(i,QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow-down 1.png"))));
        else m_toolBox->setItemIcon(i,QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))));
    }
    
}
void HGExceptionHandleWidget::slotKeyWord(QString text){

}
void HGExceptionHandleWidget::slotTimeFrom(QString text){

}
void HGExceptionHandleWidget::slotTimeTo(QString text){

}
void HGExceptionHandleWidget::slotSearch() {

}