#include "hglogwidget.h"
#include <QHeaderView>
#include "common.h"
#include <fstream>
#include <algorithm>
#include <QMessageBox>
#include <QBrush>
#include <QColor>
#include <QProgressDialog>
#include <QApplication>
#include <QElapsedTimer>


HGLogWidget::HGLogWidget(std::string lang,QWidget *parent) : QWidget(parent),
m_lang(lang),
m_curDisplayIndex(-1),
m_isSearchMode(false),
m_searchTotalCount(0),
m_searchCurrentPage(0),
m_pageSize(100),
m_sortDescending(true)
{
    RWDb::writeAuditTrailLog(loadTranslation(m_lang,"Enter")+loadTranslation(m_lang,"Log"));
    m_auditLogTableNames = RWDb::getAllAuditLogTables();
    m_searchCondition.Clear();
    RWDb::getAuditTrailLogDateRange(m_logMinTime, m_logMaxTime);

    m_layout=new QGridLayout();
    this->setLayout(m_layout);

    m_inputsearchConditionW=NULL;
    m_inputsearchConditionW=new HGInputSearchConditionWidget(HG_MAX_SEARCH_RANGE,m_lang);
    connect(m_inputsearchConditionW,SIGNAL(signalKeyWord(QString)),this,SLOT(slotKeyWord(QString)));
    connect(m_inputsearchConditionW,SIGNAL(signalTimeFrom(QString)),this,SLOT(slotTimeFrom(QString)));
    connect(m_inputsearchConditionW,SIGNAL(signalTimeTo(QString)),this,SLOT(slotTimeTo(QString)));
    connect(m_inputsearchConditionW,SIGNAL(signalSearch()),this,SLOT(slotSearch()));
    connect(m_inputsearchConditionW,SIGNAL(signalClearSearch()),this,SLOT(slotClearSearch()));

    m_manipulateGroup=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"manipulate")));
    m_manipulateGroup->setStyleSheet("QGroupBox { font-size: 12pt; font-weight:bold;}");
    m_manipulateLayout=new QGridLayout();

    m_pageLabel=new QLabel("第"+QString::number(m_curDisplayIndex+1)+"页");
    m_timeCostLabel=new QLabel("");
    m_saveLabel=new HGQLabel(false,getPath("/resources/V1/@1xmb-save 1.png")); 
    m_nextLabel=new HGQLabel(false,getPath("/resources/V1/@1xze-arrow 1.png")); 
    m_preLabel=new HGQLabel(false,getPath("/resources/V1/@1xze-arrow-left 1.png")); 
    connect(m_saveLabel,SIGNAL(leftClicked()),this,SLOT(slotSaveSearchLog()));
    connect(m_nextLabel,SIGNAL(leftClicked()),this,SLOT(slotNext()));
    connect(m_preLabel,SIGNAL(leftClicked()),this,SLOT(slotPre()));

    m_tableW=new QTableWidget(0,3);
    QStringList headers={"时间","日志内容","操作员"};
    m_tableW->setHorizontalHeaderLabels(headers);
    m_tableW->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableW->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_tableW->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_tableW->resizeRowsToContents();
    m_tableW->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    m_logTypeLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"LogType")));
    m_logTypeComboBox=new QComboBox();
    m_logTypeComboBox->addItems({QString::fromStdString(loadTranslation(m_lang,"AuditTrail")),
                                 QString::fromStdString(loadTranslation(m_lang,"RunLog"))});
    m_logTypeComboBox->setCurrentIndex(0);
    connect(m_logTypeComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(slotLogTypeChanged(int)));
    
    m_sortLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"SortOrder")));
    m_sortComboBox=new QComboBox();
    m_sortComboBox->addItems({QString::fromStdString(loadTranslation(m_lang,"Descending")),
                              QString::fromStdString(loadTranslation(m_lang,"Ascending"))});
    m_sortComboBox->setCurrentIndex(0);
    connect(m_sortComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(slotSortOrderChanged(int)));

    m_manipulateLayout->addWidget(m_saveLabel,0,2);
    m_manipulateLayout->addWidget(m_preLabel,0,3);
    m_manipulateLayout->addWidget(m_nextLabel,0,4);
    m_manipulateLayout->addWidget(m_pageLabel,0,5);
    m_manipulateLayout->addWidget(m_timeCostLabel,0,6);
    m_manipulateLayout->addWidget(m_tableW,1,0,1,10);
    m_manipulateGroup->setLayout(m_manipulateLayout);

    m_layout->addWidget(m_inputsearchConditionW,0,1,1,3);
    m_layout->addWidget(m_logTypeLabel,0,6,1,1);
    m_layout->addWidget(m_logTypeComboBox,0,7,1,1);
    m_layout->addWidget(m_sortLabel,0,8,1,1);
    m_layout->addWidget(m_sortComboBox,0,9,1,1);
    m_layout->addWidget(m_manipulateGroup,1,1,1,15);
    slotLogTypeChanged(0);
}

