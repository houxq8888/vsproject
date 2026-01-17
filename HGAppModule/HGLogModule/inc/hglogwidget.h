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
#include <QStyledItemDelegate>
#include <QPainter>
#include <QTextDocument>
#include "hginputsearchconditionwidget.h"

class HtmlDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit HtmlDelegate(QObject* parent = nullptr) : QStyledItemDelegate(parent) {}
    
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override
    {
        QStyleOptionViewItem opt = option;
        initStyleOption(&opt, index);
        
        QString text = index.data(Qt::DisplayRole).toString();
        if (text.contains("<span") || text.contains("<font")) {
            QTextDocument doc;
            doc.setHtml(text);
            doc.setTextWidth(opt.rect.width());
            
            painter->save();
            painter->translate(opt.rect.topLeft());
            doc.drawContents(painter);
            painter->restore();
        } else {
            QStyledItemDelegate::paint(painter, opt, index);
        }
    }
    
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override
    {
        QString text = index.data(Qt::DisplayRole).toString();
        if (text.contains("<span") || text.contains("<font")) {
            QTextDocument doc;
            doc.setHtml(text);
            doc.setTextWidth(option.rect.width());
            return QSize(doc.idealWidth(), doc.size().height());
        }
        return QStyledItemDelegate::sizeHint(option, index);
    }
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
    void displaySearchResults();
    void highlightKeyword(QTableWidgetItem* item, const QString& keyword);

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
    int m_searchPageCount;
    int m_currentSearchPage;
    int m_totalSearchResults;
    std::vector<std::map<std::string,std::string>> m_searchResults;
    static const int PAGE_SIZE = 1000;
};

#endif // HGLOGWIDGET_H
