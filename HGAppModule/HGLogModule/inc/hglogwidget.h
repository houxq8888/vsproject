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

// 日志条目结构
struct LogEntry {
    std::string time;
    std::string logContent;
    std::string operatorName;
    std::string sourceTable;  // 来源表名，用于定位
};

// 搜索结果分页结构
struct SearchResult {
    std::vector<LogEntry> entries;
    int totalCount;
    int currentPage;
    int totalPages;
    static const int ITEMS_PER_PAGE = 100;  // 每页显示条数
};

class HGLogWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGLogWidget(std::string,QWidget *parent = nullptr);
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
    void slotSearchNext();  // 搜索结果下一页
    void slotSearchPre();   // 搜索结果上一页

private:
    void fnReadDB(const std::string &tableName);
    int getTableNameIndex(const std::string& dbName);
    
    // 搜索相关函数
    void searchAllLogs();  // 跨所有表搜索
    void displaySearchResults();  // 显示搜索结果
    void highlightKeyword(QTableWidgetItem* item, const std::string& keyword);  // 高亮关键词
    std::vector<LogEntry> searchInTable(const std::string& tableName, const SearchCondition& condition);
    bool matchSearchCondition(const std::map<std::string,std::string>& logInfo, const SearchCondition& condition);

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
    
    // 搜索结果相关
    SearchResult m_searchResult;
    bool m_isInSearchMode;  // 是否处于搜索模式
    HGQLabel* m_searchNextLabel;  // 搜索结果下一页按钮
    HGQLabel* m_searchPreLabel;   // 搜索结果上一页按钮
    QLabel* m_searchPageLabel;    // 搜索结果页码显示
};

#endif // HGLOGWIDGET_H
