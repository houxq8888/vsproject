/*
    @file   flowtreewidget.cpp
    @brief  Implementation file
    @author XiaoQin.Hou
*/
#include "flowtreewidget.h"


FlowTreeWidget::FlowTreeWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    setDragEnabled(true);
    setDragDropMode(QTreeWidget::DragOnly);
    _isDrag = false;
}

//记录拖拽初始位置
void FlowTreeWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        auto item = this->currentItem();
        if (item)
        {
            QString itemName = item->text(0);
            emit getName(itemName.toStdString());
        }
        // startPos = event->pos();
        // auto item = this->itemAt(startPos);
        // if(!item || !item->parent()){
        //     event->ignore();
        //     setDragEnabled(false);

        //     QString text = item->text(0);
        //     emit getName(text.toStdString());
        //     QTreeWidget::mousePressEvent(event);
        //     return;
        // }
        setDragEnabled(true);
        _isDrag = true;
    }
    QTreeWidget::mousePressEvent(event);
}

void FlowTreeWidget::mouseReleaseEvent(QMouseEvent *event)
{
    _isDrag = false;
    QTreeWidget::mouseReleaseEvent(event);
}

void FlowTreeWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(_isDrag){
        auto item = this->itemAt(startPos);
        if(!item || !item->parent()){
            event->ignore();
            QTreeWidget::mousePressEvent(event);
            return;
        }

        auto rect = this->visualItemRect(item);
        if(rect.isValid() && rect.width()>5 && rect.height()> 5){

            //[1]把拖拽数据放在QMimeData容器中
            QString text = item->text(0);
            QByteArray itemData;
            QDataStream dataStream(&itemData, QIODevice::WriteOnly);
            dataStream<<text;

            QMimeData *mimeData = new QMimeData;
            mimeData->setData(myMimeType(), itemData);

            QPixmap pixmap = this->viewport()->grab(rect);

            QDrag *drag = new QDrag(this);
            drag->setMimeData(mimeData);
            drag->setPixmap(pixmap);
            auto pos = startPos;
            pos.setY(pos.y()-rect.y());
            drag->setHotSpot(pos);
            drag->exec(Qt::CopyAction);
        }
    }else{
        QTreeWidget::mouseMoveEvent(event);
    }

}

