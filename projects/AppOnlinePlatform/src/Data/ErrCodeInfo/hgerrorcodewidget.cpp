#include "hgerrorcodewidget.h"
#include <QHeaderView>
#include "common.h"
#include <QMessageBox>
#include <QDialog>

HGErrorCodeWidget::HGErrorCodeWidget(std::string lang,QWidget *parent) : QWidget(parent),
m_lang(lang)
{
    RWDb::writeAuditTrailLog(loadTranslation(m_lang,"Enter")+loadTranslation(m_lang,"WarningInfo"));
    m_searchCondition.Clear();
    m_layout=new QGridLayout();
    this->setLayout(m_layout);
    m_leftlayout=new QGridLayout();

    m_inputsearchConditionW=NULL;
    m_inputsearchConditionW=new HGInputSearchConditionWidget(HG_MAX_SEARCH_RANGE,m_lang);
    connect(m_inputsearchConditionW,SIGNAL(signalKeyWord(QString)),this,SLOT(slotKeyWord(QString)));
    connect(m_inputsearchConditionW,SIGNAL(signalTimeFrom(QString)),this,SLOT(slotTimeFrom(QString)));
    connect(m_inputsearchConditionW,SIGNAL(signalTimeTo(QString)),this,SLOT(slotTimeTo(QString)));
    connect(m_inputsearchConditionW,SIGNAL(signalSearch()),this,SLOT(slotSearch()));

    m_leftGroup=new QGroupBox("");
    m_leftGroup->setLayout(m_leftlayout);
    m_rightlayout=new QGridLayout();
    m_rightGroup=new QGroupBox("");
    m_rightGroup->setLayout(m_rightlayout);

    m_manipulateGroup=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"manipulate")));//"操作");
    m_manipulateGroup->setStyleSheet("QGroupBox { font-size: 12pt; font-weight:bold;}");
    m_manipulateLayout=new QGridLayout();

    // m_exportLabel=new HGQLabel(false,getPath("/resources/V1/@1xiconPark-export 1.png"));
    // m_deleteLabel=new HGQLabel(false,getPath("/resources/V1/@1xif-bin 1.png"));
    m_saveLabel=new HGQLabel(false,getPath("/resources/V1/@1xmb-save 1.png"));
    connect(m_saveLabel,SIGNAL(leftClicked()),this,SLOT(slotSaveSearchWarn()));
    // connect(m_deleteLabel,SIGNAL(leftClicked()),this,SLOT(slotDeleteSearchWarn()));

    // m_displayBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Scan")));//"查看");

    m_tableW=new QTableWidget(0,5);
    QStringList headers={QString::fromStdString(loadTranslation(m_lang,"Time")),
        QString::fromStdString(loadTranslation(m_lang,"WarnType")),
        QString::fromStdString(loadTranslation(m_lang,"WarnContent")),
        QString::fromStdString(loadTranslation(m_lang,"Value")),
        QString::fromStdString(loadTranslation(m_lang,"Operator"))};
    m_tableW->setHorizontalHeaderLabels(headers);
    m_tableW->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableW->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_tableW->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_tableW->resizeRowsToContents();
    m_tableW->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(m_tableW,SIGNAL(cellClicked(int,int)),this,SLOT(onCellClicked(int,int)));
    

    // m_manipulateLayout->addWidget(m_exportLabel,0,1);
    m_manipulateLayout->addWidget(m_saveLabel,0,2);
    // m_manipulateLayout->addWidget(m_deleteLabel,0,3);
    // m_manipulateLayout->addWidget(m_displayBtn,0,9);
    m_manipulateLayout->addWidget(m_tableW,1,0,1,10);
    m_manipulateGroup->setLayout(m_manipulateLayout);

    m_leftlayout->addWidget(m_inputsearchConditionW,0,1);
    m_leftlayout->addWidget(m_manipulateGroup,1,1);

    m_horsplitter=new QSplitter();
    m_horsplitter->setOrientation(Qt::Horizontal);
    m_horsplitter->setStyleSheet("QSplitter::handle{background-color:lightgray;}");

    m_infoLabel=new QLabel();
    m_infoLabel->setText(QString::fromStdString(loadTranslation(m_lang,"Detail")));//"详细信息")
    m_helpBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Help")));
    connect(m_helpBtn,&QPushButton::clicked,[&](){
        emit signalJumpToExceptionPage();
    });

    m_rightlayout->addWidget(m_infoLabel,0,0,10,10);
    m_rightlayout->addWidget(m_helpBtn,0,11,1,1);

    m_horsplitter->addWidget(m_leftGroup);
    m_horsplitter->addWidget(m_rightGroup);
    m_horsplitter->setStretchFactor(0,1);
    m_horsplitter->setStretchFactor(1,3);
    m_layout->addWidget(m_horsplitter);

}
bool HGErrorCodeWidget::closeWindow()
{
    if (m_inputsearchConditionW){
        if (m_inputsearchConditionW->closeWindow()){
            SAFE_DELETE(m_inputsearchConditionW);
        }
    }
    return true;
}
HGErrorCodeWidget::~HGErrorCodeWidget()
{
    
}
void HGErrorCodeWidget::onCellClicked(int row,int col){
    m_tableW->selectRow(row);
    m_infoLabel->setText("");
}
void HGErrorCodeWidget::slotDeleteSearchWarn(){
    int row = getSelectedRow(m_tableW);
    if (row >= m_tableW->rowCount()) return;
    if (row==-1) {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), QString::fromStdString(loadTranslation(m_lang,"SelectOneRecord")));//"请选中一条记录！");
        return;
    }
}
void HGErrorCodeWidget::slotSaveSearchWarn(){
    if (m_tableW->rowCount()==0){
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),
                             QString::fromStdString(loadTranslation(m_lang,"NoData")));
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle(QString::fromStdString(loadTranslation(m_lang,"InputSaveName")));
    dialog.setWindowModality(Qt::ApplicationModal);
    QLabel* saveLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"LogSaveType")));
    QComboBox* saveCombox=new QComboBox();
    saveCombox->addItems({"txt","csv","pdf"});
    enum {
        SAVE_TEXT,
        SAVE_CSV,
        SAVE_PDF
    };
    int logsavetype=SAVE_TEXT;
    connect(saveCombox,&QComboBox::currentTextChanged,[&](QString text){
        if (text=="txt"){
            logsavetype=SAVE_TEXT;
            // saveLabel->setText(QString::fromStdString(loadTranslation(m_lang,"LogSaveName")));
        }else if (text=="csv"){
            logsavetype=SAVE_CSV;
        } else{
            logsavetype=SAVE_PDF;
            // saveLabel->setText(QString::fromStdString(loadTranslation(m_lang,"LogSaveName")));
        }
    });
    QPushButton *okBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Ok")));
    QPushButton *cancelBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Cancel")));
    connect(okBtn,&QPushButton::clicked,[&](){
        std::vector<std::map<std::string,std::string>> logList;
        for (int i=0;i<m_tableW->rowCount();i++){
            std::map<std::string,std::string> log;
            for (int j=0;j<m_tableW->columnCount();j++){
                log[m_tableW->horizontalHeaderItem(j)->text().toStdString()]=m_tableW->item(i,j)->text().toStdString();
            }
            logList.push_back(log);
        }
        std::string outlogPath=HgOnlinePlatformModule::getDirPath()+"/outlog/";
        HGMkDir(outlogPath);
        HGExactTime curTime = HGExactTime::currentTime();
        std::string syncslice = curTime.toStringFromYearToSec();
        std::string logname=outlogPath+syncslice;
        switch (logsavetype){
            case SAVE_TEXT:{
                logname+=".txt";
                saveTableToTxt(logList,logname);
                break;
            }
            case SAVE_CSV:{
                logname+=".csv";
                saveTableToCsv(logList,logname);
                break;
            }
            case SAVE_PDF:{
                logname+=".pdf";
                saveTableToPdf(logList,logname,getPath("/resources/simhei.ttf"));
                break;
            }
            default:{
                break;
            }
        }
        dialog.close();
    });
    connect(cancelBtn,&QPushButton::clicked,[&](){
        dialog.close();
    });
    QGridLayout *layout=new QGridLayout(&dialog);
    layout->addWidget(saveLabel,0,0);
    layout->addWidget(saveCombox,0,1);
    layout->addWidget(okBtn,1,0);
    layout->addWidget(cancelBtn,1,1);
    dialog.setLayout(layout);
    dialog.exec();
}

