#include "hgtaskeditwidget.h"
#include <QHeaderView>
#include <sstream>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include "common.h"

HGTaskEditWidget::HGTaskEditWidget(std::string lang,const std::string& taskSeqName,QWidget *parent) : QWidget(parent),
m_lang(lang),
m_taskSeqName(taskSeqName)
{
    m_startFlag=false;
    m_restartFlag=true;
    m_deviceOnlineFlag=true;
    m_index=-1;
    m_fillContent.clear();
    m_editGroupBox=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"Sequence")));//"序列");
    m_editlayout=new QGridLayout();
    m_layout=new QGridLayout();
    this->setLayout(m_layout);
    m_editGroupBox->setLayout(m_editlayout);

    m_openLabel=new HGQLabel(false,getPath("/resources/V1/@1xantOutline-folder-open 1.png"));
    m_saveLabel=new HGQLabel(false,getPath("/resources/V1/@1xmb-save 1.png"));
    m_deleteLabel=new HGQLabel(false,getPath("/resources/V1/@1xif-ui-delete 1.png"));
    m_startLabel=new HGQLabel(false,getPath("/resources/V1/@1xif-play-alt-2 1.png"));
    m_pauseLabel=new HGQLabel(false,getPath("/resources/V1/@1xif-stop 1.png"));
    m_downLabel=new HGQLabel(false,getPath("/resources/V1/@down.png"));
    m_upLabel=new HGQLabel(false,getPath("/resources/V1/@up.png"));
    connect(m_openLabel,SIGNAL(leftClicked()),this,SLOT(slotOpenSequence()));
    connect(m_saveLabel,SIGNAL(leftClicked()),this,SLOT(slotSaveSequence()));
    connect(m_deleteLabel,SIGNAL(leftClicked()),this,SLOT(slotDeleteTask()));
    connect(m_startLabel,SIGNAL(leftClicked()),this,SLOT(slotStartSequence()));
    connect(m_pauseLabel,SIGNAL(leftClicked()),this,SLOT(slotPauseSequence()));
    connect(m_upLabel,SIGNAL(leftClicked()),this,SLOT(slotUpTask()));
    connect(m_downLabel,SIGNAL(leftClicked()),this,SLOT(slotDownTask()));


    QStringList headers;
#ifdef ENABLE_CAS6
    headers = QStringList{
        "序号", "盘位", "样品名称", "批号",
        "样品编号", "样品总量", "通道","流程","方法", "空白", "状态"};
#else
    headers = QStringList{
        "序号", "样品编号", "样品来源", "取样方式",
        "样品名称", "样品总量", "通道", "流程", "方法", "工作模式", "状态"};
