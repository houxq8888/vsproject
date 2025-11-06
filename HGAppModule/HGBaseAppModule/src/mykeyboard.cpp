/*
    @file   mykeyboard.cpp
    @brief  Implementation file
    @author XiaoQin.Hou
*/
#include "mykeyboard.h"
#include <QMessageBox>
#include <iostream>
#include <QDebug>
#include <iomanip>
#include "HGMacroData.h"
#include "common.h"
#include "libpinyinHelp.h"



KBTimeWithHourEdit::KBTimeWithHourEdit(bool flag,QWidget*parent):QWidget(parent)
{
    m_flag=flag;
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowModality(Qt::ApplicationModal);
    init(50,100);
}
KBTimeWithHourEdit::~KBTimeWithHourEdit()
{

}
void KBTimeWithHourEdit::init(int MyHeight,int MyWidth)
{
    this->resize(160, 150);
    gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(0);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setContentsMargins(0, 0, 0, 0);

    m_yearLabel = new QLabel("年");
    m_monthLabel=new QLabel("月");
    m_dayLabel=new QLabel("日");
    m_hourLabel=new QLabel("时");
    m_minuteLabel=new QLabel("分");
    m_secondLabel=new QLabel("秒");
    m_okBtn=new QPushButton("确定");
    m_cancelBtn=new QPushButton("取消");
    m_everRadioBtn=new QRadioButton("长期");

    m_yearEdit=new QComboBox();
    HGExactTime curTime=HGExactTime::currentTime();
    for (int i=0;i<100;i++){
        QString name=QString::number(curTime.tm_year+i);
        m_yearEdit->addItem(name);
    }
    m_monthEdit=new QComboBox();
    for (int i=0;i<12;i++){
        QString name=QString::number(i+1);
        m_monthEdit->addItem(name);
    }
    m_monthEdit->setCurrentIndex(curTime.tm_mon-1);
    m_dayEdit=new QComboBox();
    fillDayCombo(curTime.tm_mon);
    m_dayEdit->setCurrentIndex(curTime.tm_mday-1);

    m_hourEdit=new QComboBox();
    for (int i=0;i<24;i++){
        QString name=QString::number(i);
        m_hourEdit->addItem(name);
    }
    
    m_minuteEdit=new QComboBox();
    for (int i=0;i<60;i++){
        QString name=QString::number(i);
        m_minuteEdit->addItem(name);
    }

    m_secondEdit=new QComboBox();
    for (int i=0;i<60;i++){
        QString name=QString::number(i);
        m_secondEdit->addItem(name);
    }
    gridLayout->addWidget(m_yearLabel,0,0);
    gridLayout->addWidget(m_yearEdit,0,1);
    gridLayout->addWidget(m_monthLabel,0,2);
    gridLayout->addWidget(m_monthEdit,0,3);
    gridLayout->addWidget(m_dayLabel,0,4);
    gridLayout->addWidget(m_dayEdit,0,5);
    gridLayout->addWidget(m_hourLabel,1,0);
    gridLayout->addWidget(m_hourEdit,1,1);
    gridLayout->addWidget(m_minuteLabel,1,2);
    gridLayout->addWidget(m_minuteEdit,1,3);
    gridLayout->addWidget(m_secondLabel,1,4);
    gridLayout->addWidget(m_secondEdit,1,5);

    if (m_flag) gridLayout->addWidget(m_everRadioBtn,2,0);
    gridLayout->addWidget(m_okBtn,2,1);
    gridLayout->addWidget(m_cancelBtn,2,5);

    connect(m_everRadioBtn,&QRadioButton::clicked,[=](){
        if (m_everRadioBtn->isChecked()){

        }
    });
    connect(m_yearEdit,&QComboBox::currentTextChanged,[=](QString str){
        int month=m_monthEdit->currentText().toInt();
        fillDayCombo(month);
    });
    connect(m_monthEdit, &QComboBox::currentTextChanged, [=](QString str)
    {
        int month=str.toInt();
        fillDayCombo(month);
    });
    connect(m_okBtn,&QPushButton::clicked,[=](){
        if (m_everRadioBtn->isChecked()){
            str = m_everRadioBtn->text();
            emit MykeyboardSignal(str.toStdString());
            this->close();
        }
        else
        {
            std::stringstream ss;
            ss << m_yearEdit->currentText().toInt()
                   << std::setw(2) << std::setfill('0') << m_monthEdit->currentText().toInt()
                   << std::setw(2) << std::setfill('0') << m_dayEdit->currentText().toInt()
                   << std::setw(2) << std::setfill('0') << m_hourEdit->currentText().toInt()
                   << std::setw(2) << std::setfill('0') << m_minuteEdit->currentText().toInt()
                   << std::setw(2) << std::setfill('0') << m_secondEdit->currentText().toInt();
            emit MykeyboardSignal(ss.str());
            this->close();
        }
    });
    connect(m_cancelBtn,&QPushButton::clicked,[=](){this->close();});
   
}
void KBTimeWithHourEdit::setValue(const std::string& str){
    if (str=="长期") m_everRadioBtn->setChecked(true);
    else {
        if (str.length() >= 4)
        {
            std::string year = (str.substr(0, 4).c_str());
            m_yearEdit->setCurrentText(QString::fromStdString(year));
        }
        if (str.length() >= 6)
        {
            std::string mon = (str.substr(4, 2).c_str());
            m_monthEdit->setCurrentText(QString::fromStdString(mon));
        }
        if (str.length() >= 8)
        {
            std::string mday = (str.substr(6, 2).c_str());
            m_dayEdit->setCurrentText(QString::fromStdString(mday));
        }
    }
}
void KBTimeWithHourEdit::fillDayCombo(int month)
{
    m_dayEdit->clear();
    if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
    {
        for (int i = 0; i < 31; i++)
        {
            QString name = QString::number(i + 1);
            m_dayEdit->addItem(name);
        }
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        for (int i = 0; i < 30; i++)
        {
            QString name = QString::number(i + 1);
            m_dayEdit->addItem(name);
        }
    }
    else if (month == 2)
    {
        int year=m_yearEdit->currentText().toInt();
        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
        {
            for (int i = 0; i < 29; i++)
            {
                QString name = QString::number(i + 1);
                m_dayEdit->addItem(name);
            }
        }
        else
        {
            for (int i = 0;i < 28; i++){
                QString name = QString::number(i + 1);
                m_dayEdit->addItem(name);
            }
        }
    }
}

