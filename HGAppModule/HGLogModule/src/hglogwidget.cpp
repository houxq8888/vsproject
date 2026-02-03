#include "hglogwidget.h"
#include <QHeaderView>
#include "common.h"
#include <fstream>
#include <algorithm>
#include <QMessageBox>
#include <QApplication>
#include <ctime>


HGLogWidget::HGLogWidget(std::string lang,QWidget *parent) : QWidget(parent),
m_lang(lang),
m_curDisplayIndex(-1),
m_searchResultCurrentPage(0),
m_isInSearchMode(false)
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
    
}
void HGLogWidget::slotNext(){
    // 搜索模式下的分页
    if (m_isInSearchMode) {
        int totalPages = (m_searchResults.size() + SEARCH_RESULTS_PER_PAGE - 1) / SEARCH_RESULTS_PER_PAGE;
        if (m_searchResultCurrentPage < totalPages - 1) {
            m_searchResultCurrentPage++;
            displaySearchResultsPage(m_searchResultCurrentPage);
        } else {
            QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),
                             QString::fromStdString(loadTranslation(m_lang, "LastPage")));
        }
        return;
    }
    
    // 普通模式下的分页
    if (m_curDisplayIndex < 0) return;
    if (m_curDisplayIndex < int(m_auditLogTableNames.size())-1) m_curDisplayIndex++;
    else {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),
                         QString::fromStdString(loadTranslation(m_lang, "LastPage")));
        m_curDisplayIndex=m_auditLogTableNames.size()-1;
    }
    std::string dbName=m_auditLogTableNames[m_curDisplayIndex];
    fnReadDB(dbName);
}
void HGLogWidget::slotPre(){
    // 搜索模式下的分页
    if (m_isInSearchMode) {
        if (m_searchResultCurrentPage > 0) {
            m_searchResultCurrentPage--;
            displaySearchResultsPage(m_searchResultCurrentPage);
        } else {
            QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),
                             QString::fromStdString(loadTranslation(m_lang, "FirstPage")));
        }
        return;
    }
    
    // 普通模式下的分页
    if (m_curDisplayIndex < 0) {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),
                         QString::fromStdString(loadTranslation(m_lang, "FirstPage")));
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
    HGExactTime start,end,start1,end1;
    std::vector<std::map<std::string,std::string>> loginfos;
    m_tableW->setUpdatesEnabled(false);  // 禁用更新
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
                    m_tableW->setItem(m_tableW->rowCount()-1, 0, new QTableWidgetItem(QString::fromStdString(line.substr(0,pos-1))));
                    m_tableW->setItem(m_tableW->rowCount()-1, 1, new QTableWidgetItem(QString::fromStdString(line.substr(pos+1,line.length()-pos-1))));
                    m_tableW->setItem(m_tableW->rowCount()-1, 2, new QTableWidgetItem(QString::fromStdString(GlobalSingleton::instance().getSystemInfo("loginName"))));
                }

                file.close(); // 关闭文件
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
    // 如果有搜索条件，执行全库搜索
    if (!m_searchCondition.isInit()) {
        performGlobalSearch();
    } else {
        // 没有搜索条件，恢复普通模式
        m_isInSearchMode = false;
        m_searchResults.clear();
        m_tableW->setRowCount(0);
        fnReadDB(m_auditLogTableNames[m_curDisplayIndex]);
    }
}
void HGLogWidget::slotClearSearch(){ 
    m_searchCondition.Clear();
    m_isInSearchMode = false;
    m_searchResults.clear();
    m_searchResultCurrentPage = 0;
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

// 检查日志条目是否匹配搜索条件
bool HGLogWidget::matchSearchCondition(const std::map<std::string, std::string>& logEntry)
{
    // 检查时间范围
    if (m_searchCondition.timeRangeFrom != "" || m_searchCondition.timeRangeTo != "") {
        std::string timeStr = logEntry.at("Time");
        TIME_STRUECT timeS;
        decodeStandardTime(timeStr, timeS);
        
        // 将日志日期转换为整数格式进行比较 (YYYYMMDD)
        int logDateInt = timeS.year * 10000 + timeS.month * 100 + timeS.day;
        
        if (m_searchCondition.timeRangeFrom != "") {
            int fromDateInt = atoi(m_searchCondition.timeRangeFrom.c_str());
            if (logDateInt < fromDateInt)
                return false;
        }
        
        if (m_searchCondition.timeRangeTo != "") {
            int toDateInt = atoi(m_searchCondition.timeRangeTo.c_str());
            if (logDateInt > toDateInt)
                return false;
        }
    }
    
    // 检查关键词
    if (m_searchCondition.key != "") {
        bool found = false;
        auto it = logEntry.find("Time");
        if (it != logEntry.end() && it->second.find(m_searchCondition.key) != std::string::npos)
            found = true;
        
        it = logEntry.find("Operator");
        if (it != logEntry.end() && it->second.find(m_searchCondition.key) != std::string::npos)
            found = true;
        
        it = logEntry.find("LogContent");
        if (it != logEntry.end() && it->second.find(m_searchCondition.key) != std::string::npos)
            found = true;
        
        if (!found)
            return false;
    }
    
    return true;
}

// 搜索单个表 - 使用 SQL 条件查询优化性能
std::vector<SearchResult> HGLogWidget::searchTableWithCondition(const std::string& tableName)
{
    std::vector<SearchResult> results;
    
    // 格式化时间为 SQL 格式 (YYYY-MM-DD HH:MM:SS)
    std::string timeFromStr = "";
    std::string timeToStr = "";
    
    if (!m_searchCondition.timeRangeFrom.empty()) {
        timeFromStr = m_searchCondition.timeRangeFrom.substr(0, 4) + "-" +
                      m_searchCondition.timeRangeFrom.substr(4, 2) + "-" +
                      m_searchCondition.timeRangeFrom.substr(6, 2) + " 00:00:00";
    }
    
    if (!m_searchCondition.timeRangeTo.empty()) {
        timeToStr = m_searchCondition.timeRangeTo.substr(0, 4) + "-" +
                    m_searchCondition.timeRangeTo.substr(4, 2) + "-" +
                    m_searchCondition.timeRangeTo.substr(6, 2) + " 23:59:59";
    }
    
    // 使用优化的 SQL 查询方法
    std::vector<std::map<std::string, std::string>> loginfos = 
        RWDb::readAuditTrailLogWithCondition(
            tableName,
            timeFromStr,
            timeToStr,
            m_searchCondition.key,
            5000  // 每表最多返回 5000 条
        );
    
    // 转换为 SearchResult
    for (const auto& logEntry : loginfos) {
        SearchResult result;
        result.tableName = tableName;
        result.time = logEntry.at("Time");
        result.logContent = logEntry.at("LogContent");
        result.oper = logEntry.at("Operator");
        results.push_back(result);
    }
    
    return results;
}

// 搜索所有表 - 优化版本
std::vector<SearchResult> HGLogWidget::searchAllTablesWithCondition()
{
    std::vector<SearchResult> allResults;
    const int MAX_TOTAL_RESULTS = 10000;  // 最大总结果数限制
    
    // 从最新的表开始搜索
    for (int i = int(m_auditLogTableNames.size()) - 1; i >= 0; i--) {
        std::vector<SearchResult> tableResults = searchTableWithCondition(m_auditLogTableNames[i]);
        allResults.insert(allResults.end(), tableResults.begin(), tableResults.end());
        
        // 如果已经收集足够的结果，提前退出
        if (allResults.size() >= MAX_TOTAL_RESULTS) {
            allResults.resize(MAX_TOTAL_RESULTS);
            break;
        }
        
        // 每处理完一个表，更新 UI 防止卡顿
        if (i % 5 == 0) {
            QApplication::processEvents();
        }
    }
    
    return allResults;
}

// 创建带高亮的 QLabel
QLabel* HGLogWidget::createHighlightLabel(const std::string& text, const std::string& keyword)
{
    QString qText = QString::fromStdString(text);
    
    QLabel* label = new QLabel();
    label->setTextInteractionFlags(Qt::TextSelectableByMouse);
    
    if (keyword.empty() || text.empty()) {
        label->setText(qText);
        return label;
    }
    
    QString qKeyword = QString::fromStdString(keyword);
    
    // 快速检查是否包含关键词（不区分大小写）
    if (!qText.contains(qKeyword, Qt::CaseInsensitive)) {
        label->setText(qText);
        return label;
    }
    
    // 使用富文本实现高亮
    QString result;
    result.reserve(qText.length() + 100);  // 预分配内存
    
    int lastPos = 0;
    int pos = 0;
    
    while ((pos = qText.indexOf(qKeyword, lastPos, Qt::CaseInsensitive)) != -1) {
        // 添加前面的普通文本
        result.append(qText.mid(lastPos, pos - lastPos));
        // 添加高亮的文本
        result.append("<span style='background-color: yellow;'>");
        result.append(qText.mid(pos, qKeyword.length()));
        result.append("</span>");
        // 更新位置
        lastPos = pos + qKeyword.length();
    }
    result.append(qText.mid(lastPos));
    
    label->setText(result);
    return label;
}

// 显示搜索结果页 - 优化版本
void HGLogWidget::displaySearchResultsPage(int pageIndex)
{
    // 使用更细粒度的更新控制
    m_tableW->setVisible(false);
    
    // 清除所有 cell widget
    for (int i = 0; i < m_tableW->rowCount(); i++) {
        for (int j = 0; j < m_tableW->columnCount(); j++) {
            QWidget* widget = m_tableW->cellWidget(i, j);
            if (widget) {
                delete widget;
            }
        }
    }
    m_tableW->clearContents();
    
    int totalResults = m_searchResults.size();
    int totalPages = (totalResults + SEARCH_RESULTS_PER_PAGE - 1) / SEARCH_RESULTS_PER_PAGE;
    
    if (pageIndex < 0 || pageIndex >= totalPages) {
        m_tableW->setRowCount(0);
        m_tableW->setVisible(true);
        return;
    }
    
    int startIndex = pageIndex * SEARCH_RESULTS_PER_PAGE;
    int endIndex = std::min(startIndex + SEARCH_RESULTS_PER_PAGE, totalResults);
    int rowCount = endIndex - startIndex;
    
    // 预设置行数
    m_tableW->setRowCount(rowCount);
    
    // 批量创建和设置 cell widgets
    const std::string& keyword = m_searchCondition.key;
    
    for (int i = 0; i < rowCount; i++) {
        const SearchResult& result = m_searchResults[startIndex + i];
        
        // 时间列 - 使用 QLabel 显示富文本高亮
        QLabel* timeLabel = createHighlightLabel(result.time, keyword);
        timeLabel->setAlignment(Qt::AlignCenter);
        m_tableW->setCellWidget(i, 0, timeLabel);
        
        // 日志内容列 - 使用 QLabel 显示富文本高亮
        QLabel* contentLabel = createHighlightLabel(result.logContent, keyword);
        contentLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        m_tableW->setCellWidget(i, 1, contentLabel);
        
        // 操作员列 - 使用 QLabel 显示富文本高亮
        QLabel* operLabel = createHighlightLabel(result.oper, keyword);
        operLabel->setAlignment(Qt::AlignCenter);
        m_tableW->setCellWidget(i, 2, operLabel);
        
        // 每处理 20 行更新一次 UI，防止界面卡顿
        if (i % 20 == 0 && i > 0) {
            QApplication::processEvents();
        }
    }
    
    // 更新页码显示
    m_pageLabel->setText(QString("%1/%2 (%3)")
        .arg(pageIndex + 1)
        .arg(totalPages)
        .arg(totalResults));
    
    m_tableW->setVisible(true);
}

// 执行全局搜索 - 优化版本
void HGLogWidget::performGlobalSearch()
{
    if (m_searchCondition.isInit()) {
        return;
    }
    
    // 设置搜索模式
    m_isInSearchMode = true;
    m_searchResultCurrentPage = 0;
    m_searchResults.clear();
    
    // 显示搜索中提示并禁用按钮防止重复点击
    m_pageLabel->setText(QString::fromStdString(loadTranslation(m_lang, "Searching")) + "...");
    m_nextLabel->setEnabled(false);
    m_preLabel->setEnabled(false);
    QApplication::processEvents();
    
    // 记录开始时间（使用 time_t）
    time_t startTimeT = time(nullptr);
    
    // 执行搜索
    m_searchResults = searchAllTablesWithCondition();
    
    // 计算耗时
    time_t endTimeT = time(nullptr);
    double searchTimeMs = difftime(endTimeT, startTimeT) * 1000.0;
    
    // 恢复按钮状态
    m_nextLabel->setEnabled(true);
    m_preLabel->setEnabled(true);
    
    // 显示第一页结果
    if (!m_searchResults.empty()) {
        displaySearchResultsPage(0);
        
        // 如果搜索耗时较长，显示提示
        if (searchTimeMs > 5000) {
            QString msg = QString::fromStdString(loadTranslation(m_lang, "SearchComplete"))
                + QString(" %1s, %2 ")
                    .arg(searchTimeMs / 1000.0, 0, 'f', 2)
                    .arg(m_searchResults.size())
                + QString::fromStdString(loadTranslation(m_lang, "Records"));
            // 可选：显示状态提示
        }
    } else {
        m_tableW->clearContents();
        m_tableW->setRowCount(0);
        m_pageLabel->setText("0/0");
        QMessageBox::information(this, QString::fromStdString(HG_DEVICE_NAME),
                                QString::fromStdString(loadTranslation(m_lang, "NoSearchResults")));
    }
}