bool HGLogWidget::closeWindow()
{
    if (m_inputsearchConditionW){
        if (m_inputsearchConditionW->closeWindow()){
            SAFE_DELETE(m_inputsearchConditionW);
        }
    }
    return true;
}
HGLogWidget::~HGLogWidget()
{
}

void HGLogWidget::slotSortOrderChanged(int index){
    m_sortDescending = (index == 0);
    if (m_isSearchMode) {
        fnDisplaySearchResults(m_searchCurrentPage * m_pageSize);
    }
}

void HGLogWidget::slotNext(){
    if (m_isSearchMode) {
        int totalPages = (m_searchTotalCount + m_pageSize - 1) / m_pageSize;
        if (m_searchCurrentPage < totalPages - 1) {
            m_searchCurrentPage++;
            fnDisplaySearchResults(m_searchCurrentPage * m_pageSize);
        } else {
            QMessageBox::information(this, QString::fromStdString(HG_DEVICE_NAME),
                             QString::fromStdString(loadTranslation(m_lang,"LastPage")));
        }
        return;
    }
    
    if (m_curDisplayIndex < 0) return;
    if (m_curDisplayIndex < int(m_auditLogTableNames.size())-1) m_curDisplayIndex++;
    else {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),
                         "已经是最后一页");
        m_curDisplayIndex=m_auditLogTableNames.size()-1;
    }
    std::string dbName=m_auditLogTableNames[m_curDisplayIndex];
    fnReadDB(dbName);
}

void HGLogWidget::slotPre(){
    if (m_isSearchMode) {
        if (m_searchCurrentPage > 0) {
            m_searchCurrentPage--;
            fnDisplaySearchResults(m_searchCurrentPage * m_pageSize);
        } else {
            QMessageBox::information(this, QString::fromStdString(HG_DEVICE_NAME),
                             QString::fromStdString(loadTranslation(m_lang,"FirstPage")));
        }
        return;
    }
    
    if (m_curDisplayIndex < 0) {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),
                         "已经是第一页");
        m_curDisplayIndex=0;
    } else {
        m_curDisplayIndex--;
    }
    std::string dbName=m_auditLogTableNames[m_curDisplayIndex];
    fnReadDB(dbName);
}

int HGLogWidget::getTableNameIndex(const std::string &tableName){
    for (int i=0;i<int(m_auditLogTableNames.size());i++){
        if (m_auditLogTableNames[i] == tableName) {
            m_curDisplayIndex = i;
            break;
        }
    }
    if (tableName=="") m_curDisplayIndex=m_auditLogTableNames.size()-1;
    return m_curDisplayIndex;
}

