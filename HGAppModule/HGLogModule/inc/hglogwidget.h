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
    std::string operato;
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
    
    // 搜索相关函数
    void performGlobalSearch();
    void displaySearchResultsPage(int pageIndex);
    void highlightKeyword(QTableWidgetItem* item, const std::string& keyword);
    bool matchesSearchCondition(const std::map<std::string,std::string>& logEntry);
    bool matchesSearchCondition(const LogEntry& logEntry);
    
    // 性能优化：清除搜索缓存
    void clearSearchCache();

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
    bool m_isSearchMode;
    std::vector<LogEntry> m_searchResults;
    int m_searchResultsPageSize;
    int m_searchResultsTotalPages;
    int m_currentSearchPage;
};

#endif // HGLOGWIDGET_H
