#ifndef HGTASKMANAGEWIDGET_H
#define HGTASKMANAGEWIDGET_H

#include <QWidget>
#include "hgqlabel.h"
#include "hgtaskeditwidget.h"
#include "hgtaskinfowidget.h"
#include <QFrame>

class HGTaskManageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGTaskManageWidget(std::string,const std::string& taskSeqName,QWidget *parent = nullptr);
    bool closeWindow();
    ~HGTaskManageWidget();

signals:

private slots:
    void slotDisplayTaskInfo(int,Task);
    void slotTaskInfoShow(Task);

private:
    HGTaskEditWidget* m_taskEditW;
    HGTaskInfoWidget* m_taskInfoW;
    QGridLayout* m_layout;
    QFrame* m_vline;
    std::string m_lang;
    std::string m_taskSeqName;
};

#endif // HGTASKMANAGEWIDGET_H