void HGLogWidget::fnReadDB(const std::string &tableName){
    m_tableW->setRowCount(0);
    std::vector<std::map<std::string,std::string>> loginfos;
    m_tableW->setUpdatesEnabled(false);
    
    switch (m_logTypeComboBox->currentIndex()){
        case 0:
        {
            const int MAXROW = 1000;
            m_tableW->setRowCount(MAXROW);
            loginfos=RWDb::readAuditTrailLog(tableName);
            getTableNameIndex(tableName);
            updatePageLabel();
            int traillogIndex = 0;
            m_tableW->setRowCount(MAXROW > int(loginfos.size()) ? MAXROW : int(loginfos.size()));
            for (int i =int(loginfos.size())-1;i>=0;i--){
                for (auto info:loginfos[i]){
                    int nameColIndex=m_logContentMap[info.first];
                    if (nameColIndex<0||nameColIndex>=m_tableW->columnCount())
                        continue;
                    QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(info.second));
                    m_tableW->setItem(traillogIndex,nameColIndex,item);
                }
                traillogIndex++;
            }
            m_tableW->setRowCount(traillogIndex);
            break;
        }
        case 1:
        {
            std::vector<FileInfo> fileList;
            HGGetFilesNoBytes("/app/log/",".log",fileList);
            std::sort(fileList.begin(), fileList.end(), [](const FileInfo& a, const FileInfo& b) {
                return a.createtime < b.createtime;
            });
            for (int i = int(fileList.size()-1); i < int(fileList.size()); i++)
            {
                std::ifstream file(fileList[i].filename);
                if (!file.is_open())
                {
                    continue;
                }

                std::string line;
                while (std::getline(file, line))
                {
                    m_tableW->insertRow(m_tableW->rowCount());
                    int pos=line.find_first_of(">");
                    m_tableW->setItem(m_tableW->rowCount()-1, 0, new QTableWidgetItem(QString::fromStdString(line.substr(0,pos-1))));
                    m_tableW->setItem(m_tableW->rowCount()-1, 1, new QTableWidgetItem(QString::fromStdString(line.substr(pos+1,line.length()-pos-1))));
                    m_tableW->setItem(m_tableW->rowCount()-1, 2, new QTableWidgetItem(QString::fromStdString(GlobalSingleton::instance().getSystemInfo("loginName"))));
                }

                file.close();
            }
            break;
        }
        default:{
            break;
        }
    }
    m_tableW->setUpdatesEnabled(true);
}

void HGLogWidget::slotLogTypeChanged(int index){
    m_tableW->clear();
    m_tableW->setRowCount(0);
    m_logContentMap["Time"]=0;
    m_logContentMap["LogContent"]=1;
    m_logContentMap["Operator"]=2;
    switch (index){
        case 0:{
        QStringList headers={QString::fromStdString(loadTranslation(m_lang,"Time")),
                             QString::fromStdString(loadTranslation(m_lang,"LogContent")),
                             QString::fromStdString(loadTranslation(m_lang,"Operator"))};
        m_tableW->setColumnCount(headers.size());
        m_tableW->setHorizontalHeaderLabels(headers);
        m_tableW->horizontalHeaderItem(0)->setToolTip("Time");
        m_tableW->horizontalHeaderItem(1)->setToolTip("LogContent");
        m_tableW->horizontalHeaderItem(2)->setToolTip("Operator");
        break;
        }
        case 1:{
        QStringList headers1={QString::fromStdString(loadTranslation(m_lang,"Time")),
                                QString::fromStdString(loadTranslation(m_lang,"LogContent")),
                                QString::fromStdString(loadTranslation(m_lang,"Operator"))};
        m_tableW->setColumnCount(headers1.size());
        m_tableW->setHorizontalHeaderLabels(headers1);
        m_tableW->horizontalHeaderItem(0)->setToolTip("Time");
        m_tableW->horizontalHeaderItem(1)->setToolTip("LogContent");
        m_tableW->horizontalHeaderItem(2)->setToolTip("Operator");
        break;
        }
        default:
        break;
    }
    m_isSearchMode = false;
    m_searchTotalCount = 0;
    m_searchCurrentPage = 0;
    fnReadDB("");
}

