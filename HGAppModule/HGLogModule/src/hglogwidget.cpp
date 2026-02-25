#include "hglogwidget.h"
#include <QHeaderView>
#include "common.h"
#include <fstream>
#include <algorithm>
#include <QMessageBox>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include <QLabel>

// 高亮表格项实现
HighlightedTableItem::HighlightedTableItem(const QString& text)
    : QTableWidgetItem(text)
    , m_highlightText()
{
}

void HighlightedTableItem::setHighlightText(const QString& highlightText)
{
    m_highlightText = highlightText;
}

QString HighlightedTableItem::highlightText() const
{
    return m_highlightText;
}

HGLogWidget::HGLogWidget(std::string lang,QWidget *parent) : QWidget(parent),
m_lang(lang),
m_curDisplayIndex(-1),
m_isSearchMode(false),
m_searchCurrentPage(0),
m_searchTotalCount(0),
m_searchPageSize(100),  // 每页100条记录
m_sortColumn(0),        // 默认按时间列排序
m_sortAscending(false)  // 默认降序（最新的在前）
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
    
    // 连接表头点击信号用于排序
    m_tableW->horizontalHeader()->setSortIndicatorShown(true);
    m_tableW->horizontalHeader()->setSortIndicator(0, Qt::DescendingOrder);  // 默认时间降序
    connect(m_tableW->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(slotHeaderClicked(int)));
    
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
    if (m_logTypeComboBox->currentIndex() == 0) {
        // 审计日志模式
        if (m_isSearchMode) {
            // 搜索模式下的分页
            int totalPages = (m_searchTotalCount + m_searchPageSize - 1) / m_searchPageSize;
            if (m_searchCurrentPage < totalPages - 1) {
                m_searchCurrentPage++;
                fnReadDBSearchMode();
            } else {
                QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),
                                 "已经是最后一页");
            }
        } else {
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
    } else {
        // 运行日志模式（保持原有逻辑）
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),
                         "已经是最后一页");
    }
}

