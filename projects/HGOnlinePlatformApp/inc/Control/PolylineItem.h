#ifndef MYPOLYLINEITEM_H
#define MYPOLYLINEITEM_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QMenu>
#include <QAction>
#include <QGraphicsEllipseItem>
#include <QContextMenuEvent>
#include <QPen>
#include <QPolygonF>
#include <QList>
#include <QGraphicsSceneMouseEvent>
#include "PointHandle.h"


class PolylineItem : public QObject, public QGraphicsItem {
    Q_OBJECT  // 需要定义 Q_OBJECT 宏来支持信号槽机制

public:
    PolylineItem() {
        setParentItem(nullptr);
        // 初始化一些默认的关键点
        addHandle(QPointF(50, 50));
        addHandle(QPointF(150, 100));
        addHandle(QPointF(250, 50));
        setFlags(QGraphicsItem::ItemIsMovable | 
            QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);  // 设置为可移动
    }

    QRectF boundingRect() const override {
        if (handles.isEmpty()) return QRectF();
        
        qreal left = handles.first()->pos().x(), top = handles.first()->pos().y();
        qreal right = left, bottom = top;
        
        for (auto handle : handles) {
            left = qMin(left, handle->pos().x());
            top = qMin(top, handle->pos().y());
            right = qMax(right, handle->pos().x());
            bottom = qMax(bottom, handle->pos().y());
        }
        return QRectF(left - 10, top - 10, right - left + 20, bottom - top + 20);  // 增加间距
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        QPen pen(Qt::black, 2);
        painter->setPen(pen);

        QPolygonF poly;
        for (auto handle : handles) {
            poly.append(handle->pos());
        }

        if (poly.size() > 1) {
            painter->drawPolyline(poly);  // 绘制折线
        }
    }
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override {
        QGraphicsItem::mouseMoveEvent(event);
        update();
    }

    void addHandle(const QPointF& pos) {
        MyPointHandle* newHandle = new MyPointHandle(this);
        newHandle->setPos(pos);
        handles.append(newHandle);
        // scene()->addItem(newHandle);  // 把控制点添加到场景

        // 连接信号与槽
        connect(newHandle, &MyPointHandle::positionChanged, this, &PolylineItem::updatePolyline);

        update();  // 更新折线显示
    }

    void removeHandle(MyPointHandle* handle) {
        handles.removeAll(handle);
        scene()->removeItem(handle);  // 从场景中移除控制点
        delete handle;
        update();  // 更新折线显示
    }

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override {
        QMenu menu;
        QAction* addPointAction = menu.addAction("添加关键点");
        connect(addPointAction, &QAction::triggered, this, [this, event]() {
            QPointF pos = event->pos();  // 获取鼠标点击位置
            addHandle(pos);  // 增加新关键点
        });

        // 添加删除关键点菜单项
        QAction* removePointAction = menu.addAction("删除最后一个关键点");
        connect(removePointAction, &QAction::triggered, this, [this]() {
            if (!handles.isEmpty()) {
                removeHandle(handles.last());
            }
        });

        menu.exec(event->screenPos());  // 显示菜单
    }

private:
    QList<MyPointHandle*> handles;  // 存储所有关键点

private slots:
    // 当关键点位置变化时更新折线
    void updatePolyline(const QPointF& newPos) {
        update();  // 强制刷新折线
    }
};

#endif // MYPOLYLINEITEM_H