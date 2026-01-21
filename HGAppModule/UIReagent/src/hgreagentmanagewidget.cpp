#include "hgreagentmanagewidget.h"
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QPalette>
#include <QDialog>
#include <QMessageBox>
#include "common.h"
#include <QLineEdit>

HGReagentManageWidget::HGReagentManageWidget(std::string lang,QWidget *parent) : QWidget(parent),
m_lang(lang)
{
    RWDb::writeAuditTrailLog(loadTranslation(m_lang,"Enter")+loadTranslation(m_lang,"ReagentManage"));
    m_whichW=-1;
    m_curReagentRow=-1;
    m_reagentS.clear();
    m_openHGLabel=new HGQLabel(false,getPath("/resources/V1/@1xantOutline-folder-open 1.png"));
    m_saveHGLabel=new HGQLabel(false,getPath("/resources/V1/@1xmb-save 1.png"));
    m_addHGLabel=new HGQLabel(false,getPath("/resources/V1/@1xze-plus 1.png"));
    m_closeHGLabel=new HGQLabel(false,getPath("/resources/V1/@1xze-cross 1.png"));
    m_editHGLabel=new HGQLabel(false,getPath("/resources/V1/@1xze-edit 1.png"));
    m_okHGLabel=new HGQLabel(false,getPath("/resources/V1/@1xze-success 1.png"));
    connect(m_openHGLabel,SIGNAL(leftClicked()),this,SLOT(slotOpenReagent()));
    connect(m_saveHGLabel,SIGNAL(leftClicked()),this,SLOT(slotSaveReagent()));
    connect(m_addHGLabel,SIGNAL(leftClicked()),this,SLOT(slotAddReagent()));
    connect(m_closeHGLabel,SIGNAL(leftClicked()),this,SLOT(slotCloseReagent()));
    connect(m_editHGLabel,SIGNAL(leftClicked()),this,SLOT(slotEditReagent()));
    connect(m_okHGLabel,SIGNAL(leftClicked()),this,SLOT(slotOkReagent()));

  
    m_didingjiLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"Titrant")));//"滴定剂");
    m_biaodingBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Calibration")));//"标定");
    connect(m_biaodingBtn,SIGNAL(clicked()),this,SLOT(slotBiaodingClicked()));

    m_layout=new QGridLayout();
    this->setLayout(m_layout);
 
    m_tableWidget=new QTableWidget(0,8);
    QStringList headers={"试剂类别","试剂编码","试剂名称","滴定度浓度","标定日期","标定周期","试剂余量","当前状态"};
    m_tableWidget->setHorizontalHeaderLabels(headers);
    m_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    // m_tableWidget->verticalHeader()->setVisible(false);
    m_tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableWidget->resizeRowsToContents();
    // set header bk color
    QHeaderView* header=m_tableWidget->horizontalHeader();
    header->setStyleSheet("QHeaderView::section {background-color: rgb(127,131,247);}");

    connect(m_tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(onCellClicked(int,int)));
    connect(m_tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(onCellChanged(int,int)));

    m_deviceTableW=new QTableWidget(0,5);
    m_deviceTableW->setHorizontalHeaderLabels({"指标设备单元", "接口", "联机", "当前通道", "选择"});
    m_deviceTableW->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_deviceTableW->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    // m_deviceTableW->verticalHeader()->setVisible(false);
    m_deviceTableW->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_deviceTableW->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_deviceTableW->resizeRowsToContents();

    connect(m_deviceTableW,SIGNAL(cellClicked(int,int)),this,SLOT(onDeviceCellClicked(int,int)));
    connect(m_deviceTableW,SIGNAL(cellChanged(int,int)),this,SLOT(onDeviceCellChanged(int,int)));

    m_horsplitter=new QSplitter();
    m_horsplitter->setOrientation(Qt::Horizontal);
    m_horsplitter->setStyleSheet("QSplitter::handle{background-color:lightgray;}");
    m_horsplitter->addWidget(m_tableWidget);
    m_horsplitter->addWidget(m_deviceTableW);

    m_layout->addWidget(m_didingjiLabel, 0, 0);
    m_layout->addWidget(m_biaodingBtn,0,1);
    m_layout->addWidget(m_openHGLabel,0,12);
    m_layout->addWidget(m_saveHGLabel,0,13);
    m_layout->addWidget(m_addHGLabel,0,14);
    m_layout->addWidget(m_closeHGLabel,0,15);
    m_layout->addWidget(m_editHGLabel,0,16);
    m_layout->addWidget(m_okHGLabel,0,17);
    m_layout->addWidget(m_horsplitter,1,0,1,18);

    loadReagent();

}
void HGReagentManageWidget::slotClickAdd(){
    m_deviceTableW->clearContents();
    m_deviceTableW->setRowCount(0);
    int rowCount = m_tableWidget->rowCount() - 1;
    m_tableWidget->insertRow(rowCount);
    REAGENT reagent;
    m_reagentS.push_back(reagent);
}
void HGReagentManageWidget::slotClickLinkDevice(){
    if (m_tableWidget->currentIndex().row() >= m_tableWidget->rowCount()) return;
    int row = getSelectedRow(m_tableWidget);
    if (row != -1)
    {
        int rowCount=m_deviceTableW->rowCount()-1;
        m_deviceTableW->insertRow(rowCount);
    }
    else
    {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), QString::fromStdString(loadTranslation(m_lang,"SelectOneRecord")));//"请选中一条记录！");
    }
}
bool HGReagentManageWidget::closeWindow()
{
    return true;
}
void HGReagentManageWidget::onCellClicked(int row,int column){
    m_whichW=REAGENT_MANIPULATE;
    m_curReagentRow=row;
    m_tableWidget->selectRow(row);
    if (row >= int(m_reagentS.size())) return;

    m_deviceTableW->clearContents();
    m_deviceTableW->setRowCount(0);

    for (auto device : m_reagentS[row].linkDevices){
        std::map<std::string,std::string> reagentSInfo;
        reagentSInfo=RWDb::getMapFromDevices(device.second);
        fnDisplayReagentDeviceInfo(device.first,reagentSInfo);
    }
    int addRow=m_deviceTableW->rowCount();
    m_deviceTableW->insertRow(addRow);
    m_linkDeviceBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"AddLineDevice")));//"添加关联设备");
    m_linkDeviceBtn->setFocusPolicy(Qt::NoFocus);
    m_linkDeviceBtn->setStyleSheet("color: red;");
    m_deviceTableW->setCellWidget(addRow,0,m_linkDeviceBtn);
    connect(m_linkDeviceBtn,SIGNAL(clicked()),this,SLOT(slotClickLinkDevice()));
    m_deviceTableW->setSpan(addRow,0,1,5);
}
void HGReagentManageWidget::onDeviceCellClicked(int row,int column){
    m_whichW=DEVICE_MANIPULATE;
    m_deviceTableW->selectRow(row);
}
void HGReagentManageWidget::onCellChanged(int row,int column){
    if (!m_tableWidget->item(row,column)) return;
    std::string text=m_tableWidget->item(row,column)->text().toStdString();
    std::string fieldName=m_tableWidget->horizontalHeaderItem(column)->text().toStdString();
    REAGENT reagent;
    if (row>=int(m_reagentS.size())){
        m_reagentS.push_back(reagent);
    } 
    if (fieldName=="序号") m_reagentS[m_curReagentRow].index=row+1;
    else if (fieldName=="试剂类别") m_reagentS[row].type=text;
    else if (fieldName=="试剂编码") m_reagentS[row].serialNumber=text;
    else if (fieldName=="试剂名称") m_reagentS[row].name=text;
    else if (fieldName=="滴定度浓度") m_reagentS[row].concertraction=text;
    else if (fieldName=="标定日期") m_reagentS[row].calibrationDate=text;
    else if (fieldName=="标定周期") m_reagentS[row].calibrationCircle=text;
    else if (fieldName=="试剂余量") m_reagentS[row].reagentRemainAmount=text;
    else if (fieldName=="当前状态") m_reagentS[row].currentState=text;

    std::vector<std::string> reagentNoEditName=RWDb::getReagentNoEditName();
    if (std::find(reagentNoEditName.begin(),reagentNoEditName.end(),fieldName)!=reagentNoEditName.end())
    {
        if (!(m_tableWidget->item(row,column)->flags() & Qt::ItemIsEditable)) return;
        m_tableWidget->item(row,column)->setFlags(m_tableWidget->item(row,column)->flags() & ~Qt::ItemIsEditable);
    }
}
void HGReagentManageWidget::onDeviceCellChanged(int row,int column){
    if (!m_deviceTableW->item(row,column)) return;
    std::string text=m_deviceTableW->item(row,column)->text().toStdString();
    std::string fieldName=m_deviceTableW->horizontalHeaderItem(column)->text().toStdString();
    if (fieldName=="指标设备单元")  m_reagentS[m_curReagentRow].linkDevices[row].name=text;
    else if(fieldName=="接口") m_reagentS[m_curReagentRow].linkDevices[row].interfaceName=text;
    else if(fieldName=="联机") m_reagentS[m_curReagentRow].linkDevices[row].linkState=text;
    else if(fieldName=="当前通道") m_reagentS[m_curReagentRow].linkDevices[row].channel=text;
    else if(fieldName=="选择") m_reagentS[m_curReagentRow].linkDevices[row].choice=text;
    m_reagentS[m_curReagentRow].linkDevices[row].index=row+1;
   
    std::vector<std::string> reagentLinkDeviceNoEditName=RWDb::getReagentLinkDeviceNoEditName();
    if (std::find(reagentLinkDeviceNoEditName.begin(),reagentLinkDeviceNoEditName.end(),
        fieldName)!=reagentLinkDeviceNoEditName.end())
    {
        if (!(m_deviceTableW->item(row,column)->flags() & Qt::ItemIsEditable)) return;
        m_deviceTableW->item(row,column)->setFlags(m_deviceTableW->item(row,column)->flags() & ~Qt::ItemIsEditable);
    }
}
HGReagentManageWidget::~HGReagentManageWidget()
{
    
}
void HGReagentManageWidget::fnDisplayReagentInfo(int count,std::map<std::string,std::string> fillContent)
{
    if (count>=m_tableWidget->rowCount()) m_tableWidget->insertRow(m_tableWidget->rowCount());

    for (auto content: fillContent){
        int nameColIndex=getColumnIndexByName(m_tableWidget,QString::fromStdString(content.first));
        if (nameColIndex<0||nameColIndex>=m_tableWidget->columnCount())
            continue;
        m_tableWidget->setItem(count,nameColIndex,new QTableWidgetItem(QString::fromStdString(content.second)));
        std::vector<std::string> reagentNoEditName=RWDb::getReagentNoEditName();
        if (std::find(reagentNoEditName.begin(),reagentNoEditName.end(),content.first)!=reagentNoEditName.end())
        {
            m_tableWidget->item(count,nameColIndex)->setFlags(m_tableWidget->item(count,nameColIndex)->flags() & ~Qt::ItemIsEditable);
        }
    }
}
void HGReagentManageWidget::fnDisplayReagentDeviceInfo(int count,std::map<std::string,std::string> fillContent)
{
    if (count>=m_deviceTableW->rowCount()) m_deviceTableW->insertRow(m_deviceTableW->rowCount());
    for (auto content:fillContent){
        int nameColIndex=getColumnIndexByName(m_deviceTableW,QString::fromStdString(content.first));
        if (nameColIndex<0||nameColIndex>=m_deviceTableW->columnCount())
            continue;
        m_deviceTableW->setItem(count,nameColIndex,new QTableWidgetItem(QString::fromStdString(content.second)));
        std::vector<std::string> reagentLinkDeviceNoEditName=RWDb::getReagentLinkDeviceNoEditName();
        if (std::find(reagentLinkDeviceNoEditName.begin(),reagentLinkDeviceNoEditName.end(),content.first)!=reagentLinkDeviceNoEditName.end())
        {
            m_deviceTableW->item(count,nameColIndex)->setFlags(m_deviceTableW->item(count,nameColIndex)->flags() & ~Qt::ItemIsEditable);
        }
    }
}
void HGReagentManageWidget::slotBiaodingClicked()
{
    m_ddjiCalibrateW=new HGDDjiCalibrationWidget(m_lang,this);
    int posy=m_didingjiLabel->pos().y();
    m_ddjiCalibrateW->move(m_didingjiLabel->pos().x(),posy);
    m_ddjiCalibrateW->resize(this->width(),this->height()-posy);
    
    m_ddjiCalibrateW->show();
}
void HGReagentManageWidget::loadReagent(){
    m_reagentS.clear();
    m_tableWidget->clearContents();
    m_tableWidget->setRowCount(0);
    m_deviceTableW->clearContents();
    m_deviceTableW->setRowCount(0);
    m_reagentS=RWDb::readReagentInfo("");

    for (int index=0;index<int(m_reagentS.size());index++){
        std::map<std::string,std::string> fillContent=RWDb::getMapFromReagent(m_reagentS[index]);
        fnDisplayReagentInfo(index,fillContent);
    }
    m_tableWidget->insertRow(m_reagentS.size());

    int addRow=m_tableWidget->rowCount()-1;
    m_addBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"AddLine")));//"添加行");
    m_addBtn->setStyleSheet("color: red;");
    m_tableWidget->setCellWidget(addRow,0,m_addBtn);
    connect(m_addBtn,SIGNAL(clicked()),this,SLOT(slotClickAdd()));
    m_tableWidget->setSpan(addRow,0,1,8);
}
void HGReagentManageWidget::slotOpenReagent(){
    std::string tableName="";
    // list squence 
    QDialog dialog(this);
    dialog.setWindowTitle(QString::fromStdString(loadTranslation(m_lang,"SelectReagentManage")));//"选择要执行的试剂管理");
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
    std::vector<std::string> names=RWDb::getAllTables(ReagentDBName);
    for (const auto &name:names){
        if (name.find(ReagentLinkDeviceDBName)!=std::string::npos) continue;
        listW->addItem(QString::fromStdString(name));
    }
    // setFocusByName(listW,HgOnlinePlatformModule::getReagentName());

    QVBoxLayout *layout=new QVBoxLayout(&dialog);
    layout->addWidget(listW);
    layout->addWidget(okbtn);
    layout->addWidget(cancelbtn);
    dialog.setLayout(layout);
    dialog.exec();

    if (tableName!=""){
        // HgOnlinePlatformModule::setReagentName(tableName);
        loadReagent();
        
        m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
}
void HGReagentManageWidget::fnUpdateContent(){
    for (int row=0;row<m_tableWidget->rowCount();++row){
        for (int col=0;col<m_tableWidget->columnCount();++col){
            QWidget* editor=m_tableWidget->cellWidget(row,col);
            if (editor){
                QLineEdit* lineEdit=qobject_cast<QLineEdit*>(editor);
                if (lineEdit)
                {
                    std::string fieldName = m_tableWidget->horizontalHeaderItem(col)->text().toStdString();
                    std::string value = lineEdit->text().toStdString();
                    if (row >= int(m_reagentS.size()))
                    {
                        std::map<std::string, std::string> fillContent;
                        fillContent[fieldName] = value;
                        fillContent["序号"] = std::to_string(row + 1);
                    }
                    else
                    {
                        m_reagentS[row].index=(row+1);
                    }
                }
            }
            QTableWidgetItem* item=m_tableWidget->item(row,col);
            if (item)
            {
                std::string fieldName=m_tableWidget->horizontalHeaderItem(col)->text().toStdString();
                std::string value=item->text().toStdString();
                if (row >= int(m_reagentS.size()))
                {
                    std::map<std::string,std::string> fillContent;
                    fillContent[fieldName]=value;
                    fillContent["序号"]=std::to_string(row+1);
                }
                else
                {
                    m_reagentS[row].index=(row+1);
                }
            }
        }
    }
}
void HGReagentManageWidget::slotSaveReagent(){
    fnUpdateContent();
    std::string tableName="";
    bool coverFlag=false;
    // list squence 
    QDialog dialog(this);
    dialog.setWindowTitle(QString::fromStdString(loadTranslation(m_lang,"InputSaveName")));//"请输入保存名称");
    dialog.setWindowModality(Qt::ApplicationModal);

    QLineEdit* saveNameEdit=new QLineEdit(&dialog);
    saveNameEdit->setPlaceholderText(QString::fromStdString(loadTranslation(m_lang,"InputSaveName")));//"请输入保存名称");
    QPushButton* coverBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"OverwriteFile"))/*"覆盖当前文件"*/,&dialog);
    QPushButton* okbtn=new QPushButton(&dialog);
    okbtn->setText(QString::fromStdString(loadTranslation(m_lang,"Ok")));//"确定");
    QPushButton* cancelbtn=new QPushButton(&dialog);
    cancelbtn->setText(QString::fromStdString(loadTranslation(m_lang,"Cancel")));//"取消");
    connect(coverBtn,&QPushButton::clicked,[&](){
        tableName=""; // HgOnlinePlatformModule::getReagentName();
        saveNameEdit->setText(QString::fromStdString(tableName));
        coverFlag=true;
    });
    connect(okbtn,&QPushButton::clicked,[&](){
       tableName=saveNameEdit->text().toStdString();
       if (!coverFlag) tableName=ReagentDBName+std::string("_")+tableName;
    //    HgOnlinePlatformModule::setReagentName(tableName);
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
        RWDb::writeReagentRecord(coverFlag,"",m_reagentS);
        m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
}
void HGReagentManageWidget::slotAddReagent()
{
    m_reagentS.clear();
    m_tableWidget->clearContents();
    m_tableWidget->setRowCount(0);
    m_tableWidget->insertRow(m_reagentS.size());

    m_deviceTableW->clearContents();
    m_deviceTableW->setRowCount(0);
    // m_deviceTableW->insertRow(0);

    int addRow=m_tableWidget->rowCount()-1;
    m_addBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"AddLine")));//"添加行");
    m_addBtn->setStyleSheet("color: red;");
    m_tableWidget->setCellWidget(addRow,0,m_addBtn);
    connect(m_addBtn,SIGNAL(clicked()),this,SLOT(slotClickAdd()));
    m_tableWidget->setSpan(addRow,0,1,8);
}
void HGReagentManageWidget::slotCloseReagent()
{
    switch (m_whichW){
        case REAGENT_MANIPULATE:
        {
            fnDeleteReagent();
            break;
        }
        case DEVICE_MANIPULATE:
        {
            fnDeleteDevice();
            break;
        }
    };
}
void HGReagentManageWidget::fnDeleteDevice(){
    if (m_deviceTableW->currentIndex().row() >= m_deviceTableW->rowCount()) return;
    int row = getSelectedRow(m_deviceTableW);
    if (row != -1)
    {
        QString szInfo;
        std::string name = m_deviceTableW->item(row, 1)->text().toStdString();
        szInfo = "确定删除序号[" + QString::number(row+1) + "]设备?";
        if (QMessageBox::Yes == QMessageBox::question(this, QString::fromStdString(HG_DEVICE_NAME),
                                                      szInfo,
                                                      QMessageBox::Yes | QMessageBox::No))
        {
            m_deviceTableW->removeRow(row);
            m_reagentS[m_curReagentRow].linkDevices.erase(row);
            int lstIndex=-1;
            for (auto device : m_reagentS[m_curReagentRow].linkDevices){
                if (device.first > row){
                    m_reagentS[m_curReagentRow].linkDevices[device.first-1]=device.second;
                    m_reagentS[m_curReagentRow].linkDevices[device.first-1].index-=1;
                    lstIndex=device.first-1;
                }
            }
            if (lstIndex!=-1) m_reagentS[m_curReagentRow].linkDevices.erase(lstIndex+1);
        }
    }
    else
    {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), QString::fromStdString(loadTranslation(m_lang,"SelectOneRecord")));//"请选中一条记录！");
    }
}
void HGReagentManageWidget::fnDeleteReagent(){
    if (m_tableWidget->currentIndex().row() >= m_tableWidget->rowCount()) return;
    int row = getSelectedRow(m_tableWidget);
    if (row != -1)
    {
        QString szInfo;
        std::string name = m_tableWidget->item(row, 1)->text().toStdString();
        szInfo = "确定删除序号[" + QString::number(row+1) + "]试剂?";
        if (QMessageBox::Yes == QMessageBox::question(this, QString::fromStdString(HG_DEVICE_NAME),
                                                      szInfo,
                                                      QMessageBox::Yes | QMessageBox::No))
        {
            m_tableWidget->removeRow(row);
            m_deviceTableW->clearContents();
            m_deviceTableW->setRowCount(0);
            m_reagentS.erase(m_reagentS.begin()+row);
            for (int i=0;i<int(m_reagentS.size());i++){
                m_reagentS[i].index=(i+1);
            }
        }
    }
    else
    {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), QString::fromStdString(loadTranslation(m_lang,"SelectOneRecord")));//"请选中一条记录！");
    }
}
void HGReagentManageWidget::slotEditReagent()
{
    m_tableWidget->setEditTriggers(QAbstractItemView::AllEditTriggers);
    m_deviceTableW->setEditTriggers(QAbstractItemView::AllEditTriggers);
}
void HGReagentManageWidget::slotOkReagent()
{
    if (!m_tableWidget->currentItem()) return;
    int row=m_tableWidget->currentItem()->row();
    int col=m_tableWidget->currentItem()->column();
    if (row<0||row>=m_tableWidget->rowCount()||col<0||col>=m_tableWidget->columnCount())
        return;
    QString text=m_tableWidget->currentItem()->text();
    m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableWidget->closePersistentEditor(m_tableWidget->item(row,col));
    m_tableWidget->setItem(row,col,new QTableWidgetItem(text));

    if (!m_deviceTableW->currentItem()) return;
    row=m_deviceTableW->currentItem()->row();
    col=m_deviceTableW->currentItem()->column();
    if (row<0||row>=m_deviceTableW->rowCount()||col<0||col>=m_deviceTableW->columnCount())
        return;
    text=m_deviceTableW->currentItem()->text();
    m_deviceTableW->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_deviceTableW->closePersistentEditor(m_deviceTableW->item(row,col));
    m_deviceTableW->setItem(row,col,new QTableWidgetItem(text));
}
