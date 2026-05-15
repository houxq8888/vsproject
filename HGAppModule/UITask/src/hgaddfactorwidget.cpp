#include "hgaddfactorwidget.h"
#include <sstream>
#include "common.h"

HGAddFactorWidget::HGAddFactorWidget(std::string lang,int index,QWidget *parent) : QWidget(parent),
m_lang(lang)
{   
    m_index=index;
    m_layout=new QGridLayout();
    this->setLayout(m_layout);

    m_factorLabel=new QLabel("目标成分"+QString::number(m_index));
    m_factorEdit=new QLineEdit();
    m_contentLabel=new QLabel("样品总量"+QString::number(m_index));
    m_contentEdit=new QLineEdit();
    m_unitLabel=new QLabel("单位"+QString::number(m_index));
    m_unitCombo=new QComboBox();
    m_unitCombo->addItems({"mg","g","ul","ml"});
    m_requireLabel=new QLabel("指标要求"+QString::number(m_index));
    m_andLabel=new QLabel("and");
    m_conditionCombo=new QComboBox();
    m_conditionCombo->addItems({"<",">","==",">=","<="});
    m_condition2Combo=new QComboBox();
    m_condition2Combo->addItems({"<",">","==",">=","<="});
    m_conditionEdit=new QLineEdit();
    m_condition2Edit=new QLineEdit();

    m_layout->addWidget(m_factorLabel,0,0);
    m_layout->addWidget(m_factorEdit,0,1);
    m_layout->addWidget(m_contentLabel,1,0);
    m_layout->addWidget(m_contentEdit,1,1);
    m_layout->addWidget(m_unitLabel,2,0);
    m_layout->addWidget(m_unitCombo,2,1);
    m_layout->addWidget(m_requireLabel,3,0);
    m_layout->addWidget(m_conditionCombo,4,1);
    m_layout->addWidget(m_conditionEdit,4,2);
    m_layout->addWidget(m_andLabel,5,0);
    m_layout->addWidget(m_condition2Combo,5,1);
    m_layout->addWidget(m_condition2Edit,5,2);

    fnSetReadOnly(true);
}
bool HGAddFactorWidget::closeWindow(){
    return true;
}
void HGAddFactorWidget::fnSetReadOnly(bool flag){
    m_factorEdit->setReadOnly(flag);
    m_contentEdit->setReadOnly(flag);
    m_unitCombo->setEnabled(!flag);
    m_conditionEdit->setReadOnly(flag);
    m_condition2Edit->setReadOnly(flag);
    m_conditionCombo->setEnabled(!flag);
    m_condition2Combo->setEnabled(!flag);
    if (flag){
        m_unitCombo->setFocusPolicy(Qt::NoFocus);
        m_conditionCombo->setFocusPolicy(Qt::NoFocus);
        m_condition2Combo->setFocusPolicy(Qt::NoFocus);
    } else {
        m_unitCombo->setFocusPolicy(Qt::StrongFocus);
        m_conditionCombo->setFocusPolicy(Qt::StrongFocus);
        m_condition2Combo->setFocusPolicy(Qt::StrongFocus);
    }
}
void HGAddFactorWidget::fnSetTaskData(Task info){
    #ifdef ENABLE_CAS6
    #else
    m_factorEdit->setText(QString::fromStdString(info.targetElement));
    m_contentEdit->setText(QString::number(info.content,'f',2));
    m_unitCombo->setCurrentText(QString::fromStdString(info.unit));
    m_conditionEdit->setText(QString::number(info.standard1,'f',2));
    m_condition2Edit->setText(QString::number(info.standard2,'f',2));
    m_conditionCombo->setCurrentText(QString::fromStdString(info.standard1Condition));
    m_condition2Combo->setCurrentText(QString::fromStdString(info.standard2Condition));
    #endif
}
void HGAddFactorWidget::fnUpdateTaskInfo(Task &info){
    #ifdef ENABLE_CAS6
    #else
    info.targetElement=m_factorEdit->text().toStdString();
    info.content=m_contentEdit->text().toDouble();
    info.unit=m_unitCombo->currentText().toStdString();
    info.standard1Condition=m_conditionCombo->currentText().toStdString();
    info.standard1=m_conditionEdit->text().toDouble();
    info.standard2Condition=m_condition2Combo->currentText().toStdString();
    info.standard2=m_condition2Edit->text().toDouble();
    #endif
}