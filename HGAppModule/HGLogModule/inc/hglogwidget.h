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
#include <QLabel>
#include "hginputsearchconditionwidget.h"

// 搜索结果结构体
struct SearchResult {
    std::string tableName;      // 来源表名
    std::string time;
         // 时间
    std::string logContent;     // 日志内容
    std::string oper;           // 操作员
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

private:
    void fnReadDB(const std::string &tableName);
    int getTableNameIndex(const std::string& dbName);
    
    // 全库搜索相关方法
    void performGlobalSearch();
    void displaySearchResultsPage(int pageIndex);
    QLabel* createHighlightLabel(const std::string& text, const std::string& keyword);
    bool matchSearchCondition(const std::map<std::string, std::string>& logEntry);
    
    // 优化后的数据库查询方法
    std::vector<SearchResult> searchAllTablesWithCondition();
    std::vector<SearchResult> searchTableWithCondition(const std::string& tableName);

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
    
    // 全库搜索相关成员
    std::vector<SearchResult> m_searchResults;      // 所有搜索结果
    int m_searchResultCurrentPage;                   // 当前显示的搜索结果页
    static const int SEARCH_RESULTS_PER_PAGE = 100;  // 每页显示的结果数
    bool m_isInSearchMode;                           // 是否处于搜索模式
};

#endif // HGLOGWIDGET_H
