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

class HGLogWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGLogWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();
    ~HGLogWidget();

signals:
    void searchCompleted(int totalCount);

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
    void slotSearchFinished();
    void slotSortOrderChanged(int);

private:
    void fnReadDB(const std::string &tableName);
    int getTableNameIndex(const std::string& dbName);
    void fnSearchAllLogs();
    void fnDisplaySearchResults(int offset);
    void highlightKeyword(QTableWidgetItem* item, const QString& keyword);
    void updatePageLabel();

private:
    QLabel* m_pageLabel;
    QLabel* m_timeCostLabel;
    HGQLabel *m_saveLabel, *m_exportLabel;
    HGQLabel* m_nextLabel, *m_preLabel;
    QGroupBox *m_manipulateGroup;
    QGridLayout *m_manipulateLayout, *m_layout;
    QLabel* m_logTypeLabel;
    QLabel* m_sortLabel;
    QComboBox* m_logTypeComboBox;
    QComboBox* m_sortComboBox;
    HGInputSearchConditionWidget* m_inputsearchConditionW;
    QTableWidget* m_tableW;
    std::string m_lang;

    SearchCondition m_searchCondition;
    
    std::map<std::string, int> m_logContentMap;
    int m_curDisplayIndex;
    std::vector<std::string> m_auditLogTableNames;
    
    bool m_isSearchMode;
    int m_searchTotalCount;
    int m_searchCurrentPage;
    int m_pageSize;
    bool m_sortDescending;
    HGExactTime m_logMinTime;
    HGExactTime m_logMaxTime;
};

#endif // HGLOGWIDGET_H
