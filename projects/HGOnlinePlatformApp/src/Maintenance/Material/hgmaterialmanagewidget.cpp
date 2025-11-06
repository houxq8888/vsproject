#include "hgmaterialmanagewidget.h"
#include <QHeaderView>
#include <QFileDialog>
#include "common.h"
#include <QMessageBox>
#include "hgxlsx.hpp"
#include "HGAuthorityService.h"
#include "HGOnlinePlatformInterface.h"

HGMaterialManageWidget::HGMaterialManageWidget(int loginAuthority,std::string lang,QWidget *parent) : QWidget(parent),
m_lang(lang),
m_loginAuthority(loginAuthority)
{
    RWDb::writeAuditTrailLog(loadTranslation(m_lang,"Enter")+loadTranslation(m_lang,"MaterialManage"));
    m_materialInfos.clear();
    m_markLabel1=new QLabel("*");
    m_markLabel1->setStyleSheet("color: red;");
    m_markLabel2=new QLabel("*");
    m_markLabel2->setStyleSheet("color: red;");

    m_layout=new QGridLayout();
    this->setLayout(m_layout);

    m_infoGroup=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"MaterialInfo")));//"物料信息");
    m_infoLayout=new QGridLayout();
    m_infoGroup->setLayout(m_infoLayout);

    m_searchLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"KeyWord")));//"关键词");
    m_searchEdit=new QLineEdit();
    m_searchEdit->setPlaceholderText(QString::fromStdString(loadTranslation(m_lang,"Input")));//"请输入");

    m_searchHGLabel=new HGQLabel(false,getPath("/resources/V1/@1xmb-search 1.png"));

    m_materialNumberLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"MaterialNo")));//"物料编号");
    m_materialNumberEdit=new QLineEdit();
    m_materialNumberEdit->setPlaceholderText(QString::fromStdString(loadTranslation(m_lang,"Input")));//"请输入");

    m_getNumberLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"Amount")));//"数量");
    m_getNumberEdit=new QLineEdit();
    m_getNumberEdit->setPlaceholderText(QString::fromStdString(loadTranslation(m_lang,"Input")));//"请输入");

    m_openLabel=new HGQLabel(false,getPath("/resources/V1/@1xantOutline-folder-open 1.png"));
    m_saveLabel=new HGQLabel(false,getPath("/resources/V1/@1xmb-save 1.png"));
    m_deleteLabel=new HGQLabel(false,getPath("/resources/V1/@1xif-ui-delete 1.png"));

    m_inputCabinBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Insert")));//"入库");
    m_getMaterialBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Receive")));//"领用");
    connect(m_inputCabinBtn,&QPushButton::clicked,this,[=](){
        if (!(m_loginAuthority==SOFTWARE_MANAGER||m_loginAuthority==SYSTEM_MANAGER)) {
            QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),
                QString::fromStdString(loadTranslation(m_lang,"NoPermission")));
            return;
        }
        if (m_materialNumberEdit->text().isEmpty()){
            QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),
                QString::fromStdString(loadTranslation(m_lang,"PleaseInput"))+
                QString::fromStdString(loadTranslation(m_lang,"MaterialNo")));
            return;
        }
        if (m_getNumberEdit->text().isEmpty()){
            QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),
                QString::fromStdString(loadTranslation(m_lang,"PleaseInput"))+
                QString::fromStdString(loadTranslation(m_lang,"Amount")));
            return;
        }
        std::string materialNo=m_materialNumberEdit->text().toStdString();
        int index=hasMaterialNo(materialNo);
        if (index < 0)
        {
            std::map<std::string, std::string> materialInfo;
            materialInfo["MaterialNo"] = m_materialNumberEdit->text().toStdString();
            materialInfo["ReserveQuantity"] = "";
            materialInfo["MaterialName"] = "";
            materialInfo["MaterialManaqer"] = "";
            materialInfo["Recipient"] = "";
            materialInfo["MaterialDescription"] = "";
            materialInfo["AvailableQuantity"] = m_getNumberEdit->text().toStdString();
            materialInfo["Specification"] = "";
            materialInfo["MaterialDescription"] = "";
            m_materialInfos.push_back(materialInfo);
            fnWriteDB(materialInfo);
            fnDisplayMaterialInfo(m_materialInfos.size()-1,m_materialInfos[m_materialInfos.size()-1]);
        }
        else
        {
            m_materialInfos[index]["MaterialNo"] = materialNo;
            m_materialInfos[index]["AvailableQuantity"] = std::to_string(std::atoi(m_getNumberEdit->text().toStdString().c_str()) +
                                                                       std::atoi(m_materialInfos[index]["AvailableQuantity"].c_str()));
            fnWriteDB(m_materialInfos[index]);
            fnDisplayMaterialInfo(index,m_materialInfos[index]);
        }
    });
    connect(m_getMaterialBtn,&QPushButton::clicked,this,[=](){
        int index=hasMaterialNo(m_materialNumberEdit->text().toStdString());
        int availableQuantity=std::atoi(m_materialInfos[index]["AvailableQuantity"].c_str());
        int getQuantity=std::atoi(m_getNumberEdit->text().toStdString().c_str());
        if (getQuantity > availableQuantity){
            QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),
                QString::fromStdString(loadTranslation(m_lang,"MaxGetQuantity"))+
                QString::number(availableQuantity)+"!");
            return;
        }
        m_materialInfos[index]["AvailableQuantity"] = std::to_string(availableQuantity - getQuantity);
        fnWriteDB(m_materialInfos[index]);
        fnDisplayMaterialInfo(index,m_materialInfos[index]);
    });

    m_tableW=new QTableWidget(0,8);
    QStringList headers={QString::fromStdString(loadTranslation(m_lang,"MaterialNo")),
        QString::fromStdString(loadTranslation(m_lang,"MaterialName")),
        QString::fromStdString(loadTranslation(m_lang,"Specification")),
        QString::fromStdString(loadTranslation(m_lang,"MaterialDescription")),
        QString::fromStdString(loadTranslation(m_lang,"ReserveQuantity")),
        QString::fromStdString(loadTranslation(m_lang,"AvailableQuantity")),
        QString::fromStdString(loadTranslation(m_lang,"MaterialManaqer")),
        QString::fromStdString(loadTranslation(m_lang,"Recipient"))
    };
    m_tableW->setHorizontalHeaderLabels(headers);
    m_tableW->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableW->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_tableW->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_tableW->resizeRowsToContents();
    // m_tableW->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(m_tableW,&QTableWidget::cellClicked,[=](int row,int column){
        m_tableW->selectRow(row);
        m_materialNumberEdit->setText(m_tableW->item(row,0)->text());
    });
    connect(m_tableW,&QTableWidget::cellChanged,[=](int row,int column){
        QString value = m_tableW->item(row, column) ? m_tableW->item(row, column)->text() : "";
        QString columnName = m_tableW->horizontalHeaderItem(column)->text();
        std::string message = findTranslationKey(m_lang,columnName.toStdString());
        m_materialInfos[row][message] = value.toStdString();

        fnWriteDB(m_materialInfos[row]);

    });
    
    m_infoLayout->addWidget(m_openLabel,0,1);
    m_infoLayout->addWidget(m_saveLabel,0,2);
    m_infoLayout->addWidget(m_deleteLabel,0,3);
    m_infoLayout->addWidget(m_materialNumberLabel,0,8);
    m_infoLayout->addWidget(m_materialNumberEdit,0,9);
    m_infoLayout->addWidget(m_markLabel1,0,10);
    m_infoLayout->addWidget(m_getNumberLabel,0,11);
    m_infoLayout->addWidget(m_getNumberEdit,0,12);
    m_infoLayout->addWidget(m_markLabel2,0,13);
    m_infoLayout->addWidget(m_searchLabel,1,1);
    m_infoLayout->addWidget(m_searchEdit,1,4);
    m_infoLayout->addWidget(m_searchHGLabel,1,5);
    m_infoLayout->addWidget(m_inputCabinBtn,1,10);
    m_infoLayout->addWidget(m_getMaterialBtn,1,11);
    m_infoLayout->addWidget(m_tableW,2,1,1,13);

    m_layout->addWidget(m_infoGroup);

    fnReadDB();

    connect(m_searchHGLabel,SIGNAL(leftClicked()),this,SLOT(slotSearch()));
    connect(m_openLabel,SIGNAL(leftClicked()),this,SLOT(slotOpenXLSX()));
    connect(m_saveLabel,SIGNAL(leftClicked()),this,SLOT(slotSaveXLSX()));
    connect(m_deleteLabel,SIGNAL(leftClicked()),this,SLOT(slotDeleteRecord()));
}
int HGMaterialManageWidget::hasMaterialNo(const std::string &materialNo){
    for (int i=0;i<int(m_materialInfos.size());i++){
        if (m_materialInfos[i]["MaterialNo"]==materialNo) return i;
    }
    return -1;
}
bool HGMaterialManageWidget::closeWindow()
{
    return true;
}
HGMaterialManageWidget::~HGMaterialManageWidget()
{
    
}
void HGMaterialManageWidget::fnReadDB(){
    m_materialInfos=HGOnlineRWDB::readMaterialInfo();
    for (int i=0;i<int(m_materialInfos.size());i++){
        fnDisplayMaterialInfo(i,m_materialInfos[i]);
    }
}
void HGMaterialManageWidget::fnWriteDB(const std::map<std::string,std::string> &materialInfo){
    HGOnlineRWDB::writeMaterialInfo(materialInfo);
}
void HGMaterialManageWidget::fnDisplayMaterialInfo(int count,std::map<std::string,std::string> fillContent){
    if (count >= m_tableW->rowCount()) m_tableW->insertRow(m_tableW->rowCount());
   
    for (auto content : fillContent)
    {
        QString name=QString::fromStdString(loadTranslation(m_lang,content.first));
        int nameColIndex = getColumnIndexByName(m_tableW, name);
        if (nameColIndex < 0 || nameColIndex >= m_tableW->columnCount())
            continue;
        m_tableW->setItem(count, nameColIndex, new QTableWidgetItem(QString::fromStdString(content.second)));
        if (content.first == "MaterialNo"|| content.first == "AvailableQuantity"){
            m_tableW->item(count,nameColIndex)->setFlags(m_tableW->item(count,nameColIndex)->flags() & ~Qt::ItemIsEditable);
        }
    }
}


