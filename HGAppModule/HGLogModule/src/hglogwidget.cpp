#include "hglogwidget.h"
#include <QHeaderView>
#include "common.h"
#include <fstream>
#include <algorithm>
#include <QMessageBox>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QBrush>
#include <QColor>
#include <QProgressDialog>
#include <QApplication>
#include <QRegularExpression>


HGLogWidget::HGLogWidget(std::string lang,QWidget *parent) : QWidget(parent),
m_lang(lang),
m_curDisplayIndex(-1),
m_isInSearchMode(false)
{
    RWDb::writeAuditTrailLog(loadTranslation(m_lang,"Enter")+loadTranslation(m_lang,"Log"));
    m_auditLogTableNames = RWDb::getAllAuditLogTables();
    m_searchCondition.Clear();
    
    // 初始化搜索结果
    m_searchResult.totalCount = 0;
    m_searchResult.currentPage = 0;
    m_searchResult.totalPages = 0;

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

    m_pageLabel=new QLabel("第"+QString::number(m_curDisplayIndex)+"页");
    m_saveLabel=new HGQLabel(false,getPath("/resources/V1/@1xmb-save 1.png")); 
    m_nextLabel=new HGQLabel(false,getPath("/resources/V1/@1xze-arrow 1.png")); 
    m_preLabel=new HGQLabel(false,getPath("/resources/V1/@1xze-arrow-left 1.png")); 
    
    // 搜索结果分页控件
    m_searchNextLabel=new HGQLabel(false,getPath("/resources/V1/@1xze-arrow 1.png")); 
    m_searchPreLabel=new HGQLabel(false,getPath("/resources/V1/@1xze-arrow-left 1.png")); 
    m_searchPageLabel=new QLabel("");
    m_searchNextLabel->setVisible(false);
    m_searchPreLabel->setVisible(false);
    m_searchPageLabel->setVisible(false);
    
    connect(m_saveLabel,SIGNAL(leftClicked()),this,SLOT(slotSaveSearchLog()));
    connect(m_nextLabel,SIGNAL(leftClicked()),this,SLOT(slotNext()));
    connect(m_preLabel,SIGNAL(leftClicked()),this,SLOT(slotPre()));
    connect(m_searchNextLabel,SIGNAL(leftClicked()),this,SLOT(slotSearchNext()));
    connect(m_searchPreLabel,SIGNAL(leftClicked()),this,SLOT(slotSearchPre()));

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

    m_manipulateLayout->addWidget(m_saveLabel,0,2);
    m_manipulateLayout->addWidget(m_preLabel,0,3);
    m_manipulateLayout->addWidget(m_nextLabel,0,4);
    m_manipulateLayout->addWidget(m_pageLabel,0,5);
    m_manipulateLayout->addWidget(m_searchPreLabel,0,6);
    m_manipulateLayout->addWidget(m_searchNextLabel,0,7);
    m_manipulateLayout->addWidget(m_searchPageLabel,0,8);
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
    if (m_isInSearchMode) {
        // 搜索模式下禁用普通分页
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
    if (m_isInSearchMode) {
        // 搜索模式下禁用普通分页
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

void HGLogWidget::slotSearchNext(){
    if (!m_isInSearchMode) return;
    if (m_searchResult.currentPage < m_searchResult.totalPages - 1) {
        m_searchResult.currentPage++;
        displaySearchResults();
    } else {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),
                         "已经是最后一页");
    }
}

void HGLogWidget::slotSearchPre(){
    if (!m_isInSearchMode) return;
    if (m_searchResult.currentPage > 0) {
        m_searchResult.currentPage--;
        displaySearchResults();
    } else {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),
                         "已经是第一页");
    }
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

bool HGLogWidget::matchSearchCondition(const std::map<std::string,std::string>& logInfo, const SearchCondition& condition){
    // 时间范围过滤
    if (condition.timeRangeFrom != "" && condition.timeRangeTo != "") {
        std::string timeStr = logInfo.at("Time");
        HGExactTime testTimer = HGExactTime::currentTime();
        TIME_STRUECT timeS;
        decodeStandardTime(timeStr, timeS);
        testTimer.tm_year = timeS.year; 
        testTimer.tm_mon = timeS.month; 
        testTimer.tm_mday = timeS.day; 
        if (testTimer < condition.timeFrom)
            return false;
        if (testTimer > condition.timeTo)
            return false;
    }
    
    // 关键词过滤
    if (condition.key != ""){
        bool found = false;
        auto it = logInfo.find("Time");
        if (it != logInfo.end() && it->second.find(condition.key) != std::string::npos)
            found = true;
        it = logInfo.find("Operator");
        if (it != logInfo.end() && it->second.find(condition.key) != std::string::npos)
            found = true;
        it = logInfo.find("LogContent");
        if (it != logInfo.end() && it->second.find(condition.key) != std::string::npos)
            found = true;
        if (!found)
            return false;
    }
    
    return true;
}