#endif
    m_tableW=new QTableWidget(0,headers.size());
    m_tableW->setHorizontalHeaderLabels(headers);
    m_tableW->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableW->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_tableW->verticalHeader()->setVisible(false);
    m_tableW->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableW->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_tableW->resizeRowsToContents();

    connect(m_tableW,SIGNAL(cellClicked(int,int)),this,SLOT(onCellClicked(int,int)));

    QString content="说明:通道与流程匹配时，进行自检；或发现匹配不上，则报警。\n" \
                    "比如: \n" \
                    "   流程中注入试剂“缓冲溶液”，但与缓冲溶液关联的CAN1－泵，并未在通道1中进行配置，\n" \
                    "则报警“注入试剂”配置不成功。";
    m_attentionContentLabel=new QLabel(content);
    m_attentionContentLabel->setAlignment(Qt::AlignCenter);

    m_editlayout->addWidget(m_openLabel,0,1);
    m_editlayout->addWidget(m_saveLabel,0,2);
    m_editlayout->addWidget(m_deleteLabel,0,3);
    m_editlayout->addWidget(m_upLabel,0,4);
    m_editlayout->addWidget(m_downLabel,0,5);
    // m_editlayout->addWidget(m_startLabel,0,8);
    // m_editlayout->addWidget(m_pauseLabel,0,9);
    m_editlayout->addWidget(m_tableW,1,0,1,12);
    m_editlayout->addWidget(m_attentionContentLabel,2,0,8,9);

    m_layout->addWidget(m_editGroupBox,0,0);

    m_fillContent=RWDb::readTaskInfo(taskSeqName);
    for (int index=0;index<int(m_fillContent.size());index++){
        fnDisplayTaskInfo(index,m_fillContent[index]);
    }

    fnReadDB();
    m_pauseLabel->setEnabled(m_startFlag);

    if (m_startFlag){
        m_restartFlag=false;
        m_startLabel->changePixmap(getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"),24);
    }
    else {
        m_startLabel->changePixmap(getPath("/resources/V1/@1xif-play-alt-2 1.png"),24);
    }

    m_timer=new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(slotUpdateRunStatus()));
    m_timer->start(1000);
    slotUpdateRunStatus();
}
bool HGTaskEditWidget::closeWindow()
{
    RWDb::writeTaskRecord(true,m_taskSeqName,m_fillContent);
    fnWriteDB();
    return true;
}
void HGTaskEditWidget::onCellClicked(int row,int column){
    m_index=row;
    m_tableW->selectRow(row);
    if (row >= int(m_fillContent.size())) return;
    Task task=RWDb::getTaskSFromMap(m_fillContent[row]);
    emit taskInfoWShow(task);
}
void HGTaskEditWidget::slotUpdateRunStatus(){
    m_deviceOnlineFlag=true;
    #ifdef ENABLE_CAS6
    #else
    if (!m_startFlag){
        std::vector<std::string> statusList;//=getSelfCheckStatus();
        for (int i=0;i<int(m_fillContent.size());i++){
            if (i>=int(statusList.size())) continue;
            if (statusList[i]!="[在线]") m_deviceOnlineFlag=false;
            m_fillContent[i]["状态"]=statusList[i];
            if (i>=m_tableW->rowCount()) continue;
            m_tableW->setItem(i, 10, new QTableWidgetItem(QString::fromStdString(statusList[i])));
        }
    }
    else
    {
        std::string status;
        int runIndex ;//= getCurRunTaskIndex(status);
        if (runIndex < 0 || runIndex >= m_tableW->rowCount())
            return;
        m_tableW->setItem(runIndex, 10, new QTableWidgetItem(QString::fromStdString(status)));
        m_fillContent[runIndex]["状态"] = status;
        for (int i = runIndex + 1; i < m_tableW->rowCount(); i++)
            m_fillContent[i]["状态"] = "待测试";
    }
    #endif
}
void HGTaskEditWidget::fnDisplayTaskInfo(int count,std::map<std::string,std::string> fillContent){
    if (count >= m_tableW->rowCount()) m_tableW->insertRow(m_tableW->rowCount());
    m_tableW->setEditTriggers(QAbstractItemView::AllEditTriggers);
    m_tableW->setItem(count, 0, new QTableWidgetItem(QString::fromStdString(std::to_string(count))));

    for (auto content : fillContent)
    {
        int nameColIndex = getColumnIndexByName(m_tableW, QString::fromStdString(content.first));
        if (nameColIndex < 0 || nameColIndex >= m_tableW->columnCount())
            continue;
        m_tableW->setItem(count, nameColIndex, new QTableWidgetItem(QString::fromStdString(content.second)));
    }
    int index=-1;
    // int index = getColumnIndexByName(m_tableW, "指标要求");
    std::ostringstream ss;
    // ss << fillContent["condition1"] << fillContent["value1"] << " " << fillContent["condition2"] << fillContent["value2"];

    // m_tableW->setItem(count, index, new QTableWidgetItem(QString::fromStdString(ss.str())));
    // m_tableW->item(count,index)->setTextAlignment(Qt::AlignCenter);
    index = getColumnIndexByName(m_tableW, "工作模式");
    ss.str("");

    ss << fillContent["工作模式"] << "/" << fillContent["间隔时间"];
    if (index!=-1)
        m_tableW->setItem(count, index, new QTableWidgetItem(QString::fromStdString(ss.str())));
    // m_tableW->item(count,index)->setTextAlignment(Qt::AlignCenter);

    m_tableW->setEditTriggers(QAbstractItemView::NoEditTriggers);

}
void HGTaskEditWidget::displayTaskInfo(int type,Task task){
    switch (type)
    {
    case TASK_CREATE:
    {
        int count = m_fillContent.size();
        std::map<std::string, std::string> fillContent = RWDb::getTaskMap(count + 1, task);
        m_fillContent.push_back(fillContent);
        fnDisplayTaskInfo(count, fillContent);
        break;
    }
    case TASK_RECTIFY:
    {
        if (m_index < 0 || m_index >= int(m_fillContent.size())){
            QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), QString::fromStdString(loadTranslation(m_lang,"SelectOneRecord")));//"请选中一条记录！");
            return;
        }
        std::map<std::string,std::string> fillContent=RWDb::getTaskMap(m_index+1,task);
        m_fillContent[m_index]=fillContent;
        for (auto content : fillContent)
        {
            int nameColIndex = getColumnIndexByName(m_tableW, QString::fromStdString(content.first));
            if (nameColIndex < 0 || nameColIndex >= m_tableW->columnCount())
                continue;
            m_tableW->setItem(m_index, nameColIndex, new QTableWidgetItem(QString::fromStdString(content.second)));
        }
        int index=-1;
        // index = getColumnIndexByName(m_tableW, "指标要求");
        std::ostringstream ss;
        // ss << fillContent["condition1"] << fillContent["value1"] << " " << fillContent["condition2"] << fillContent["value2"];

        // m_tableW->setItem(m_index, index, new QTableWidgetItem(QString::fromStdString(ss.str())));
        
        index = getColumnIndexByName(m_tableW, "工作模式");
        ss.str("");
        ss << fillContent["工作模式"] << "/" << fillContent["间隔时间"];
        if (index!=-1)
            m_tableW->setItem(m_index, index, new QTableWidgetItem(QString::fromStdString(ss.str())));
        break;
    }
    }
}
void HGTaskEditWidget::slotUpTask(){
    if (m_tableW->currentIndex().row() >= m_tableW->rowCount()) return;
    int row = getSelectedRow(m_tableW);
    if (row != -1)
    {
        
    }
    else
    {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), QString::fromStdString(loadTranslation(m_lang,"SelectOneRecord")));//"请选中一条记录！");
    }
}
void HGTaskEditWidget::slotDownTask(){
    if (m_tableW->currentIndex().row() >= m_tableW->rowCount()) return;
    int row = getSelectedRow(m_tableW);
    if (row != -1)
    {
        
    }
    else
    {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), QString::fromStdString(loadTranslation(m_lang,"SelectOneRecord")));//"请选中一条记录！");
    }
}
void HGTaskEditWidget::slotOpenSequence(){
    std::string tableName="";
    // list squence 
    QDialog dialog(this);
    dialog.setWindowTitle("选择要执行的任务序列");
    dialog.setWindowModality(Qt::ApplicationModal);

    QListWidget* listW=new QListWidget(&dialog);
    QPushButton* okbtn=new QPushButton(&dialog);
    okbtn->setText(QString::fromStdString(loadTranslation(m_lang,"Ok")));//"确定");
    QPushButton* cancelbtn=new QPushButton(&dialog);
    cancelbtn->setText(QString::fromStdString(loadTranslation(m_lang,"Cancel")));//"取消");
    connect(okbtn,&QPushButton::clicked,[&](){
       if (listW->count()!=0) tableName=listW->currentItem()->text().toStdString();
       dialog.close();
    });
    connect(cancelbtn,&QPushButton::clicked,[&](){
        dialog.close();
    });
    std::vector<std::string> names=RWDb::getAllTables(TaskSequenceDBName);
    for (const auto &name:names){
        listW->addItem(QString::fromStdString(name));
    }
    setFocusByName(listW,m_taskSeqName/* HgOnlinePlatformModule::getTaskSeqName()*/);

    QVBoxLayout *layout=new QVBoxLayout(&dialog);
    layout->addWidget(listW);
    layout->addWidget(okbtn);
    layout->addWidget(cancelbtn);
    dialog.setLayout(layout);
    dialog.exec();

    if (tableName!=""){
        // HgOnlinePlatformModule::setTaskSeqName(tableName);
        m_fillContent.clear();
        m_tableW->clearContents();
        m_tableW->setRowCount(0);
        m_fillContent=RWDb::readTaskInfo(m_taskSeqName);
        for (int index=0;index<int(m_fillContent.size());index++){
           fnDisplayTaskInfo(index,m_fillContent[index]);
        }

        // updateStyleInfo();
    }
}
void HGTaskEditWidget::slotSaveSequence(){
    std::string tableName="";
    bool coverFlag=false;
    // list squence 
    QDialog dialog(this);
    dialog.setWindowTitle(QString::fromStdString(loadTranslation(m_lang,"InputSaveName")));//"请输入保存名称");
    dialog.setWindowModality(Qt::ApplicationModal);

    QLineEdit* saveNameEdit=new QLineEdit(&dialog);
    saveNameEdit->setPlaceholderText(QString::fromStdString(loadTranslation(m_lang,"InputSaveName")));
    QPushButton* coverBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"OverwriteFile"))/*"覆盖当前文件"*/,&dialog);
    QPushButton* okbtn=new QPushButton(&dialog);
    okbtn->setText(QString::fromStdString(loadTranslation(m_lang,"Ok")));//"确定");
    QPushButton* cancelbtn=new QPushButton(&dialog);
    cancelbtn->setText(QString::fromStdString(loadTranslation(m_lang,"Cancel")));//"取消");
    connect(coverBtn,&QPushButton::clicked,[&](){
        tableName=m_taskSeqName; // HgOnlinePlatformModule::getTaskSeqName();
        saveNameEdit->setText(QString::fromStdString(tableName));
        coverFlag=true;
    });
    connect(okbtn,&QPushButton::clicked,[&](){
       tableName=saveNameEdit->text().toStdString();
       if (!coverFlag) tableName=TaskSequenceDBName+std::string("_")+tableName;
    //    HgOnlinePlatformModule::setTaskSeqName(tableName);
       dialog.close();
    });
    connect(cancelbtn,&QPushButton::clicked,[&](){
        dialog.close();
    });

    QVBoxLayout *layout=new QVBoxLayout(&dialog);
    layout->addWidget(saveNameEdit);
    layout->addWidget(coverBtn);
    layout->addWidget(okbtn);
    layout->addWidget(cancelbtn);
    dialog.setLayout(layout);
    dialog.exec();

    if (tableName!=""){
        RWDb::writeTaskRecord(coverFlag,m_taskSeqName,m_fillContent);
    }
}