void HGLogWidget::slotKeyWord(QString text){
    m_searchCondition.key=text.toStdString();
}

void HGLogWidget::slotTimeFrom(QString text){
    m_searchCondition.timeRangeFrom=text.toStdString();
    m_searchCondition.timeFrom=HGExactTime::currentTime();
    m_searchCondition.timeFrom.tm_year = atoi(m_searchCondition.timeRangeFrom.substr(0, 4).c_str());
    m_searchCondition.timeFrom.tm_mon = atoi(m_searchCondition.timeRangeFrom.substr(4, 2).c_str());
    m_searchCondition.timeFrom.tm_mday = atoi(m_searchCondition.timeRangeFrom.substr(6, 2).c_str());
    m_searchCondition.timeFrom.tm_hour = 0;
    m_searchCondition.timeFrom.tm_min = 0;
    m_searchCondition.timeFrom.tm_sec = 0;

}

void HGLogWidget::slotTimeTo(QString text){
    m_searchCondition.timeRangeTo=text.toStdString();
    m_searchCondition.timeTo=HGExactTime::currentTime();
    m_searchCondition.timeTo.tm_year = atoi(m_searchCondition.timeRangeTo.substr(0, 4).c_str());
    m_searchCondition.timeTo.tm_mon = atoi(m_searchCondition.timeRangeTo.substr(4, 2).c_str());
    m_searchCondition.timeTo.tm_mday = atoi(m_searchCondition.timeRangeTo.substr(6, 2).c_str());
    m_searchCondition.timeTo.tm_hour = 23;
    m_searchCondition.timeTo.tm_min = 59;
    m_searchCondition.timeTo.tm_sec = 59;
}

void HGLogWidget::slotSearch(){
    if (m_logTypeComboBox->currentIndex() == 0) {
        fnSearchAllLogs();
    } else {
        m_tableW->setRowCount(0);
        fnReadDB(m_auditLogTableNames[m_curDisplayIndex]);
    }
}

void HGLogWidget::fnSearchAllLogs(){
    m_tableW->setRowCount(0);
    m_tableW->setUpdatesEnabled(false);
    
    QElapsedTimer timer;
    timer.start();
    
    HGExactTime timeFrom = m_searchCondition.timeFrom;
    HGExactTime timeTo = m_searchCondition.timeTo;
    
    if (m_searchCondition.timeRangeFrom.empty()) {
        timeFrom = m_logMinTime;
    }
    if (m_searchCondition.timeRangeTo.empty()) {
        timeTo = m_logMaxTime;
    }
    
    QProgressDialog progress(QString::fromStdString(loadTranslation(m_lang,"Searching")), 
                             QString::fromStdString(loadTranslation(m_lang,"Cancel")), 
                             0, 0, this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(0);
    progress.setValue(0);
    
    QApplication::processEvents();
    
    m_searchTotalCount = RWDb::searchAllAuditTrailLogCount(
        m_searchCondition.key, timeFrom, timeTo);
    
    progress.close();
    
    qint64 countTime = timer.elapsed();
    
    if (m_searchTotalCount == 0) {
        m_tableW->setUpdatesEnabled(true);
        m_timeCostLabel->setText(QString("搜索耗时: %1ms").arg(countTime));
        QMessageBox::information(this, QString::fromStdString(HG_DEVICE_NAME),
                         QString::fromStdString(loadTranslation(m_lang,"NoData")));
        return;
    }
    
    m_isSearchMode = true;
    m_searchCurrentPage = 0;
    
    fnDisplaySearchResults(0);
    
    qint64 totalTime = timer.elapsed();
    m_timeCostLabel->setText(QString("搜索耗时: %1ms").arg(totalTime));
    
    m_tableW->setUpdatesEnabled(true);
}

void HGLogWidget::fnDisplaySearchResults(int offset){
    QElapsedTimer timer;
    timer.start();
    
    m_tableW->setUpdatesEnabled(false);
    m_tableW->setRowCount(0);
    
    HGExactTime timeFrom = m_searchCondition.timeFrom;
    HGExactTime timeTo = m_searchCondition.timeTo;
    
    if (m_searchCondition.timeRangeFrom.empty()) {
        timeFrom = m_logMinTime;
    }
    if (m_searchCondition.timeRangeTo.empty()) {
        timeTo = m_logMaxTime;
    }
    
    std::vector<std::map<std::string,std::string>> results = RWDb::searchAllAuditTrailLog(
        m_searchCondition.key, timeFrom, timeTo, offset, m_pageSize, m_sortDescending);
    
    m_tableW->setRowCount(results.size());
    QString keyword = QString::fromStdString(m_searchCondition.key);
    
    for (size_t i = 0; i < results.size(); i++) {
        for (auto info : results[i]) {
            int nameColIndex = m_logContentMap[info.first];
            if (nameColIndex < 0 || nameColIndex >= m_tableW->columnCount())
                continue;
            QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(info.second));
            if (!keyword.isEmpty()) {
                highlightKeyword(item, keyword);
            }
            m_tableW->setItem(i, nameColIndex, item);
        }
    }
    
    m_tableW->setUpdatesEnabled(true);
    m_tableW->viewport()->update();
    
    qint64 displayTime = timer.elapsed();
    m_timeCostLabel->setText(QString("翻页耗时: %1ms").arg(displayTime));
    
    updatePageLabel();
}

