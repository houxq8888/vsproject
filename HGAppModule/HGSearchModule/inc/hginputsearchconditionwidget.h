#ifndef HGINPUTSEARCHCONDITIONWIDGET_H
#define HGINPUTSEARCHCONDITIONWIDGET_H

#include <QWidget>
#include "hgqlabel.h"
#include <QSplitter>
#include <QPushButton>
#include <QTableWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QLineEdit>
#include "mykeyboard.h"
#include "common.h"

typedef struct tagSearchCondition
{
    std::string key;
    std::string timeRangeFrom;
    std::string timeRangeTo;
    HGExactTime timeFrom;
    HGExactTime timeTo;
    void Clear(){
        key="";
        timeRangeFrom="";
        timeRangeTo="";
    }
    bool isInit(){
        if (key==""&&timeRangeFrom==""&&timeRangeTo=="")
            return true;
        else return false;
    }
}SearchCondition;

class HGInputSearchConditionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGInputSearchConditionWidget(const int& maxRange,std::string,QWidget *parent = nullptr);
    bool closeWindow();
    ~HGInputSearchConditionWidget();

signals:
    void signalTimeFrom(QString text);
    void signalTimeTo(QString text);
    void signalKeyWord(QString);
    void signalSearch();
    void signalClearSearch();

private slots:
    void slotTimeFrom(QString text);
    void slotTimeTo(QString text);
    void slotKeyWord(QString);
    void slotSearch();
    void slotClearSearch();

private:
    bool eventFilter(QObject* obj,QEvent* event);

private:
    QLabel* m_keyLabel, *m_timeRangeLabel, *m_timeRangeLabel1;
    QLineEdit* m_keyEdit, *m_timeRangeFromEdit, *m_timeRangeToEdit;
    HGQLabel* m_searchLabel, *m_clearSearchLabel;
    QGroupBox* m_searchGroup;
    QGridLayout* m_layout, *m_searchLayout;
    std::string m_lang;

    KBTimeEdit* m_timeEditFrom,*m_timeEditTo;
    KBCharEdit* m_keyEditCtrl;

    QLabel*m_markLabel1,*m_markLabel2;
    int m_maxRange;

};

#endif // HGINPUTSEARCHCONDITIONWIDGET_H
