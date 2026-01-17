#include "hglogwidget.h"
#include <QHeaderView>
#include "common.h"
#include <fstream>
#include <algorithm>
#include <QMessageBox>
#include <ctime>


HGLogWidget::HGLogWidget(std::string lang,QWidget *parent) : QWidget(parent),
m_lang(lang),
m_curDisplayIndex(-1),
m_curSearchPage(0),
m_totalSearchPages(0),
m_isSearchingAll(false)
{
    RWDb::writeAuditTrailLog(loadTranslation(m_lang,"Enter")+loadTranslation(m_lang,"Log"));
    m_auditLogTableNames = RWDb::getAllAuditLogTables();
    m_searchCondition.Clear();

    m_layout=new QGridLayout();
    this->setLayout(m_layout);

    m_inputsearchConditionW=NULL;
    m_inputsearchConditionW=new HGInputSearchConditionWidget(HG_MAX_SEARCH_RANGE,m_lang);
    connect(m_inputsearchConditionW,SIGNAL(signalKeyWord(QString)),this,SLOT(slotKeyWord(QString)));
    connect(m_inputsearchConditionW,SIGNAL(signalTimeFrom(QString)),this,SLOT(slotTimeFrom(QString)));
    connect(m_inputsearchConditionW,SIGNAL(signalTimeTo(QString)),this,SLOT(slotTimeTo(QString)));
    connect(m_inputsearchConditionW,SIGNAL(signalSearch()),this,SLOT(slotSearch()));
    connect(m_inputsearchConditionW,SIGNAL(signalClearSearch()),this,SLOT(slotClearSearch()));

    m_manipulateGroup=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"manipulate")));//"操作");
    m_manipulateGroup->setStyleSheet("QGroupBox { font-size: 12pt; font-weight:bold;}");
    m_manipulateLayout=new QGridLayout();

    // m_exportLabel=new HGQLabel(false,getPath("/resources/V1/@1xiconPark-export 1.png"));
    m_pageLabel=new QLabel("第"+QString::number(m_curDisplayIndex)+"页");
    m_totalPageLabel=new QLabel("共"+QString::number(0)+"页");
    m_saveLabel=new HGQLabel(false,getPath("/resources/V1/@1xmb-save 1.png")); 
    m_nextLabel=new HGQLabel(false,getPath("/resources/V1/@1xze-arrow 1.png")); 
    m_preLabel=new HGQLabel(false,getPath("/resources/V1/@1xze-arrow-left 1.png")); 
    connect(m_saveLabel,SIGNAL(leftClicked()),this,SLOT(slotSaveSearchLog()));
    connect(m_nextLabel,SIGNAL(leftClicked()),this,SLOT(slotNext()));
    connect(m_preLabel,SIGNAL(leftClicked()),this,SLOT(slotPre()));

    m_tableW=new QTableWidget(0,3);
    QStringList headers={"时间",/*,"通道","采样电位","日志类型",*/"日志内容","操作员"};
    m_tableW->setHorizontalHeaderLabels(headers);
    m_tableW->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableW->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_tableW->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_tableW->resizeRowsToContents();
    m_tableW->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    m_logTypeLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"LogType")));//"日志类型");
    m_logTypeComboBox=new QComboBox();
    m_logTypeComboBox->addItems({QString::fromStdString(loadTranslation(m_lang,"AuditTrail")),
                                 QString::fromStdString(loadTranslation(m_lang,"RunLog"))});
    m_logTypeComboBox->setCurrentIndex(0);
    connect(m_logTypeComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(slotLogTypeChanged(int)));
    slotLogTypeChanged(0);

    // m_manipulateLayout->addWidget(m_exportLabel,0,1);
    m_manipulateLayout->addWidget(m_saveLabel,0,2);
    m_manipulateLayout->addWidget(m_preLabel,0,3);
    m_manipulateLayout->addWidget(m_nextLabel,0,4);
    m_manipulateLayout->addWidget(m_pageLabel,0,5);
    m_manipulateLayout->addWidget(m_totalPageLabel,0,6);
    m_manipulateLayout->addWidget(m_tableW,1,0,1,10);
    m_manipulateGroup->setLayout(m_manipulateLayout);

    m_layout->addWidget(m_inputsearchConditionW,0,1,1,3);
    m_layout->addWidget(m_logTypeLabel,0,6,1,1);
    m_layout->addWidget(m_logTypeComboBox,0,7,1,1);
    m_layout->addWidget(m_manipulateGroup,1,1,1,15);
    fnReadDB("");
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
void HGLogWidget::slotNext(){
    if (m_isSearchingAll) {
        if (m_curSearchPage < m_totalSearchPages - 1) {
            m_curSearchPage++;
            fnDisplaySearchPage();
        } else {
            QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),
                             QString::fromStdString("已经是最后一页"));
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
    if (m_isSearchingAll) {
        if (m_curSearchPage > 0) {
            m_curSearchPage--;
            fnDisplaySearchPage();
        } else {
            QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),
                             QString::fromStdString("已经是第一页"));
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
    m_tableW->setUpdatesEnabled(false);
    
    m_isSearchingAll = false;
    
    switch (m_logTypeComboBox->currentIndex()){
        case 0:
        {
            const int MAXROW = 1000;
            std::vector<std::map<std::string, std::string>> loginfos;
            
            if (m_dataCache.find(tableName) != m_dataCache.end()) {
                loginfos = m_dataCache[tableName];
            } else {
                int auditTrailLogCount=RWDb::readAuditTrailLogCount(tableName);
                if (auditTrailLogCount > 10000){
                    if (m_searchCondition.isInit()){
                        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),
                                         QString::fromStdString(loadTranslation(m_lang,"TooManagLogFiles")));
                        m_tableW->setUpdatesEnabled(true);
                        return;
                    }
                }
                loginfos=RWDb::readAuditTrailLog(tableName);
                m_dataCache[tableName] = loginfos;
            }
            
            m_currentPageData = loginfos;
            getTableNameIndex(tableName);
            m_pageLabel->setText("第"+QString::number(m_curDisplayIndex+1)+"页/");
            m_totalPageLabel->setText(QString::fromStdString("共") + QString::number(m_auditLogTableNames.size()) + QString::fromStdString("页"));
            
            int traillogIndex = 0;
            m_tableW->setRowCount(MAXROW);
            
            for (int i =int(loginfos.size())-1;i>=0;i--){
                bool showLog = true;
                
                if (m_searchCondition.isInit()){
                    std::string timeStr = loginfos[i]["Time"];
                    HGExactTime testTimer;
                    TIME_STRUECT timeS;
                    decodeStandardTime(timeStr, timeS);
                    testTimer.tm_year = timeS.year;
                    testTimer.tm_mon = timeS.mon;
                    testTimer.tm_mday = timeS.day;
                    testTimer.tm_hour = timeS.hour;
                    testTimer.tm_min = timeS.minute;
                    testTimer.tm_sec = timeS.second;
                    testTimer.tm_millisec = 0;
                    testTimer.tm_microsec = 0; 
                    
                    if (testTimer < m_searchCondition.timeFrom || testTimer > m_searchCondition.timeTo){
                        showLog = false;
                    }
                    
                    if (showLog && !m_searchCondition.key.empty()){
                        bool hasKeyword = (loginfos[i]["Time"].find(m_searchCondition.key)!=std::string::npos
                            || loginfos[i]["Operator"].find(m_searchCondition.key)!=std::string::npos
                            || loginfos[i]["LogContent"].find(m_searchCondition.key)!=std::string::npos);
                        if (!hasKeyword){
                            showLog = false;
                        }
                    }
                }
                
                if (showLog){
                    for (auto info:loginfos[i]){
                        int nameColIndex=m_logContentMap[info.first];
                        if (nameColIndex<0||nameColIndex>=m_tableW->columnCount())
                            continue;
                        
                        QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(info.second));
                        if (!m_searchCondition.key.empty()){
                            fnHighlightKeywords(item, m_searchCondition.key);
                        }
                        m_tableW->setItem(traillogIndex, nameColIndex, item);
                    }
                    traillogIndex++;
                }
            }
            
            m_tableW->setRowCount(traillogIndex);
            break;
        }
        case 1:
        {
            std::vector<FileInfo> fileList;
            HGGetFilesNoBytes("/app/log/",".log",fileList);
            // printf("log count:%d\n",int(fileList.size()));
            // 按创建时间排序（从旧到新）
            std::sort(fileList.begin(), fileList.end(), [](const FileInfo& a, const FileInfo& b) {
                return a.createtime < b.createtime;
            });
            bool beyondMaxFileCount=false;
            if (fileList.size() > 100){
                if (m_searchCondition.isInit()){
                    beyondMaxFileCount=true;
                    QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),
                                     QString::fromStdString(loadTranslation(m_lang,"TooManagLogFiles")));
                    return;
                }
            }
            int totalLines = 0;
            std::vector<std::tuple<std::string, std::string, std::string>> runLogResults;
            
            for (int i = int(fileList.size()-1); i >= 0; i--)
            {
                bool skipFile = false;
                
                if (beyondMaxFileCount)
                {
                    int timepos = fileList[i].filename.find_last_of("/");
                    std::string filename = fileList[i].filename.substr(timepos + 1, fileList[i].filename.length() - timepos - 1);
                    timepos = filename.find_first_of("_");
                    std::string timestr = filename.substr(0, timepos);
                    HGExactTime testTimer = HGExactTime::currentTime();
                    testTimer.tm_year = atoi(timestr.substr(0, 4).c_str());
                    testTimer.tm_mon = atoi(timestr.substr(4, 2).c_str());
                    testTimer.tm_mday = atoi(timestr.substr(6, 2).c_str());

                    if (testTimer < m_searchCondition.timeFrom || testTimer > m_searchCondition.timeTo)
                        skipFile = true;
                }
                
                if (skipFile) continue;
                
                std::ifstream file(fileList[i].filename);
                if (!file.is_open()) continue;

                std::string line;
                while (std::getline(file, line))
                {                                   
                    bool showLine = true;
                    
                    if (m_searchCondition.key.length() > 0){
                        bool hasKeyword = (line.find(m_searchCondition.key) != std::string::npos);
                        std::string loginName = GlobalSingleton::instance().getSystemInfo("loginName");
                        bool isLoginName = (loginName.find(m_searchCondition.key) != std::string::npos);
                        
                        if (!hasKeyword && !isLoginName)
                            showLine = false;
                    }
                    
                    if (showLine){
                        int pos = line.find_first_of(">");
                        std::string timeStr = line.substr(0, pos-1);
                        std::string contentStr = line.substr(pos+1, line.length()-pos-1);
                        std::string operatorStr = GlobalSingleton::instance().getSystemInfo("loginName");
                        
                        runLogResults.emplace_back(timeStr, contentStr, operatorStr);
                        totalLines++;
                    }
                }

                file.close();
            }
            
            m_tableW->setRowCount(runLogResults.size());
            
            for (int i = 0; i < (int)runLogResults.size(); i++){
                const auto& log = runLogResults[i];
                
                QTableWidgetItem* timeItem = new QTableWidgetItem(QString::fromStdString(std::get<0>(log)));
                QTableWidgetItem* contentItem = new QTableWidgetItem(QString::fromStdString(std::get<1>(log)));
                QTableWidgetItem* operatorItem = new QTableWidgetItem(QString::fromStdString(std::get<2>(log)));
                
                if (!m_searchCondition.key.empty()){
                    fnHighlightKeywords(timeItem, m_searchCondition.key);
                    fnHighlightKeywords(contentItem, m_searchCondition.key);
                    fnHighlightKeywords(operatorItem, m_searchCondition.key);
                }
                
                m_tableW->setItem(i, 0, timeItem);
                m_tableW->setItem(i, 1, contentItem);
                m_tableW->setItem(i, 2, operatorItem);
            }
            
            break;
        }
        default:{
            break;
        }
    }
    m_tableW->setUpdatesEnabled(true);   // 恢复更新
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
                                // "通道","采样电位","日志类型",
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
    m_tableW->setRowCount(0);
    fnSearchAllPages();
}
void HGLogWidget::slotClearSearch(){ 
    m_searchCondition.Clear();
    m_isSearchingAll = false;
    fnReadDB(m_auditLogTableNames[m_curDisplayIndex]);
}

