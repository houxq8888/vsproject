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
#include <QString>
#include "hginputsearchconditionwidget.h"

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
    void slotSearchPageNext();
    void slotSearchPagePre();
    void slotToggleSortOrder();

private:
    void fnReadDB(const std::string &tableName);
    void fnDisplaySearchResults();
    int getTableNameIndex(const std::string& dbName);
    QString highlightKeywordHtml(const QString& text, const QString& keyword);

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
    bool m_isSearching;
    bool m_sortAscending;
    int m_searchTotalCount;
    int m_searchPageIndex;
    int m_searchPageSize;
    
    std::map<std::string, int> m_logContentMap;
    int m_curDisplayIndex;
    std::vector<std::string> m_auditLogTableNames;
    HGQLabel* m_sortLabel;
};

#endif // HGLOGWIDGET_H
