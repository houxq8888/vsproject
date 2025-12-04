#ifndef HGTASKEDITWIDGET_H
#define HGTASKEDITWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QTableWidget>
#include "labelwithimg.h"
#include "hgqlabel.h"
#include <QGridLayout>
#include <QTimer>
#include <QListWidget>
#include "common.h"

class HGTaskEditWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGTaskEditWidget(std::string,const std::string& taskSeqName,QWidget *parent = nullptr);
    bool closeWindow();
    void displayTaskInfo(int,Task);

signals:
    void taskInfoWShow(Task);

private slots:
    void slotOpenSequence();
    void slotSaveSequence();
    void slotDeleteTask();
    void slotStartSequence();
    void slotPauseSequence();
    void onCellClicked(int,int);
    void slotUpdateRunStatus();
    void slotUpTask();
    void slotDownTask();

private:
    void fnDisplayTaskInfo(int,std::map<std::string,std::string>);
    void fnWriteDB();
    void fnReadDB();

private:
    QGroupBox* m_editGroupBox;
    QGridLayout* m_editlayout, *m_layout;
    HGQLabel* m_openLabel, *m_saveLabel, *m_deleteLabel, *m_startLabel,*m_pauseLabel;
    QTableWidget* m_tableW;
    HGQLabel* m_downLabel, *m_upLabel;

    QLabel* m_attentionContentLabel;

    std::vector<std::map<std::string,std::string>> m_fillContent;
    int m_index;

    bool m_startFlag,m_restartFlag;
    bool m_deviceOnlineFlag;

    QTimer* m_timer;
    std::string m_lang;
    std::string m_taskSeqName;
};

#endif // HGTASKEDITWIDGET_H
