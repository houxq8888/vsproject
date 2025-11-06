#include "myscene.h"
#include <QDebug>

MyScene::MyScene(QObject *parent) : 
    QGraphicsScene(parent)
{

}

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    // return;
    QGraphicsItem *clickedItem = itemAt(event->scenePos(), QTransform()); // 获取点击的Item
    if (clickedItem) {
        qDebug() << "Clicked Item Type:" << clickedItem->type();
        if (clickedItem->type() == QGraphicsRectItem::Type) {
            qDebug() << "Clicked a Rectangle!";
            clickedItem->setOpacity(0.5); // 点击后改变透明度
        } else if (clickedItem->type() == QGraphicsEllipseItem::Type) {
            qDebug() << "Clicked an Ellipse!";
            clickedItem->setOpacity(0.5); // 点击后改变透明度
        } else {
            qDebug() << "Clicked an unknown item type.";
        }
    } else {
        qDebug() << "Clicked on empty space.";
    }

   /* if (event->button() == Qt::LeftButton) {
        QPointF point = event->scenePos();

        if (!drawing) {  // 第一次点击，创建路径
            path.moveTo(point);
            pathItem = new QGraphicsPathItem(path);
            pathItem->setPen(QPen(Qt::red, 2));
            addItem(pathItem);
            drawing = true;
        } else {  // 追加点
            path.lineTo(point);
            pathItem->setPath(path);
        }
    } else if (event->button() == Qt::RightButton && drawing) {
        drawing = false;  // 右键结束绘制
    }*/

    QGraphicsScene::mousePressEvent(event);
}
void MyScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem *hoverItem=itemAt(event->scenePos(), QTransform());
    if (hoverItem){
        if (hoverItem->type() == QGraphicsItem::Type){
            // hoverItem->setToolTip(hoverItem->data(0).toString());
            hoverItem->setToolTip("Hello");
        }
    } 
    QGraphicsScene::mouseMoveEvent(event);
}

void MyScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    // if (drawingLine) {
    //     drawingLine = false;
    //     tempLine = nullptr;  // 停止绘制
    // }
    QGraphicsScene::mouseReleaseEvent(event);
}