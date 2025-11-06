/*
**
**	版权:	
**
**	名称:	rectitem.h
**
**	作者:	侯 晓 琴
**
**	日期:	2022/10/20
**
**	描述:
**
**	功能:	流程编辑节点文本类
**
**	备注:
**
*/

#ifndef RECTITEM_H
#define RECTITEM_H


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
#include <QBrush>
#include <QCursor>
#include <QtMath>
#include <QDebug>
#include <QRect>
class  TextItem : public QGraphicsSimpleTextItem
{
public:
    TextItem(QGraphicsItem *parent_item = Q_NULLPTR);
    ~TextItem() override;
    QRectF boundingRect() const;
    QPainterPath shape() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};
class  ControlPosItem : public QObject,public QGraphicsItem
{
    Q_OBJECT
public:
    explicit  ControlPosItem(int index, QGraphicsItem *parent_item = Q_NULLPTR);

    int index() const;
    void setIndex(int newIndex);

protected:
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    //
    qreal _radius;
    int _index;
signals:
    void ControlPosItemchang();
};

class RectItem : public QObject,public QGraphicsItem
{
    Q_OBJECT
public:
    /*explicit  */RectItem(const QPointF &start,const QPointF &end,const QString &name,QGraphicsItem *parent = Q_NULLPTR);
    //virtual
    ~RectItem() override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

//    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

    bool adjustArea(int index, const QPointF &scenePos);
    void resetRect();
    void init(const QString &text,const QList<QPointF> &pointFs, QColor& color);
    QString GetName();

    void ctrlPosPressed(int index);
    void ctrlPosRelease(int index);
    void returnpos();
    const QList<ControlPosItem *> &ctrlItems() const;
    void setrectbrush(bool);
signals:
    void returnpossignal();
    void rectitemdoubleclicke(QString);
    void rightsignal(RectItem*);
private slots:
    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) override;
    void returnposslot();
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
private:
    QList<ControlPosItem *>_ctrlItems; //4个控制点(左上，右上，右下，左下)
    QList<ControlPosItem *>_ctrlMidItems; //4个控制点中间的控制点
    int _index {-1};
    QList<QPointF> _oldCtrlPosList;
    TextItem * _textItem; //文本 item
    QColor m_currentColor;
    QColor m_currentcolorchang;
    bool brushflag;
};

#endif // RECTITEM_H
