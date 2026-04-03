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
#include <QtConcurrent>
#include <QProgressDialog>
#include <QApplication>

// 定义每页显示的记录数
#define LOG_PAGE_SIZE 100

HGLogWidget::HGLogWidget(std::string lang,QWidget *parent) : QWidget(parent),
m_lang(lang),
m_curDisplayIndex(-1),
m_isSearchMode(false),
m_searchResultsPageSize(LOG_PAGE_SIZE),
m_searchResultsTotalPages(0),
m_currentSearchPage(0)
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
    // 启用富文本显示
    m_tableW->setTextElideMode(Qt::ElideNone);
    
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
    clearSearchCache();
}

void HGLogWidget::slotNext(){
    if (m_isSearchMode) {
        // 搜索模式下的分页
        if (m_currentSearchPage < m_searchResultsTotalPages - 1) {
            m_currentSearchPage++;
            displaySearchResultsPage(m_currentSearchPage);
        } else {
            QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),
                             "已经是最后一页");
        }
        return;
    }
    
    // 普通模式下的分页
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
        // 搜索模式下的分页
        if (m_currentSearchPage > 0) {
            m_currentSearchPage--;
            displaySearchResultsPage(m_currentSearchPage);
        } else {
            QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),
                             "已经是第一页");
        }
        return;
    }
    
    // 普通模式下的分页
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

bool HGLogWidget::matchesSearchCondition(const std::map<std::string,std::string>& logEntry) {
    // 安全获取字段值，避免at()抛出异常
    auto itTime = logEntry.find("Time");
    auto itOperator = logEntry.find("Operator");
    auto itLogContent = logEntry.find("LogContent");
    
    std::string timeStr = (itTime != logEntry.end()) ? itTime->second : "";
    std::string operatorStr = (itOperator != logEntry.end()) ? itOperator->second : "";
    std::string logContentStr = (itLogContent != logEntry.end()) ? itLogContent->second : "";
    
    // 时间范围过滤
    if (!m_searchCondition.timeRangeFrom.empty() || !m_searchCondition.timeRangeTo.empty()) {
        if (timeStr.empty()) return false;
        
        HGExactTime testTimer = HGExactTime::currentTime();
        TIME_STRUECT timeS;
        decodeStandardTime(timeStr, timeS);
        testTimer.tm_year = timeS.year;
        testTimer.tm_mon = timeS.month;
        testTimer.tm_mday = timeS.day;

        if (!m_searchCondition.timeRangeFrom.empty() && testTimer < m_searchCondition.timeFrom)
            return false;
        if (!m_searchCondition.timeRangeTo.empty() && testTimer > m_searchCondition.timeTo)
            return false;
    }

    // 关键词过滤
    if (!m_searchCondition.key.empty()) {
        bool found = (timeStr.find(m_searchCondition.key) != std::string::npos) ||
                     (operatorStr.find(m_searchCondition.key) != std::string::npos) ||
                     (logContentStr.find(m_searchCondition.key) != std::string::npos);
        if (!found) return false;
    }

    return true;
}

bool HGLogWidget::matchesSearchCondition(const LogEntry& logEntry) {
    // 时间范围过滤
    if (!m_searchCondition.timeRangeFrom.empty() || !m_searchCondition.timeRangeTo.empty()) {
        HGExactTime testTimer = HGExactTime::currentTime();
        TIME_STRUECT timeS;
        decodeStandardTime(logEntry.time, timeS);
        testTimer.tm_year = timeS.year; 
        testTimer.tm_mon = timeS.month; 
        testTimer.tm_mday = timeS.day; 
        
        if (!m_searchCondition.timeRangeFrom.empty() && testTimer < m_searchCondition.timeFrom)
            return false;
        if (!m_searchCondition.timeRangeTo.empty() && testTimer > m_searchCondition.timeTo)
            return false;
    }
    
    // 关键词过滤
    if (!m_searchCondition.key.empty()) {
        bool found = (logEntry.time.find(m_searchCondition.key) != std::string::npos) ||
                     (logEntry.operato.find(m_searchCondition.key) != std::string::npos) ||
                     (logEntry.logContent.find(m_searchCondition.key) != std::string::npos);
        if (!found) return false;
    }
    
    return true;
}

