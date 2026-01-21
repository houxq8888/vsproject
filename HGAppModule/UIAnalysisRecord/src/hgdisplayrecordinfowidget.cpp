#include "hgdisplayrecordinfowidget.h"
#include <QHeaderView>
#include <QDialog>
#include "labelwithimg.h"
#include "common.h"


// std::vector<std::string> recordNames={"流程","方法","检测器","滴定剂","反应液","溶剂","分析时长","运行状态"};

HGDisplayRecordInfoWidget::HGDisplayRecordInfoWidget(
    std::string lang,
    const std::string &name,QWidget *parent) : QWidget(parent),
    m_lang(lang)
{
    for (auto name:recordNames){
        m_recordLabels[name]=recordLabel();
    }

    fnReadDB();
    m_dragDialog=NULL;
    m_layout=new QGridLayout();
    this->setLayout(m_layout);

    m_nameLabel=new QLabel(QString::fromStdString(name));

    m_displayTypeLabel=new HGQLabel(false,getPath("/resources/V1/@1xze-bars 1.png"));
    m_closeLabel=new HGQLabel(false,getPath("/resources/V1/@1xze-cross 1.png"));

    connect(m_displayTypeLabel,SIGNAL(leftClicked()),this,SLOT(slotDisplayType()));

    m_tableW=new QTableWidget(0,2);
    m_tableW->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableW->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableW->verticalHeader()->setVisible(false);
    m_tableW->horizontalHeader()->setVisible(false);
    m_tableW->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_layout->addWidget(m_nameLabel,0,0);
    m_layout->addWidget(m_displayTypeLabel,0,1);
    // m_layout->addWidget(m_closeLabel,0,2);
    m_layout->addWidget(m_tableW,1,0,1,3);


    for (auto name:recordNames){
        fnUpdateDisplay(m_recordLabels[name].flag,name);
    }
}
void HGDisplayRecordInfoWidget::fnWriteDB(){
    for (auto name:recordNames){
        if (m_recordLabels[name].flag) GlobalSingleton::instance().setDataChartInfo(name,"true");
        else GlobalSingleton::instance().setDataChartInfo(name,"false");
        GlobalSingleton::instance().saveDataChartInfo();
    }
}
void HGDisplayRecordInfoWidget::fnReadDB(){
    for (auto name:recordNames){
        if (GlobalSingleton::instance().getDataChartInfo(name)=="true") m_recordLabels[name].flag=true;
        else m_recordLabels[name].flag=false;
    }
}
bool HGDisplayRecordInfoWidget::closeWindow()
{
    fnWriteDB();
    return true;
}
HGDisplayRecordInfoWidget::~HGDisplayRecordInfoWidget()
{
    
}
void HGDisplayRecordInfoWidget::slotDisplayType(){
    if (m_dragDialog!=NULL) {
        SAFE_DELETE(m_dragDialog);
        return;
    }
    m_dragDialog=new DraggableDialog(this);

    QVBoxLayout* layout=new QVBoxLayout(m_dragDialog);

    LabelWithImg* nameLabel=new LabelWithImg(IMGTOP,12,getPath("/resources/V1/@1xze-bars 1.png"),
        loadTranslation(m_lang,"DisplayContent"));// "显示内容");

    for (auto name : recordNames)
    {
        if (m_recordLabels[name].flag)
            m_recordLabels[name].label = new LabelWithImg(IMGLEFT, 12, getPath("/resources/V1/@1xze-certificate 1.png"), name);
        else
            m_recordLabels[name].label = new LabelWithImg(IMGLEFT, 12, getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"), name);
    }

    layout->addWidget(nameLabel);
    for (auto it:m_recordLabels){
        connect(m_recordLabels[it.first].label,SIGNAL(clickLeftName(std::string)),this,SLOT(slotLabelClicked(std::string)));
        layout->addWidget(it.second.label);
    }

    m_dragDialog->setLayout(layout);
    m_dragDialog->show();
}
void HGDisplayRecordInfoWidget::slotLabelClicked(std::string name){
    m_recordLabels[name].flag=!m_recordLabels[name].flag;
    if (!m_recordLabels[name].flag) {
        m_recordLabels[name].label->setImg(getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"));
        fnUpdateDisplay(false,name);
    }
    else {
        m_recordLabels[name].label->setImg(getPath("/resources/V1/@1xze-certificate 1.png"));
        fnUpdateDisplay(true,name);
    }    
}
void HGDisplayRecordInfoWidget::fnUpdateDisplay(bool flag,std::string name){
    if (flag){
        m_tableW->insertRow(m_tableW->rowCount());
        m_tableW->setItem(m_tableW->rowCount()-1,0,new QTableWidgetItem(QString::fromStdString(name)));
    } else {
        int index=-1;
        for (int row = 0; row < m_tableW->rowCount(); ++row) {
            QTableWidgetItem *item = m_tableW->item(row, 0);  // 获取第0列的单元格
            if (item && item->text().toStdString() == name) {
                index=row;  // 找到匹配的名称
            }
        }
        if (index!=-1) m_tableW->removeRow(index);
    }
}
void HGDisplayRecordInfoWidget::showTestInfo(std::map<std::string,std::string> info)
{
    if (RWDb::getTaskRunFlag() == "true") return;
    for (int i=0;i<int(m_tableW->rowCount());i++){
        QString name=m_tableW->item(i,0)->text();
        std::string content=info[name.toStdString()];
        m_tableW->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(content)));
    }
}