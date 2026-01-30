#include "hglogwidget.h"
#include <QHeaderView>
#include <QLabel>
#include "common.h"
#include <fstream>
#include <algorithm>
#include <QMessageBox>
#include <set>


HGLogWidget::HGLogWidget(std::string lang,QWidget *parent) : QWidget(parent),
m_lang(lang),
m_curDisplayIndex(-1),
m_isSearching(false),
m_searchResultPageIndex(0),
m_searchResultPageSize(100),
m_searchResultTotalPages(0),
m_currentSortColumn(-1),
m_currentSortOrder(Qt::AscendingOrder),
m_filterColumn(-1),
m_filterValue("")
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
    m_searchResultPageLabel=new QLabel("搜索结果: 第0页");
    m_searchResultPageLabel->setVisible(false);
    m_searchResultNextLabel=new HGQLabel(false,getPath("/resources/V1/@1xze-arrow 1.png")); 
    m_searchResultPreLabel=new HGQLabel(false,getPath("/resources/V1/@1xze-arrow-left 1.png")); 
    m_searchResultNextLabel->setVisible(false);
    m_searchResultPreLabel->setVisible(false);
    connect(m_saveLabel,SIGNAL(leftClicked()),this,SLOT(slotSaveSearchLog()));
    connect(m_nextLabel,SIGNAL(leftClicked()),this,SLOT(slotNext()));
    connect(m_preLabel,SIGNAL(leftClicked()),this,SLOT(slotPre()));
    connect(m_searchResultNextLabel,SIGNAL(leftClicked()),this,SLOT(slotSearchResultPageChanged()));
    connect(m_searchResultPreLabel,SIGNAL(leftClicked()),this,SLOT(slotSearchResultPageChanged()));

    m_tableW=new QTableWidget(0,3);
    QStringList headers={"时间",/*,"通道","采样电位","日志类型",*/"日志内容","操作员"};
    m_tableW->setHorizontalHeaderLabels(headers);
    m_tableW->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableW->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_tableW->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_tableW->resizeRowsToContents();
    m_tableW->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableW->setSortingEnabled(true);
    m_tableW->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_tableW->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(slotHeaderClicked(int)));
    connect(m_tableW->horizontalHeader(), SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotHeaderCustomContextMenuRequested(QPoint)));
    
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
    m_manipulateLayout->addWidget(m_searchResultPreLabel,0,6);
    m_manipulateLayout->addWidget(m_searchResultNextLabel,0,7);
    m_manipulateLayout->addWidget(m_searchResultPageLabel,0,8);
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
    if (m_searchCondition.isInit()){
        m_isSearching = false;
        m_searchResults.clear();
        m_searchResultPageLabel->setVisible(false);
        m_searchResultNextLabel->setVisible(false);
        m_searchResultPreLabel->setVisible(false);
        m_pageLabel->setVisible(true);
        m_nextLabel->setVisible(true);
        m_preLabel->setVisible(true);
        m_tableW->setRowCount(0);
        fnReadDB(m_auditLogTableNames[m_curDisplayIndex]);
    } else {
        searchAllLogs();
    }
}
void HGLogWidget::slotClearSearch(){ 
    m_searchCondition.Clear();
    m_isSearching = false;
    m_searchResults.clear();
    m_searchResultPageLabel->setVisible(false);
    m_searchResultNextLabel->setVisible(false);
    m_searchResultPreLabel->setVisible(false);
    m_pageLabel->setVisible(true);
    m_nextLabel->setVisible(true);
    m_preLabel->setVisible(true);
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
void HGLogWidget::searchAllLogs(){
    m_searchResults.clear();
    m_searchResultPageIndex = 0;
    m_isSearching = true;

    m_pageLabel->setVisible(false);
    m_nextLabel->setVisible(false);
    m_preLabel->setVisible(false);
    m_searchResultPageLabel->setVisible(true);
    m_searchResultNextLabel->setVisible(true);
    m_searchResultPreLabel->setVisible(true);

    std::string timeFromStr, timeToStr;
    if (m_searchCondition.timeFrom.tm_year > 0){
        char buf[32];
        sprintf(buf, "%04d-%02d-%02d 00:00:00", 
                m_searchCondition.timeFrom.tm_year,
                m_searchCondition.timeFrom.tm_mon,
                m_searchCondition.timeFrom.tm_mday);
        timeFromStr = buf;
    }
    if (m_searchCondition.timeTo.tm_year > 0){
        char buf[32];
        sprintf(buf, "%04d-%02d-%02d 23:59:59", 
                m_searchCondition.timeTo.tm_year,
                m_searchCondition.timeTo.tm_mon,
                m_searchCondition.timeTo.tm_mday);
        timeToStr = buf;
    }

    for (const std::string& tableName : m_auditLogTableNames){
        std::vector<std::map<std::string,std::string>> loginfos = 
            RWDb::readAuditTrailLogWithCondition(tableName, m_searchCondition.key, timeFromStr, timeToStr);
        
        for (auto& loginfo : loginfos){
            m_searchResults.push_back(loginfo);
        }
    }

    std::sort(m_searchResults.begin(), m_searchResults.end(),
        [](const std::map<std::string,std::string>& a, const std::map<std::string,std::string>& b) {
            return a.at("Time") < b.at("Time");
        });
    
    m_searchResultTotalPages = (m_searchResults.size() + m_searchResultPageSize - 1) / m_searchResultPageSize;
    
    if (m_searchResults.size() > 10000){
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),
                         QString::fromStdString(loadTranslation(m_lang,"TooManagLogFiles")));
        return;
    }
    
    if (m_searchResults.empty()){
        m_tableW->setRowCount(0);
        m_searchResultPageLabel->setText(QString("搜索结果: 共0条"));
        return;
    }
    
    displaySearchResults(0);
}
void HGLogWidget::displaySearchResults(int pageIndex){
    if (pageIndex < 0 || pageIndex >= m_searchResultTotalPages){
        return;
    }
    
    m_searchResultPageIndex = pageIndex;
    m_tableW->setRowCount(0);
    m_tableW->setUpdatesEnabled(false);
    
    int startIndex = pageIndex * m_searchResultPageSize;
    int endIndex = std::min(startIndex + m_searchResultPageSize, (int)m_searchResults.size());
    
    m_tableW->setRowCount(endIndex - startIndex);
    
    for (int i = startIndex; i < endIndex; i++){
        int rowIndex = i - startIndex;
        const auto& loginfo = m_searchResults[i];
        
        for (const auto& info : loginfo){
            int nameColIndex = m_logContentMap[info.first];
            if (nameColIndex < 0 || nameColIndex >= m_tableW->columnCount())
                continue;
            
            std::string displayText = info.second;
            if (!m_searchCondition.key.empty()){
                displayText = highlightText(displayText, m_searchCondition.key);
                QLabel* label = new QLabel(QString::fromStdString(displayText));
                label->setTextFormat(Qt::RichText);
                label->setWordWrap(true);
                label->setAlignment(Qt::AlignLeft | Qt::AlignTop);
                m_tableW->setCellWidget(rowIndex, nameColIndex, label);
            } else {
                QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(displayText));
                m_tableW->setItem(rowIndex, nameColIndex, item);
            }
        }
    }
    
    m_searchResultPageLabel->setText(QString("搜索结果: 第%1/%2页, 共%3条")
                                    .arg(pageIndex + 1)
                                    .arg(m_searchResultTotalPages)
                                    .arg(m_searchResults.size()));
    m_tableW->setUpdatesEnabled(true);
}
std::string HGLogWidget::highlightText(const std::string& text, const std::string& keyword){
    if (keyword.empty()) return text;
    
    std::string result = text;
    size_t pos = 0;
    std::string lowerResult = result;
    std::string lowerKeyword = keyword;
    
    std::transform(lowerResult.begin(), lowerResult.end(), lowerResult.begin(), ::tolower);
    std::transform(lowerKeyword.begin(), lowerKeyword.end(), lowerKeyword.begin(), ::tolower);
    
    while ((pos = lowerResult.find(lowerKeyword, pos)) != std::string::npos){
        result.insert(pos, "<font color=\"red\">");
        result.insert(pos + keyword.length() + 18, "</font>");
        lowerResult.insert(pos, "<font color=\"red\">");
        lowerResult.insert(pos + keyword.length() + 18, "</font>");
        pos += keyword.length() + 35;
    }
    
    return result;
}
void HGLogWidget::slotSearchResultPageChanged(){
    HGQLabel* senderLabel = qobject_cast<HGQLabel*>(sender());
    if (senderLabel == m_searchResultNextLabel){
        if (m_searchResultPageIndex < m_searchResultTotalPages - 1){
            displaySearchResults(m_searchResultPageIndex + 1);
        } else {
            QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),
                             "已经是最后一页");
        }
    } else if (senderLabel == m_searchResultPreLabel){
        if (m_searchResultPageIndex > 0){
            displaySearchResults(m_searchResultPageIndex - 1);
        } else {
            QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),
                             "已经是第一页");
        }
    }
}
void HGLogWidget::slotHeaderClicked(int column){
    if (m_currentSortColumn == column){
        m_currentSortOrder = (m_currentSortOrder == Qt::AscendingOrder) ? Qt::DescendingOrder : Qt::AscendingOrder;
    } else {
        m_currentSortColumn = column;
        m_currentSortOrder = Qt::AscendingOrder;
    }
    
    if (m_isSearching && !m_searchResults.empty()){
        std::string sortKey;
        switch(column){
            case 0: sortKey = "Time"; break;
            case 1: sortKey = "LogContent"; break;
            case 2: sortKey = "Operator"; break;
            default: return;
        }
        
        std::sort(m_searchResults.begin(), m_searchResults.end(),
            [sortKey, this](const std::map<std::string,std::string>& a, const std::map<std::string,std::string>& b) {
                auto itA = a.find(sortKey);
                auto itB = b.find(sortKey);
                if (itA == a.end() || itB == b.end()) return false;
                
                if (m_currentSortOrder == Qt::AscendingOrder){
                    return itA->second < itB->second;
                } else {
                    return itA->second > itB->second;
                }
            });
        
        displaySearchResults(m_searchResultPageIndex);
    } else {
        m_tableW->sortByColumn(column, m_currentSortOrder);
    }
    
    m_tableW->horizontalHeader()->setSortIndicator(column, m_currentSortOrder);
}
void HGLogWidget::slotHeaderCustomContextMenuRequested(const QPoint& pos){
    int column = m_tableW->horizontalHeader()->logicalIndexAt(pos);
    if (column < 0) return;
    
    QMenu menu(this);
    
    QAction* sortAscAction = menu.addAction("升序排序");
    QAction* sortDescAction = menu.addAction("降序排序");
    menu.addSeparator();
    
    std::set<std::string> uniqueValues;
    std::string sortKey;
    switch(column){
        case 0: sortKey = "Time"; break;
        case 1: sortKey = "LogContent"; break;
        case 2: sortKey = "Operator"; break;
        default: return;
    }
    
    if (m_isSearching && !m_searchResults.empty()){
        for (const auto& row : m_searchResults){
            auto it = row.find(sortKey);
            if (it != row.end()){
                uniqueValues.insert(it->second);
            }
        }
    } else {
        for (int row = 0; row < m_tableW->rowCount(); row++){
            QTableWidgetItem* item = m_tableW->item(row, column);
            if (item){
                uniqueValues.insert(item->text().toStdString());
            }
        }
    }
    
    QMenu* filterMenu = menu.addMenu("筛选");
    QAction* clearFilterAction = filterMenu->addAction("清除筛选");
    filterMenu->addSeparator();
    
    int count = 0;
    const int MAX_FILTER_ITEMS = 50;
    for (const auto& value : uniqueValues){
        if (count >= MAX_FILTER_ITEMS) break;
        QAction* action = filterMenu->addAction(QString::fromStdString(value));
        action->setData(column);
        connect(action, SIGNAL(triggered()), this, SLOT(slotFilterTriggered()));
        count++;
    }
    
    if (uniqueValues.size() > MAX_FILTER_ITEMS){
        filterMenu->addAction("... (更多值未显示)")->setEnabled(false);
    }
    
    QAction* selectedAction = menu.exec(m_tableW->horizontalHeader()->mapToGlobal(pos));
    
    if (selectedAction == sortAscAction){
        m_currentSortColumn = column;
        m_currentSortOrder = Qt::AscendingOrder;
        slotHeaderClicked(column);
    } else if (selectedAction == sortDescAction){
        m_currentSortColumn = column;
        m_currentSortOrder = Qt::DescendingOrder;
        slotHeaderClicked(column);
    } else if (selectedAction == clearFilterAction){
        slotClearFilter();
    }
}
void HGLogWidget::slotFilterTriggered(){
    QAction* action = qobject_cast<QAction*>(sender());
    if (!action) return;
    
    int column = action->data().toInt();
    std::string filterValue = action->text().toStdString();
    
    m_filterColumn = column;
    m_filterValue = filterValue;
    
    if (m_isSearching && !m_originalSearchResults.empty()){
        m_searchResults = m_originalSearchResults;
    }
    
    std::string sortKey;
    switch(column){
        case 0: sortKey = "Time"; break;
        case 1: sortKey = "LogContent"; break;
        case 2: sortKey = "Operator"; break;
        default: return;
    }
    
    if (m_isSearching && !m_searchResults.empty()){
        if (m_originalSearchResults.empty()){
            m_originalSearchResults = m_searchResults;
        }
        
        std::vector<std::map<std::string,std::string>> filtered;
        for (const auto& row : m_searchResults){
            auto it = row.find(sortKey);
            if (it != row.end() && it->second == filterValue){
                filtered.push_back(row);
            }
        }
        m_searchResults = filtered;
        m_searchResultTotalPages = (m_searchResults.size() + m_searchResultPageSize - 1) / m_searchResultPageSize;
        displaySearchResults(0);
    } else {
        for (int row = 0; row < m_tableW->rowCount(); row++){
            QTableWidgetItem* item = m_tableW->item(row, column);
            bool match = (item && item->text().toStdString() == filterValue);
            m_tableW->setRowHidden(row, !match);
        }
    }
}
void HGLogWidget::slotClearFilter(){
    m_filterColumn = -1;
    m_filterValue = "";
    
    if (m_isSearching && !m_originalSearchResults.empty()){
        m_searchResults = m_originalSearchResults;
        m_originalSearchResults.clear();
        m_searchResultTotalPages = (m_searchResults.size() + m_searchResultPageSize - 1) / m_searchResultPageSize;
        displaySearchResults(0);
    } else {
        for (int row = 0; row < m_tableW->rowCount(); row++){
            m_tableW->setRowHidden(row, false);
        }
    }
}