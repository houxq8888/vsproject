#ifndef HGERRORCODEWIDGET_H
#define HGERRORCODEWIDGET_H

#include <QWidget>
#include "hgqlabel.h"
#include <QSplitter>
#include <QPushButton>
#include <QTableWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QLineEdit>
#include "hginputsearchconditionwidget.h"
#include "HGOnlinePlatformInterface.h"
#include "common.h"

class HGErrorCodeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGErrorCodeWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();
    ~HGErrorCodeWidget();
    

signals:
    void signalJumpToExceptionPage();
private slots:
    void slotSaveSearchWarn();
    void slotKeyWord(QString);
    void slotTimeFrom(QString);
    void slotTimeTo(QString);
    void slotSearch();
    void onCellClicked(int,int);
    void slotDeleteSearchWarn();

private:
    void fnReadDB();

private:
    HGQLabel *m_deleteLabel,*m_saveLabel, *m_exportLabel;
    QPushButton *m_displayBtn,*m_helpBtn;
    QGroupBox *m_manipulateGroup, *m_leftGroup,*m_rightGroup;
    QGridLayout* m_leftlayout, *m_manipulateLayout, *m_layout,*m_rightlayout;

    HGInputSearchConditionWidget* m_inputsearchConditionW;
    QTableWidget* m_tableW;

    QLabel* m_infoLabel;
    QSplitter *m_horsplitter;
    std::string m_lang;


    SearchCondition m_searchCondition;
};

#endif // HGERRORCODEWIDGET_H