void HGErrorCodeWidget::fnReadDB()
{
    std::vector<std::map<std::string, std::string>> loginfos;
    int auditTrailLogCount = HGOnlineRWDB::readAuditTrailLogCount();
    if (auditTrailLogCount > 10000)
    {
        if (m_searchCondition.isInit())
        {
            QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),
                                 QString::fromStdString(loadTranslation(m_lang, "TooManagLogFiles")));
            return;
        }
    }
    loginfos = HGOnlineRWDB::readAuditTrailLog();
    for (int i = 0; i < int(loginfos.size()); i++)
    {
        m_tableW->insertRow(i);
        for (auto info : loginfos[i])
        {
            int nameColIndex = getColumnIndexByName(m_tableW, QString::fromStdString(loadTranslation(m_lang, info.first)));
            if (nameColIndex < 0 || nameColIndex >= m_tableW->columnCount())
                continue;
            m_tableW->setItem(i, nameColIndex, new QTableWidgetItem(QString::fromStdString(info.second)));
        }
    }
}

void HGErrorCodeWidget::slotKeyWord(QString text){
    m_searchCondition.key=text.toStdString();
}
void HGErrorCodeWidget::slotTimeFrom(QString text){
    m_searchCondition.timeRangeFrom=text.toStdString();
    m_searchCondition.timeFrom=HGExactTime::currentTime();
    m_searchCondition.timeFrom.tm_year = atoi(m_searchCondition.timeRangeFrom.substr(0, 4).c_str());
    m_searchCondition.timeFrom.tm_mon = atoi(m_searchCondition.timeRangeFrom.substr(4, 2).c_str());
    m_searchCondition.timeFrom.tm_mday = atoi(m_searchCondition.timeRangeFrom.substr(6, 2).c_str());
    m_searchCondition.timeFrom.tm_hour = 0;
    m_searchCondition.timeFrom.tm_min = 0;
    m_searchCondition.timeFrom.tm_sec = 0;

}
void HGErrorCodeWidget::slotTimeTo(QString text){
    m_searchCondition.timeRangeTo=text.toStdString();
    m_searchCondition.timeTo=HGExactTime::currentTime();
    m_searchCondition.timeTo.tm_year = atoi(m_searchCondition.timeRangeTo.substr(0, 4).c_str());
    m_searchCondition.timeTo.tm_mon = atoi(m_searchCondition.timeRangeTo.substr(4, 2).c_str());
    m_searchCondition.timeTo.tm_mday = atoi(m_searchCondition.timeRangeTo.substr(6, 2).c_str());
    m_searchCondition.timeTo.tm_hour = 23;
    m_searchCondition.timeTo.tm_min = 59;
    m_searchCondition.timeTo.tm_sec = 59;
}
void HGErrorCodeWidget::slotSearch(){
    m_tableW->setRowCount(0);
    fnReadDB();
}