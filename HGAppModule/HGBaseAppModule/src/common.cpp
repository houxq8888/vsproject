#include "common.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QDebug>


QRect getScreenGeometry()
{
    QDesktopWidget* desktopWidget=QApplication::desktop();
    QRect applicationRect=desktopWidget->screenGeometry();
    return applicationRect;
}
int getSelectedRow(QTableWidget *tableWidget)
{
    // 获取选中的所有单元格
    QList<QTableWidgetItem *> selectedItems = tableWidget->selectedItems();

    // 如果没有选中的单元格
    if (selectedItems.isEmpty())
    {
        return -1; // 返回 -1 表示没有选中行
    }

    // 获取选中单元格的行号
    int row = selectedItems.first()->row(); // 获取第一个选中单元格的行号

    // 遍历所有选中的单元格，确保它们都在同一行
    for (QTableWidgetItem *item : selectedItems)
    {
        if (item->row() != row)
        {
            return -1; // 如果选中的单元格不在同一行，返回 -1
        }
    }

    return row; // 如果选中的所有单元格都在同一行，返回该行号
}
void setFocusByName(QListWidget *listWidget, const std::string &name)
{
    QList<QListWidgetItem *> items = listWidget->findItems(QString::fromStdString(name), Qt::MatchExactly);
    if (!items.isEmpty())
    {
        QListWidgetItem *item = items.first();
        listWidget->setCurrentItem(item);
        listWidget->scrollToItem(item);
    }
}
int getColumnIndexByName(QTableWidget* table,const QString &columnName){
    int columnCount=table->columnCount();
    for (int i=0;i<columnCount;++i){
        if (table->horizontalHeaderItem(i)->text()==columnName){
            return i;
        }
    }
    return -1;
}
int getColumnIndexByToolTip(QTableWidget* table,const QString &toolTip){
    int columnCount=table->columnCount();
    for (int i=0;i<columnCount;++i){
        QString currentToolTip = table->horizontalHeaderItem(i)->toolTip();
        // printf("curTip:%s,toolTip:%s\n",currentToolTip.toStdString().c_str(),toolTip.toStdString().c_str());
        if (currentToolTip==toolTip){
            return i;
        }
    }
    return -1;
}
void setTableWColNoEdit(QTableWidget* table,int column)
{
    int rowCount=table->rowCount();
    for (int row=0;row<rowCount;++row){
        QTableWidgetItem* item=table->item(row,column);
        if (!item){
            item=new QTableWidgetItem();
            table->setItem(row,column,item);
        }
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    }
}
bool findWidgetPosition(QGridLayout* layout,const std::string& name,int &row,int &column,bool &stable){
    for (int r=0;r<layout->rowCount();++r){
        for (int c=0;c<layout->columnCount();++c){
            QLayoutItem *item=layout->itemAtPosition(r,c);
            if (item&&item->widget()){
                if (QLabel *label=qobject_cast<QLabel*>(item->widget())){
                    if (label->text().toStdString()==name){
                        row=r;
                        column=c;
                        stable=false;
                        return true;
                    }
                } else if (LabelWithImg *label=qobject_cast<LabelWithImg*>(item->widget())){
                    if (label->getTextLabel()->text().toStdString()==name){
                        row=r;
                        column=c;
                        stable=true;
                        return true;
                    }
                }
            } 
        }
    }
    return false;
}
void setWidgetTextAtPosition(QGridLayout* layout,int row,int column,const std::string&text){
    QLayoutItem *item=layout->itemAtPosition(row,column);
    if (item && item->widget()){
        if (QLabel *label=qobject_cast<QLabel*>(item->widget())){
            label->setText(QString::fromStdString(text));
        } else if (QPushButton *button=qobject_cast<QPushButton*>(item->widget())){
            button->setText(QString::fromStdString(text));
        } else if (QComboBox *combo=qobject_cast<QComboBox*>(item->widget())){
            combo->setCurrentText(QString::fromStdString(text));
        } else if (QLineEdit *edit=qobject_cast<QLineEdit*>(item->widget())) {
            edit->setText(QString::fromStdString(text));
        } else if (LabelWithImg *labelWithImg=qobject_cast<LabelWithImg*>(item->widget())){
            labelWithImg->setImg(text);
        } else if (QCheckBox *checkBox=qobject_cast<QCheckBox*>(item->widget())){
            checkBox->setText(QString::fromStdString(text));
        }
        else {
            printf("unsupported widget\n");
        }
    } else {
        printf("no widget found at the specified position\n");
    }
}
std::string getPath(std::string inPath){
    std::ostringstream ss;
    ss<<RWDb::readCurDirPath()<<inPath;
    return (ss.str());
}
// QObject* findObjectByName(QLayout* layout,const std::string &name){
//     if (!layout) return nullptr;
//     for (int i=0;i<layout->count();++i){
//         QLayoutItem *item=layout->itemAt(i);
//         if (!item) continue;
//         // if control,check name
//         QWidget* widget=item->widget();
//         if (widget && widget->objectName() == name){
//             return widget;
//         }
//         // if layout, recursive check
//         QLayout *childLayout=item->layout();
//         if (childLayout){
//             QObject *result=findObjectByName(childLayout,name);
//             if (result) return result;
//         }
//     }
//     return nullptr;
// }
void setControlText(QGridLayout* layout,const std::string& param){
    std::map<std::string,std::string> wparam=getParamMap(param);
    for(auto w:wparam){
        int row=-1,column=-1;
        bool stable=false;
        if (findWidgetPosition(layout,w.first,row,column,stable))
        {
            if (row==-1) continue;
            if (stable){
                setWidgetTextAtPosition(layout,row,column,w.second);
            } else
                setWidgetTextAtPosition(layout,row,column+1,w.second);
        }
        printf("name,value(%s,%s),row=%d,col=%d\n",w.first.c_str(),w.second.c_str(),row,column);
    }
}
void moveToolBoxItemUp(QToolBox* toolBox,int index){
    if (index <=0 || index>=toolBox->count()){
        return;
    }
    if ((index-1) <=0) return;
    QWidget* currentPage=toolBox->widget(index);
    QString currentTitle=toolBox->itemText(index);
    QWidget*previousPage=toolBox->widget(index-1);
    QString previousTitle=toolBox->itemText(index-1);
    toolBox->removeItem(index);
    toolBox->removeItem(index-1);
    toolBox->insertItem(index-1,currentPage,currentTitle);
    toolBox->insertItem(index,previousPage,previousTitle);
    toolBox->setCurrentIndex(index-1);
}
void setUniformSizePolicy(QLayout *layout,QSizePolicy::Policy hPolicy,
    QSizePolicy::Policy vPolicy)
{
    if (!layout) return;
    for (int i=0;i<layout->count();++i){
        QLayoutItem *item=layout->itemAt(i);
        if (!item) continue;

        QWidget* widget=item->widget();
        if (widget){
            widget->setSizePolicy(hPolicy,vPolicy);
        }

        QLayout* childLayout=item->layout();
        if (childLayout){
            setUniformSizePolicy(childLayout,hPolicy,vPolicy);
        }
    }
}
void moveToolBoxItemDown(QToolBox* toolBox,int index){
    if (index < 0 || index>=toolBox->count()-1){
        return;
    }
    int index1=index;
    int index2=index+1;
    // toolBox->removeItem(index1);
    toolBox->insertItem(index2, toolBox->widget(index1), toolBox->itemText(index1));
    // // 获取页面内容
    // QList<QWidget *> pages;
    // QList<QString> titles;
    // for (int i = 0; i < toolBox->count(); ++i) {
    //     pages.append(toolBox->widget(i));
    //     titles.append(toolBox->itemText(i));
    // }

    // // 删除所有页面
    // for (int i = toolBox->count() - 1; i >= 0; --i) {
    //     toolBox->removeItem(i);
    // }

    // 按新的顺序添加页面（假设交换第0页和第1页）
    // for (int i = 0; i < index; ++i) {
    //     toolBox->addItem(pages[i], titles[i]);
    // }
    // toolBox->addItem(pages[index+1], titles[index+1]);
    // toolBox->addItem(pages[index], titles[index]);
    // if (index+2 >= pages.size()) return;
    // for (int i = index+2; i < pages.size(); ++i) {
    //     toolBox->addItem(pages[i], titles[i]);
    // }
}
bool IsValidNumber(QString &qstrSrc)
{
    QByteArray ba=qstrSrc.toLatin1();
    const char* s=ba.data();
    bool bret=true;
    while (*s)
    {
        if ((*s>='0'&&*s<='9') || (*s=='.')||(*s=='-'))
        {}
        else {
            bret=false;
            break;
        }
        s++;
    }
    return bret;
}
void removeWidgetsFromLayout(QGridLayout *layout) {
    // 获取布局中总的 widget 数量
    int itemCount = layout->count();
    for (int i = itemCount - 1; i >= 0; --i) { // 从后往前遍历
        QLayoutItem *item = layout->takeAt(i); // 移除布局中的 item
        if (item) {
            QWidget *widget = item->widget(); // 获取 widget
            if (widget) {
                // 移除 widget 但不释放内存
                widget->hide(); // 可以选择隐藏 widget
                // widget->setParent(nullptr); // 解除父子关系（如果需要）
            }
            delete item; // 删除布局项，但不删除 widget
        }
    }
}
void selectCellsUsingSelectionModel(QTableWidget* tableWidget, const QString& targetText) {
    tableWidget->clearSelection();
    QItemSelectionModel* selectionModel = tableWidget->selectionModel();
    QItemSelection selection;
    
    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        for (int col = 0; col < tableWidget->columnCount(); ++col) {
            QTableWidgetItem* item = tableWidget->item(row, col);
            if (item && item->text() == targetText) {
                QModelIndex index = tableWidget->model()->index(row, col);
                selection.merge(QItemSelection(index, index), QItemSelectionModel::Select);
            }
        }
    }
    selectionModel->select(selection, QItemSelectionModel::Select);
}
std::vector<std::pair<size_t,size_t>> matchStartEndPairs(
    const std::vector<size_t>& starts,
    const std::vector<size_t>& ends,
    size_t maxLength
)
{
    std::vector<std::pair<size_t,size_t>> result;
    size_t i=0,j=0;
    while (i<starts.size() && j<ends.size()){
        size_t start = starts[i];
        size_t end = ends[i];
        if (start >= maxLength){
            ++i;
            continue;
        }
        if (end > start && end <= maxLength){
            result.emplace_back(start,end);
            ++i;
            ++j;
        } else {
            ++j;
        }
    }
    return result;
}