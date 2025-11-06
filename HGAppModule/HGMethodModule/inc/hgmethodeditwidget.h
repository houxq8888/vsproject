#ifndef HGMETHODEDITWIDGET_H
#define HGMETHODEDITWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QTableWidget>
#include "labelwithimg.h"
#include "hgqlabel.h"
#include <QGridLayout>
#include <QComboBox>
#include <QKeyEvent>
#include "common.h"

class HGMethodEditWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGMethodEditWidget(std::string,std::vector<std::string> methods,
        QWidget *parent = nullptr);
    bool closeWindow();
    void addMethodType(std::vector<std::string>,int index=-1);
    void saveParam(std::string param);

signals:
    void createModule(std::string);
    void getMethodLists();
    void methodInfoWShow(std::string,std::string);
    void saveSignal();

private slots:
    void slotSaveMethod();
    void slotDeleteMethod();
    void slotAddMethod();
    void slotEditMethod();
    void slotOkMethod();
    void onCellClicked(int row,int column);
    void slotCellChanged(int,int);


private:
    void fnDisplayMethodInfo(int count,std::map<std::string,std::string> fillContent);
    std::string getIndexTableName(std::string tableName,int &index);

private:
    QGroupBox* m_editGroupBox;
    QGridLayout* m_editlayout, *m_layout;
    HGQLabel/** m_openLabel, */*m_saveLabel, *m_deleteLabel, *m_addLabel,*m_editLabel,*m_okLabel;
    QTableWidget* m_tableW;
    QComboBox* m_typeCombo;

    Method m_method;
    std::vector<std::map<std::string,std::string>> m_fillContent;
    int m_index,m_curCol,m_curRow;

    std::vector<std::string> m_methodlists;
    std::string m_curSaveDBName;
    bool m_coverFlag;

    QKeyEvent* m_enterEvent;
    std::string m_lang;
};

#endif // HGMETHODEDITWIDGET_H
