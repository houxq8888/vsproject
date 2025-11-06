#ifndef HGSOFTWAREINFOWIDGET_H
#define HGSOFTWAREINFOWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QRadioButton>
#include <QGroupBox>
#include <QMouseEvent>
#include "labelwithimg.h"
#include "mykeyboard.h"

class HGSoftwareInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGSoftwareInfoWidget(std::string,QWidget *parent = nullptr);
    ~HGSoftwareInfoWidget();
    bool closeWindow();
signals:

private slots:
    void clickEnable();
    void slotAuthorityDeadLine(QString str);
    
private:
    void fnInit();
    void fnWriteDB();
    void fnReadDB();
    bool eventFilter(QObject* obj,QEvent* event);

    

private:
    QLabel* m_productIDQLabel;
    QLabel* m_productKeyQLabel;
    QLineEdit* m_productIDEdit;
    QLineEdit* m_productKeyEdit;
    QPushButton* m_enableBtn;
    QGridLayout* m_layout, *m_widgetLayout;
    QGroupBox* m_groupBox;
    LabelWithImg* m_enableStatusLabel;
    bool m_isEnable;

    KBTimeEdit * m_authorityDeadLineCtrl;
    int m_loginAuthority;
    std::string m_lang;
};

#endif // HGSOFTWAREINFOWIDGET_H
