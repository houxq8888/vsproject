#ifndef HGFLOWEDITWIDGET_H
#define HGFLOWEDITWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QTableWidget>
#include "labelwithimg.h"
#include "hgqlabel.h"
#include <QGridLayout>
#include <QComboBox>
#include "common.h"


class HGFlowEditWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGFlowEditWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();
    bool isSelectedRow();
    void saveSteps(std::vector<StepOfFlow>);
    
signals:
    void saveSignal();
    void flowInfoWShow(FlowOfTask);

private slots:
    // void slotOpenFlow();
    void slotSaveFlow();
    void slotDeleteFlow();
    void slotAddFlow();
    void slotEditFlow();
    void slotOkFlow();
    void onCellClicked(int row,int column);

private:
    void fnDisplayFlowInfo(int count,std::map<std::string,std::string> fillContent);
    std::string getIndexTableName(std::string tableName,int &index);

private:
    QGroupBox* m_editGroupBox;
    QGridLayout* m_editlayout, *m_layout;
    HGQLabel/** m_openLabel,*/ *m_saveLabel, *m_deleteLabel, *m_addLabel,*m_editLabel,*m_okLabel;
    QTableWidget* m_tableW;

    int m_index;
    int m_curCol,m_curRow;
    std::vector<std::map<std::string,std::string>> m_fillContent;
    FlowOfTask m_flow;
    std::string m_curSaveDBName;
    bool m_coverFlag;
    std::string m_lang;
};

#endif // HGFLOWEDITWIDGET_H