void HGLogWidget::fnSearchAllPages(){
    if (m_logTypeComboBox->currentIndex() != 0) {
        fnReadDB(m_auditLogTableNames[m_curDisplayIndex]);
        return;
    }
    
    m_searchResults.clear();
    m_curSearchPage = 0;
    m_isSearchingAll = true;
    
    HGExactTime startTime = HGExactTime::currentTime();
    
    std::vector<std::map<std::string, std::string>> allLogs = RWDb::readAllAuditTrailLog();
    
    for (const auto& log : allLogs) {
        bool showLog = true;
        
        if (m_searchCondition.isInit()){
            std::string timeStr = log.at("Time");
            HGExactTime testTimer;
            TIME_STRUECT timeS;
            decodeStandardTime(timeStr, timeS);
            testTimer.tm_year = timeS.year;
            testTimer.tm_mon = timeS.mon;
            testTimer.tm_mday = timeS.day;
            testTimer.tm_hour = timeS.hour;
            testTimer.tm_min = timeS.minute;
            testTimer.tm_sec = timeS.second;
            testTimer.tm_millisec = 0;
            testTimer.tm_microsec = 0; 
            
            if (testTimer < m_searchCondition.timeFrom || testTimer > m_searchCondition.timeTo){
                showLog = false;
            }
            
            if (showLog && !m_searchCondition.key.empty()){
                bool hasKeyword = (log.at("Time").find(m_searchCondition.key)!=std::string::npos
                    || log.at("Operator").find(m_searchCondition.key)!=std::string::npos
                    || log.at("LogContent").find(m_searchCondition.key)!=std::string::npos);
                if (!hasKeyword){
                    showLog = false;
                }
            }
        }
        
        if (showLog){
            m_searchResults.push_back(log);
        }
    }
    
    m_totalSearchPages = (m_searchResults.size() + SEARCH_PAGE_SIZE - 1) / SEARCH_PAGE_SIZE;
    
    HGExactTime endTime = HGExactTime::currentTime();
    
    std::tm startTimeTm = static_cast<std::tm>(startTime);
    std::tm endTimeTm = static_cast<std::tm>(endTime);
    std::time_t startTime_t = std::mktime(&startTimeTm);
    std::time_t endTime_t = std::mktime(&endTimeTm);
    double searchTime = std::difftime(endTime_t, startTime_t) + 
                       (endTime.tm_microsec - startTime.tm_microsec) / 1000000.0 +
                       (endTime.tm_millisec - startTime.tm_millisec) / 1000.0;
    
    QMessageBox::information(this, QString::fromStdString(HG_DEVICE_NAME),
        QString::fromStdString("搜索完成，找到") + QString::number(m_searchResults.size()) + 
        QString::fromStdString("条记录，耗时") + QString::number(searchTime, 'f', 2) + QString::fromStdString("秒"));
    
    fnDisplaySearchPage();
}

