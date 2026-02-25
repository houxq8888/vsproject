#ifndef HGLOGWIDGET_H
#define HGLOGWIDGET_H

#include <QWidget>
#include "hgqlabel.h"
#include <QSplitter>
#include <QPushButton>
#include <QTableWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QComboBox>
#include <QLineEdit>
#include "hginputsearchconditionwidget.h"

// 自定义表格项，支持富文本高亮
class HighlightedTableItem : public QTableWidgetItem
{
public:
    explicit HighlightedTableItem(const QString& text = QString());
    void setHighlightText(const QString& highlightText);
    QString highlightText() const;

private:
    QString m_highlightText;
};

class HGLogWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGLogWidget(std::string lang,QWidget *parent = nullptr);
    bool closeWindow();
    ~HGLogWidget();

signals:

private slots:
    void slotLogTypeChanged(int);
    void slotKeyWord(QString);
    void slotTimeFrom(QString);
    void slotTimeTo(QString);
    void slotSearch();
    void slotSaveSearchLog();
    void slotNext();
    void slotPre();
    void slotClearSearch();
    void slotHeaderClicked(int column);  // 表头点击排序

private:
    void fnReadDB(const std::string &tableName);
    int getTableNameIndex(const std::string& dbName);
    
    // 搜索模式读取数据（全库搜索）
    void fnReadDBSearchMode();
    
    // 普通模式读取数据（单表分页）
    void fnReadDBNormalMode(const std::string& tableName);
    
    // 更新分页标签
    void updatePageLabel();
    
    // 创建高亮显示的表格项
    QTableWidgetItem* createHighlightedItem(const std::string& text, const std::string& keyword);

private:
    QLabel* m_pageLabel;
    HGQLabel *m_saveLabel, *m_exportLabel;
    HGQLabel* m_nextLabel, *m_preLabel;
    QGroupBox *m_manipulateGroup;
    QGridLayout *m_manipulateLayout, *m_layout;
    QLabel* m_logTypeLabel;
    QComboBox* m_logTypeComboBox;
    HGInputSearchConditionWidget* m_inputsearchConditionW;
    QTableWidget* m_tableW;
    std::string m_lang;

    SearchCondition m_searchCondition;
    
    std::map<std::string, int> m_logContentMap;
    int m_curDisplayIndex;
    std::vector<std::string> m_auditLogTableNames;
    
    // 搜索模式相关
    bool m_isSearchMode;                    // 是否为搜索模式
    int m_searchCurrentPage;                // 搜索结果当前页
    int m_searchTotalCount;                 // 搜索结果总数
    int m_searchPageSize;                   // 每页显示数量
    std::vector<std::map<std::string,std::string>> m_searchResults; // 当前页搜索结果
    
    // 排序相关
    int m_sortColumn;                       // 当前排序列（0=时间，1=内容，2=操作员）
    bool m_sortAscending;                   // 是否升序排序
};

#endif // HGLOGWIDGET_H