void HGLogWidget::slotPre(){
    if (m_logTypeComboBox->currentIndex() == 0) {
        // 审计日志模式
        if (m_isSearchMode) {
            // 搜索模式下的分页
            if (m_searchCurrentPage > 0) {
                m_searchCurrentPage--;
                fnReadDBSearchMode();
            } else {
                QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),
                                 "已经是第一页");
            }
        } else {
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
    } else {
        // 运行日志模式（保持原有逻辑）
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

void HGLogWidget::updatePageLabel()
{
    if (m_isSearchMode) {
        int totalPages = (m_searchTotalCount + m_searchPageSize - 1) / m_searchPageSize;
        if (totalPages == 0) totalPages = 1;
        m_pageLabel->setText(QString("第%1/%2页 (共%3条)").arg(m_searchCurrentPage + 1).arg(totalPages).arg(m_searchTotalCount));
    } else {
        m_pageLabel->setText("第"+QString::number(m_curDisplayIndex+1)+"页");
    }
}

QTableWidgetItem* HGLogWidget::createHighlightedItem(const std::string& text, const std::string& keyword)
{
    QString displayText = QString::fromStdString(text);
    
    // 创建表格项（用于排序和存储数据）
    QTableWidgetItem* item = new QTableWidgetItem(displayText);
    item->setToolTip(displayText);
    
    if (keyword.empty() || text.find(keyword) == std::string::npos) {
        // 不需要高亮
        return item;
    }
    
    // 需要高亮的情况 - 设置标记，后续用QLabel显示
    item->setData(Qt::UserRole, QString::fromStdString(keyword));  // 存储关键词用于高亮
    
    return item;
}

void HGLogWidget::fnReadDBSearchMode()
{
    m_tableW->setRowCount(0);
    m_tableW->setUpdatesEnabled(false);
    
    // 转换时间格式：从 YYYYMMDD 转换为 YYYY-MM-DD（数据库中时间格式为 YYYY-MM-DD_HH:MM:SS 时区）
    std::string timeFrom, timeTo;
    if (!m_searchCondition.timeRangeFrom.empty()) {
        // 格式：YYYYMMDD -> YYYY-MM-DD
        timeFrom = m_searchCondition.timeRangeFrom.substr(0, 4) + "-" +
                   m_searchCondition.timeRangeFrom.substr(4, 2) + "-" +
                   m_searchCondition.timeRangeFrom.substr(6, 2);
    }
    if (!m_searchCondition.timeRangeTo.empty()) {
        // 格式：YYYYMMDD -> YYYY-MM-DD
        timeTo = m_searchCondition.timeRangeTo.substr(0, 4) + "-" +
                 m_searchCondition.timeRangeTo.substr(4, 2) + "-" +
                 m_searchCondition.timeRangeTo.substr(6, 2);
    }
    
    // 计算偏移量
    int offset = m_searchCurrentPage * m_searchPageSize;
    
    // 执行搜索
    int currentTotal = 0;
    m_searchResults = RWDb::searchAuditTrailLog(
        m_searchCondition.key,
        timeFrom,
        timeTo,
        offset,
        m_searchPageSize,
        currentTotal
    );
    
    // 只在第一页或总数有效时更新总数
    if (m_searchCurrentPage == 0 || currentTotal >= 0) {
        m_searchTotalCount = currentTotal;
    }
    
    // 设置表格行数
    m_tableW->setRowCount(static_cast<int>(m_searchResults.size()));
    
    // 填充数据 - 使用批量设置提高性能
    for (int i = 0; i < static_cast<int>(m_searchResults.size()); i++) {
        QString keywordQt = QString::fromStdString(m_searchCondition.key);
        
        // 时间列
        QString timeText = QString::fromStdString(m_searchResults[i]["Time"]);
        QTableWidgetItem* timeItem = new QTableWidgetItem(timeText);
        timeItem->setToolTip(timeText);
        m_tableW->setItem(i, 0, timeItem);
        // 如果需要高亮，使用QLabel
        if (!m_searchCondition.key.empty() && m_searchResults[i]["Time"].find(m_searchCondition.key) != std::string::npos) {
            QLabel* timeLabel = new QLabel();
            QString highlightedText = timeText;
            highlightedText.replace(keywordQt, "<span style='background-color: #FFFF00; color: #FF0000; font-weight: bold;'>" + keywordQt + "</span>");
            timeLabel->setText(highlightedText);
            timeLabel->setTextFormat(Qt::RichText);
            m_tableW->setCellWidget(i, 0, timeLabel);
        }
        
        // 日志内容列
        QString contentText = QString::fromStdString(m_searchResults[i]["LogContent"]);
        QTableWidgetItem* contentItem = new QTableWidgetItem(contentText);
        contentItem->setToolTip(contentText);
        m_tableW->setItem(i, 1, contentItem);
        // 如果需要高亮，使用QLabel
        if (!m_searchCondition.key.empty() && m_searchResults[i]["LogContent"].find(m_searchCondition.key) != std::string::npos) {
            QLabel* contentLabel = new QLabel();
            QString highlightedText = contentText;
            highlightedText.replace(keywordQt, "<span style='background-color: #FFFF00; color: #FF0000; font-weight: bold;'>" + keywordQt + "</span>");
            contentLabel->setText(highlightedText);
            contentLabel->setTextFormat(Qt::RichText);
            contentLabel->setWordWrap(true);
            m_tableW->setCellWidget(i, 1, contentLabel);
        }
        
        // 操作员列
        QString operatorText = QString::fromStdString(m_searchResults[i]["Operator"]);
        QTableWidgetItem* operatorItem = new QTableWidgetItem(operatorText);
        operatorItem->setToolTip(operatorText);
        m_tableW->setItem(i, 2, operatorItem);
        // 如果需要高亮，使用QLabel
        if (!m_searchCondition.key.empty() && m_searchResults[i]["Operator"].find(m_searchCondition.key) != std::string::npos) {
            QLabel* operatorLabel = new QLabel();
            QString highlightedText = operatorText;
            highlightedText.replace(keywordQt, "<span style='background-color: #FFFF00; color: #FF0000; font-weight: bold;'>" + keywordQt + "</span>");
            operatorLabel->setText(highlightedText);
            operatorLabel->setTextFormat(Qt::RichText);
            m_tableW->setCellWidget(i, 2, operatorLabel);
        }
    }
    
    m_tableW->setUpdatesEnabled(true);
    updatePageLabel();
}

void HGLogWidget::fnReadDBNormalMode(const std::string& tableName)
{
    m_tableW->setRowCount(0);
    HGExactTime start,end,start1,end1;
    std::vector<std::map<std::string,std::string>> loginfos;
    m_tableW->setUpdatesEnabled(false);
    
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
    
    m_tableW->setUpdatesEnabled(true);
    updatePageLabel();
}

void HGLogWidget::fnReadDB(const std::string &tableName){
    if (m_logTypeComboBox->currentIndex() != 0) {
        // 运行日志模式（保持原有逻辑）
        m_tableW->setRowCount(0);
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
        return;
    }
    
    // 审计日志模式
    if (m_isSearchMode) {
        fnReadDBSearchMode();
    } else {
        fnReadDBNormalMode(tableName);
    }
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
    
    // 重置搜索模式
    m_isSearchMode = false;
    m_searchCurrentPage = 0;
    m_searchTotalCount = 0;
    
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
    if (m_logTypeComboBox->currentIndex() != 0) {
        // 运行日志模式（保持原有逻辑）
        m_tableW->setRowCount(0);
        fnReadDB(m_auditLogTableNames[m_curDisplayIndex]);
        return;
    }
    
    // 审计日志模式 - 启用搜索模式
    m_isSearchMode = true;
    m_searchCurrentPage = 0;
    m_searchTotalCount = 0;
    // 重置排序状态为默认（时间降序）
    m_sortColumn = 0;
    m_sortAscending = false;
    m_tableW->horizontalHeader()->setSortIndicator(0, Qt::DescendingOrder);
    
    fnReadDB("");
}

void HGLogWidget::slotClearSearch(){ 
    m_searchCondition.Clear();
    m_isSearchMode = false;
    m_searchCurrentPage = 0;
    m_searchTotalCount = 0;
    // 重置排序状态
    m_sortColumn = 0;
    m_sortAscending = false;
    m_tableW->horizontalHeader()->setSortIndicator(0, Qt::DescendingOrder);
    fnReadDB(m_auditLogTableNames[m_curDisplayIndex]);
}

void HGLogWidget::slotHeaderClicked(int column)
{
    // 只在搜索模式下支持排序
    if (!m_isSearchMode) {
        return;
    }
    
    // 切换排序方向
    if (m_sortColumn == column) {
        m_sortAscending = !m_sortAscending;
    } else {
        m_sortColumn = column;
        m_sortAscending = true;  // 新列默认升序
    }
    
    // 更新排序指示器
    m_tableW->horizontalHeader()->setSortIndicator(column, m_sortAscending ? Qt::AscendingOrder : Qt::DescendingOrder);
    
    // 对当前页结果进行排序
    std::sort(m_searchResults.begin(), m_searchResults.end(), 
        [this, column](const std::map<std::string,std::string>& a, const std::map<std::string,std::string>& b) {
            QString keyA, keyB;
            switch (column) {
                case 0:  // 时间列
                    keyA = QString::fromStdString(a.at("Time"));
                    keyB = QString::fromStdString(b.at("Time"));
                    break;
                case 1:  // 日志内容列
                    keyA = QString::fromStdString(a.at("LogContent"));
                    keyB = QString::fromStdString(b.at("LogContent"));
                    break;
                case 2:  // 操作员列
                    keyA = QString::fromStdString(a.at("Operator"));
                    keyB = QString::fromStdString(b.at("Operator"));
                    break;
                default:
                    return false;
            }
            
            if (m_sortAscending) {
                return keyA < keyB;
            } else {
                return keyA > keyB;
            }
        });
    
    // 刷新显示
    fnReadDBSearchMode();
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
