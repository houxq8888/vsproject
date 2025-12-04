#ifndef HGDISPLAYRECORDWIDGET_H
#define HGDISPLAYRECORDWIDGET_H

#include <QWidget>
#include "hgqlabel.h"
#include <QSplitter>
#include <QPushButton>
#include <QTableWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QLineEdit>
#include "hginputsearchconditionwidget.h"
#include <QTimer>

class HGDisplayRecordWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGDisplayRecordWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();
    ~HGDisplayRecordWidget();
    void showDataInfo(const std::map<std::string,std::string> &info);

signals:
    void signalShowDataInfo(std::map<std::string,std::string> info);
    void showPrinter();

private slots:
    void onCellClicked(int,int);
    void onTimerTimeout();
    void slotMultipleChoose();
    void slotScan();
    void slotStatistics();
    void slotPrint();

private:
    void fnDisplayDataInfo(int count,std::map<std::string,std::string> fillContent);

private:
    HGQLabel *m_deleteLabel,*m_saveLabel, *m_exportLabel;
    QPushButton* m_multipleChooseBtn, *m_displayBtn, *m_staticBtn, *m_printBtn;
    QGroupBox *m_manipulateGroup;
    QGridLayout* m_layout, *m_manipulateLayout;

    HGInputSearchConditionWidget* m_inputsearchConditionW;
    QTableWidget* m_tableW;

    std::vector<std::map<std::string,std::string>> m_taskInfos;
    QTimer* m_timer;
    std::string m_lang;

};

#endif // HGDISPLAYRECORDWIDGET_H