KBTimeEdit::KBTimeEdit(bool flag,QWidget*parent):QWidget(parent)
{
    m_flag=flag;
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowModality(Qt::ApplicationModal);
    init(50,100);
}
KBTimeEdit::~KBTimeEdit()
{

}
void KBTimeEdit::init(int MyHeight,int MyWidth)
{
    this->resize(160, 150);
    gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(0);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setContentsMargins(0, 0, 0, 0);

    m_yearLabel = new QLabel("年");
    m_monthLabel=new QLabel("月");
    m_dayLabel=new QLabel("日");
    m_okBtn=new QPushButton("确定");
    m_cancelBtn=new QPushButton("取消");
    m_everRadioBtn=new QRadioButton("长期");

    m_yearEdit=new QComboBox();
    HGExactTime curTime=HGExactTime::currentTime();
    for (int i=0;i<100;i++){
        QString name=QString::number(curTime.tm_year+i);
        m_yearEdit->addItem(name);
    }
    m_monthEdit=new QComboBox();
    for (int i=0;i<12;i++){
        QString name=QString::number(i+1);
        m_monthEdit->addItem(name);
    }
    m_monthEdit->setCurrentIndex(curTime.tm_mon-1);
    m_dayEdit=new QComboBox();
    fillDayCombo(curTime.tm_mon);
    m_dayEdit->setCurrentIndex(curTime.tm_mday-1);

    gridLayout->addWidget(m_yearLabel,0,0);
    gridLayout->addWidget(m_yearEdit,0,1);
    gridLayout->addWidget(m_monthLabel,0,2);
    gridLayout->addWidget(m_monthEdit,0,3);
    gridLayout->addWidget(m_dayLabel,0,4);
    gridLayout->addWidget(m_dayEdit,0,5);
    if (m_flag) gridLayout->addWidget(m_everRadioBtn,1,0);
    gridLayout->addWidget(m_okBtn,1,1);
    gridLayout->addWidget(m_cancelBtn,1,5);

    connect(m_everRadioBtn,&QRadioButton::clicked,[=](){
        if (m_everRadioBtn->isChecked()){

        }
    });
    connect(m_yearEdit,&QComboBox::currentTextChanged,[=](QString str){
        int month=m_monthEdit->currentText().toInt();
        fillDayCombo(month);
    });
    connect(m_monthEdit, &QComboBox::currentTextChanged, [=](QString str)
    {
        int month=str.toInt();
        fillDayCombo(month);
    });
    connect(m_okBtn,&QPushButton::clicked,[=](){
        if (m_everRadioBtn->isChecked()){
            str = m_everRadioBtn->text();
            emit MykeyboardSignal(str);
            this->close();
        }
        else
        {
            HGExactTime curTimer = HGExactTime::currentTime();
            HGExactTime deadlineTimer = HGExactTime::currentTime();
            deadlineTimer.tm_year = m_yearEdit->currentText().toInt();
            deadlineTimer.tm_mon = m_monthEdit->currentText().toInt();
            deadlineTimer.tm_mday = m_dayEdit->currentText().toInt();
            if (m_flag && curTimer > deadlineTimer)
            {
                QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),
                                     "授权截止日期早于当前日期，请重新选择！");
            }
            else
            {
                std::stringstream ss;
                ss << m_yearEdit->currentText().toInt()
                   << std::setw(2) << std::setfill('0') << m_monthEdit->currentText().toInt()
                   << std::setw(2) << std::setfill('0') << m_dayEdit->currentText().toInt();
                str = QString::fromStdString(ss.str());
                emit MykeyboardSignal(str);
                this->close();
            }
        }
    });
    connect(m_cancelBtn,&QPushButton::clicked,[=](){this->close();});
   
}
void KBTimeEdit::setValue(const std::string& str){
    if (str=="长期") m_everRadioBtn->setChecked(true);
    else {
        if (str.length() >= 4)
        {
            std::string year = (str.substr(0, 4).c_str());
            m_yearEdit->setCurrentText(QString::fromStdString(year));
        }
        if (str.length() >= 6)
        {
            int mon = std::atoi(str.substr(4, 2).c_str());
            m_monthEdit->setCurrentIndex(mon-1);
        }
        if (str.length() >= 8)
        {
            int mday = std::atoi(str.substr(6, 2).c_str());
            m_dayEdit->setCurrentIndex(mday-1);
        }
    }
}

void KBTimeEdit::fillDayCombo(int month)
{
    m_dayEdit->clear();
    if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
    {
        for (int i = 0; i < 31; i++)
        {
            QString name = QString::number(i + 1);
            m_dayEdit->addItem(name);
        }
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        for (int i = 0; i < 30; i++)
        {
            QString name = QString::number(i + 1);
            m_dayEdit->addItem(name);
        }
    }
    else if (month == 2)
    {
        int year=m_yearEdit->currentText().toInt();
        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
        {
            for (int i = 0; i < 29; i++)
            {
                QString name = QString::number(i + 1);
                m_dayEdit->addItem(name);
            }
        }
        else
        {
            for (int i = 0;i < 28; i++){
                QString name = QString::number(i + 1);
                m_dayEdit->addItem(name);
            }
        }
    }
}
KBUIntEdit::KBUIntEdit(bool secretFlag, QWidget *parent) : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowModality(Qt::ApplicationModal);
    m_secretFlag = secretFlag;
    init(30,100);
}
KBUIntEdit::~KBUIntEdit()
{
    SAFE_DELETE(pushButton_2);
    SAFE_DELETE(pushButton_7);
    SAFE_DELETE(pushButton_9);
    SAFE_DELETE(pushButton_6);
    SAFE_DELETE(pushButton_8);
    SAFE_DELETE(pushButton_5);
    SAFE_DELETE(pushButton_1);
    SAFE_DELETE(pushButton_3);
    SAFE_DELETE(pushButton_4);
    SAFE_DELETE(pushButton_clear);
    SAFE_DELETE(pushButton_enter);
    SAFE_DELETE(pushButton_space);
    SAFE_DELETE(pushButton_esc);
    SAFE_DELETE(pushButton_0);
    SAFE_DELETE(lineEdit);
    SAFE_DELETE(widget);
}
void KBUIntEdit::init(int MyHeight,int MyWidth){
    this->resize(160, 150);
    gridLayout_2 = new QGridLayout(this);
    gridLayout_2->setSpacing(0);
    gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
    gridLayout_2->setContentsMargins(0, 0, 0, 0);
    lineEdit = new QLineEdit(this);
    lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
    lineEdit->setMinimumSize(QSize(0, MyHeight));
    if (m_secretFlag)
        lineEdit->setEchoMode(QLineEdit::Password);

    gridLayout_2->addWidget(lineEdit, 0, 0, 1, 1);

    widget = new QWidget(this);
    widget->setObjectName(QString::fromUtf8("widget"));
    gridLayout = new QGridLayout(widget);
    gridLayout->setSpacing(0);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setContentsMargins(0, 0, 0, 0);
    pushButton_2 = new QPushButton(widget);
    pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
    pushButton_2->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_2->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_2, 0, 1, 1, 1);

    pushButton_7 = new QPushButton(widget);
    pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));
    pushButton_7->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_7->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_7, 2, 0, 1, 1);

    pushButton_9 = new QPushButton(widget);
    pushButton_9->setObjectName(QString::fromUtf8("pushButton_9"));
    pushButton_9->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_9->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_9, 2, 2, 1, 1);

    pushButton_6 = new QPushButton(widget);
    pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
    pushButton_6->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_6->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_6, 1, 2, 1, 1);

    pushButton_8 = new QPushButton(widget);
    pushButton_8->setObjectName(QString::fromUtf8("pushButton_8"));
    pushButton_8->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_8->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_8, 2, 1, 1, 1);

    pushButton_clear = new QPushButton(widget);
    pushButton_clear->setObjectName(QString::fromUtf8("pushButton_clear"));
    pushButton_clear->setMinimumSize(QSize(MyWidth, MyHeight));
    pushButton_clear->setMaximumSize(QSize(MyWidth, MyHeight));

    gridLayout->addWidget(pushButton_clear, 1, 3, 1, 1);

    pushButton_5 = new QPushButton(widget);
    pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
    pushButton_5->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_5->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_5, 1, 1, 1, 1);

    pushButton_esc = new QPushButton(widget);
    pushButton_esc->setObjectName(QString::fromUtf8("pushButton_esc"));
    pushButton_esc->setMinimumSize(QSize(MyWidth, MyHeight));
    pushButton_esc->setMaximumSize(QSize(MyWidth, MyHeight));

    gridLayout->addWidget(pushButton_esc, 2, 3, 1, 1);

    pushButton_0 = new QPushButton(widget);
    pushButton_0->setObjectName(QString::fromUtf8("pushButton_0"));

    gridLayout->addWidget(pushButton_0, 3, 0, 1, 3);

    pushButton_enter = new QPushButton(widget);
    pushButton_enter->setObjectName(QString::fromUtf8("pushButton_enter"));
    pushButton_enter->setMinimumSize(QSize(MyWidth, MyHeight));

    gridLayout->addWidget(pushButton_enter, 3, 3, 1, 1);

    pushButton_1 = new QPushButton(widget);
    pushButton_1->setObjectName(QString::fromUtf8("pushButton_1"));
    pushButton_1->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_1->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_1, 0, 0, 1, 1);

    pushButton_3 = new QPushButton(widget);
    pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
    pushButton_3->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_3->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_3, 0, 2, 1, 1);

    pushButton_4 = new QPushButton(widget);
    pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
    pushButton_4->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_4->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_4, 1, 0, 1, 1);

    pushButton_space = new QPushButton(widget);
    pushButton_space->setObjectName(QString::fromUtf8("pushButton_space"));
    pushButton_space->setMinimumSize(QSize(MyWidth, MyHeight));
    pushButton_space->setMaximumSize(QSize(MyWidth, MyHeight));

    gridLayout->addWidget(pushButton_space, 0, 3, 1, 1);


    gridLayout_2->addWidget(widget, 1, 0, 1, 1);

    pushButton_space->setText("Backspace");
    pushButton_1->setText("1");
    pushButton_enter->setText("Enter");
    pushButton_2->setText("2");
    pushButton_3->setText("3");
    pushButton_4->setText("4");
    pushButton_9->setText("9");
    pushButton_clear->setText("Clear All");
    pushButton_0->setText("0");
    pushButton_7->setText("7");
    pushButton_5->setText("5");
    pushButton_6->setText("6");
    pushButton_esc->setText("Esc");
    pushButton_8->setText("8");

    connect(pushButton_0,&QPushButton::clicked,[=](){insertContent("0");});
    connect(pushButton_1,&QPushButton::clicked,[=](){insertContent("1");});
    connect(pushButton_2,&QPushButton::clicked,[=](){insertContent("2");});
    connect(pushButton_3,&QPushButton::clicked,[=](){insertContent("3");});
    connect(pushButton_4,&QPushButton::clicked,[=](){insertContent("4");});
    connect(pushButton_5,&QPushButton::clicked,[=](){insertContent("5");});
    connect(pushButton_6,&QPushButton::clicked,[=](){insertContent("6");});
    connect(pushButton_7,&QPushButton::clicked,[=](){insertContent("7");});
    connect(pushButton_8,&QPushButton::clicked,[=](){insertContent("8");});
    connect(pushButton_9,&QPushButton::clicked,[=](){insertContent("9");});
    connect(pushButton_space,&QPushButton::clicked,[=](){str.clear();str.append(lineEdit->text());if(str.size()>=1){int curpos=lineEdit->cursorPosition();str.remove(curpos-1,1);lineEdit->setText(str);}});
    connect(pushButton_esc,&QPushButton::clicked,[=](){delete this;});
    connect(pushButton_enter,&QPushButton::clicked,[=](){saveEditInfo();});
    connect(pushButton_clear,&QPushButton::clicked,[=](){str.clear();lineEdit->setText(str);});
}
void KBUIntEdit::insertContent(QString text)
{
    str.clear();
    if (m_isSelectedAllFlag) {
        int curpos=0;
        str.insert(curpos,text);
        lineEdit->setText(str);
        lineEdit->setCursorPosition(curpos+1);
        m_isSelectedAllFlag=false;
    }
    else {
        str.append(lineEdit->text());
        int curpos=lineEdit->cursorPosition();
        str.insert(curpos,text);
        lineEdit->setText(str);
        lineEdit->setCursorPosition(curpos+1);
    }
}
void KBUIntEdit::saveEditInfo()
{
    if(lineEdit->text()!=str){
        str.clear();
        str.append(lineEdit->text());
    }

    if (!m_secretFlag) {
        if (!IsValidNumber(str)) {
            QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),"仅支持数字，请重新输入");
            return;
        }

        double dValue = str.toDouble();
        if (dValue > m_dUpperLimit || dValue < m_dLowerLimit)
        {
            QString temp;
            temp = QString("请输入一个有效整数，介于[%1,%2]之间！").arg(QString::number(m_dLowerLimit), QString::number(m_dUpperLimit));
            QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),  temp);
            lineEdit->setFocus();
        }
        else
        {
            emit MykeyboardSignal(str);
            delete this;
        }
    }
    else {
        emit MykeyboardSignal(str);
        delete this;
    }
}
void KBUIntEdit::keyPressEvent(QKeyEvent *event){
    switch(event->key())
    {
    case Qt::Key_Return: // enter键
    case Qt::Key_Enter: {// 数字键盘的enter键
        saveEditInfo();
        break;
    }
    default:
        break;
    }
}
void KBUIntEdit::setValue(unsigned int value)
{
    lineEdit->setText(QString::number(value));
    lineEdit->setFocus();
    lineEdit->selectAll();
    m_isSelectedAllFlag=true;
}

