#ifndef HGMANIPULATEWIDGET_H
#define HGMANIPULATEWIDGET_H

#include <QWidget>
#include "hgqlabel.h"
#include <QHBoxLayout>


class HGManipulateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGManipulateWidget(int type,std::string,QWidget *parent = nullptr);
    bool closeWindow();

signals:
    void manipulateObject(std::string,int);

private slots:
    void slotDownObjectleftClicked();
    void slotUpObjectleftClicked();
    void slotCloseObjectleftClicked();

private:
    std::string m_name;
    HGQLabel* m_downLabel,*m_upLabel,*m_closeLabel;
    QHBoxLayout* m_layout;
    int m_type;
};

#endif // HGMANIPULATEWIDGET_H