void HGLogWidget::highlightKeyword(QTableWidgetItem* item, const QString& keyword){
    if (keyword.isEmpty() || item == nullptr) return;
    
    QString text = item->text();
    if (text.contains(keyword, Qt::CaseInsensitive)) {
        item->setBackground(QBrush(QColor(255, 255, 0)));
        item->setForeground(QBrush(QColor(255, 0, 0)));
        item->setFont(this->font());
    }
}

void HGLogWidget::updatePageLabel(){
    if (m_isSearchMode) {
        int totalPages = (m_searchTotalCount + m_pageSize - 1) / m_pageSize;
        m_pageLabel->setText(QString("搜索结果 第%1/%2页 (共%3条)")
            .arg(m_searchCurrentPage + 1)
            .arg(totalPages)
            .arg(m_searchTotalCount));
    } else {
        m_pageLabel->setText("第"+QString::number(m_curDisplayIndex+1)+"页");
    }
}

void HGLogWidget::slotClearSearch(){ 
    m_searchCondition.Clear();
    m_isSearchMode = false;
    m_searchTotalCount = 0;
    m_searchCurrentPage = 0;
    m_timeCostLabel->setText("");
    fnReadDB(m_auditLogTableNames[m_curDisplayIndex]);
}

void HGLogWidget::slotSaveSearchLog(){
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
        }else if (text=="csv"){
            logsavetype=SAVE_CSV;
        } else{
            logsavetype=SAVE_PDF;
        }
    });
    QPushButton *okBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Ok")));
    QPushButton *cancelBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Cancel")));
    connect(okBtn,&QPushButton::clicked,[&](){
        std::vector<std::map<std::string,std::string>> logList;
        for (int i=0;i<m_tableW->rowCount();i++){
            std::map<std::string,std::string> log;
            for (int j=0;j<m_tableW->columnCount();j++){
                std::string key=m_tableW->horizontalHeaderItem(j)->text().toStdString();
                if (m_tableW->item(i,j)==nullptr) continue;
                log[key]=m_tableW->item(i,j)->text().toStdString();
            }
            logList.push_back(log);
        }
        std::string outlogPath=FileConfig::getDirPath()+"/outlog/";
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

void HGLogWidget::slotSearchFinished(){
    updatePageLabel();
}
