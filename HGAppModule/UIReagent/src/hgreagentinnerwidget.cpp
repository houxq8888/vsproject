#include "hgreagentinnerwidget.h"
#include <QHeaderView>

HGReagentInnerWidget::HGReagentInnerWidget(std::string lang,QWidget *parent) : QWidget(parent),
m_lang(lang)
{
    m_layout=new QGridLayout();
    // this->setLayout(m_layout);

    m_tableW=new QTableWidget(1,5,this);
    m_tableW->setHorizontalHeaderLabels({"指标设备单元", "接口", "联机", "当前通道", "选择"});
    m_tableW->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableW->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_tableW->verticalHeader()->setVisible(false);
    m_tableW->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_tableW->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableW->resizeRowsToContents();

    // m_addBtn=new QPushButton("添加行");
    // m_linkDeviceBtn->setFocusPolicy(Qt::NoFocus);
    // m_addBtn->setStyleSheet("color: red;");
    // m_linkDeviceBtn->setStyleSheet("color: red;");
    // m_tableWidget->setCellWidget(addRow,0,m_addBtn);
    // m_tableWidget->setCellWidget(addRow,4,m_linkDeviceBtn);
    // connect(m_addBtn,SIGNAL(clicked()),this,SLOT(slotClickAdd()));
    // connect(m_linkDeviceBtn,SIGNAL(clicked()),this,SLOT(slotClickLinkDevice()));
    // m_tableWidget->setSpan(addRow,0,1,4);
    // m_tableWidget->setSpan(addRow,4,1,4);

    // m_layout->addWidget(m_tableW,0,0);
}
bool HGReagentInnerWidget::closeWindow()
{
    return true;
}
int HGReagentInnerWidget::getRowCount(){
    return m_tableW->rowCount();
}
std::vector<std::string> HGReagentInnerWidget::getFields(){
    std::vector<std::string> names;
    int colCount=m_tableW->columnCount();
    for (int col=0;col<colCount;++col){
        QTableWidgetItem* headerItem=m_tableW->horizontalHeaderItem(col);
        if (headerItem){
            names.push_back(headerItem->text().toStdString());
        }
    }
    return names;
}