KBIntEdit::KBIntEdit(QWidget *parent) : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
   // this->setAttribute(Qt::WA_ShowModal, true);
    this->setWindowModality(Qt::ApplicationModal);
    init(30,100);
}
KBIntEdit::~KBIntEdit()
{
    SAFE_DELETE(pushButton_space);
    SAFE_DELETE(pushButton_enter);
    SAFE_DELETE(pushButton_clear);
    SAFE_DELETE(pushButton_0);
    SAFE_DELETE(pushButton_1);
    SAFE_DELETE(pushButton_2);
    SAFE_DELETE(pushButton_3);
    SAFE_DELETE(pushButton_4);
    SAFE_DELETE(pushButton_9);
    SAFE_DELETE(pushButton_7);
    SAFE_DELETE(pushButton_5);
    SAFE_DELETE(pushButton_6);
    SAFE_DELETE(pushButton_esc);
    SAFE_DELETE(pushButton_add);
    SAFE_DELETE(pushButton_point);
    SAFE_DELETE(pushButton_8);
    SAFE_DELETE(lineEdit);
    SAFE_DELETE(widget);
}
void KBIntEdit::init(int MyHeight,int MyWidth){
    this->resize(160, 150);
    gridLayout_2 = new QGridLayout(this);
    gridLayout_2->setSpacing(0);
    gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
    gridLayout_2->setContentsMargins(0, 0, 0, 0);
    widget = new QWidget(this);
    widget->setObjectName(QString::fromUtf8("widget"));
    gridLayout = new QGridLayout(widget);
    gridLayout->setSpacing(0);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setContentsMargins(0, 0, 0, 0);
    pushButton_space = new QPushButton(widget);
    pushButton_space->setObjectName(QString::fromUtf8("pushButton_space"));
    pushButton_space->setMinimumSize(QSize(MyWidth, MyHeight));
    pushButton_space->setMaximumSize(QSize(MyWidth, MyHeight));

    gridLayout->addWidget(pushButton_space, 0, 3, 1, 1);

    pushButton_1 = new QPushButton(widget);
    pushButton_1->setObjectName(QString::fromUtf8("pushButton_1"));
    pushButton_1->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_1->setMaximumSize(QSize(MyHeight, MyHeight));
    gridLayout->addWidget(pushButton_1, 0, 0, 1, 1);

    pushButton_enter = new QPushButton(widget);
    pushButton_enter->setObjectName(QString::fromUtf8("pushButton_enter"));
    pushButton_enter->setMinimumSize(QSize(MyWidth, MyHeight));

    gridLayout->addWidget(pushButton_enter, 3, 3, 1, 1);

    pushButton_2 = new QPushButton(widget);
    pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
    pushButton_2->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_2->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_2, 0, 1, 1, 1);

    pushButton_3 = new QPushButton(widget);
    pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
    pushButton_3->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_3->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_3, 0, 2, 1, 1);

    pushButton_4 = new QPushButton(widget);
    pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
    pushButton_4->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_4->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_4, 1, 0, 1, 1);

    pushButton_9 = new QPushButton(widget);
    pushButton_9->setObjectName(QString::fromUtf8("pushButton_9"));
    pushButton_9->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_9->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_9, 2, 2, 1, 1);

    pushButton_clear = new QPushButton(widget);
    pushButton_clear->setObjectName(QString::fromUtf8("pushButton_clear"));
    pushButton_clear->setMinimumSize(QSize(MyWidth, MyHeight));
    pushButton_clear->setMaximumSize(QSize(MyWidth, MyHeight));

    gridLayout->addWidget(pushButton_clear, 1, 3, 1, 1);

    pushButton_0 = new QPushButton(widget);
    pushButton_0->setObjectName(QString::fromUtf8("pushButton_0"));
    pushButton_0->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_0->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_0, 3, 0, 1, 1);

    pushButton_7 = new QPushButton(widget);
    pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));
    pushButton_7->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_7->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_7, 2, 0, 1, 1);

    pushButton_5 = new QPushButton(widget);
    pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
    pushButton_5->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_5->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_5, 1, 1, 1, 1);

    pushButton_6 = new QPushButton(widget);
    pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
    pushButton_6->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_6->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_6, 1, 2, 1, 1);

    pushButton_esc = new QPushButton(widget);
    pushButton_esc->setObjectName(QString::fromUtf8("pushButton_esc"));
    pushButton_esc->setMinimumSize(QSize(MyWidth, MyHeight));
    pushButton_esc->setMaximumSize(QSize(MyWidth, MyHeight));

    gridLayout->addWidget(pushButton_esc, 2, 3, 1, 1);

    pushButton_add = new QPushButton(widget);
    pushButton_add->setObjectName(QString::fromUtf8("pushButton_add"));
    pushButton_add->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_add->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_add, 3, 1, 1, 1);

    pushButton_point = new QPushButton(widget);
    pushButton_point->setObjectName(QString::fromUtf8("pushButton_point"));
    pushButton_point->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_point->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_point, 3, 2, 1, 1);

    pushButton_8 = new QPushButton(widget);
    pushButton_8->setObjectName(QString::fromUtf8("pushButton_8"));
    pushButton_8->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_8->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_8, 2, 1, 1, 1);


    gridLayout_2->addWidget(widget, 1, 0, 1, 1);

    lineEdit = new QLineEdit(this);
    lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
    lineEdit->setMinimumSize(QSize(0, MyHeight));

    gridLayout_2->addWidget(lineEdit, 0, 0, 1, 1);

    pushButton_space->setText("Backspace");
    pushButton_1->setText("1");
    pushButton_enter->setText("Enter");
    pushButton_2->setText("2");
    pushButton_3->setText("3");
    pushButton_4->setText("4");
    pushButton_9->setText("9");
    pushButton_clear->setText("Clear All");
    pushButton_0->setText("0");
    pushButton_7->setText("7");
    pushButton_5->setText("5");
    pushButton_6->setText("6");
    pushButton_esc->setText("Esc");
    pushButton_add->setText("+/-");
    pushButton_point->setText(".");
    pushButton_8->setText("8");

    connect(pushButton_0,&QPushButton::clicked,[=](){insertContent("0");});
    connect(pushButton_1,&QPushButton::clicked,[=](){insertContent("1");});
    connect(pushButton_2,&QPushButton::clicked,[=](){insertContent("2");});
    connect(pushButton_3,&QPushButton::clicked,[=](){insertContent("3");});
    connect(pushButton_4,&QPushButton::clicked,[=](){insertContent("4");});
    connect(pushButton_5,&QPushButton::clicked,[=](){insertContent("5");});
    connect(pushButton_6,&QPushButton::clicked,[=](){insertContent("6");});
    connect(pushButton_7,&QPushButton::clicked,[=](){insertContent("7");});
    connect(pushButton_8,&QPushButton::clicked,[=](){insertContent("8");});
    connect(pushButton_9,&QPushButton::clicked,[=](){insertContent("9");});
    connect(pushButton_space,&QPushButton::clicked,[=](){str.clear();str.append(lineEdit->text());if(str.size()>=1){int curpos=lineEdit->cursorPosition();str.remove(curpos-1,1);lineEdit->setText(str);}});
    connect(pushButton_point,&QPushButton::clicked,[=](){insertContent(".");});

    connect(pushButton_esc,&QPushButton::clicked,[=](){delete this;});
    connect(pushButton_enter,&QPushButton::clicked,[=](){
        saveEditInfo();
    });

    connect(pushButton_clear,&QPushButton::clicked,[=](){str.clear();lineEdit->setText(str);});    connect(pushButton_add,&QPushButton::clicked,[=](){
        str.clear();
        str.append(lineEdit->text());
        if(str.size()<=0) return ;
        if(str.count("-")){
            str = str.mid(1,str.size()-1);
        }else{
            str = "-"+str;
        }
        lineEdit->setText(str);
    });
}
void KBIntEdit::insertContent(QString text)
{
    str.clear();
    if (m_isSelectedAllFlag) {
        int curpos=0;
        str.insert(curpos,text);
        lineEdit->setText(str);
        lineEdit->setCursorPosition(curpos+1);
        m_isSelectedAllFlag=false;
    }
    else {
        str.append(lineEdit->text());
        int curpos=lineEdit->cursorPosition();
        str.insert(curpos,text);
        lineEdit->setText(str);
        lineEdit->setCursorPosition(curpos+1);
    }
}
void KBIntEdit::saveEditInfo()
{
    if(lineEdit->text()!=str){
        str.clear();
        str.append(lineEdit->text());
    }

    if (!IsValidNumber(str)) {
        QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),"仅支持数字，请重新输入");
        return;
    }

    double dValue = str.toDouble();
    if (dValue > m_UpperLimit || dValue < m_LowerLimit)
    {
        QString temp;
        temp = QString("请输入一个有效整数，介于[%1,%2]之间!").arg(QString::number(m_LowerLimit), QString::number(m_UpperLimit));
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),  temp);
        lineEdit->setFocus();
    }
    else
    {
        emit MykeyboardSignal(str);
        delete this;
    }
}
void KBIntEdit::setValue(int value)
{
    lineEdit->setText(QString::number(value));
    lineEdit->setFocus();
    lineEdit->selectAll();
    m_isSelectedAllFlag=true;
}
void KBIntEdit::keyPressEvent(QKeyEvent *event){
    switch(event->key())
    {
    case Qt::Key_Return: // enter键
    case Qt::Key_Enter: {// 数字键盘的enter键
        saveEditInfo();
        break;
    }
    default:
        break;
    }
}

