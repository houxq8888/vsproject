#include "hgmanipulatewidget.h"
#include "common.h"


HGManipulateWidget::HGManipulateWidget(int type,std::string name,QWidget *parent) : QWidget(parent)
{
    m_type=type;
    m_name=name;
    m_downLabel=new HGQLabel(false,getPath("/resources/V1/@1xiconPark-arrow-circle-down 1.png"));
    m_upLabel=new HGQLabel(false,getPath("/resources/V1/@1xiconPark-arrow-circle-left 1.png"));
    m_closeLabel=new HGQLabel(false,getPath("/resources/V1/@1xze-close 1.png"));
    connect(m_downLabel,SIGNAL(leftClicked()),this,SLOT(slotDownObjectleftClicked()));
    connect(m_upLabel,SIGNAL(leftClicked()),this,SLOT(slotUpObjectleftClicked()));
    connect(m_closeLabel,SIGNAL(leftClicked()),this,SLOT(slotCloseObjectleftClicked()));

    m_layout=new QHBoxLayout();
    this->setLayout(m_layout);

    switch (m_type){
        case MANIPULATE_ALL:{
            m_layout->addWidget(m_downLabel);
            m_layout->addWidget(m_upLabel);
            m_layout->addWidget(m_closeLabel);
            break;
        }
        case MANIPULATE_UP:{
            m_layout->addWidget(m_upLabel);
            break;
        }
        case MANIPULATE_DOWN:{
            m_layout->addWidget(m_downLabel);
            break;
        }
        case MANIPULATE_CLOSE:{
            m_layout->addWidget(m_closeLabel);
            break;
        }
    }
}
bool HGManipulateWidget::closeWindow(){
    return true;
}
void HGManipulateWidget::slotDownObjectleftClicked(){
    emit manipulateObject(m_name,MANIPULATE_DOWN);
}
void HGManipulateWidget::slotUpObjectleftClicked(){
    emit manipulateObject(m_name,MANIPULATE_UP);
}
void HGManipulateWidget::slotCloseObjectleftClicked(){
    emit manipulateObject(m_name,MANIPULATE_CLOSE);
}