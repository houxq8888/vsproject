#include "hglogwidget.h"
#include <QHeaderView>
#include "common.h"
#include <fstream>
#include <algorithm>
#include <QMessageBox>

// 辅助函数：高亮文本中的关键词
static QString highlightKeyword(const QString& text, const QString& keyword) {
    if (keyword.isEmpty()) return text;
    
    QString result = text;
    QString lowerText = text.toLower();
    QString lowerKeyword = keyword.toLower();
    
    int pos = 0;
    while ((pos = lowerText.indexOf(lowerKeyword, pos)) != -1) {
        // 在找到的位置插入高亮标记
        QString before = result.left(pos);
        QString match = result.mid(pos, keyword.length());
        QString after = result.mid(pos + keyword.length());
        
        result = before + "[【" + match + "】]" + after;
        
        // 更新搜索位置（跳过插入的标记）
        pos += keyword.length() + 6; // 6是标记的长度 [【 和 】]
        lowerText = result.toLower();
    }
    return result;
}

HGLogWidget::HGLogWidget(std::string lang,QWidget *parent) : QWidget(parent),
m_lang(lang),
m_curDisplayIndex(-1),
m_isSearchMode(false),
m_searchCurrentPage(0),
m_searchTotalCount(0)
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

    m_manipulateGroup=new QGroupBox("操作");
    m_manipulateGroup->setStyleSheet("QGroupBox { font-size: 12pt; font-weight:bold;}");
    m_manipulateLayout=new QGridLayout();

    // m_exportLabel=new HGQLabel(false,getPath("/resources/V1/@1xiconPark-export 1.png"));
    m_pageLabel=new QLabel("第"+QString::number(m_curDisplayIndex)+"页");
    m_saveLabel=new HGQLabel(false,getPath("/resources/V1/@1xmb-save 1.png")); 
    m_nextLabel=new HGQLabel(false,getPath("/resources/V1/@1xze-arrow 1.png")); 
    m_preLabel=new HGQLabel(false,getPath("/resources/V1/@1xze-arrow-left 1.png")); 
    // 硬编码按钮文字（独立测试用）
    m_saveLabel->setText("保存");
    m_nextLabel->setText("下一页▶");
    m_preLabel->setText("◀上一页");
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
    
    m_logTypeLabel=new QLabel("日志类型");
    m_logTypeComboBox=new QComboBox();
    m_logTypeComboBox->addItems({"审计日志", "运行日志"});
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
    if (m_isSearchMode) {
        // 搜索模式下的分页
        int totalPages = (m_searchTotalCount + PAGE_SIZE - 1) / PAGE_SIZE;
        if (m_searchCurrentPage < totalPages - 1) {
            m_searchCurrentPage++;
            fnSearchAuditLog();
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
        if (m_searchCurrentPage > 0) {
            m_searchCurrentPage--;
            fnSearchAuditLog();
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

void HGLogWidget::updatePageLabel() {
    if (m_isSearchMode) {
        int totalPages = (m_searchTotalCount + PAGE_SIZE - 1) / PAGE_SIZE;
        if (totalPages == 0) totalPages = 1;
        m_pageLabel->setText(QString("第%1/%2页 (共%3条)")
            .arg(m_searchCurrentPage + 1)
            .arg(totalPages)
            .arg(m_searchTotalCount));
    } else {
        m_pageLabel->setText(QString("第%1页").arg(m_curDisplayIndex + 1));
    }
}

void HGLogWidget::fnSearchAuditLog() {
    m_tableW->setRowCount(0);
    m_tableW->setUpdatesEnabled(false);
    
    int totalCount = 0;
    std::vector<std::map<std::string,std::string>> results = RWDb::searchAuditTrailLog(
        m_searchCondition.key,
        m_searchCondition.timeRangeFrom,
        m_searchCondition.timeRangeTo,
        m_searchCurrentPage,
        PAGE_SIZE,
        totalCount);
    
    m_searchTotalCount = totalCount;
    fnDisplaySearchResults(results);
    updatePageLabel();
    
    m_tableW->setUpdatesEnabled(true);
}

void HGLogWidget::fnDisplaySearchResults(const std::vector<std::map<std::string,std::string>>& results) {
    m_tableW->setRowCount(results.size());
    
    QString keyword = QString::fromStdString(m_searchCondition.key);
    
    for (int i = 0; i < int(results.size()); i++) {
        // 时间列 - 高亮关键词
        QString timeText = QString::fromStdString(results[i].at("Time"));
        m_tableW->setItem(i, 0, new QTableWidgetItem(highlightKeyword(timeText, keyword)));
        
        // 日志内容列 - 高亮关键词
        QString contentText = QString::fromStdString(results[i].at("LogContent"));
        m_tableW->setItem(i, 1, new QTableWidgetItem(highlightKeyword(contentText, keyword)));
        
        // 操作员列 - 高亮关键词
        QString operatorText = QString::fromStdString(results[i].at("Operator"));
        m_tableW->setItem(i, 2, new QTableWidgetItem(highlightKeyword(operatorText, keyword)));
    }
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
            start =HGExactTime::currentTime();
            int auditTrailLogCount=RWDb::readAuditTrailLogCount(tableName);
            printf("fnReadDB: tableName=%s, auditTrailLogCount=%d\n", tableName.c_str(), auditTrailLogCount);
            printf("fnReadDB: isInit=%d, key=%s, timeFrom=%s, timeTo=%s\n", 
                   m_searchCondition.isInit(), 
                   m_searchCondition.key.c_str(),
                   m_searchCondition.timeRangeFrom.c_str(),
                   m_searchCondition.timeRangeTo.c_str());
            if (auditTrailLogCount > 10000){
                if (m_searchCondition.isInit()){
                    QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),
                                     QString::fromStdString(loadTranslation(m_lang,"TooManagLogFiles")));
                    return;
                }
            }
            loginfos=RWDb::readAuditTrailLog(tableName);
            printf("fnReadDB: loginfos.size()=%zu\n", loginfos.size());
            getTableNameIndex(tableName);
            m_isSearchMode = false;
            updatePageLabel();
            int traillogIndex = 0;
            QString keyword = QString::fromStdString(m_searchCondition.key);
            
            // 辅助函数：从日志时间字符串解析日期
            // 支持格式: "2026-03-08 00:39:45 CST" 或 "2026-03-08T00:39:45"
            auto parseLogTime = [](const std::string& timeStr, HGExactTime& outTime) -> bool {
                if (timeStr.length() < 10) return false;
                // 格式: YYYY-MM-DD HH:MM:SS ...
                //       0123456789012345678
                try {
                    outTime.tm_year = std::stoi(timeStr.substr(0, 4));
                    outTime.tm_mon = std::stoi(timeStr.substr(5, 2));
                    outTime.tm_mday = std::stoi(timeStr.substr(8, 2));
                    return true;
                } catch (...) {
                    return false;
                }
            };
            
            // 先计算过滤后的数量，以便正确设置行数
            std::vector<std::map<std::string,std::string>> filteredLogs;
            for (int i =int(loginfos.size())-1;i>=0;i--){
                if (!m_searchCondition.isInit()){
                    std::string timeStr = loginfos[i]["Time"];
                    HGExactTime testTimer = HGExactTime::currentTime();
                    if (!parseLogTime(timeStr, testTimer)) {
                        // 时间解析失败，跳过过滤，直接显示
                        filteredLogs.push_back(loginfos[i]);
                        continue;
                    }
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
                filteredLogs.push_back(loginfos[i]);
            }
            
            // 设置正确的行数
            m_tableW->setRowCount(filteredLogs.size());
            printf("fnReadDB: filteredLogs.size()=%zu\n", filteredLogs.size());
            
            // 填充数据
            for (const auto& log : filteredLogs){
                for (const auto& info : log){
                    int nameColIndex=m_logContentMap[info.first];
                    if (nameColIndex<0||nameColIndex>=m_tableW->columnCount())
                        continue;
                    QString text = QString::fromStdString(info.second);
                    m_tableW->setItem(traillogIndex,nameColIndex,
                        new QTableWidgetItem(highlightKeyword(text, keyword)));
                }
                traillogIndex++;
            }
            printf("fnReadDB: traillogIndex=%d\n", traillogIndex);
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
    // 只有在审计日志模式下才使用全局搜索
    if (m_logTypeComboBox->currentIndex() == 0) {
        m_isSearchMode = true;
        m_searchCurrentPage = 0;
        fnSearchAuditLog();
    } else {
        // RunLog模式下使用原有逻辑
        m_isSearchMode = false;
        m_tableW->setRowCount(0);
        fnReadDB(m_auditLogTableNames[m_curDisplayIndex]);
    }
}
void HGLogWidget::slotClearSearch(){ 
    m_searchCondition.Clear();
    m_isSearchMode = false;
    m_searchCurrentPage = 0;
    m_searchTotalCount = 0;
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