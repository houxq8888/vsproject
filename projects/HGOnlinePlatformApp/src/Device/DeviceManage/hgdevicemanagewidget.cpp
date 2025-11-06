#include "hgdevicemanagewidget.h"
#include <QDebug>
#include <unordered_map>
#include <QMessageBox>
#include "common.h"
#include <algorithm>
#include <QMenu>
#include <QFileDialog>
#include "common.h"
#include "HGOnlinePlatformInterface.h"



HGDeviceManageWidget::HGDeviceManageWidget(std::string lang,QWidget *parent) : QWidget(parent),
    m_lang(lang)
{
    RWDb::writeAuditTrailLog(loadTranslation(m_lang,"Enter")+loadTranslation(m_lang,"DeviceManage"));
    m_deviceInterfaces.clear();
    m_linkStatus=getLinkStatus();
    m_hgphDJW=NULL;
    m_colorsensorW=NULL;
    m_photometerW=NULL;
    m_signalDJW=NULL;
    m_backUnitW=NULL;
    m_auxiliaryPumpW=NULL;
    m_rs485W=NULL;
    m_externalDeviceW=NULL;
    m_zidongjinyangqiW=NULL;
    m_shuifenyiW=NULL;
    m_jiarebanW=NULL;
    m_layout= new QGridLayout();
    this->setLayout(m_layout);
    m_inputEdit=new QLineEdit();
    m_inputEdit->setPlaceholderText(QString::fromStdString(loadTranslation(m_lang,"Input")));//"请输入");
    m_searchLabel=new HGQLabel(false,getPath("/resources/V1/@1xmb-search 1.png"));
    connect(m_searchLabel,SIGNAL(leftClicked()),this,SLOT(slotLeftClickSearchLabel()));

    m_vline=new QFrame;
    m_vline->setFrameShape(QFrame::VLine);
    m_vline->setFrameShadow(QFrame::Sunken);

    m_treeWidget=new QTreeWidget();
    connect(m_treeWidget,SIGNAL(itemClicked(QTreeWidgetItem *,int)),
        this,SLOT(slotGetDeviceName(QTreeWidgetItem *,int)));
    // 启用右键菜单
    m_treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_treeWidget, &QTreeWidget::customContextMenuRequested, 
            this, &HGDeviceManageWidget::showContextMenu);

    m_layout->addWidget(m_inputEdit,0,0);
    m_layout->addWidget(m_searchLabel,0,1);
    m_layout->addWidget(m_treeWidget,2,0,1,2);
    m_layout->addWidget(m_vline,0,2,9,1);

    m_layout->setColumnStretch(0,1);
    m_layout->setColumnStretch(1,1);
    m_layout->setColumnStretch(2,1);
    m_layout->setColumnStretch(3,10);

    fnInitDeviceStatus();
}
void HGDeviceManageWidget::showContextMenu(const QPoint &pos) {
    QTreeWidgetItem *item = m_treeWidget->itemAt(pos);  // 获取点击的项
    QMenu menu(this);

    if (item) {  // 右键点击了某个节点
        QAction *updateAction = menu.addAction(QString::fromStdString(loadTranslation(m_lang,"Update")));

        connect(updateAction, &QAction::triggered, this, [=]() { updateSingleFirmware(item); });
    } else {  // 右键点击空白处
        
    }

    // 显示菜单（转换为全局坐标）
    menu.exec(m_treeWidget->mapToGlobal(pos));
}
void HGDeviceManageWidget::updateSingleFirmware(QTreeWidgetItem *item) {
    if (!item) return;
    QString binFileName=QFileDialog::getOpenFileName(NULL,
        QString::fromStdString(loadTranslation(m_lang,"SelectUpdateBinFile")),
        "./",tr("bin files(*.bin);;All files(*.*)"));
        
}
void HGDeviceManageWidget::fnInitTreeWidget(const std::vector<std::string> &existDevices)
{
    m_deviceInterfaces.clear();
    // printf("dbName:%s\n",curDeviceParamDBName().c_str());
    if (LINK_ONLINE==m_linkStatus){
        m_deviceInterfaces=HGOnlineRWDB::readDeviceInfo(curDeviceParamDBName());
    }
    m_treeWidget->clear();

// std::map<std::string,std::map<std::string,std::vector<DeviceName>>> gDeviceNames
    for (std::string key:insertDeviceNameOrder){
        QTreeWidgetItem *a = new QTreeWidgetItem(m_treeWidget);
        QString qstr = QString::fromStdString(key);
        a->setText(0,qstr);
        a->setFlags(a->flags() & ~Qt::ItemIsSelectable);
        for (auto element:gDeviceNames[key]){
            QTreeWidgetItem *b = new QTreeWidgetItem(a);
            qstr = QString::fromStdString(element.first);
            b->setText(0,qstr);

            std::vector<DeviceName> listdevices=gDeviceNames[key][element.first];
            if (std::find(gNoexpandDeviceNames.begin(),gNoexpandDeviceNames.end(),key)!=gNoexpandDeviceNames.end()){
                // b->setFlags(b->flags() | Qt::ItemIsSelectable);
                // b->setCheckState(0,Qt::Unchecked);
                if (listdevices.size()>0){
                    if (std::find(existDevices.begin(),existDevices.end(),listdevices[0].pinyin)!=existDevices.end()){
                        b->setBackground(0,QBrush(Qt::green));
                    }
                }
                continue;
            }

            // if (listvector.size() <=0) {
            //     b->setCheckState(0,Qt::Unchecked);
            //     // b->setFlags(b->flags() & Qt::ItemIsSelectable);
            // } else b->setFlags(b->flags() & ~Qt::ItemIsSelectable);
            for (int i=0;i<int(listdevices.size());i++){
                QTreeWidgetItem *c=new QTreeWidgetItem(b);
                qstr=QString::fromStdString(listdevices[i].zhongwen);
                c->setText(0,qstr);
                if (std::find(existDevices.begin(),existDevices.end(),listdevices[i].pinyin)!=existDevices.end()){
                    c->setBackground(0,QBrush(Qt::green));
                }
                
                // c->setFlags(c->flags() | Qt::ItemIsSelectable);
                // c->setCheckState(0,Qt::Unchecked);
            }
        }
    }
    
    for (int i=0;i<m_treeWidget->topLevelItemCount();++i){
        QTreeWidgetItem *rootNode=m_treeWidget->topLevelItem(i);
        if (rootNode){
            setCheckStateRecursive(rootNode,Qt::Checked);
        }
    }
    
    m_treeWidget->expandAll();
    
    connect(m_treeWidget,SIGNAL(itemChanged(QTreeWidgetItem *,int)),
        this,SLOT(slotCheckItemStatus(QTreeWidgetItem*,int)));
}
void HGDeviceManageWidget::setCheckStateRecursive(QTreeWidgetItem* item,Qt::CheckState state){
    if (!item) return;
    std::string name=item->text(0).toStdString();
    // printf("module name:%s\n",name.c_str());
    // printf("interface count:%d\n",m_deviceInterfaces.size());
    if (-1!=fnIsInDevices(name))  item->setCheckState(0,state);
    for (int i=0;i<item->childCount();++i){
        setCheckStateRecursive(item->child(i),state);
    }
}
int HGDeviceManageWidget::fnIsInDevices(std::string name){
    int index1=-1;
    for (int index=0;index<int(m_deviceInterfaces.size());index++){
        if (m_deviceInterfaces[index].name==name){
            index1=index;
            break;
        }
    }
    return index1;
}
bool HGDeviceManageWidget::closeWindow()
{
    if (m_hgphDJW){
        int index=fnIsInDevices(m_hgphDJW->getName());
        if (index!=-1){
            m_deviceInterfaces[index].param=m_hgphDJW->getParamStr();
        }
        if (m_hgphDJW->closeWindow()){
            SAFE_DELETE(m_hgphDJW);
        }
    }
    if (m_rs485W){
        int index=fnIsInDevices(m_rs485W->getName());
        if (index!=-1){
            m_deviceInterfaces[index].param=m_rs485W->getParamStr();
        }
        if (m_rs485W->closeWindow()){
            SAFE_DELETE(m_rs485W);
        }
    }
    if (m_externalDeviceW){
        int index=fnIsInDevices(m_externalDeviceW->getName());
        if (index!=-1){
            m_deviceInterfaces[index].param=m_externalDeviceW->getParamStr();
        }
        if (m_externalDeviceW->closeWindow()){
            SAFE_DELETE(m_externalDeviceW);
        }
    }
    if (m_jiarebanW){
        int index=fnIsInDevices(m_jiarebanW->getName());
        if (index!=-1){
            m_deviceInterfaces[index].param=m_jiarebanW->getParamStr();
        }
        if (m_jiarebanW->closeWindow()){
            SAFE_DELETE(m_jiarebanW);
        }
    }
    if (m_shuifenyiW){
        int index=fnIsInDevices(m_shuifenyiW->getName());
        if (index!=-1){
            m_deviceInterfaces[index].param=m_shuifenyiW->getParamStr();
        }
        if (m_shuifenyiW->closeWindow()){
            SAFE_DELETE(m_shuifenyiW);
        }
    }
    if (m_zidongjinyangqiW){
        int index=fnIsInDevices(m_zidongjinyangqiW->getName());
        if (index!=-1){
            m_deviceInterfaces[index].param=m_zidongjinyangqiW->getParamStr();
        }
        if (m_zidongjinyangqiW->closeWindow()){
            SAFE_DELETE(m_zidongjinyangqiW);
        }
    }
    if (m_colorsensorW){
        int index=fnIsInDevices(m_colorsensorW->getName());
        if (index!=-1){
            m_deviceInterfaces[index].param=m_colorsensorW->getParamStr();
        }
        if (m_colorsensorW->closeWindow()){
            SAFE_DELETE(m_colorsensorW);
        }
    }
    if (m_photometerW){
        int index=fnIsInDevices(m_photometerW->getName());
        if (index!=-1){
            m_deviceInterfaces[index].param=m_photometerW->getParamStr();
        }
        if (m_photometerW->closeWindow()){
            SAFE_DELETE(m_photometerW);
        }
    }
    if (m_signalDJW){
        int index=fnIsInDevices(m_signalDJW->getName());
        if (index!=-1){
            m_deviceInterfaces[index].param=m_signalDJW->getParamStr();
        }
        if (m_signalDJW->closeWindow()){
            SAFE_DELETE(m_signalDJW);
        }
    }
    if (m_backUnitW){
        int index=fnIsInDevices(m_backUnitW->getName());
        if (index!=-1){
            m_deviceInterfaces[index].param=m_backUnitW->getParamStr();
        }
        if (m_backUnitW->closeWindow()){
            SAFE_DELETE(m_backUnitW);
        }
    }
    if (m_auxiliaryPumpW){
        int index=fnIsInDevices(m_auxiliaryPumpW->getName());
        if (index!=-1){
            m_deviceInterfaces[index].param=m_auxiliaryPumpW->getParamStr();
        }
        if (m_auxiliaryPumpW->closeWindow()){
            SAFE_DELETE(m_auxiliaryPumpW);
        }
    }

    if (LINK_ONLINE == m_linkStatus)
    {
        HGOnlineRWDB::deleteDB(curDeviceParamDBName());
        for (int index = 0; index < int(m_deviceInterfaces.size()); index++)
        {
            std::map<std::string, std::string> content = HGOnlineRWDB::getDeviceInfoMap(index + 1, m_deviceInterfaces[index]);
            HGOnlineRWDB::writeRecord(curDeviceParamDBName(), "序号", content);
        }
    }
    // std::string comInfo;
    // writeEnterComInfo(comInfo);
    return true;
}
HGDeviceManageWidget::~HGDeviceManageWidget()
{
    
}
void HGDeviceManageWidget::slotCheckItemStatus(QTreeWidgetItem* item,int column){
    if (column==0){
        Qt::CheckState state=item->checkState(0);
        std::string name=item->text(column).toStdString();
        std::string parentName=item->parent()->text(column).toStdString();
        if (parentName=="扩展接口" && name.find("COM")!=std::string::npos){
            // if (state==Qt::Checked){
            //     item->setCheckState(column,Qt::Unchecked);
                
            // } else {
            //     item->setCheckState(column,Qt::Checked);
            // }
            return;
        }
        int index=fnIsInDevices(name);
        if (-1==index) {
            if (state == Qt::Checked) // clicked check
            {
                // printf("already no , need checked\n");
                // check if out of range
                if (int(m_deviceInterfaces.size()) >= getInterfaceCount())
                {
                    // printf("out of range , show warning\n");
                    std::ostringstream ss;
                    ss << "连接设备只有" << getInterfaceCount() << "个接口，现已选择" << int(m_deviceInterfaces.size()) << "个接口，请确认！";
                    QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), QString::fromStdString(ss.str()));
                    item->setCheckState(column, Qt::Unchecked);
                    return;
                }
                else
                {
                    // printf("not out of range, push back\n");
                    DeviceInterface deviceInter;
                    deviceInter.name = name;
                    deviceInter.rootName = item->parent()->text(column).toStdString();

                    m_deviceInterfaces.push_back(deviceInter);
                }
            } else // clicked uncheck
            {
                // printf("already no,unchecked\n");
            }
        } else {
            if (state==Qt::Checked){
                // printf("already has , need checked\n");
            }
            else {
                // printf("delete already has index:%d,name:%s\n",index,name.c_str());
                m_deviceInterfaces.erase(m_deviceInterfaces.begin()+index);
            }
        }
    }
}
void HGDeviceManageWidget::slotGetDeviceName(QTreeWidgetItem *item,int column)
{
    if (item->isDisabled()) {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "接口未连接！");
        return;
    }
    std::string name=(item->text(column)).toStdString();
    closeWindow();
    int index=fnIsInDevices(name);
    if (name=="电极接口1#-Upol"){
        m_hgphDJW=new HGPHdjWidget(m_lang,name);
        if (index!=-1) m_hgphDJW->setParamStr(m_deviceInterfaces[index].param);
        m_layout->addWidget(m_hgphDJW,0,3,9,1);
    } else if (name=="SENSOR 1#"){
        m_colorsensorW=new HGColorSensor(m_lang,name);
        if (index!=-1) m_colorsensorW->setParamStr(m_deviceInterfaces[index].param);
        m_layout->addWidget(m_colorsensorW,0,3,9,1);
    } else if (name=="SENSOR 2#"){
        m_photometerW=new HGPhotometerWidget(m_lang,name);
        if (index!=-1) m_photometerW->setParamStr(m_deviceInterfaces[index].param);
        m_layout->addWidget(m_photometerW,0,3,9,1);
    } else if (name=="电极接口2#-lpol"){
        m_signalDJW=new HGSignalDJWidget(m_lang,name);
        if (index!=-1) m_signalDJW->setParamStr(m_deviceInterfaces[index].param);
        m_layout->addWidget(m_signalDJW,0,3,9,1);
    } else if (name.find("滴定组件")!=std::string::npos){
        m_backUnitW=new HGBackUnitWidget(m_lang,name);
        if (index!=-1) m_backUnitW->setParamStr(m_deviceInterfaces[index].param);
        m_layout->addWidget(m_backUnitW,0,3,9,1);
    } else if (name.find("PW-")!=std::string::npos 
        || name.find("CAN-")!=std::string::npos){
        if (item->parent()->text(column).toStdString().find("辅助泵")!=std::string::npos){
            m_auxiliaryPumpW=new HGAuxiliaryPumpWidget(m_lang,name);
            if (index!=-1) m_auxiliaryPumpW->setParamStr(m_deviceInterfaces[index].param);
            m_layout->addWidget(m_auxiliaryPumpW,0,3,9,1);
        }
    } else if (name.find("搅拌")!=std::string::npos&&name!="搅拌器"){
        m_auxiliaryPumpW=new HGAuxiliaryPumpWidget(m_lang,name);
        if (index!=-1) m_auxiliaryPumpW->setParamStr(m_deviceInterfaces[index].param);
        m_layout->addWidget(m_auxiliaryPumpW,0,3,9,1);
    } else if (name=="COM 2#"||name=="COM 3#"){
        m_rs485W=new HGRs485Widget(m_lang,name);
        if (index!=-1) m_rs485W->setParamStr(m_deviceInterfaces[index].param);
        m_layout->addWidget(m_rs485W,0,3,9,1);
    } else if (name=="COM 1#"){
        m_externalDeviceW=new HGExternalDeviceWidget(m_lang,name);
        if (index!=-1) m_externalDeviceW->setParamStr(m_deviceInterfaces[index].param);
        m_layout->addWidget(m_externalDeviceW,0,3,9,1);
    } else if (name=="自动进样器"){
        m_zidongjinyangqiW=new HGZidongjinyangqiWidget(m_lang,name);
        if (index!=-1) m_zidongjinyangqiW->setParamStr(m_deviceInterfaces[index].param);
        m_layout->addWidget(m_zidongjinyangqiW,0,3,9,1);
    } else if (name=="加热板"){
        m_jiarebanW=new HGJiarebanWidget(m_lang,name);
        if (index!=-1) m_jiarebanW->setParamStr(m_deviceInterfaces[index].param);
        m_layout->addWidget(m_jiarebanW,0,3,9,1);
    } else if (name=="水分仪"){
        m_shuifenyiW=new HGShuifenyiWidget(m_lang,name);
        if (index!=-1) m_shuifenyiW->setParamStr(m_deviceInterfaces[index].param);
        m_layout->addWidget(m_shuifenyiW,0,3,9,1);
    }
}
void HGDeviceManageWidget::slotLeftClickSearchLabel(){
    QString text = m_inputEdit->text();
    QList<QTreeWidgetItem*> foundItems=findItemsInAllColumns(m_treeWidget,text,
        Qt::CaseSensitive); 
    if (!foundItems.isEmpty()){
        QTreeWidgetItem* foundItem=foundItems.first();
        m_treeWidget->setCurrentItem(foundItem);
    } else {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), "没有找到项目！");
    }
}
// 自定义函数，在所有列中查找匹配的项
QList<QTreeWidgetItem*> HGDeviceManageWidget::findItemsInAllColumns(QTreeWidget* treeWidget, const QString& text, Qt::CaseSensitivity flags) {
    QList<QTreeWidgetItem*> foundItems;

    // 遍历所有顶级项
    for (int i = 0; i < treeWidget->topLevelItemCount(); ++i) {
        QTreeWidgetItem* rootItem = treeWidget->topLevelItem(i);

        // 检查顶级项的每列
        if (rootItem->text(0).contains(text, flags)) {
            foundItems.append(rootItem);
        }

        // 查找子项
        findItemsInAllColumnsRecursively(rootItem, text, flags, foundItems);
    }

    return foundItems;
}
// 递归查找所有子项
void HGDeviceManageWidget::findItemsInAllColumnsRecursively(QTreeWidgetItem* parentItem, const QString& text, Qt::CaseSensitivity flags, QList<QTreeWidgetItem*>& foundItems) {
    for (int i = 0; i < parentItem->childCount(); ++i) {
        QTreeWidgetItem* childItem = parentItem->child(i);

        // 检查当前项的每列
        for (int col = 0; col < childItem->columnCount(); ++col) {
            if (childItem->text(col).contains(text, flags)) {
                foundItems.append(childItem);
                break;  // 如果找到了匹配的项，退出当前列的检查
            }
        }

        // 递归检查子项
        findItemsInAllColumnsRecursively(childItem, text, flags, foundItems);
    }
}

void HGDeviceManageWidget::fnInitDeviceStatus()
{
    std::vector<std::string> deviceNames;
    #ifdef ENABLE_CAS6
    deviceNames=HGOnlineRWDB::getExistCAS6DeviceNames();
    #else
    deviceNames = HGOnlineRWDB::getExistDeviceNames();
    #endif

    std::ostringstream logtext;
    for (int i=0;i<int(deviceNames.size());i++){
        logtext<<"deviceType:"<<deviceNames[i]<<" ";
    }
    HGLog4Cplus::getLogInstance(LOG_PATH)->logout(logtext.str(),LOGINFO);

    fnInitTreeWidget(deviceNames);
    
}