void HGLogWidget::performGlobalSearch() {
    m_searchResults.clear();

    // 创建进度对话框 - 使用通用文本避免翻译键不存在的问题
    QProgressDialog progress(tr("正在搜索..."),
                            tr("取消"),
                            0, m_auditLogTableNames.size(), this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(0);
    
    // 遍历所有表进行搜索
    for (size_t tableIdx = 0; tableIdx < m_auditLogTableNames.size(); ++tableIdx) {
        progress.setValue(tableIdx);
        if (progress.wasCanceled()) {
            break;
        }
        
        const std::string& tableName = m_auditLogTableNames[tableIdx];
        std::vector<std::map<std::string,std::string>> loginfos = RWDb::readAuditTrailLog(tableName);
        
        // 对每个日志条目进行过滤
        for (const auto& logEntry : loginfos) {
            if (matchesSearchCondition(logEntry)) {
                LogEntry entry;
                auto itTime = logEntry.find("Time");
                auto itLogContent = logEntry.find("LogContent");
                auto itOperator = logEntry.find("Operator");
                entry.time = (itTime != logEntry.end()) ? itTime->second : "";
                entry.logContent = (itLogContent != logEntry.end()) ? itLogContent->second : "";
                entry.operato = (itOperator != logEntry.end()) ? itOperator->second : "";
                m_searchResults.push_back(entry);
            }
        }
        
        // 每处理10个表刷新一次UI，保持响应
        if (tableIdx % 10 == 0) {
            QApplication::processEvents();
        }
    }
    
    progress.setValue(m_auditLogTableNames.size());
    
    // 按时间倒序排序
    std::sort(m_searchResults.begin(), m_searchResults.end(), 
              [](const LogEntry& a, const LogEntry& b) {
                  return a.time > b.time;
              });
    
    // 计算总页数
    m_searchResultsTotalPages = (m_searchResults.size() + m_searchResultsPageSize - 1) / m_searchResultsPageSize;
    if (m_searchResultsTotalPages == 0) m_searchResultsTotalPages = 1;
    m_currentSearchPage = 0;
}

void HGLogWidget::displaySearchResultsPage(int pageIndex) {
    if (pageIndex < 0 || pageIndex >= m_searchResultsTotalPages) return;
    
    m_tableW->setRowCount(0);
    m_tableW->setUpdatesEnabled(false);
    
    int startIdx = pageIndex * m_searchResultsPageSize;
    int endIdx = std::min(startIdx + m_searchResultsPageSize, (int)m_searchResults.size());
    
    m_tableW->setRowCount(endIdx - startIdx);
    
    for (int i = startIdx; i < endIdx; ++i) {
        int row = i - startIdx;
        const LogEntry& entry = m_searchResults[i];
        
        // 时间列
        QTableWidgetItem* timeItem = new QTableWidgetItem(QString::fromStdString(entry.time));
        m_tableW->setItem(row, 0, timeItem);
        if (!m_searchCondition.key.empty()) {
            highlightKeyword(timeItem, m_searchCondition.key);
        }
        
        // 日志内容列
        QTableWidgetItem* contentItem = new QTableWidgetItem(QString::fromStdString(entry.logContent));
        m_tableW->setItem(row, 1, contentItem);
        if (!m_searchCondition.key.empty()) {
            highlightKeyword(contentItem, m_searchCondition.key);
        }
        
        // 操作员列
        QTableWidgetItem* operatorItem = new QTableWidgetItem(QString::fromStdString(entry.operato));
        m_tableW->setItem(row, 2, operatorItem);
        if (!m_searchCondition.key.empty()) {
            highlightKeyword(operatorItem, m_searchCondition.key);
        }
    }
    
    m_tableW->setUpdatesEnabled(true);
    m_pageLabel->setText("第" + QString::number(pageIndex + 1) + "/" + QString::number(m_searchResultsTotalPages) + "页");
}

void HGLogWidget::highlightKeyword(QTableWidgetItem* item, const std::string& keyword) {
    if (!item || keyword.empty()) return;

    QString text = item->text();
    QString keywordStr = QString::fromStdString(keyword);

    // 检查是否包含关键词（不区分大小写）
    if (text.contains(keywordStr, Qt::CaseInsensitive)) {
        // 使用富文本格式高亮关键词
        QString highlightedText = text;
        Qt::CaseSensitivity cs = Qt::CaseInsensitive;

        // 替换所有匹配的关键词为带样式的HTML
        int pos = 0;
        while ((pos = highlightedText.indexOf(keywordStr, pos, cs)) != -1) {
            QString before = highlightedText.left(pos);
            QString match = highlightedText.mid(pos, keywordStr.length());
            QString after = highlightedText.mid(pos + keywordStr.length());

            highlightedText = before + "<span style='background-color: yellow; color: red; font-weight: bold;'>" +
                            match + "</span>" + after;
            pos += QString("<span style='background-color: yellow; color: red; font-weight: bold;'>").length() +
                   keywordStr.length() + QString("</span>").length();
        }

        // 使用setData设置Qt::DisplayRole，让表格支持HTML渲染
        item->setData(Qt::DisplayRole, highlightedText);
        // 设置文本对齐方式
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    }
}

void HGLogWidget::clearSearchCache() {
    m_searchResults.clear();
    m_searchResults.shrink_to_fit();
}

void HGLogWidget::fnReadDB(const std::string &tableName){
    m_tableW->setRowCount(0);
    std::vector<std::map<std::string,std::string>> loginfos;
    m_tableW->setUpdatesEnabled(false);
    switch (m_logTypeComboBox->currentIndex()){
        case 0:
        {
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

            // 先过滤符合条件的记录，计算实际行数
            std::vector<std::map<std::string,std::string>> filteredLogs;
            for (int i =int(loginfos.size())-1;i>=0;i--){
                if (!m_searchCondition.isInit()){
                    auto itTime = loginfos[i].find("Time");
                    if (itTime != loginfos[i].end()) {
                        std::string timeStr = itTime->second;
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
                    }

                    if (m_searchCondition.key!=""){
                        auto itTime = loginfos[i].find("Time");
                        auto itOperator = loginfos[i].find("Operator");
                        auto itLogContent = loginfos[i].find("LogContent");
                        std::string timeVal = (itTime != loginfos[i].end()) ? itTime->second : "";
                        std::string operatorVal = (itOperator != loginfos[i].end()) ? itOperator->second : "";
                        std::string logContentVal = (itLogContent != loginfos[i].end()) ? itLogContent->second : "";

                        if (!(timeVal.find(m_searchCondition.key)!=std::string::npos
                            ||operatorVal.find(m_searchCondition.key)!=std::string::npos
                            ||logContentVal.find(m_searchCondition.key)!=std::string::npos)){
                            continue;
                        }
                    }
                }
                filteredLogs.push_back(loginfos[i]);
            }

            // 设置正确的行数
            m_tableW->setRowCount(filteredLogs.size());

            // 填充数据
            for (const auto& logEntry : filteredLogs){
                for (const auto& info : logEntry){
                    auto itCol = m_logContentMap.find(info.first);
                    if (itCol == m_logContentMap.end()) continue;
                    int nameColIndex = itCol->second;
                    if (nameColIndex<0||nameColIndex>=m_tableW->columnCount())
                        continue;
                    QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(info.second));
                    m_tableW->setItem(traillogIndex,nameColIndex,item);
                    // 高亮关键词
                    if (!m_searchCondition.key.empty()) {
                        highlightKeyword(item, m_searchCondition.key);
                    }
                }
                traillogIndex++;
            }
            break;
        }
        case 1:
        {
            std::vector<FileInfo> fileList;
            HGGetFilesNoBytes("/app/log/",".log",fileList);
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
            // printf("search fileList\n");
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
                std::ifstream file(fileList[i].filename); // 打开文件
                if (!file.is_open())
                { // 检查文件是否成功打开
                    // std::cerr << "无法打开文件: " << filename << std::endl;
                    continue;
                }

                std::string line;
                while (std::getline(file, line))
                {                                   // 逐行读取文件
                    if (m_searchCondition.key.length()>0){
                        if (line.find(m_searchCondition.key)==std::string::npos &&
                            GlobalSingleton::instance().getSystemInfo("loginName").find(m_searchCondition.key)!=std::string::npos)
                            continue;
                    } 
                    m_tableW->insertRow(m_tableW->rowCount());
                    int pos=line.find_first_of(">");
                    QTableWidgetItem* timeItem = new QTableWidgetItem(QString::fromStdString(line.substr(0,pos-1)));
                    QTableWidgetItem* contentItem = new QTableWidgetItem(QString::fromStdString(line.substr(pos+1,line.length()-pos-1)));
                    QTableWidgetItem* operatorItem = new QTableWidgetItem(QString::fromStdString(GlobalSingleton::instance().getSystemInfo("loginName")));
                    
                    m_tableW->setItem(m_tableW->rowCount()-1, 0, timeItem);
                    m_tableW->setItem(m_tableW->rowCount()-1, 1, contentItem);
                    m_tableW->setItem(m_tableW->rowCount()-1, 2, operatorItem);
                    
                    // 高亮关键词
                    if (!m_searchCondition.key.empty()) {
                        highlightKeyword(timeItem, m_searchCondition.key);
                        highlightKeyword(contentItem, m_searchCondition.key);
                        highlightKeyword(operatorItem, m_searchCondition.key);
                    }
                }

                file.close(); // 关闭文件
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
    // 检查是否有搜索条件
    if (m_searchCondition.isInit()) {
        // 没有搜索条件，返回普通模式
        m_isSearchMode = false;
        clearSearchCache();
        fnReadDB(m_auditLogTableNames[m_curDisplayIndex]);
        return;
    }
    
    // 进入搜索模式
    m_isSearchMode = true;
    
    // 执行全局搜索
    performGlobalSearch();
    
    // 显示第一页结果
    displaySearchResultsPage(0);
    
    // 显示搜索结果统计
    if (m_searchResults.empty()) {
        QMessageBox::information(this, QString::fromStdString(HG_DEVICE_NAME),
                                tr("未找到匹配的日志记录"));
    }
}

void HGLogWidget::slotClearSearch(){ 
    m_searchCondition.Clear();
    m_isSearchMode = false;
    clearSearchCache();
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
                // 去除HTML标签
                QString text = m_tableW->item(i,j)->text();
                text.remove(QRegExp("<[^>]*>"));
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