std::vector<LogEntry> HGLogWidget::searchInTable(const std::string& tableName, const SearchCondition& condition){
    std::vector<LogEntry> results;
    
    std::map<std::string,std::string> infoS = {
        {"Operator",""},
        {"Time",""},
        {"LogContent",""}
    };
    
    std::vector<std::map<std::string,std::string>> loginfos = RWDb::readAuditTrailLog(tableName);
    
    for (const auto& logInfo : loginfos){
        if (matchSearchCondition(logInfo, condition)){
            LogEntry entry;
            entry.time = logInfo.at("Time");
            entry.logContent = logInfo.at("LogContent");
            entry.operatorName = logInfo.at("Operator");
            entry.sourceTable = tableName;
            results.push_back(entry);
        }
    }
    
    return results;
}

void HGLogWidget::searchAllLogs(){
    if (m_auditLogTableNames.empty()) return;
    
    // 显示进度对话框
    QProgressDialog progress(QString::fromStdString(loadTranslation(m_lang,"Searching...")), 
                            QString::fromStdString(loadTranslation(m_lang,"Cancel")), 
                            0, m_auditLogTableNames.size(), this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(0);
    
    std::vector<LogEntry> allResults;
    allResults.reserve(10000);  // 预分配空间
    
    // 从最新的表开始搜索（倒序）
    for (int i = int(m_auditLogTableNames.size()) - 1; i >= 0; i--){
        progress.setValue(int(m_auditLogTableNames.size()) - 1 - i);
        if (progress.wasCanceled()){
            break;
        }
        
        QApplication::processEvents();  // 保持UI响应
        
        auto tableResults = searchInTable(m_auditLogTableNames[i], m_searchCondition);
        allResults.insert(allResults.end(), tableResults.begin(), tableResults.end());
        
        // 限制最大搜索结果数量，避免内存溢出
        if (allResults.size() > 50000) {
            QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),
                QString::fromStdString(loadTranslation(m_lang,"TooManyResults")));
            break;
        }
    }
    
    progress.setValue(m_auditLogTableNames.size());
    
    // 按时间倒序排序
    std::sort(allResults.begin(), allResults.end(), [](const LogEntry& a, const LogEntry& b){
        return a.time > b.time;
    });
    
    // 填充搜索结果结构
    m_searchResult.entries = allResults;
    m_searchResult.totalCount = int(allResults.size());
    m_searchResult.totalPages = (m_searchResult.totalCount + SearchResult::ITEMS_PER_PAGE - 1) / SearchResult::ITEMS_PER_PAGE;
    m_searchResult.currentPage = 0;
    
    m_isInSearchMode = true;
    
    // 更新UI显示模式
    m_nextLabel->setVisible(false);
    m_preLabel->setVisible(false);
    m_pageLabel->setVisible(false);
    m_searchNextLabel->setVisible(m_searchResult.totalPages > 1);
    m_searchPreLabel->setVisible(m_searchResult.totalPages > 1);
    m_searchPageLabel->setVisible(true);
    
    displaySearchResults();
}

void HGLogWidget::highlightKeyword(QTableWidgetItem* item, const std::string& keyword){
    if (keyword.empty() || !item) return;
    
    QString text = item->text();
    QString keywordStr = QString::fromStdString(keyword);
    
    if (!text.contains(keywordStr, Qt::CaseInsensitive)) return;
    
    // 使用富文本高亮关键词
    QString highlightedText = text;
    Qt::CaseSensitivity cs = Qt::CaseInsensitive;
    
    int pos = 0;
    QString result;
    while (pos < text.length()) {
        int idx = text.indexOf(keywordStr, pos, cs);
        if (idx == -1) {
            result += text.mid(pos);
            break;
        }
        result += text.mid(pos, idx - pos);
        result += "<span style='background-color: yellow; color: red; font-weight: bold;'>" 
                + text.mid(idx, keywordStr.length()) + "</span>";
        pos = idx + keywordStr.length();
    }
    
    item->setText(result);
}

void HGLogWidget::displaySearchResults(){
    m_tableW->setRowCount(0);
    
    if (m_searchResult.entries.empty()){
        m_searchPageLabel->setText(QString::fromStdString(loadTranslation(m_lang,"NoResults")));
        QMessageBox::information(this, QString::fromStdString(HG_DEVICE_NAME),
                         QString::fromStdString(loadTranslation(m_lang,"NoMatchingLogs")));
        return;
    }
    
    // 计算当前页的数据范围
    int startIdx = m_searchResult.currentPage * SearchResult::ITEMS_PER_PAGE;
    int endIdx = std::min(startIdx + SearchResult::ITEMS_PER_PAGE, m_searchResult.totalCount);
    
    m_tableW->setUpdatesEnabled(false);
    m_tableW->setRowCount(endIdx - startIdx);
    
    for (int i = startIdx; i < endIdx; i++){
        const LogEntry& entry = m_searchResult.entries[i];
        int row = i - startIdx;
        
        QTableWidgetItem* timeItem = new QTableWidgetItem(QString::fromStdString(entry.time));
        QTableWidgetItem* contentItem = new QTableWidgetItem(QString::fromStdString(entry.logContent));
        QTableWidgetItem* operatorItem = new QTableWidgetItem(QString::fromStdString(entry.operatorName));
        
        // 高亮关键词
        if (!m_searchCondition.key.empty()){
            highlightKeyword(timeItem, m_searchCondition.key);
            highlightKeyword(contentItem, m_searchCondition.key);
            highlightKeyword(operatorItem, m_searchCondition.key);
        }
        
        m_tableW->setItem(row, 0, timeItem);
        m_tableW->setItem(row, 1, contentItem);
        m_tableW->setItem(row, 2, operatorItem);
    }
    
    m_tableW->setUpdatesEnabled(true);
    
    // 更新页码显示
    m_searchPageLabel->setText(QString("%1/%2页(共%3条)")
        .arg(m_searchResult.currentPage + 1)
        .arg(m_searchResult.totalPages)
        .arg(m_searchResult.totalCount));
}