void HGTaskEditWidget::slotDeleteTask()
{
    if (m_tableW->currentIndex().row() >= m_tableW->rowCount()) return;
    int row = getSelectedRow(m_tableW);
    if (row != -1)
    {
        QString szInfo;
        szInfo = "确定删除序号[" + m_tableW->item(row, 0)->text() + "]任务?";
        if (QMessageBox::Yes == QMessageBox::question(this, QString::fromStdString(HG_DEVICE_NAME),
                                                      szInfo,
                                                      QMessageBox::Yes | QMessageBox::No))
        {
            m_tableW->removeRow(m_tableW->currentIndex().row());
            m_fillContent.erase(m_fillContent.begin()+row);
            int rowCount=m_tableW->rowCount();
            for (int i=0;i<rowCount;i++){
                m_tableW->item(i,0)->setText(QString("%1").arg(i+1));
                m_fillContent[i]["序号"]=std::to_string(i+1);
            }
        }
    }
    else
    {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), QString::fromStdString(loadTranslation(m_lang,"SelectOneRecord")));//"请选中一条记录！");
    }
}
void HGTaskEditWidget::fnWriteDB(){
    if (m_startFlag)
        RWDb::setTaskRunFlag("true");
    else 
        RWDb::setTaskRunFlag("false");
}
void HGTaskEditWidget::fnReadDB(){
    if (RWDb::getTaskRunFlag()=="true") m_startFlag=true;
    else m_startFlag=false;
}
void HGTaskEditWidget::slotStartSequence(){
    if (m_index < 0 || m_index >= int(m_fillContent.size())){
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), QString::fromStdString(loadTranslation(m_lang,"SelectOneRecord")));//"请选中一条记录！");
        return;
    }