void HGMaterialManageWidget::slotSearch(){
    if (m_searchEdit->text().isEmpty()){
        QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),
            QString::fromStdString(loadTranslation(m_lang,"PleaseInputSearchContent")));
        return;
    }
    for (int row=0;row<m_tableW->rowCount();row++){
        for (int col=0;col<m_tableW->columnCount();col++){
            if (!m_tableW->item(row,col)){
                continue;
            }
            if (m_tableW->item(row,col)->text().contains(m_searchEdit->text())){
                m_tableW->item(row,col)->setSelected(true);
            }
        }
    }
}
void HGMaterialManageWidget::slotOpenXLSX(){
    std::vector<std::vector<std::string>> data;
    std::string filePath =QFileDialog::getOpenFileName(NULL,"选择要录入的excel文件","./",tr("xlsx files(*.xlsx);;All files(*.*)")).toStdString();
    if (filePath.empty()) return;
    int ret=readExcel(filePath,data);
    if (ret<0) return;
    if (data.size()<=0) return;
    m_materialInfos.clear();
    m_tableW->clear();
    m_tableW->setRowCount(0);
    QStringList headers;
    for (int i=0;i<int(data.size());i++){
        if (i==0){
            for (int j=0;j<int(data[i].size());j++){
                headers.append(QString::fromStdString(loadTranslation(m_lang,data[i][j])));
            }
        } else {
            m_materialInfos.push_back(std::map<std::string,std::string>());
            for (int j=0;j<int(data[i].size());j++){
                m_materialInfos[m_materialInfos.size()-1][data[0][j]]=data[i][j];
            }
        }
    }
    m_tableW->setColumnCount(headers.size());
    m_tableW->setHorizontalHeaderLabels(headers);
    for (int i=0;i<int(m_materialInfos.size());i++){
        fnDisplayMaterialInfo(i,m_materialInfos[i]);
    }
}
void HGMaterialManageWidget::slotSaveXLSX(){
    std::string outMaterialPath=HgOnlinePlatformModule::getDirPath()+"/outmaterial/";
    HGMkDir(outMaterialPath);
    HGExactTime curTime = HGExactTime::currentTime();
    std::string syncslice = curTime.toStringFromYearToSec();
    std::string filePath=outMaterialPath+syncslice+".xlsx";
    std::vector<std::vector<std::string>> data;
    std::vector<std::string> temp;
    for (int i=0;i<m_tableW->columnCount();i++){
        temp.push_back(m_tableW->horizontalHeaderItem(i)->text().toStdString());
    }
    if (temp.size()>0)    data.push_back(temp);
    for (int i=0;i<m_tableW->rowCount();i++){
        data.push_back(std::vector<std::string>());
        for (int j=0;j<m_tableW->columnCount();j++){
            data[data.size()-1].push_back(m_tableW->item(i,j)->text().toStdString());
        }
    }
    writeExcel(filePath,data);
}
void HGMaterialManageWidget::slotDeleteRecord(){
    int row = getSelectedRow(m_tableW);
    if (row >= m_tableW->rowCount()) return;
    if (row==-1) {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), QString::fromStdString(loadTranslation(m_lang,"SelectOneRecord")));//"请选中一条记录！");
        return;
    }
    HGOnlineRWDB::deleteRecord(MATERIALINFODBNAME,"MaterialNo",m_materialInfos[row]["MaterialNo"]);
    m_materialInfos.erase(m_materialInfos.begin()+row);
    m_tableW->removeRow(row);
}