void HGLogWidget::fnDisplaySearchPage(){
    m_tableW->setUpdatesEnabled(false);
    m_tableW->setRowCount(0);
    
    int startIndex = m_curSearchPage * SEARCH_PAGE_SIZE;
    int endIndex = std::min(startIndex + SEARCH_PAGE_SIZE, (int)m_searchResults.size());
    
    m_tableW->setRowCount(endIndex - startIndex);
    
    int rowIndex = 0;
    for (int i = startIndex; i < endIndex; i++){
        const auto& log = m_searchResults[i];
        
        for (auto info : log){
            int nameColIndex = m_logContentMap[info.first];
            if (nameColIndex < 0 || nameColIndex >= m_tableW->columnCount())
                continue;
            
            QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(info.second));
            if (!m_searchCondition.key.empty()){
                fnHighlightKeywords(item, m_searchCondition.key);
            }
            m_tableW->setItem(rowIndex, nameColIndex, item);
        }
        rowIndex++;
    }
    
    m_pageLabel->setText("第"+QString::number(m_curSearchPage+1)+"页/");
    m_totalPageLabel->setText(QString::fromStdString("共") + QString::number(m_totalSearchPages) + QString::fromStdString("页"));
    
    m_tableW->setUpdatesEnabled(true);
}

void HGLogWidget::fnHighlightKeywords(QTableWidgetItem* item, const std::string& keyword){
    if (keyword.empty() || !item) return;
    
    QString text = item->text();
    QString searchKeyword = QString::fromStdString(keyword);
    
    if (text.contains(searchKeyword, Qt::CaseInsensitive)){
        item->setForeground(QBrush(Qt::red));
        item->setFont(QFont(item->font().family(), item->font().pointSize(), QFont::Bold));
    }
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