#ifdef ENABLE_CAS6
    std::vector<std::map<std::string, std::string>> methodContent = HGOnlineRWDB::readMethodInfo();
    std::string paramstr = "";
    for (int index = 0; index < int(methodContent.size()); index++)
    {
        std::string methodName="["+methodContent[index]["序号"]+"_"+methodContent[index]["名称"]+"]";
        if (methodName == m_fillContent[m_index]["方法"])
        {
            paramstr = methodContent[index]["参数"];
        }
    }
    printf("param:%s\n",paramstr.c_str());
    std::map<std::vector<uint8_t>,std::vector<uint8_t>> sendMethodInfo;
    std::map<std::string, std::string> wparam = getParamMap(paramstr);
    for (const auto &pair : gMapSendMethod)
    {
        if (wparam.find(pair.first) == wparam.end())
            continue;
        std::string paramStr = wparam[pair.first];
        printf("paramStr:%s\n",paramStr.c_str());
        std::vector<uint8_t> input = std::vector<uint8_t>(paramStr.begin(), paramStr.end());
        sendMethodInfo[pair.second] = input;
    }

    setMethodInfoToMCU(sendMethodInfo);
#else
    if (!m_deviceOnlineFlag) {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), QString::fromStdString(loadTranslation(m_lang,"SelfCheckNoPass")));//"设备自检未通过，请根据提示排查!");
        return;
    }
    if (m_restartFlag)
    {
        std::string tableName=getStandardCurTime();
        RWDb::setTaskRunRecordDataDB(tableName);
        RWDb::insertTaskRunInfo(tableName,m_taskSeqName);
    }
    m_startFlag=!m_startFlag;
    m_restartFlag=false;
    m_pauseLabel->setEnabled(true);
    if (m_startFlag){
        m_startLabel->changePixmap(getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"),24);
        // startRunTaskSequence();
    }
    else {
        m_startLabel->changePixmap(getPath("/resources/V1/@1xif-play-alt-2 1.png"),24);
        // pauseTaskSequence();
    }
    m_openLabel->setEnabled(false);
    m_saveLabel->setEnabled(false);
    m_deleteLabel->setEnabled(false);
    #endif    
}
void HGTaskEditWidget::slotPauseSequence(){
    if (!m_startFlag) return;
    m_restartFlag=true;
    // resetTaskSequence();
    RWDb::setTaskRunRecordDataDB("");
    m_startFlag=false;
    m_startLabel->changePixmap(getPath("/resources/V1/@1xif-play-alt-2 1.png"),24);
    m_pauseLabel->setEnabled(false);
    m_openLabel->setEnabled(true);
    m_saveLabel->setEnabled(true);
    m_deleteLabel->setEnabled(true);
}