#ifndef MYPOINTHANDLE_H
#define MYPOINTHANDLE_H

#include <QGraphicsEllipseItem>
#include <QGraphicsItem>
#include <QObject>
#include <QBrush>
#include <QApplication>

class MyPointHandle : public QObject, public QGraphicsEllipseItem {
    Q_OBJECT  // 需要定义 Q_OBJECT 宏来支持信号槽机制

public:
    MyPointHandle(QGraphicsItem* parent = nullptr) 
        : QGraphicsEllipseItem(-5, -5, 5, 5,parent) {
        setBrush(QBrush(Qt::blue));  // 设置为一个圆形控制点
        setFlag(QGraphicsItem::ItemIsMovable);  // 设置为可移动
        setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
        setFlag(QGraphicsItem::ItemIsSelectable);
    }

signals:
    void positionChanged(const QPointF& newPos);  // 信号：当位置变化时

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override {
        if (change == ItemPositionChange) {
            emit positionChanged(value.toPointF());  // 位置发生改变时，触发信号
        }
        return QGraphicsEllipseItem::itemChange(change, value);
    }
};

#endif // MYPOINTHANDLE_H