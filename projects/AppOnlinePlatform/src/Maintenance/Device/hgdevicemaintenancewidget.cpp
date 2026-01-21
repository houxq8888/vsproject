#include "hgdevicemaintenancewidget.h"
#include <QHeaderView>
#include <QDebug>
#include "common.h"

HGDeviceMaintenanceWidget::HGDeviceMaintenanceWidget(std::string lang,QWidget *parent) : QWidget(parent),
m_lang(lang)
{
    RWDb::writeAuditTrailLog(loadTranslation(m_lang,"Enter")+loadTranslation(m_lang,"DeviceMaintenance"));
    m_layout=new QGridLayout();
    this->setLayout(m_layout);
    fnInitMaintainence();
    // m_inputsearchConditionW=NULL;
    // m_inputsearchConditionW=new HGInputSearchConditionWidget(HG_MAX_SEARCH_RANGE,m_lang);

    m_upperGroup1=new QGroupBox();
    m_upperLayout1=new QGridLayout();
    m_upperGroup1->setLayout(m_upperLayout1);
    m_warnSetGroup=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"WarningSet")));//"操作");
    m_warnSetGroup->setStyleSheet("QGroupBox { font-size: 12pt; font-weight:bold;}");
    m_warnSetLayout=new QGridLayout();

    // m_exportLabel=new HGQLabel(false,getPath("/resources/V1/@1xiconPark-export 1.png"));
    // m_saveLabel=new HGQLabel(false,getPath("/resources/V1/@1xmb-save 1.png")); 
    // m_deleteLabel=new HGQLabel(false,getPath("/resources/V1/@1xif-bin 1.png"));      
    QStringList headers={
        QString::fromStdString(loadTranslation(m_lang,"Index")),
        QString::fromStdString(loadTranslation(m_lang,"Object")),
        QString::fromStdString(loadTranslation(m_lang,"No.")),
        QString::fromStdString(loadTranslation(m_lang,"ErrorType")),
        QString::fromStdString(loadTranslation(m_lang,"WarnType")),
        QString::fromStdString(loadTranslation(m_lang,"LinkedDevice")),
        QString::fromStdString(loadTranslation(m_lang,"DeviceNumber")),
        QString::fromStdString(loadTranslation(m_lang,"CreateTime")),
        QString::fromStdString(loadTranslation(m_lang,"Operator"))
    };
    m_tableW=new QTableWidget(0,headers.size());
    m_tableW->setHorizontalHeaderLabels(headers);
    m_tableW->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableW->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_tableW->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_tableW->resizeRowsToContents();

    m_definitionGroup=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"DefinitionList")));
    m_definitionLayout=new QGridLayout();
    m_definitionGroup->setLayout(m_definitionLayout);
    m_definitionLayout->addWidget(m_tableW,0,0);
    // m_definitionLayout->addWidget(m_errJudgeBtn,0,1);

    m_versplitter=new QSplitter();
    m_versplitter->setOrientation(Qt::Vertical);
    m_versplitter->setStyleSheet("QSplitter::handle{background-color:lightgray;}");

    m_typeLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"SelectType")));
    m_linkedDeviceLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"LinkedDevice")));
    m_errTypeLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"ErrorType")));
    m_objectLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"Object")));
    m_deviceNoLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"DeviceNumber")));
    m_warnTypeLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"WarnType")));
    m_typeCombo=new QComboBox();
    m_typeCombo->addItems({
        QString::fromStdString(loadTranslation(m_lang,"Device")),
        QString::fromStdString(loadTranslation(m_lang,"Reagent")),
        QString::fromStdString(loadTranslation(m_lang,"Consumable")),
        QString::fromStdString(loadTranslation(m_lang,"IPC")),
        QString::fromStdString(loadTranslation(m_lang,"SoftwareF")),
        QString::fromStdString(loadTranslation(m_lang,"AppF"))
    });
    m_linkedDeviceCombo=new QComboBox();
    m_errTypeCombo=new QComboBox();
    m_objectCombo=new QComboBox();
    m_deviceNoCombo=new QComboBox();
    m_warnTypeCombo=new QComboBox();
    m_warnTypeCombo->addItems({
        QString::fromStdString(loadTranslation(m_lang,"Tips")),
        QString::fromStdString(loadTranslation(m_lang,"Warn")),
        QString::fromStdString(loadTranslation(m_lang,"Error"))
    });
    m_okbtn = new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Ok")));
    m_warnSetLayout->addWidget(m_typeLabel,0,1);
    m_warnSetLayout->addWidget(m_typeCombo,0,2);
    m_warnSetLayout->addWidget(m_linkedDeviceLabel,0,3);
    m_warnSetLayout->addWidget(m_linkedDeviceCombo,0,4);
    m_warnSetLayout->addWidget(m_errTypeLabel,0,5);
    m_warnSetLayout->addWidget(m_errTypeCombo,0,6);
    m_warnSetLayout->addWidget(m_objectLabel,1,1);
    m_warnSetLayout->addWidget(m_objectCombo,1,2);
    m_warnSetLayout->addWidget(m_deviceNoLabel,1,3);
    m_warnSetLayout->addWidget(m_deviceNoCombo,1,4);
    m_warnSetLayout->addWidget(m_warnTypeLabel,1,5);
    m_warnSetLayout->addWidget(m_warnTypeCombo,1,6);
    m_warnSetLayout->addWidget(m_okbtn,1,7);
    m_warnSetGroup->setLayout(m_warnSetLayout);

    connect(m_okbtn,SIGNAL(clicked()),this,SLOT(slotClickOk()));

    // m_errJudgeContent=new QLabel();

    m_toolBox=new QToolBox();
    // m_toolBox->addItem(m_noProcesGroup,QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))),
    //     QString::fromStdString(loadTranslation(m_lang,"UntreatedInfo")));//"未处理设备信息");
    // m_toolBox->addItem(m_errJudgeContent,QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))),
    //     QString::fromStdString(loadTranslation(m_lang,"Diagnostic")));//"诊断信息");
    m_toolBox->addItem(m_maintenanceGroup,QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))),
        QString::fromStdString(loadTranslation(m_lang,"MaintainManage")));//"维护管理");
    connect(m_toolBox,SIGNAL(currentChanged(int)),this,SLOT(togglePage(int)));

    
    m_upperLayout1->addWidget(m_warnSetGroup,0,1);
    m_upperLayout1->addWidget(m_definitionGroup,1,1);

    m_versplitter->addWidget(m_upperGroup1);
    m_versplitter->addWidget(m_toolBox);

    m_layout->addWidget(m_versplitter);
}
bool HGDeviceMaintenanceWidget::closeWindow()
{
    // if (m_inputsearchConditionW){
    //     if (m_inputsearchConditionW->closeWindow()){
    //         SAFE_DELETE(m_inputsearchConditionW);
    //     }
    // }
    return true;
}
HGDeviceMaintenanceWidget::~HGDeviceMaintenanceWidget()
{

}
void HGDeviceMaintenanceWidget::fnInitMaintainence()
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
void HGDeviceMaintenanceWidget::togglePage(int index){
    int cnt=m_toolBox->count();
    for (int i=0;i<cnt;i++){
        if (i==index) m_toolBox->setItemIcon(i,QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow-down 1.png"))));
        else m_toolBox->setItemIcon(i,QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))));
    }
    
}
void HGDeviceMaintenanceWidget::slotClickOk(){

}