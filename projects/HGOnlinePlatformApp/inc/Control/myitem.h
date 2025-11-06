#ifndef MYITEM_H
#define MYITEM_H


#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QPointer>
#include <QGraphicsTextItem>
#include <QStyle>
#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QPen>
#include <QMenu>
#include <QBrush>
#include <QCursor>
#include <QtMath>
#include <QDebug>
#include <QRect>
#include <QPolygonF>
#include <QVector>
#include "hgxml.h"

using namespace HGMACHINE;


class MyHighAccuracyPumpItem : public QGraphicsItem
{
public:
    explicit MyHighAccuracyPumpItem(QRectF rect, std::string title,std::string lang,
        QColor fillColor = Qt::blue, QColor borderColor = Qt::black, int borderWidth = 2);

    // 1️⃣ 定义矩形边界（用于碰撞检测、刷新区域等）
    QRectF boundingRect() const override {
        return m_rect.adjusted(-m_borderWidth / 2, -m_borderWidth / 2, m_borderWidth / 2, m_borderWidth / 2);
    }

private:
    // 2️⃣ 自定义绘制方法
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;

    // // 3️⃣ 支持鼠标交互（点击、拖拽）
    // void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    // // 重写右键菜单事件
    // void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    // void showImage();

private:
    QRectF m_rect;       // 矩形区域
    QColor m_fillColor;  // 填充颜色
    QColor m_borderColor;// 边框颜色
    int m_borderWidth;   // 边框宽度
    std::string m_lang;
    std::string m_title;
    QPixmap m_image;  // 用来存储图像
};


class MyAuxiliaryPumpItem : public QGraphicsItem
{
public:
    explicit MyAuxiliaryPumpItem(QRectF rect, std::string title,std::string lang,
        QColor fillColor = Qt::blue, QColor borderColor = Qt::black, int borderWidth = 2);

    // 1️⃣ 定义矩形边界（用于碰撞检测、刷新区域等）
    QRectF boundingRect() const override {
        return m_rect.adjusted(-m_borderWidth / 2, -m_borderWidth / 2, m_borderWidth / 2, m_borderWidth / 2);
    }

private:
    // 2️⃣ 自定义绘制方法
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;

    // // 3️⃣ 支持鼠标交互（点击、拖拽）
    // void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    // // 重写右键菜单事件
    // void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    // void showImage();

private:
    QRectF m_rect;       // 矩形区域
    QColor m_fillColor;  // 填充颜色
    QColor m_borderColor;// 边框颜色
    int m_borderWidth;   // 边框宽度
    std::string m_lang;
    std::string m_title;
    QPixmap m_image;  // 用来存储图像
};

class MyRectangleItem : public QGraphicsItem
{
public:
    explicit MyRectangleItem(QRectF rect, std::string title,std::string lang,
        QColor fillColor = Qt::blue, QColor borderColor = Qt::black, int borderWidth = 2);

    // 1️⃣ 定义矩形边界（用于碰撞检测、刷新区域等）
    QRectF boundingRect() const override {
        return m_rect.adjusted(-m_borderWidth / 2, -m_borderWidth / 2, m_borderWidth / 2, m_borderWidth / 2);
    }

private:
    // 2️⃣ 自定义绘制方法
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;

    // 3️⃣ 支持鼠标交互（点击、拖拽）
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    // 重写右键菜单事件
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    void showImage();

private:
    QRectF m_rect;       // 矩形区域
    QColor m_fillColor;  // 填充颜色
    QColor m_borderColor;// 边框颜色
    int m_borderWidth;   // 边框宽度
    std::string m_lang;
    std::string m_title;
    QPixmap m_image;  // 用来存储图像
};

// // 关键点（可拖动、可删除）
// class PointHandle : public QGraphicsEllipseItem {
//     public:
//         PointHandle(QGraphicsItem* parent = nullptr)
//             : QGraphicsEllipseItem(-5, -5, 10, 10, parent) {
//             setBrush(Qt::blue);
//             setFlag(QGraphicsItem::ItemIsMovable);
//             setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
//         }
    
//     protected:
//         QVariant itemChange(GraphicsItemChange change, const QVariant& value) override {
//             if (change == ItemPositionChange) {
//                 QPointF newPos = value.toPointF();
//                 newPos.setX(qRound(newPos.x() / 10) * 10);
//                 newPos.setY(qRound(newPos.y() / 10) * 10);
//                 return newPos;
//             }
//             return QGraphicsEllipseItem::itemChange(change, value);
//         }
    
//         void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override {
//             QMenu menu;
//             QAction* deleteAction = menu.addAction("删除关键点");
//             QAction* selectedAction = menu.exec(event->screenPos());
//             if (selectedAction == deleteAction) {
//                 delete this;
//             }
//         }
//     };
    
//     // 可编辑折线
//     class PolylineItem : public QGraphicsItem {
//     public:
//         PolylineItem(const QVector<QPointF>& points, QGraphicsItem* parent = nullptr)
//             : QGraphicsItem(parent) {
//             for (const QPointF& point : points) {
//                 addHandle(point);
//             }
//         }
    
//         QRectF boundingRect() const override {
//             return shape().boundingRect();
//         }
    
//         QPainterPath shape() const override {
//             QPainterPath path;
//             if (handles.isEmpty()) return path;
//             path.moveTo(handles.first()->pos());
//             for (int i = 1; i < handles.size(); ++i) {
//                 path.lineTo(handles[i]->pos());
//             }
//             return path;
//         }
    
//         void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override {
//             QPen pen(Qt::red, 2);
//             painter->setPen(pen);
//             QPolygonF poly;
//             for (auto handle : handles) {
//                 poly.append(handle->pos());
//             }
//             if (poly.size() > 1) {
//                 painter->drawPolyline(poly);  // 使用 drawPolyline 确保不会闭合
//             }
//         }
    
//     protected:
//         void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override {
//             QMenu menu;
//             QAction* addAction = menu.addAction("增加关键点");
//             QAction* selectedAction = menu.exec(event->screenPos());
//             if (selectedAction == addAction) {
//                 addHandle(event->scenePos());
//             }
//         }
    
//     private:
//         QVector<PointHandle*> handles;
//     // 计算点到线段的最短距离
// qreal pointToLineDistance(const QPointF& pt, const QLineF& line) {
//     // 计算投影点
//     qreal len2 = line.length() * line.length();
//     if (len2 == 0.0) return QLineF(pt, line.p1()).length(); // 线段退化为一个点

//     qreal t = ((pt.x() - line.p1().x()) * (line.p2().x() - line.p1().x()) +
//                (pt.y() - line.p1().y()) * (line.p2().y() - line.p1().y())) / len2;

//     if (t < 0.0) return QLineF(pt, line.p1()).length(); // 最近点是 p1
//     if (t > 1.0) return QLineF(pt, line.p2()).length(); // 最近点是 p2

//     QPointF projection = line.pointAt(t);
//     return QLineF(pt, projection).length();
// }
//         void addHandle(const QPointF& pos) {
//             int insertIndex = 0;
//             qreal minDist = std::numeric_limits<qreal>::max();
        
//             for (int i = 0; i < handles.size() - 1; ++i) {
//                 QLineF segment(handles[i]->pos(), handles[i + 1]->pos());
//                 qreal dist = pointToLineDistance(pos, segment);
//                 if (dist < minDist) {
//                     minDist = dist;
//                     insertIndex = i + 1;
//                 }
//             }
        
//             auto* handle = new PointHandle(this);
//             handle->setPos(pos);
//             handles.insert(insertIndex, handle);
//             update();
//         }
//     };


#endif // MYITEM_H