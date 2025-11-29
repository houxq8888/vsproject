#include "hgmethodeditwidget.h"
#include <QHeaderView>
#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include "common.h"
#include <QLineEdit>
#include <QMessageBox>
#include <QApplication>
#include <sstream>

HGMethodEditWidget::HGMethodEditWidget(std::string lang,std::vector<std::string> methods,
    QWidget *parent) : QWidget(parent),
    m_lang(lang)
{
    m_methodlists=methods;
    m_index=-1;
    m_fillContent.clear();
    m_editGroupBox=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"Method")));//"方法");
    m_editlayout=new QGridLayout();
    m_layout=new QGridLayout();
    this->setLayout(m_layout);
    m_editGroupBox->setLayout(m_editlayout);

    // m_openLabel=new HGQLabel();
    // m_openLabel->setPixmap(QPixmap("./resources/V1/@1xantOutline-folder-open 1.png"));
    m_saveLabel=new HGQLabel(false,getPath("/resources/V1/@1xmb-save 1.png"));
    m_deleteLabel=new HGQLabel(false,getPath("/resources/V1/@1xif-ui-delete 1.png"));
    m_addLabel=new HGQLabel(false,getPath("/resources/V1/@1xze-add-o 1.png"));
    m_editLabel=new HGQLabel(false,getPath("/resources/V1/@1xze-edit 1.png"));
    m_okLabel=new HGQLabel(false,getPath("/resources/V1/@1xze-certificate 1.png"));
    
    connect(m_saveLabel,SIGNAL(leftClicked()),this,SLOT(slotSaveMethod()));
    connect(m_deleteLabel,SIGNAL(leftClicked()),this,SLOT(slotDeleteMethod()));
    connect(m_addLabel,SIGNAL(leftClicked()),this,SLOT(slotAddMethod()));
    connect(m_editLabel,SIGNAL(leftClicked()),this,SLOT(slotEditMethod()));
    connect(m_okLabel,SIGNAL(leftClicked()),this,SLOT(slotOkMethod()));

    m_tableW=new QTableWidget(0,4);
    QStringList headers={"序号","名称","类型","创建时间"};
    m_tableW->setHorizontalHeaderLabels(headers);
    m_tableW->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableW->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_tableW->verticalHeader()->setVisible(false);
    m_tableW->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableW->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_tableW->resizeRowsToContents();    

    m_enterEvent=new QKeyEvent(QEvent::KeyPress,Qt::Key_Return,Qt::NoModifier);
    connect(m_tableW,SIGNAL(cellClicked(int,int)),this,SLOT(onCellClicked(int,int)));
    connect(m_tableW,SIGNAL(cellChanged(int,int)),this,SLOT(slotCellChanged(int,int)));

    // m_editlayout->addWidget(m_openLabel,0,1);
    m_editlayout->addWidget(m_saveLabel,0,2);
    m_editlayout->addWidget(m_deleteLabel,0,3);
    m_editlayout->addWidget(m_addLabel,0,5);
    m_editlayout->addWidget(m_editLabel,0,6);
    m_editlayout->addWidget(m_okLabel,0,7);
    m_editlayout->addWidget(m_tableW,1,0,1,8);

    m_layout->addWidget(m_editGroupBox,0,0);

    m_fillContent=RWDb::readMethodInfo();
    for (int index=0;index<int(m_fillContent.size());index++){
        m_fillContent[index]["参数"]=RWDb::readMethodParam(m_fillContent[index]["DBName"]).param;
        fnDisplayMethodInfo(index,m_fillContent[index]);
        addMethodType(m_methodlists,index);
    }
}
bool HGMethodEditWidget::closeWindow()
{
    return true;
}
void HGMethodEditWidget::onCellClicked(int row,int column){
    m_index=row;
    m_tableW->selectRow(row);
    m_curCol=column;
    m_curRow=row;
    if (row >= int(m_fillContent.size())) return;
    std::string param=m_fillContent[row]["参数"];
    emit methodInfoWShow(m_fillContent[row]["类型"],param);
}
std::string HGMethodEditWidget::getIndexTableName(std::string tableName,int &index){
    for (int i=0;i<int(m_fillContent.size());i++){
        if (m_fillContent[i]["DBName"]==tableName){
            index=i;
            return m_fillContent[i]["序号"];
        }
    }
    return "";
}
void HGMethodEditWidget::saveParam(std::string param){
    int row=getSelectedRow(m_tableW);
    m_method.param=param;
    m_method.dbName=m_curSaveDBName;
    m_method.name=m_tableW->item(row,1)->text().toStdString();
    m_method.indexStr=m_tableW->item(row,0)->text().toStdString();
    m_method.createTime=m_tableW->item(row,3)->text().toStdString();
    m_method.type=m_tableW->item(row,2)->text().toStdString();
    
    std::vector<std::map<std::string,std::string>> infoS;
    infoS=RWDb::getMethodParamMap(m_method);
    RWDb::writeMethodRecord(m_method.dbName,m_coverFlag,infoS);
}
void HGMethodEditWidget::slotSaveMethod(){
    int row = getSelectedRow(m_tableW);
    if (row >= m_tableW->rowCount()) return;
    m_tableW->setEditTriggers(QAbstractItemView::NoEditTriggers);
    if (row==-1) {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), QString::fromStdString(loadTranslation(m_lang,"SelectOneRecord")));//"请选中一条记录！");
        return;
    }
    std::string tableName="";
    bool coverFlag=false;
    // list method
    QDialog dialog(this);
    dialog.setWindowTitle(QString::fromStdString(loadTranslation(m_lang,"InputSaveName")));
    dialog.setWindowModality(Qt::ApplicationModal);

    QListWidget* listW=new QListWidget(&dialog);
    QLineEdit* saveNameEdit=new QLineEdit(&dialog);
    saveNameEdit->setPlaceholderText(QString::fromStdString(loadTranslation(m_lang,"InputSaveName")));
    QPushButton* coverBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"OverwriteFile"))/*"覆盖原文件"*/,&dialog);
    QPushButton* okbtn=new QPushButton(&dialog);
    okbtn->setText(QString::fromStdString(loadTranslation(m_lang,"Ok")));//"确定");
    QPushButton* cancelbtn=new QPushButton(&dialog);
    cancelbtn->setText(QString::fromStdString(loadTranslation(m_lang,"Cancel")));//"取消");

    std::vector<std::string> names=RWDb::getAllTables(MethodDBName);

    int selectRow=row, coverRow=-1;
    std::string selectIndex=m_fillContent[row]["序号"];
    std::string coverIndex="";
    if (m_fillContent[row]["DBName"]=="") 
        coverBtn->setEnabled(false);

    connect(listW,&QListWidget::itemClicked,[&](){
        saveNameEdit->setText(listW->currentItem()->text());
    });
    connect(coverBtn,&QPushButton::clicked,[&](){
        tableName=m_fillContent[row]["DBName"];
        saveNameEdit->setText(QString::fromStdString(tableName));
        coverFlag=true;
    });
    connect(okbtn,&QPushButton::clicked,[&](){
        tableName=saveNameEdit->text().toStdString();
        if (std::find(names.begin(),names.end(),tableName)!=names.end()){
            coverIndex=getIndexTableName(tableName,coverRow);
            if (QMessageBox::Yes!=QMessageBox::question(this, QString::fromStdString(HG_DEVICE_NAME), 
                                        "确定要覆盖已有文件["+QString::fromStdString(tableName)+"]?记录将以新的为准!",
                                        QMessageBox::Yes|QMessageBox::No))
            {
                coverFlag=false;
                QMessageBox::information(this,QString::fromStdString(HG_DEVICE_NAME),
                                        "已有文件序号为["+QString::fromStdString(coverIndex)+"]");
                return;
            } else {
                coverFlag=true;
            }
        } else {
            tableName=MethodDBName+std::string("_")+tableName;
        }
       
       dialog.close();
    });
    connect(cancelbtn,&QPushButton::clicked,[&](){
        dialog.close();
    });

    
    for (const auto &name:names){
        if (name==METHODMANAGENAME) continue;
        listW->addItem(QString::fromStdString(name));
    }
    setFocusByName(listW,m_fillContent[row]["DBName"]);
    QVBoxLayout *layout=new QVBoxLayout(&dialog);
    layout->addWidget(listW);
    layout->addWidget(saveNameEdit);
    layout->addWidget(coverBtn);
    layout->addWidget(okbtn);
    layout->addWidget(cancelbtn);
    dialog.setLayout(layout);
    dialog.exec();

    m_curSaveDBName=tableName;
    m_coverFlag=coverFlag;
    if (tableName!=""){
        m_fillContent[row]["DBName"]=tableName;
        if (coverRow!=-1&&coverFlag&&selectRow!=coverRow){
            // delete 
            RWDb::deleteRecord(METHODMANAGENAME,"序号",coverIndex);
            m_fillContent.erase(m_fillContent.begin()+coverRow);
            m_tableW->removeRow(coverRow);
            for (int i=0;i<m_tableW->rowCount();i++){
                m_tableW->item(i,0)->setText(QString("%1").arg(i+1));
                m_fillContent[i]["序号"]=std::to_string(i+1);
            }
            RWDb::clearMethodManageRecord();
        }
        for (int i=0;i<m_tableW->rowCount();++i){
            QComboBox* combox=qobject_cast<QComboBox*>(m_tableW->cellWidget(i,2));
            if (combox){
                std::string selectedValue=combox->currentText().toStdString();
                m_fillContent[i]["类型"]=selectedValue;
            }
            m_fillContent[i]["名称"]=m_tableW->item(i,1)->text().toStdString();
            
            RWDb::writeMethodManageRecord(m_fillContent[i]);
        }
        emit saveSignal();
    }
}
void HGMethodEditWidget::slotDeleteMethod(){
    int row = getSelectedRow(m_tableW);
    if (row >= m_tableW->rowCount()) return;
    m_tableW->setEditTriggers(QAbstractItemView::NoEditTriggers);
    if (row==-1) {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), QString::fromStdString(loadTranslation(m_lang,"SelectOneRecord")));//"请选中一条记录！");
        return;
    }
    if (row != -1)
    {
        
        QString szInfo;
        std::string name = m_tableW->item(row, 1)->text().toStdString();
        szInfo = "确定删除序号[" + m_tableW->item(row, 0)->text() + "]方法?";
        if (QMessageBox::Yes == QMessageBox::question(this, QString::fromStdString(HG_DEVICE_NAME),
                                                      szInfo,
                                                      QMessageBox::Yes | QMessageBox::No))
        {
            int index=std::stoi(m_tableW->item(row,0)->text().toStdString());
            RWDb::deleteRecord(METHODMANAGENAME,"序号",m_tableW->item(row,0)->text().toStdString());
            RWDb::deleteDB(m_fillContent[row]["DBName"]);
            m_fillContent.erase(m_fillContent.begin()+index-1);
            m_tableW->removeRow(row);
            for (int i=0;i<m_tableW->rowCount();i++){
                m_tableW->item(i,0)->setText(QString("%1").arg(i+1));
                m_fillContent[i]["序号"]=std::to_string(i+1);
            }
            RWDb::clearMethodManageRecord();
            for (const auto &info:m_fillContent){
                RWDb::writeMethodManageRecord(info);
            }
        }
    }
    else
    {
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME), QString::fromStdString(loadTranslation(m_lang,"SelectOneRecord")));//"请选中一条记录！");
    }
}
void HGMethodEditWidget::fnDisplayMethodInfo(int count,std::map<std::string,std::string> fillContent){
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
    m_tableW->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void HGMethodEditWidget::slotAddMethod(){
    int index=m_fillContent.size();
    m_method.indexStr=std::to_string(index+1);
    
    m_method.createTime=getStandardCurTime();

    std::map<std::string,std::string> infoS=RWDb::getMethodMap(index+1,m_method);
    m_fillContent.push_back(infoS);
    fnDisplayMethodInfo(index,m_fillContent[index]);
    m_tableW->setEditTriggers(QAbstractItemView::AllEditTriggers);

    RWDb::writeMethodManageRecord(infoS);

    setTableWColNoEdit(m_tableW,0);
    setTableWColNoEdit(m_tableW,3);
    emit getMethodLists();
}
void HGMethodEditWidget::addMethodType(std::vector<std::string> methods,int index){
    QComboBox* typeCombo=new QComboBox();
    for (int i=0;i<int(methods.size());i++)
        typeCombo->addItem(QString::fromStdString(methods[i]));
    if (index<0) index=int(m_fillContent.size()-1);
    m_tableW->setCellWidget(index,2,typeCombo);
    typeCombo->setCurrentIndex(-1);
    if (m_fillContent[index]["类型"]!="")
        typeCombo->setCurrentText(QString::fromStdString(m_fillContent[index]["类型"]));
    connect(typeCombo,&QComboBox::currentIndexChanged,
        [=](int index){
            m_fillContent[index]["类型"]=typeCombo->itemText(index).toStdString();
        // printf("click index:%s\n",typeCombo->itemText(index).toStdString().c_str());
    });
}
void HGMethodEditWidget::slotEditMethod(){
    m_tableW->setEditTriggers(QAbstractItemView::AllEditTriggers);
    setTableWColNoEdit(m_tableW,0);
    setTableWColNoEdit(m_tableW,3);
}
void HGMethodEditWidget::slotOkMethod(){
    if (!m_tableW->currentItem()) return;
    QApplication::postEvent(m_tableW,m_enterEvent);
    m_curRow=m_tableW->currentItem()->row();
    m_curCol=m_tableW->currentItem()->column();
    // QString text =m_tableW->currentItem()->text();
    // printf("text:%s\n",text.toStdString().c_str());
    m_tableW->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableW->closePersistentEditor(m_tableW->item(m_curRow,m_curCol));
    // m_tableW->setItem(m_curRow, m_curCol, new QTableWidgetItem(text));
    
}
void HGMethodEditWidget::slotCellChanged(int row,int column){
    QTableWidgetItem *item=m_tableW->item(row,column);
    std::string value=item->text().toStdString();
    // printf("text:%s\n",value.c_str());
}