KBFLoatEdit::KBFLoatEdit(QWidget *parent) : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_ShowModal, true);
    init(30,100);
}
KBFLoatEdit::~KBFLoatEdit()
{
    SAFE_DELETE(pushButton_0);
    SAFE_DELETE(pushButton_1);
    SAFE_DELETE(pushButton_2);
    SAFE_DELETE(pushButton_3);
    SAFE_DELETE(pushButton_4);
    SAFE_DELETE(pushButton_5);
    SAFE_DELETE(pushButton_6);
    SAFE_DELETE(pushButton_7);
    SAFE_DELETE(pushButton_8);
    SAFE_DELETE(pushButton_9);
    SAFE_DELETE(pushButton_add);
    SAFE_DELETE(pushButton_space);
    SAFE_DELETE(pushButton_esc);
    SAFE_DELETE(pushButton_clear);
    SAFE_DELETE(pushButton_enter);
    SAFE_DELETE(pushButton_point);
    SAFE_DELETE(lineEdit);
    SAFE_DELETE(widget);
}
void KBFLoatEdit::init(int MyHeight,int MyWidth){
    this->resize(160, 150);
    gridLayout_2 = new QGridLayout(this);
    gridLayout_2->setSpacing(0);
    gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
    gridLayout_2->setContentsMargins(0, 0, 0, 0);
    widget = new QWidget(this);
    widget->setObjectName(QString::fromUtf8("widget"));
    gridLayout = new QGridLayout(widget);
    gridLayout->setSpacing(0);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setContentsMargins(0, 0, 0, 0);
    pushButton_space = new QPushButton(widget);
    pushButton_space->setObjectName(QString::fromUtf8("pushButton_space"));
    pushButton_space->setMinimumSize(QSize(MyWidth, MyHeight));
    pushButton_space->setMaximumSize(QSize(MyWidth, MyHeight));

    gridLayout->addWidget(pushButton_space, 0, 3, 1, 1);

    pushButton_1 = new QPushButton(widget);
    pushButton_1->setObjectName(QString::fromUtf8("pushButton_1"));
    pushButton_1->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_1->setMaximumSize(QSize(MyHeight, MyHeight));
    gridLayout->addWidget(pushButton_1, 0, 0, 1, 1);

    pushButton_enter = new QPushButton(widget);
    pushButton_enter->setObjectName(QString::fromUtf8("pushButton_enter"));
    pushButton_enter->setMinimumSize(QSize(MyWidth, MyHeight));

    gridLayout->addWidget(pushButton_enter, 3, 3, 1, 1);

    pushButton_2 = new QPushButton(widget);
    pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
    pushButton_2->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_2->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_2, 0, 1, 1, 1);

    pushButton_3 = new QPushButton(widget);
    pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
    pushButton_3->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_3->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_3, 0, 2, 1, 1);

    pushButton_4 = new QPushButton(widget);
    pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
    pushButton_4->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_4->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_4, 1, 0, 1, 1);

    pushButton_9 = new QPushButton(widget);
    pushButton_9->setObjectName(QString::fromUtf8("pushButton_9"));
    pushButton_9->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_9->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_9, 2, 2, 1, 1);

    pushButton_clear = new QPushButton(widget);
    pushButton_clear->setObjectName(QString::fromUtf8("pushButton_clear"));
    pushButton_clear->setMinimumSize(QSize(MyWidth, MyHeight));
    pushButton_clear->setMaximumSize(QSize(MyWidth, MyHeight));

    gridLayout->addWidget(pushButton_clear, 1, 3, 1, 1);

    pushButton_0 = new QPushButton(widget);
    pushButton_0->setObjectName(QString::fromUtf8("pushButton_0"));
    pushButton_0->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_0->setMaximumSize(QSize(MyHeight, MyHeight));
    gridLayout->addWidget(pushButton_0, 3, 0, 1, 1);

    pushButton_7 = new QPushButton(widget);
    pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));
    pushButton_7->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_7->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_7, 2, 0, 1, 1);

    pushButton_5 = new QPushButton(widget);
    pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
    pushButton_5->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_5->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_5, 1, 1, 1, 1);

    pushButton_6 = new QPushButton(widget);
    pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
    pushButton_6->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_6->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_6, 1, 2, 1, 1);

    pushButton_esc = new QPushButton(widget);
    pushButton_esc->setObjectName(QString::fromUtf8("pushButton_esc"));
    pushButton_esc->setMinimumSize(QSize(MyWidth, MyHeight));
    pushButton_esc->setMaximumSize(QSize(MyWidth, MyHeight));

    gridLayout->addWidget(pushButton_esc, 2, 3, 1, 1);

    pushButton_add = new QPushButton(widget);
    pushButton_add->setObjectName(QString::fromUtf8("pushButton_add"));
    pushButton_add->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_add->setMaximumSize(QSize(MyHeight, MyHeight));
    gridLayout->addWidget(pushButton_add, 3, 1, 1, 1);

    pushButton_point = new QPushButton(widget);
    pushButton_point->setObjectName(QString::fromUtf8("pushButton_point"));
    pushButton_point->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_point->setMaximumSize(QSize(MyHeight, MyHeight));
    gridLayout->addWidget(pushButton_point, 3, 2, 1, 1);

    pushButton_8 = new QPushButton(widget);
    pushButton_8->setObjectName(QString::fromUtf8("pushButton_8"));
    pushButton_8->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_8->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_8, 2, 1, 1, 1);


    gridLayout_2->addWidget(widget, 1, 0, 1, 1);

    lineEdit = new QLineEdit(this);
    lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
    lineEdit->setMinimumSize(QSize(0, MyHeight));

    gridLayout_2->addWidget(lineEdit, 0, 0, 1, 1);

    pushButton_space->setText("Backspace");
    pushButton_1->setText("1");
    pushButton_enter->setText("Enter");
    pushButton_2->setText("2");
    pushButton_3->setText("3");
    pushButton_4->setText("4");
    pushButton_9->setText("9");
    pushButton_clear->setText("Clear All");
    pushButton_0->setText("0");
    pushButton_7->setText("7");
    pushButton_5->setText("5");
    pushButton_6->setText("6");
    pushButton_esc->setText("Esc");
    pushButton_add->setText("+/-");
    pushButton_point->setText(".");
    pushButton_8->setText("8");

    connect(pushButton_0,&QPushButton::clicked,[=](){insertContent("0");});
    connect(pushButton_1,&QPushButton::clicked,[=](){insertContent("1");});
    connect(pushButton_2,&QPushButton::clicked,[=](){insertContent("2");});
    connect(pushButton_3,&QPushButton::clicked,[=](){insertContent("3");});
    connect(pushButton_4,&QPushButton::clicked,[=](){insertContent("4");});
    connect(pushButton_5,&QPushButton::clicked,[=](){insertContent("5");});
    connect(pushButton_6,&QPushButton::clicked,[=](){insertContent("6");});
    connect(pushButton_7,&QPushButton::clicked,[=](){insertContent("7");});
    connect(pushButton_8,&QPushButton::clicked,[=](){insertContent("8");});
    connect(pushButton_9,&QPushButton::clicked,[=](){insertContent("9");});
    connect(pushButton_point,&QPushButton::clicked,[=](){insertContent(".");});

    connect(pushButton_esc,&QPushButton::clicked,[=](){delete this;});
    connect(pushButton_enter,&QPushButton::clicked,[=](){
        saveEditInfo();
    });
    connect(pushButton_clear,&QPushButton::clicked,[=](){str.clear();lineEdit->setText(str);});
    connect(pushButton_space,&QPushButton::clicked,[=](){str.clear();str.append(lineEdit->text());if(str.size()>=1){int curpos=lineEdit->cursorPosition();str.remove(curpos-1,1);lineEdit->setText(str);lineEdit->setCursorPosition(curpos-1);}});
    connect(pushButton_add,&QPushButton::clicked,[=](){
        str.clear();
        str.append(lineEdit->text());
        if(str.size()<=0) return ;
        if(str.count("-")){
            str = str.mid(1,str.size()-1);
        }else{
            str = "-"+str;
        }
        lineEdit->setText(str);
    });
}
void KBFLoatEdit::insertContent(QString text)
{
    str.clear();
    if (m_isSelectedAllFlag) {
        int curpos=0;
        str.insert(curpos,text);
        lineEdit->setText(str);
        lineEdit->setCursorPosition(curpos+1);
        m_isSelectedAllFlag=false;
    }
    else {
        str.append(lineEdit->text());
        int curpos=lineEdit->cursorPosition();
        str.insert(curpos,text);
        lineEdit->setText(str);
        lineEdit->setCursorPosition(curpos+1);
    }
}
void KBFLoatEdit::saveEditInfo()
{
    if(lineEdit->text()!=str){
        str.clear();
        str.append(lineEdit->text());
    }

    if (!IsValidNumber(str)) {
        QMessageBox::warning(this,QString::fromStdString(HG_DEVICE_NAME),"仅支持数字，请重新输入");
        return;
    }

    double dValue = str.toDouble();
    if (dValue > m_dUpperLimit || dValue < m_dLowerLimit)
    {
        QString temp;
        temp = QString("请输入一个有效小数，介于[%1,%2]之间！").arg(QString::number(m_dLowerLimit, 'f', 2), QString::number(m_dUpperLimit, 'f', 2));
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),  temp);
        lineEdit->setFocus();
    }
    else
    {
        emit MykeyboardSignal(str);
        delete this;
    }
}
void KBFLoatEdit::setValue(double value)
{
    lineEdit->setText(QString::number(value, 'f', 2));
    lineEdit->setFocus();
    lineEdit->selectAll();
    m_isSelectedAllFlag=true;
}
void KBFLoatEdit::keyPressEvent(QKeyEvent *event){
    switch(event->key())
    {
    case Qt::Key_Return: // enter键
    case Qt::Key_Enter: {// 数字键盘的enter键
        saveEditInfo();
        break;
    }
    default:
        break;
    }
}

