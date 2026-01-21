#include "hgchanneleditwidget.h"
#include <QHeaderView>
#include "common.h"
#include <QMessageBox>
#include <QAbstractButton>


HGChannelEditWidget::HGChannelEditWidget(std::string lang,QWidget *parent) : QWidget(parent),
m_lang(lang)
{
    m_curRow=-1;
    m_channels.clear();
    m_editGroupBox=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"ChannelSet")));//"通道配置");
    m_editlayout=new QGridLayout();
    m_layout=new QGridLayout();
    this->setLayout(m_layout);
    m_editGroupBox->setLayout(m_editlayout);

    m_closeLabel=new HGQLabel(false,getPath("/resources/V1/@1xze-cross 1.png"));
    m_editLabel=new HGQLabel(false,getPath("/resources/V1/@1xze-edit 1.png"));
    m_okLabel=new HGQLabel(false,getPath("/resources/V1/@1xze-success 1.png"));
    connect(m_closeLabel,SIGNAL(leftClicked()),this,SLOT(slotLeftClickClose()));
    connect(m_editLabel,SIGNAL(leftClicked()),this,SLOT(slotLeftClickEdit()));
    connect(m_okLabel,SIGNAL(leftClicked()),this,SLOT(slotLeftClickOk()));

    m_tableW=new QTableWidget(0,3);
    QStringList headers={"通道","检测器","检测方法"};
    m_tableW->setHorizontalHeaderLabels(headers);
    m_tableW->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableW->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    // m_tableW->verticalHeader()->setVisible(false);
    m_tableW->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableW->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_tableW->resizeRowsToContents();

    connect(m_tableW,SIGNAL(cellClicked(int,int)),this,SLOT(onCellClicked(int,int)));

    m_editlayout->addWidget(m_closeLabel,0,5);
    m_editlayout->addWidget(m_editLabel,0,6);
    m_editlayout->addWidget(m_okLabel,0,7);
    m_editlayout->addWidget(m_tableW,1,0,1,8);

    m_layout->addWidget(m_editGroupBox,0,0);

    std::vector<std::map<std::string,std::string>> fillContent=RWDb::readChannelInfo();
    for (int index=0;index<int(fillContent.size());index++){
        Channel channel;
        channel.channel=std::stoi(fillContent[index]["通道"]);
        channel.dbName=fillContent[index]["DBName"];
        channel.modules=RWDb::readModulesParam(fillContent[index]["DBName"]);
        fnDisplayChannelInfo(channel);
    }
}
bool HGChannelEditWidget::closeWindow()
{
    writeDB();
    return true;
}
void HGChannelEditWidget::onCellClicked(int row,int column){
    m_tableW->selectRow(row);
    m_curRow=row;
    if (row >= int(m_channels.size())) return;
    m_channels[row].modules=RWDb::readModulesParam(m_channels[row].dbName);
    emit modulesShow(m_channels[row]);
}
void HGChannelEditWidget::slotLeftClickClose(){
    int row=getSelectedRow(m_tableW);
    if (row >= m_tableW->rowCount()) return;
    if (row==-1) {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), QString::fromStdString(loadTranslation(m_lang,"SelectOneRecord")));//"请选中一条记录！");
        return;
    }
    // delete 
    if (row!=-1){
        QString szInfo;
        szInfo = "确定删除第[" + QString::number(row+1) + "]行的通道配置吗[?";
        if(QMessageBox::Yes == QMessageBox::question(this,QString::fromStdString(HG_DEVICE_NAME),
                                                     szInfo,
                                                     QMessageBox::Yes|QMessageBox::No
                                                     ))
        {
            RWDb::deleteRecord(CHANNELMANAGENAME,"序号",std::to_string(row+1));
            RWDb::deleteDB(m_channels[row].dbName);
            m_channels.erase(m_channels.begin()+row);
            m_tableW->removeRow(row);
            RWDb::clearChannelManageRecord();
            RWDb::deleteAllChannelModuleDB();
            for (int i = 0; i < int(m_channels.size()); i++)
            {
                std::map<std::string, std::string> infoS = RWDb::getChannelMap(i+1, m_channels[i]);
                RWDb::writeChannelManageRecord(infoS);
                std::vector<std::map<std::string, std::string>> infoSS;
                infoSS = RWDb::getModulesMap(m_channels[i]);
                m_channels[i].dbName=infoS["DBName"];
                RWDb::writeModulesRecord(infoS["DBName"], false, infoSS);
            }
        }
    } else {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), QString::fromStdString(loadTranslation(m_lang,"SelectOneRecord")));//"请选中一条记录！");
    }
}
void HGChannelEditWidget::slotLeftClickEdit(){
    emit editFlag(true);
}
void HGChannelEditWidget::slotLeftClickOk(){
    emit editFlag(false);
}
void HGChannelEditWidget::updateChannelInfo(Channel info,bool coverflag){
    if (coverflag)
    {
        if (m_curRow == -1)
            return;
        m_channels[m_curRow] = info;
        m_tableW->setItem(m_curRow, 0, new QTableWidgetItem(QString::number(info.channel)));
        for (auto content : info.modules)
        {
            if (content.typeName == "检测器")
            {
                m_tableW->setItem(m_curRow, 1, new QTableWidgetItem(QString::fromStdString(content.name)));
                std::string param = content.param;
                std::map<std::string, std::string> wparam = getParamMap(param);
                m_tableW->setItem(m_curRow, 2, new QTableWidgetItem(QString::fromStdString(wparam["检测方法"])));
            }
        }
    }
    else {
        int row=getSelectedRow(m_tableW);
        if (row >= m_tableW->rowCount()||row<0)
            fnDisplayChannelInfo(info);
        else
        {
            QString szInfo;
            szInfo = "需要覆盖第[" + QString::number(row+1) + "]行的通道配置,还是另存？";
            QMessageBox msgBox;
            msgBox.setWindowTitle(QString::fromStdString(HG_DEVICE_NAME));
            msgBox.setText(szInfo);
            msgBox.setIcon(QMessageBox::Question);
            QPushButton*coverBtn=msgBox.addButton("覆盖",QMessageBox::ActionRole);
            QPushButton*saveasBtn=msgBox.addButton("另存",QMessageBox::ActionRole);
            QPushButton*cancelBtn=msgBox.addButton(QString::fromStdString(loadTranslation(m_lang,"Cancel"))/*"取消"*/,QMessageBox::RejectRole);
            msgBox.setDefaultButton(cancelBtn);
            msgBox.exec();
            QAbstractButton* clickedButton=msgBox.clickedButton();
            if (clickedButton->text()=="覆盖")
            {
                m_channels[row] = info;
                m_tableW->setItem(row, 0, new QTableWidgetItem(QString::number(info.channel)));
                for (auto content : info.modules)
                {
                    if (content.typeName == "检测器")
                    {
                        m_tableW->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(content.name)));
                        std::string param = content.param;
                        std::map<std::string, std::string> wparam = getParamMap(param);
                        m_tableW->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(wparam["检测方法"])));
                    }
                }
            }
            else if (clickedButton->text()=="另存")
            {
                fnDisplayChannelInfo(info);
            }
            else
                return;
        }
    }
    writeDB();
}
void HGChannelEditWidget::fnDisplayChannelInfo(Channel info){
    m_channels.push_back(info);
    if (int(m_channels.size()) > m_tableW->rowCount()) m_tableW->insertRow(m_tableW->rowCount());
    m_tableW->setItem(m_channels.size()-1,0,new QTableWidgetItem(QString::number(info.channel)));
  
    for (auto content : info.modules){
        if (content.typeName=="检测器"){
            m_tableW->setItem(m_channels.size()-1,1,new QTableWidgetItem(QString::fromStdString(content.name)));
            std::string param=content.param;
            std::map<std::string,std::string> wparam=getParamMap(param);
            m_tableW->setItem(m_channels.size()-1,2,new QTableWidgetItem(QString::fromStdString(wparam["检测方法"])));
        }
    }
}
void HGChannelEditWidget::writeDB(){
    for (int i = 0; i < int(m_channels.size()); i++)
    {
        std::map<std::string, std::string> infoS = RWDb::getChannelMap(i+1, m_channels[i]);
        RWDb::writeChannelManageRecord(infoS);
        std::vector<std::map<std::string, std::string>> infoSS;
        infoSS = RWDb::getModulesMap(m_channels[i]);
        m_channels[i].dbName=infoS["DBName"];
        RWDb::writeModulesRecord(infoS["DBName"], true, infoSS);
    }
}