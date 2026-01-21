#ifndef HGADDFACTORWIDGET_H
#define HGADDFACTORWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QTableWidget>
#include "labelwithimg.h"
#include "hgqlabel.h"
#include <QGridLayout>
#include <QTimer>
#include <QListWidget>
#include <QComboBox>
#include <QLineEdit>
#include "common.h"


class HGAddFactorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGAddFactorWidget(std::string,int index,QWidget *parent = nullptr);
    bool closeWindow();
    void fnSetReadOnly(bool flag);
    void fnSetTaskData(Task info);
    void fnUpdateTaskInfo(Task &);
signals:
    

private slots:
    
private:

private:
    QLabel* m_factorLabel,*m_andLabel, *m_contentLabel,*m_unitLabel,*m_requireLabel;
    QLineEdit* m_factorEdit,*m_contentEdit;
    QComboBox* m_unitCombo;
    QComboBox* m_conditionCombo, *m_condition2Combo;
    QLineEdit *m_conditionEdit, *m_condition2Edit;
    QGridLayout* m_layout;
    int m_index;
    std::string m_lang;
};

#endif // HGADDFACTORWIDGET_H
