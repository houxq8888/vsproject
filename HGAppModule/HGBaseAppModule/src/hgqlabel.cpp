/*
    @file   hgqlabel.cpp
    @brief  Implementation file
    @author XiaoQin.Hou
*/
#include "hgqlabel.h"
#include <QDebug>
#include <iostream>


HGQLabel::HGQLabel(QWidget* parent):QLabel(parent)
{
    flag=false;
}
HGQLabel::HGQLabel(bool flipVer,const std::string &imgPath,QWidget* parent):QLabel(parent){
    pixmap.load(QString::fromStdString(imgPath));
    if (flipVer) {
        QTransform transform;
        transform.scale(1,-1);
        pixmap=pixmap.transformed(transform);
    }
    flag=true;
}
void HGQLabel::changePixmap(const std::string &imgPath,int size)
{
    pixmap.load(QString::fromStdString(imgPath));
    setPixmap(pixmap.scaled(QSize(size,size),Qt::KeepAspectRatio,Qt::SmoothTransformation));
}
void HGQLabel::init()
{
    
}

QString HGQLabel::getLabelText(){
    return m_text->text();
}
HGQLabel::~HGQLabel()
{

}
void HGQLabel::resizeEvent(QResizeEvent* event){
    if (!flag) return;
    QLabel::resizeEvent(event);
    setPixmap(pixmap.scaled(QSize(24,24),Qt::KeepAspectRatio,Qt::SmoothTransformation));
}
#if 1
void HGQLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit leftClicked();
    }
    else if (event->button() == Qt::RightButton)
    {
        emit rightClicked();
    }
}
bool HGQLabel::event(QEvent *e)
{
    if (e->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(e);
        if (mouseEvent->button() == Qt::LeftButton)
        {
            emit leftClicked();
            return true;
        }
        else if (mouseEvent->button() == Qt::RightButton)
        {
            emit rightClicked();
            return true;
        }
    }
    else if (e->type() == QEvent::MouseButtonDblClick)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(e);
        if (mouseEvent->button() == Qt::LeftButton)
        {
            emit leftdoubleClicked();
            return true;
        }
        else if (mouseEvent->button() == Qt::RightButton)
        {
            emit rightdoubleClicked();
            return true;
        }
    }
    return QLabel::event(e);
}
void HGQLabel::mouseDoubleClickEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton)
        emit leftdoubleClicked();
    else if (event->button() == Qt::RightButton)
        emit rightdoubleClicked();
    else {}
}

#endif
