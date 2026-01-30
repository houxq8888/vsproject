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
#include <QMenu>
#include <QAction>

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
    void slotSearchResultPageChanged();
    void slotHeaderClicked(int column);
    void slotHeaderCustomContextMenuRequested(const QPoint& pos);
    void slotFilterTriggered();
    void slotClearFilter();

private:
    void fnReadDB(const std::string &tableName);
    int getTableNameIndex(const std::string& dbName);
    void searchAllLogs();
    void displaySearchResults(int pageIndex);
    void highlightKeyword(QTableWidgetItem* item, const std::string& keyword);
    std::string highlightText(const std::string& text, const std::string& keyword);

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

    bool m_isSearching;
    std::vector<std::map<std::string,std::string>> m_searchResults;
    int m_searchResultPageIndex;
    int m_searchResultPageSize;
    int m_searchResultTotalPages;

    HGQLabel* m_searchResultNextLabel, *m_searchResultPreLabel;
    QLabel* m_searchResultPageLabel;

    int m_currentSortColumn;
    Qt::SortOrder m_currentSortOrder;

    int m_filterColumn;
    std::string m_filterValue;
    std::vector<std::map<std::string,std::string>> m_originalSearchResults;
};

#endif // HGLOGWIDGET_H