KBCharEdit::KBCharEdit(QWidget *parent) : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_ShowModal, true);
    init(30,100,20);
}
KBCharEdit::~KBCharEdit()
{
    SAFE_DELETE(pushButton_0);
    SAFE_DELETE(pushButton_lbracked);
    SAFE_DELETE(pushButton_rbracked);
    SAFE_DELETE(pushButton_shoulder);
    SAFE_DELETE(pushButton_colom);
    SAFE_DELETE(pushButton_bar);
    SAFE_DELETE(pushButton_underbar);
    SAFE_DELETE(pushButton_s);
    SAFE_DELETE(pushButton_d);
    SAFE_DELETE(pushButton_space);
    SAFE_DELETE(pushButton_f);
    SAFE_DELETE(pushButton_r);
    SAFE_DELETE(pushButton_e);
    SAFE_DELETE(pushButton_t);
    SAFE_DELETE(lineEdit);
    SAFE_DELETE(pushButton_a);
    SAFE_DELETE(pushButton_7);
    SAFE_DELETE(pushButton_6);
    SAFE_DELETE(pushButton_q);
    SAFE_DELETE(pushButton_j);
    SAFE_DELETE(pushButton_1);
    SAFE_DELETE(pushButton_h);
    SAFE_DELETE(pushButton_v);
    SAFE_DELETE(pushButton_8);
    SAFE_DELETE(pushButton_3);
    SAFE_DELETE(pushButton_g);
    SAFE_DELETE(pushButton_l);
    SAFE_DELETE(pushButton_b);
    SAFE_DELETE(pushButton_p);
    SAFE_DELETE(pushButton_k);
    SAFE_DELETE(pushButton_clear);
    SAFE_DELETE(pushButton_c);
    SAFE_DELETE(pushButton_4);
    SAFE_DELETE(pushButton_9);
    SAFE_DELETE(pushButton_5);
    SAFE_DELETE(pushButton_w);
    SAFE_DELETE(pushButton_o);
    SAFE_DELETE(pushButton_m);
    SAFE_DELETE(pushButton_i);
    SAFE_DELETE(pushButton_2);
    SAFE_DELETE(pushButton_n);
    SAFE_DELETE(pushButton_enter);
    SAFE_DELETE(pushButton_esc);
    SAFE_DELETE(pushButton_u);
    SAFE_DELETE(pushButton_x);
    SAFE_DELETE(pushButton_y);
    SAFE_DELETE(pushButton_z);
    SAFE_DELETE(pushButton_caps);
    SAFE_DELETE(pushButton_point);
    SAFE_DELETE(zhChBtn);
    SAFE_DELETE(zhLabel);
    SAFE_DELETE(widget_2);
    SAFE_DELETE(widget);
}
void KBCharEdit::init(int MyHeight,int MyWidth,int smallhw){
    this->resize(370, 150);
    flagcaps = true;
    gridLayout_2 = new QGridLayout(this);
    gridLayout_2->setSpacing(0);
    gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
    gridLayout_2->setContentsMargins(0, 0, 0, 0);
    widget_2 = new QWidget(this);
    widget_2->setObjectName(QString::fromUtf8("widget_2"));
    horizontalLayout = new QHBoxLayout(widget_2);
    horizontalLayout->setSpacing(0);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    horizontalLayout->setContentsMargins(0, 0, 0, 0);
    pushButton_lbracked = new QPushButton(widget_2);
    pushButton_lbracked->setObjectName(QString::fromUtf8("pushButton_lbracked"));
    pushButton_lbracked->setMinimumSize(QSize(smallhw, smallhw));
    pushButton_lbracked->setMaximumSize(QSize(smallhw, smallhw));
    horizontalLayout->addWidget(pushButton_lbracked);

    pushButton_rbracked = new QPushButton(widget_2);
    pushButton_rbracked->setObjectName(QString::fromUtf8("pushButton_rbracked"));
    pushButton_rbracked->setMinimumSize(QSize(smallhw, smallhw));
    pushButton_rbracked->setMaximumSize(QSize(smallhw, smallhw));

    horizontalLayout->addWidget(pushButton_rbracked);

    pushButton_shoulder = new QPushButton(widget_2);
    pushButton_shoulder->setObjectName(QString::fromUtf8("pushButton_shoulder"));
    pushButton_shoulder->setMinimumSize(QSize(smallhw, smallhw));
    pushButton_shoulder->setMaximumSize(QSize(smallhw, smallhw));

    horizontalLayout->addWidget(pushButton_shoulder);

    pushButton_colom = new QPushButton(widget_2);
    pushButton_colom->setObjectName(QString::fromUtf8("pushButton_colom"));
    pushButton_colom->setMinimumSize(QSize(smallhw, smallhw));
    pushButton_colom->setMaximumSize(QSize(smallhw, smallhw));

    horizontalLayout->addWidget(pushButton_colom);

    pushButton_bar = new QPushButton(widget_2);
    pushButton_bar->setObjectName(QString::fromUtf8("pushButton_bar"));
    pushButton_bar->setMinimumSize(QSize(smallhw, smallhw));
    pushButton_bar->setMaximumSize(QSize(smallhw, smallhw));

    horizontalLayout->addWidget(pushButton_bar);

    pushButton_underbar = new QPushButton(widget_2);
    pushButton_underbar->setObjectName(QString::fromUtf8("pushButton_underbar"));
    pushButton_underbar->setMinimumSize(QSize(smallhw, smallhw));
    pushButton_underbar->setMaximumSize(QSize(smallhw, smallhw));

    horizontalLayout->addWidget(pushButton_underbar);

    gridLayout_2->addWidget(widget_2, 0, 1, 1, 1);

    lineEdit = new QLineEdit(this);
    lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
    lineEdit->setMinimumSize(QSize(0, MyHeight));
    gridLayout_2->addWidget(lineEdit, 0, 0, 1, 1);

    widget = new QWidget(this);
    widget->setObjectName(QString::fromUtf8("widget"));
    gridLayout = new QGridLayout(widget);
    gridLayout->setSpacing(0);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setContentsMargins(0, 0, 0, 0);
    pushButton_s = new QPushButton(widget);
    pushButton_s->setObjectName(QString::fromUtf8("pushButton_s"));
    pushButton_s->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_s->setMaximumSize(QSize(MyHeight, MyHeight));
    gridLayout->addWidget(pushButton_s, 2, 4, 1, 1);

    pushButton_d = new QPushButton(widget);
    pushButton_d->setObjectName(QString::fromUtf8("pushButton_d"));
    pushButton_d->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_d->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_d, 0, 3, 1, 1);

    pushButton_space = new QPushButton(widget);
    pushButton_space->setObjectName(QString::fromUtf8("pushButton_space"));
    pushButton_space->setMinimumSize(QSize(MyWidth, MyHeight));
    pushButton_space->setMaximumSize(QSize(MyWidth, MyHeight));

    gridLayout->addWidget(pushButton_space, 0, 32, 1, 1);

    pushButton_f = new QPushButton(widget);
    pushButton_f->setObjectName(QString::fromUtf8("pushButton_f"));
    pushButton_f->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_f->setMaximumSize(QSize(MyHeight, MyHeight));
    gridLayout->addWidget(pushButton_f, 0, 5, 1, 1);

    pushButton_r = new QPushButton(widget);
    pushButton_r->setObjectName(QString::fromUtf8("pushButton_r"));
    pushButton_r->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_r->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_r, 2, 3, 1, 1);

    pushButton_e = new QPushButton(widget);
    pushButton_e->setObjectName(QString::fromUtf8("pushButton_e"));
    pushButton_e->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_e->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_e, 0, 4, 1, 1);

    pushButton_t = new QPushButton(widget);
    pushButton_t->setObjectName(QString::fromUtf8("pushButton_t"));
    pushButton_t->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_t->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_t, 2, 5, 1, 1);

    pushButton_a = new QPushButton(widget);
    pushButton_a->setObjectName(QString::fromUtf8("pushButton_a"));
    pushButton_a->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_a->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_a, 0, 0, 1, 1);

    pushButton_7 = new QPushButton(widget);
    pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));
    pushButton_7->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_7->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_7, 2, 29, 1, 1);

    pushButton_6 = new QPushButton(widget);
    pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
    pushButton_6->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_6->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_6, 1, 31, 1, 1);

    pushButton_q = new QPushButton(widget);
    pushButton_q->setObjectName(QString::fromUtf8("pushButton_q"));
    pushButton_q->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_q->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_q, 2, 2, 1, 1);

    pushButton_j = new QPushButton(widget);
    pushButton_j->setObjectName(QString::fromUtf8("pushButton_j"));
    pushButton_j->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_j->setMaximumSize(QSize(MyHeight, MyHeight));
    gridLayout->addWidget(pushButton_j, 1, 2, 1, 1);

    pushButton_1 = new QPushButton(widget);
    pushButton_1->setObjectName(QString::fromUtf8("pushButton_1"));
    pushButton_1->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_1->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_1, 0, 29, 1, 1);

    pushButton_h = new QPushButton(widget);
    pushButton_h->setObjectName(QString::fromUtf8("pushButton_h"));
    pushButton_h->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_h->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_h, 1, 0, 1, 1);

    pushButton_v = new QPushButton(widget);
    pushButton_v->setObjectName(QString::fromUtf8("pushButton_v"));
    pushButton_v->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_v->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_v, 3, 0, 1, 1);

    pushButton_8 = new QPushButton(widget);
    pushButton_8->setObjectName(QString::fromUtf8("pushButton_8"));
    pushButton_8->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_8->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_8, 2, MyHeight, 1, 1);

    pushButton_3 = new QPushButton(widget);
    pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
    pushButton_3->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_3->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_3, 0, 31, 1, 1);

    pushButton_g = new QPushButton(widget);
    pushButton_g->setObjectName(QString::fromUtf8("pushButton_g"));
    pushButton_g->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_g->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_g, 0, 6, 1, 1);

    pushButton_l = new QPushButton(widget);
    pushButton_l->setObjectName(QString::fromUtf8("pushButton_l"));
    pushButton_l->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_l->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_l, 1, 4, 1, 1);

    pushButton_b = new QPushButton(widget);
    pushButton_b->setObjectName(QString::fromUtf8("pushButton_b"));
    pushButton_b->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_b->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_b, 0, 1, 1, 1);

    pushButton_p = new QPushButton(widget);
    pushButton_p->setObjectName(QString::fromUtf8("pushButton_p"));
    pushButton_p->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_p->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_p, 2, 1, 1, 1);

    pushButton_k = new QPushButton(widget);
    pushButton_k->setObjectName(QString::fromUtf8("pushButton_k"));
    pushButton_k->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_k->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_k, 1, 3, 1, 1);

    pushButton_clear = new QPushButton(widget);
    pushButton_clear->setObjectName(QString::fromUtf8("pushButton_clear"));
    pushButton_clear->setMinimumSize(QSize(MyWidth, MyHeight));
    pushButton_clear->setMaximumSize(QSize(MyWidth, MyHeight));

    gridLayout->addWidget(pushButton_clear, 1, 32, 1, 1);

    pushButton_c = new QPushButton(widget);
    pushButton_c->setObjectName(QString::fromUtf8("pushButton_c"));
    pushButton_c->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_c->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_c, 0, 2, 1, 1);

    pushButton_4 = new QPushButton(widget);
    pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
    pushButton_4->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_4->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_4, 1, 29, 1, 1);

    pushButton_9 = new QPushButton(widget);
    pushButton_9->setObjectName(QString::fromUtf8("pushButton_9"));
    pushButton_9->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_9->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_9, 2, 31, 1, 1);

    pushButton_5 = new QPushButton(widget);
    pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
    pushButton_5->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_5->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_5, 1, MyHeight, 1, 1);

    pushButton_w = new QPushButton(widget);
    pushButton_w->setObjectName(QString::fromUtf8("pushButton_w"));
    pushButton_w->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_w->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_w, 3, 1, 1, 1);

    pushButton_o = new QPushButton(widget);
    pushButton_o->setObjectName(QString::fromUtf8("pushButton_o"));
    pushButton_o->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_o->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_o, 2, 0, 1, 1);

    pushButton_m = new QPushButton(widget);
    pushButton_m->setObjectName(QString::fromUtf8("pushButton_m"));
    pushButton_m->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_m->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_m, 1, 5, 1, 1);

    pushButton_i = new QPushButton(widget);
    pushButton_i->setObjectName(QString::fromUtf8("pushButton_i"));
    pushButton_i->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_i->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_i, 1, 1, 1, 1);

    pushButton_2 = new QPushButton(widget);
    pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
    pushButton_2->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_2->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_2, 0, MyHeight, 1, 1);

    pushButton_0 = new QPushButton(widget);
    pushButton_0->setObjectName(QString::fromUtf8("pushButton"));
    pushButton_0->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_0->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_0, 3, 30, 1, 1);

    pushButton_point = new QPushButton(widget);
    pushButton_point->setObjectName(QString::fromUtf8("pushButton_point"));
    pushButton_point->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_point->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_point, 3, 31, 1, 1);

    pushButton_n = new QPushButton(widget);
    pushButton_n->setObjectName(QString::fromUtf8("pushButton_n"));
    pushButton_n->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_n->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_n, 1, 6, 1, 1);

    pushButton_enter = new QPushButton(widget);
    pushButton_enter->setObjectName(QString::fromUtf8("pushButton_enter"));
    pushButton_enter->setMinimumSize(QSize(MyWidth, MyHeight));

    gridLayout->addWidget(pushButton_enter, 3, 32, 1, 1);

    pushButton_esc = new QPushButton(widget);
    pushButton_esc->setObjectName(QString::fromUtf8("pushButton_esc"));
    pushButton_esc->setMinimumSize(QSize(MyWidth, MyHeight));
    pushButton_esc->setMaximumSize(QSize(MyWidth, MyHeight));

    gridLayout->addWidget(pushButton_esc, 2, 32, 1, 1);

    pushButton_u = new QPushButton(widget);
    pushButton_u->setObjectName(QString::fromUtf8("pushButton_u"));
    pushButton_u->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_u->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_u, 2, 6, 1, 1);

    pushButton_x = new QPushButton(widget);
    pushButton_x->setObjectName(QString::fromUtf8("pushButton_x"));
    pushButton_x->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_x->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_x, 3, 2, 1, 1);

    pushButton_y = new QPushButton(widget);
    pushButton_y->setObjectName(QString::fromUtf8("pushButton_y"));
    pushButton_y->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_y->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_y, 3, 3, 1, 1);

    pushButton_z = new QPushButton(widget);
    pushButton_z->setObjectName(QString::fromUtf8("pushButton_z"));
    pushButton_z->setMinimumSize(QSize(MyHeight, MyHeight));
    pushButton_z->setMaximumSize(QSize(MyHeight, MyHeight));

    gridLayout->addWidget(pushButton_z, 3, 4, 1, 1);

    pushButton_caps = new QPushButton(widget);
    pushButton_caps->setObjectName(QString::fromUtf8("pushButton_caps"));
    pushButton_caps->setMinimumSize(QSize(60, MyHeight));
    pushButton_caps->setMaximumSize(QSize(60, MyHeight));
    gridLayout->addWidget(pushButton_caps, 3, 5, 1, 2);

    zhChBtn = new QPushButton(widget);
    zhChBtn->setObjectName(QString::fromUtf8("zhChBtn"));
    zhChBtn->setMinimumSize(QSize(MyHeight, MyHeight));
    zhChBtn->setMaximumSize(QSize(MyHeight, MyHeight));
    gridLayout->addWidget(zhChBtn, 3, 29, 1, 1);

    gridLayout_2->addWidget(widget, 1, 0, 1, 2);

    zhLabel=new QLabel(this);
    zhLabel->setObjectName(QString::fromUtf8("zhLabel"));
    gridLayout_2->addWidget(zhLabel,2,0,1,1);


    pushButton_lbracked->setText("(");
    pushButton_rbracked->setText(")");
    pushButton_shoulder->setText("\\");
    pushButton_colom->setText(":");
    pushButton_bar->setText("-");
    pushButton_underbar->setText("_");
    pushButton_space->setText("Backspace");
    pushButton_clear->setText("Clear All");
    pushButton_enter->setText("Enter");
    pushButton_esc->setText("Esc");
    pushButton_caps->setText("Caps Lock");
    pushButton_s->setText("s");
    pushButton_d->setText("d");
    pushButton_f->setText("f");
    pushButton_r->setText("r");
    pushButton_e->setText("e");
    pushButton_t->setText("t");
    pushButton_a->setText("a");
    pushButton_7->setText("7");
    pushButton_6->setText("6");
    pushButton_q->setText("q");
    pushButton_j->setText("j");
    pushButton_1->setText("1");
    pushButton_h->setText("h");
    pushButton_v->setText("v");
    pushButton_8->setText("8");
    pushButton_3->setText("3");
    pushButton_g->setText("g");
    pushButton_l->setText("l");
    pushButton_b->setText("b");
    pushButton_p->setText("p");
    pushButton_k->setText("k");
    pushButton_c->setText("c");
    pushButton_4->setText("4");
    pushButton_9->setText("9");
    pushButton_5->setText("5");
    pushButton_w->setText("w");
    pushButton_o->setText("o");
    pushButton_m->setText("m");
    pushButton_i->setText("i");
    pushButton_2->setText("2");
    pushButton_0->setText("0");
    pushButton_n->setText("n");
    pushButton_u->setText("u");
    pushButton_x->setText("x");
    pushButton_y->setText("y");
    pushButton_z->setText("z");
    pushButton_point->setText(".");
    zhChBtn->setText("英");
    flagZh=false;

    connect(zhChBtn,&QPushButton::clicked,[=]() {
        flagZh=!flagZh;
        if (flagZh) zhChBtn->setText("中");
        else zhChBtn->setText("英");
    });
    connect(pushButton_0,&QPushButton::clicked,[=]() {insertContent("0");});
    connect(pushButton_1,&QPushButton::clicked,[=]() {insertContent("1");});
    connect(pushButton_2,&QPushButton::clicked,[=]() {insertContent("2");});
    connect(pushButton_3,&QPushButton::clicked,[=]() {insertContent("3");});
    connect(pushButton_4,&QPushButton::clicked,[=]() {insertContent("4");});
    connect(pushButton_5,&QPushButton::clicked,[=]() {insertContent("5");});
    connect(pushButton_6,&QPushButton::clicked,[=]() {insertContent("6");});
    connect(pushButton_7,&QPushButton::clicked,[=]() {insertContent("7");});
    connect(pushButton_8,&QPushButton::clicked,[=]() {insertContent("8");});
    connect(pushButton_9,&QPushButton::clicked,[=]() {insertContent("9");});
    connect(pushButton_a,&QPushButton::clicked,[=]() {if(!flagcaps)insertContent("A");else insertContent("a");});
    connect(pushButton_b,&QPushButton::clicked,[=]() {if(!flagcaps)insertContent("B");else insertContent("b");});
    connect(pushButton_c,&QPushButton::clicked,[=]() {if(!flagcaps)insertContent("C");else insertContent("c");});
    connect(pushButton_d,&QPushButton::clicked,[=]() {if(!flagcaps)insertContent("D");else insertContent("d");});
    connect(pushButton_e,&QPushButton::clicked,[=]() {if(!flagcaps)insertContent("E");else insertContent("e");});
    connect(pushButton_f,&QPushButton::clicked,[=]() {if(!flagcaps)insertContent("F");else insertContent("f");});
    connect(pushButton_g,&QPushButton::clicked,[=]() {if(!flagcaps)insertContent("G");else insertContent("g");});
    connect(pushButton_h,&QPushButton::clicked,[=]() {if(!flagcaps)insertContent("H");else insertContent("h");});
    connect(pushButton_i,&QPushButton::clicked,[=]() {if(!flagcaps)insertContent("I");else insertContent("i");});
    connect(pushButton_j,&QPushButton::clicked,[=]() {if(!flagcaps)insertContent("J");else insertContent("j");});
    connect(pushButton_k,&QPushButton::clicked,[=]() {if(!flagcaps)insertContent("K");else insertContent("k");});
    connect(pushButton_l,&QPushButton::clicked,[=]() {if(!flagcaps)insertContent("L");else insertContent("l");});
    connect(pushButton_m,&QPushButton::clicked,[=]() {if(!flagcaps)insertContent("M");else insertContent("m");});
    connect(pushButton_n,&QPushButton::clicked,[=]() {if(!flagcaps)insertContent("N");else insertContent("n");});
    connect(pushButton_o,&QPushButton::clicked,[=]() {if(!flagcaps)insertContent("O");else insertContent("o");});
    connect(pushButton_p,&QPushButton::clicked,[=]() {if(!flagcaps)insertContent("P");else insertContent("p");});
    connect(pushButton_q,&QPushButton::clicked,[=]() {if(!flagcaps)insertContent("Q");else insertContent("q");});
    connect(pushButton_r,&QPushButton::clicked,[=]() {if(!flagcaps)insertContent("R");else insertContent("r");});
    connect(pushButton_s,&QPushButton::clicked,[=]() {if(!flagcaps)insertContent("S");else insertContent("s");});
    connect(pushButton_t,&QPushButton::clicked,[=]() {if(!flagcaps)insertContent("T");else insertContent("t");});
    connect(pushButton_u,&QPushButton::clicked,[=]() {if(!flagcaps)insertContent("U");else insertContent("u");});
    connect(pushButton_v,&QPushButton::clicked,[=]() {if(!flagcaps)insertContent("V");else insertContent("v");});
    connect(pushButton_w,&QPushButton::clicked,[=]() {if(!flagcaps)insertContent("W");else insertContent("w");});
    connect(pushButton_x,&QPushButton::clicked,[=]() {if(!flagcaps)insertContent("X");else insertContent("x");});
    connect(pushButton_y,&QPushButton::clicked,[=]() {if(!flagcaps)insertContent("Y");else insertContent("y");});
    connect(pushButton_z,&QPushButton::clicked,[=]() {if(!flagcaps)insertContent("Z");else insertContent("z");});

    connect(pushButton_point,   &QPushButton::clicked,[=](){insertContent(".");});
    connect(pushButton_lbracked,&QPushButton::clicked,[=](){insertContent("(");});
    connect(pushButton_rbracked,&QPushButton::clicked,[=](){insertContent(")");});
    connect(pushButton_shoulder,&QPushButton::clicked,[=](){insertContent("\\");});
    connect(pushButton_colom,   &QPushButton::clicked,[=](){insertContent(":");});
    connect(pushButton_bar,     &QPushButton::clicked,[=](){insertContent("-");});
    connect(pushButton_underbar,&QPushButton::clicked,[=](){insertContent("_");});

    connect(pushButton_esc,  &QPushButton::clicked,[=](){delete this;});
    connect(pushButton_enter,&QPushButton::clicked,[=](){saveEditInfo();});
    connect(pushButton_clear,&QPushButton::clicked,[=](){str.clear();lineEdit->setText(str);});
    connect(pushButton_space,&QPushButton::clicked,[=](){
        str.clear();
        str.append(lineEdit->text());
        if(str.size()>=1){
            if (lineEdit->hasSelectedText()) {
                int start=lineEdit->selectionStart();
                int len=lineEdit->selectionLength();
                str.remove(start,len);
                lineEdit->setText(str);
                lineEdit->setCursorPosition(start-1);
            } else {
                int curpos=lineEdit->cursorPosition();
                str.remove(curpos-1,1);
                lineEdit->setText(str);
                lineEdit->setCursorPosition(curpos-1);
            }
        }
    });
    connect(pushButton_caps,SIGNAL(clicked()),this,SLOT(button_capsslot()));
}
void KBCharEdit::insertContent(QString text)
{
    str.clear();
    if (m_isSelectedAllFlag) {
        int curpos=0;
        str.insert(curpos,text);
        lineEdit->setText(str);
        lineEdit->setCursorPosition(curpos+1);
        m_isSelectedAllFlag=false;
    }
    else {
        str.append(lineEdit->text());
        int curpos=lineEdit->cursorPosition();
        str.insert(curpos,text);
        lineEdit->setText(str);
        lineEdit->setCursorPosition(curpos+1);
    }
}
void KBCharEdit::saveEditInfo()
{
    if(str!=lineEdit->text())
    {
        str.clear();
        str.append(lineEdit->text());
    }
    emit MykeyboardSignal(str);
    delete this;
}
void KBCharEdit::button_capsslot(){
    if(flagcaps){
        flagcaps = false;
        pushButton_a->setText("A");
        pushButton_b->setText("B");
        pushButton_c->setText("C");
        pushButton_d->setText("D");
        pushButton_e->setText("E");
        pushButton_f->setText("F");
        pushButton_g->setText("G");
        pushButton_h->setText("H");
        pushButton_i->setText("I");
        pushButton_j->setText("J");
        pushButton_k->setText("K");
        pushButton_l->setText("L");
        pushButton_m->setText("M");
        pushButton_n->setText("N");
        pushButton_o->setText("O");
        pushButton_p->setText("P");
        pushButton_q->setText("Q");
        pushButton_r->setText("R");
        pushButton_s->setText("S");
        pushButton_t->setText("T");
        pushButton_u->setText("U");
        pushButton_v->setText("V");
        pushButton_w->setText("W");
        pushButton_x->setText("X");
        pushButton_y->setText("Y");
        pushButton_z->setText("Z");
    }
    else
    {
        flagcaps = true;
        pushButton_a->setText("a");
        pushButton_b->setText("b");
        pushButton_c->setText("c");
        pushButton_d->setText("d");
        pushButton_e->setText("e");
        pushButton_f->setText("f");
        pushButton_g->setText("g");
        pushButton_h->setText("h");
        pushButton_i->setText("i");
        pushButton_j->setText("j");
        pushButton_k->setText("k");
        pushButton_l->setText("l");
        pushButton_m->setText("m");
        pushButton_n->setText("n");
        pushButton_o->setText("o");
        pushButton_p->setText("p");
        pushButton_q->setText("q");
        pushButton_r->setText("r");
        pushButton_s->setText("s");
        pushButton_t->setText("t");
        pushButton_u->setText("u");
        pushButton_v->setText("v");
        pushButton_w->setText("w");
        pushButton_x->setText("x");
        pushButton_y->setText("y");
        pushButton_z->setText("z");
    }
}
void KBCharEdit::keyPressEvent(QKeyEvent *event){
    switch(event->key())
    {
    case Qt::Key_Return: // enter键
    case Qt::Key_Enter: {// 数字键盘的enter键
        saveEditInfo();
        break;
    }
    default:
        break;
    }
}
void KBCharEdit::setValue(std::string str)
{
    lineEdit->setText(QString::fromStdString(str));
    lineEdit->setFocus();
    lineEdit->selectAll();
    QString text=lineEdit->selectedText();
    m_isSelectedAllFlag=true;
}
