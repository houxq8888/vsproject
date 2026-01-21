#include "hgtaskmanagewidget.h"

HGTaskManageWidget::HGTaskManageWidget(std::string lang,const std::string& taskSeqName,QWidget *parent) : QWidget(parent),
m_lang(lang),
m_taskSeqName(taskSeqName)
{
    RWDb::writeAuditTrailLog(loadTranslation(m_lang,"Enter")+loadTranslation(m_lang,"TaskSequence"));
    // updateFlowInfo();

    m_taskEditW=NULL;
    m_taskEditW=new HGTaskEditWidget(m_lang,m_taskSeqName);
    m_taskInfoW=NULL;
    m_taskInfoW=new HGTaskInfoWidget(m_lang);
    qRegisterMetaType<Task>("Task");
    connect(m_taskInfoW,SIGNAL(updateTaskInfo(int,Task)),this,SLOT(slotDisplayTaskInfo(int,Task)));
    connect(m_taskEditW,SIGNAL(taskInfoWShow(Task)),this,SLOT(slotTaskInfoShow(Task)));

    m_vline=new QFrame;
    m_vline->setFrameShape(QFrame::VLine);
    m_vline->setFrameShadow(QFrame::Sunken);

    m_layout=new QGridLayout();
    this->setLayout(m_layout);

    m_layout->addWidget(m_taskEditW,0,0,1,3);
    m_layout->addWidget(m_vline,0,3,1,1);
    m_layout->addWidget(m_taskInfoW,0,4,1,1);
    m_layout->setColumnStretch(0,8);
    m_layout->setColumnStretch(1,1);
    m_layout->setColumnStretch(4,1);
}

bool HGTaskManageWidget::closeWindow()
{
    if (m_taskEditW){
        if (m_taskEditW->closeWindow()){
            SAFE_DELETE(m_taskEditW);
        }
    }
    if (m_taskInfoW){
        if (m_taskInfoW->closeWindow()){
            SAFE_DELETE(m_taskInfoW);
        }
    }
    return true;
}
HGTaskManageWidget::~HGTaskManageWidget()
{
    
}
void HGTaskManageWidget::slotDisplayTaskInfo(int type,Task task){
    m_taskEditW->displayTaskInfo(type,task);
}
void HGTaskManageWidget::slotTaskInfoShow(Task task){
    m_taskInfoW->displayTaskInfo(task);
}