#include "hginputsearchconditionwidget.h"
#include <QHeaderView>
#include "common.h"
#include <QMessageBox>

HGInputSearchConditionWidget::HGInputSearchConditionWidget(const int& maxRange,std::string lang,QWidget *parent) : QWidget(parent),
    m_lang(lang),
    m_maxRange(maxRange)
{
    m_layout=new QGridLayout();
    this->setLayout(m_layout);

    m_markLabel1=new QLabel("*");
    m_markLabel2=new QLabel("*");
    m_markLabel1->setStyleSheet("color: red;");
    m_markLabel2->setStyleSheet("color: red;");

    m_searchGroup=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"Search")));//"查询");
    m_searchGroup->setStyleSheet("QGroupBox { font-size: 12pt; font-weight:bold;}");
    m_searchLayout=new QGridLayout();

    QFont font;
    font.setPointSize(10);
    m_keyLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"KeyWord")));//"关键词");
    m_keyLabel->setFont(font);
    m_keyEdit=new QLineEdit();
    m_keyEdit->setPlaceholderText(QString::fromStdString(loadTranslation(m_lang,"Input")));
    m_keyEdit->installEventFilter(this);

    m_timeRangeLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"TimeFrom")));//"时间范围 从");
    m_timeRangeLabel->setFont(font);
    m_timeRangeFromEdit=new QLineEdit();
    m_timeRangeFromEdit->setPlaceholderText(QString::fromStdString(loadTranslation(m_lang,"Input")));
    m_timeRangeFromEdit->installEventFilter(this);

    m_timeRangeLabel1=new QLabel(QString::fromStdString(loadTranslation(m_lang,"To")));//"至");
    m_timeRangeLabel1->setFont(font);
    m_timeRangeToEdit=new QLineEdit();
    m_timeRangeToEdit->setPlaceholderText(QString::fromStdString(loadTranslation(m_lang,"Input")));
    m_timeRangeToEdit->installEventFilter(this);

    m_searchLabel=new HGQLabel(false,getPath("/resources/V1/@1xmb-search 1.png"));
    connect(m_searchLabel,SIGNAL(leftClicked()),this,SLOT(slotSearch()));
    m_clearSearchLabel=new HGQLabel(false,getPath("/resources/V1/@1xarcoDesign-stop 1.png"));
    connect(m_clearSearchLabel,SIGNAL(leftClicked()),this,SLOT(slotClearSearch()));

    m_searchLayout->addWidget(m_keyLabel, 0,0);
    m_searchLayout->addWidget(m_keyEdit,0,1);
    
    m_searchLayout->addWidget(m_timeRangeLabel,1,0);
    m_searchLayout->addWidget(m_timeRangeFromEdit,1,1);
    m_searchLayout->addWidget(m_markLabel1,1,2);
    m_searchLayout->addWidget(m_timeRangeLabel1,2,0);
    m_searchLayout->addWidget(m_timeRangeToEdit,2,1);
    m_searchLayout->addWidget(m_markLabel2,2,2);
    m_searchLayout->addWidget(m_searchLabel,0,2);
    m_searchLayout->addWidget(m_clearSearchLabel,1,3);
    m_searchGroup->setLayout(m_searchLayout);


    m_layout->addWidget(m_searchGroup,0,1);
}
bool HGInputSearchConditionWidget::closeWindow()
{
    return true;
}
HGInputSearchConditionWidget::~HGInputSearchConditionWidget()
{
    
}
bool HGInputSearchConditionWidget::eventFilter(QObject* obj,QEvent* event){
    if (event->type()==QEvent::MouseButtonPress){
        QMouseEvent* mouseEvent=static_cast<QMouseEvent*>(event);
        if (mouseEvent->button()==Qt::LeftButton){
            if (obj==m_timeRangeFromEdit){
                m_timeEditFrom=new KBTimeEdit(false);
                int passPosX = m_timeRangeFromEdit->mapToGlobal(QPoint(0,0)).x();
                int passPosY = m_timeRangeFromEdit->mapToGlobal(QPoint(0,0)).y() + m_timeRangeFromEdit->height();
                // if (passPosX+m_authorityDeadLineCtrl->width()>m_width) passPosX=m_width-m_authorityDeadLineCtrl->width();
                // if (passPosY+m_authorityDeadLineCtrl->height()>m_height) passPosY=m_height-m_authorityDeadLineCtrl->height();
                m_timeEditFrom->move(passPosX,passPosY);
                m_timeEditFrom->show();
                connect(m_timeEditFrom,SIGNAL(MykeyboardSignal(QString)),
                        this,SLOT(slotTimeFrom(QString)));
            } else if (obj==m_timeRangeToEdit){
                m_timeEditTo=new KBTimeEdit(false);
                int passPosX = m_timeRangeToEdit->mapToGlobal(QPoint(0,0)).x();
                int passPosY = m_timeRangeToEdit->mapToGlobal(QPoint(0,0)).y() + m_timeRangeToEdit->height();
                // if (passPosX+m_authorityDeadLineCtrl->width()>m_width) passPosX=m_width-m_authorityDeadLineCtrl->width();
                // if (passPosY+m_authorityDeadLineCtrl->height()>m_height) passPosY=m_height-m_authorityDeadLineCtrl->height();
                m_timeEditTo->move(passPosX,passPosY);
                m_timeEditTo->show();
                connect(m_timeEditTo,SIGNAL(MykeyboardSignal(QString)),
                        this,SLOT(slotTimeTo(QString)));
            } else if (obj==m_keyEdit){
                m_keyEditCtrl=new KBCharEdit();
                int passPosX = m_keyEdit->mapToGlobal(QPoint(0,0)).x();
                int passPosY = m_keyEdit->mapToGlobal(QPoint(0,0)).y() + m_keyEdit->height();
                // if (passPosX+m_authorityDeadLineCtrl->width()>m_width) passPosX=m_width-m_authorityDeadLineCtrl->width();
                // if (passPosY+m_authorityDeadLineCtrl->height()>m_height) passPosY=m_height-m_authorityDeadLineCtrl->height();
                m_keyEditCtrl->move(passPosX,passPosY);
                m_keyEditCtrl->show();
                connect(m_keyEditCtrl,SIGNAL(MykeyboardSignal(QString)),
                        this,SLOT(slotKeyWord(QString)));
            }
        }
    }
    return QWidget::eventFilter(obj,event);
}
void HGInputSearchConditionWidget::slotTimeFrom(QString text){
    if (m_timeRangeToEdit->text()!=""){
        HGExactTime timeFrom=HGExactTime::currentTime();
        timeFrom.tm_year = atoi(text.toStdString().substr(0, 4).c_str());
        timeFrom.tm_mon = atoi(text.toStdString().substr(4, 2).c_str());
        timeFrom.tm_mday = atoi(text.toStdString().substr(6, 2).c_str());
        HGExactTime timeTo=HGExactTime::currentTime();
        timeTo.tm_year = atoi(m_timeRangeToEdit->text().toStdString().substr(0, 4).c_str());
        timeTo.tm_mon = atoi(m_timeRangeToEdit->text().toStdString().substr(4, 2).c_str());
        timeTo.tm_mday = atoi(m_timeRangeToEdit->text().toStdString().substr(6, 2).c_str());

        HGExactTime preTimeFrom=HGExactTime::currentTime();
        preTimeFrom.tm_year=timeFrom.tm_year;
        preTimeFrom.tm_mon=timeFrom.tm_mon;
        preTimeFrom.tm_mday=timeFrom.tm_mday;
        preTimeFrom += m_maxRange;

        if (timeFrom>=timeTo){
            QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),
                QString::fromStdString(loadTranslation(m_lang,"Theendtimeisearlierthanstarttime")));
            m_timeRangeFromEdit->clear();
            m_timeRangeFromEdit->setFocus();
            return;
        } else if (preTimeFrom<timeTo){
            QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),
                QString::fromStdString(loadTranslation(m_lang,"Theendtimeisearlierthanstarttimeoutofmaxrange"))+
                    QString::fromStdString(std::to_string(m_maxRange)));
            m_timeRangeFromEdit->clear();
            m_timeRangeFromEdit->setFocus();
            return;
        } else {

        }
    }
    m_timeRangeFromEdit->setText(text);
    emit signalTimeFrom(text);
}
void HGInputSearchConditionWidget::slotTimeTo(QString text){

    if (m_timeRangeFromEdit->text()!=""){
        HGExactTime timeFrom=HGExactTime::currentTime();
        timeFrom.tm_year = atoi(m_timeRangeFromEdit->text().toStdString().substr(0, 4).c_str());
        timeFrom.tm_mon = atoi(m_timeRangeFromEdit->text().toStdString().substr(4, 2).c_str());
        timeFrom.tm_mday = atoi(m_timeRangeFromEdit->text().toStdString().substr(6, 2).c_str());
        HGExactTime timeTo=HGExactTime::currentTime();
        timeTo.tm_year = atoi(text.toStdString().substr(0, 4).c_str());
        timeTo.tm_mon = atoi(text.toStdString().substr(4, 2).c_str());
        timeTo.tm_mday = atoi(text.toStdString().substr(6, 2).c_str());
        HGExactTime preTimeFrom=HGExactTime::currentTime();
        preTimeFrom.tm_year=timeFrom.tm_year;
        preTimeFrom.tm_mon=timeFrom.tm_mon;
        preTimeFrom.tm_mday=timeFrom.tm_mday;
        preTimeFrom += m_maxRange;

        if ((timeFrom>=timeTo)){
            QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),
                QString::fromStdString(loadTranslation(m_lang,"Theendtimeisearlierthanstarttime")));
            
            m_timeRangeToEdit->clear();
            m_timeRangeToEdit->setFocus();
            return;
        } else if (preTimeFrom<timeTo){
            QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),
                QString::fromStdString(loadTranslation(m_lang,"Theendtimeisearlierthanstarttimeoutofmaxrange"))+
                    QString::fromStdString(std::to_string(m_maxRange))+"天");
            m_timeRangeToEdit->clear();
            m_timeRangeToEdit->setFocus();
            return;
        } else {

        }
    }
    m_timeRangeToEdit->setText(text);
    emit signalTimeTo(text);
}
void HGInputSearchConditionWidget::slotKeyWord(QString text){
    m_keyEdit->setText(text);
    emit signalKeyWord(text);
}
void HGInputSearchConditionWidget::slotClearSearch(){ 
    m_timeRangeFromEdit->clear();
    m_timeRangeToEdit->clear();
    emit signalClearSearch();
}
void HGInputSearchConditionWidget::slotSearch(){
    emit signalKeyWord(m_keyEdit->text());
    if (m_timeRangeFromEdit->text()!="" && m_timeRangeToEdit->text()!=""){
        emit signalSearch();
    } else {
        QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),
            QString::fromStdString(loadTranslation(m_lang,"Pleaseinputalltheparameters")));
        if (m_timeRangeFromEdit->text()==""){
            m_timeRangeFromEdit->setFocus();
        } else if (m_timeRangeToEdit->text()==""){
            m_timeRangeToEdit->setFocus();
        }
    }
}