void HGLogWidget::fnReadDB(const std::string &tableName){
    m_tableW->setRowCount(0);
    HGExactTime start,end,start1,end1;
    std::vector<std::map<std::string,std::string>> loginfos;
    m_tableW->setUpdatesEnabled(false);
    
    switch (m_logTypeComboBox->currentIndex()){
        case 0:
        {
            const int MAXROW = 1000;
            m_tableW->setRowCount(MAXROW);
            start =HGExactTime::currentTime();
            int auditTrailLogCount=RWDb::readAuditTrailLogCount(tableName);
            if (auditTrailLogCount > 10000){
                if (m_searchCondition.isInit()){
                    QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),
                                     QString::fromStdString(loadTranslation(m_lang,"TooManagLogFiles")));
                    return;
                }
            }
            loginfos=RWDb::readAuditTrailLog(tableName);
            getTableNameIndex(tableName);
            m_pageLabel->setText("第"+QString::number(m_curDisplayIndex+1)+"页");
            int traillogIndex = 0;
            m_tableW->setRowCount(MAXROW > int(loginfos.size()) ? MAXROW : int(loginfos.size()));
            for (int i =int(loginfos.size())-1;i>=0;i--){
                if (!m_searchCondition.isInit()){
                    std::string timeStr = loginfos[i]["Time"];
                    HGExactTime testTimer = HGExactTime::currentTime();
                    TIME_STRUECT timeS;
                    decodeStandardTime(timeStr, timeS);
                    testTimer.tm_year = timeS.year; 
                    testTimer.tm_mon = timeS.month; 
                    testTimer.tm_mday = timeS.day; 
                    if (testTimer < m_searchCondition.timeFrom)
                        continue;
                    if (testTimer > m_searchCondition.timeTo)
                        continue;
                    
                    if (m_searchCondition.key!=""){
                        if (!(loginfos[i]["Time"].find(m_searchCondition.key)!=std::string::npos
                            ||loginfos[i]["Operator"].find(m_searchCondition.key)!=std::string::npos
                            ||loginfos[i]["LogContent"].find(m_searchCondition.key)!=std::string::npos)){
                            continue;
                        }
                    }
                }
                for (auto info:loginfos[i]){
                    int nameColIndex=m_logContentMap[info.first];
                    if (nameColIndex<0||nameColIndex>=m_tableW->columnCount())
                        continue;
                    m_tableW->setItem(traillogIndex,nameColIndex,new QTableWidgetItem(QString::fromStdString(info.second)));
                }
                traillogIndex++;
            }
            break;
        }
        case 1:
        {
            std::vector<FileInfo> fileList;
            HGGetFilesNoBytes("/app/log/",".log",fileList);
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
            for (int i = int(fileList.size()-1); i < int(fileList.size()); i++)
            {
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

                    if (testTimer < m_searchCondition.timeFrom)
                        continue;
                    if (testTimer > m_searchCondition.timeTo)
                        continue;
                }
                std::ifstream file(fileList[i].filename);
                if (!file.is_open())
                {
                    continue;
                }

                std::string line;
                while (std::getline(file, line))
                {
                    if (m_searchCondition.key.length()>0){
                        if (line.find(m_searchCondition.key)==std::string::npos &&
                            GlobalSingleton::instance().getSystemInfo("loginName").find(m_searchCondition.key)!=std::string::npos)
                            continue;
                    } 
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
    // 执行跨所有表的搜索
    searchAllLogs();
}

void HGLogWidget::slotClearSearch(){ 
    m_searchCondition.Clear();
    m_isInSearchMode = false;
    
    // 恢复普通分页控件
    m_nextLabel->setVisible(true);
    m_preLabel->setVisible(true);
    m_pageLabel->setVisible(true);
    m_searchNextLabel->setVisible(false);
    m_searchPreLabel->setVisible(false);
    m_searchPageLabel->setVisible(false);
    
    // 清空搜索结果
    m_searchResult.entries.clear();
    m_searchResult.totalCount = 0;
    m_searchResult.currentPage = 0;
    m_searchResult.totalPages = 0;
    
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
                // 保存时去除HTML标签
                QString text = m_tableW->item(i,j)->text();
                QRegularExpression htmlTagRegex("<[^>]*>");
                text.remove(htmlTagRegex);
                log[key]=text.toStdString();
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
