#ifndef HGSIGNALDJWIDGET_H
#define HGSIGNALDJWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include "labelwithimg.h"
#include <QTableWidget>

class HGSignalDJWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGSignalDJWidget(std::string,std::string,QWidget *parent = nullptr);

    std::string getName() {return m_name;}
    void setParamStr(std::string);
    std::string getParamStr();
    bool closeWindow();

signals:

private:
    QLabel* m_nameLabel;
    QGroupBox* m_baseInfoGroup, *m_advancedInfoGroup;
    QGridLayout* m_baseInfoLayout, *m_advancedInfoLayout, *m_layout;

    QLabel* m_typeLabel, *m_serialLabel,*m_workCurrentLabel;
    QLineEdit* m_typeEdit, *m_serialEdit,*m_workCurrentEdit;

    std::string m_name;
    std::string m_lang;
};

#endif // HGSIGNALDJWIDGET_H
