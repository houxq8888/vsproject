/*
**
**	版权:	
**
**	名称:	rectellipseitem.h
**
**	作者:	侯 晓 琴
**
**	日期:	2022/10/20
**
**	描述:
**
**	功能:	流程编辑节点控制点类
**
**	备注:
**
*/

#ifndef RECTELLIPSEITEM_H
#define RECTELLIPSEITEM_H


#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QPointer>
#include <QGraphicsTextItem>
#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QCursor>
#include <QtMath>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>
class RectEllipseItem : public QObject,public QGraphicsItem
{
    Q_OBJECT
public:
    RectEllipseItem(QGraphicsItem *parent = Q_NULLPTR );
    //virtual
    ~RectEllipseItem() override;
    void init();
    int setMousePos(const QPointF &pos);
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    QRectF resetRect(const QPointF &rectLeftTop, const QPointF &rectRightBottom, qreal tranAngle) const;
    void rotateAndPaintRect(QPainter *painter, const QRectF &rect, qreal angle, int xRnd, int yRnd);
    void initEllipsePosRand();
    void drawArrow(QPainter *painter, const QLineF &lineF1);
    qreal height() const;
    qreal width() const;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    void init(qreal tranAngle, const QPointF &leftTop, const QPointF &leftBottom, const QPointF &rightTop, const QPointF &rightBottom, const QPointF &ellipsePos, const QPointF &transformPos, const QString &text);
    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value);
    qreal tranAngle() const;

    QRectF resetRect() const;
    void setText(QString);
    QString getText();
    void init(const QString &text, qreal tranAngle, const QPointF &center, qreal halfWidth, qreal halfHeight, const QPointF &ellipsePos);
signals:
    void senddatasignal(QString text, qreal tranAngle, QPointF center, qreal halfWidth, qreal halfHeight, QPointF ellipsePos);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
public:

    int _xRnd;
    int _yRnd;
    QString _text;
    bool _isInit ;
    //移动窗口
    bool _isMove;//移动窗口标志
    bool _isMoveItem; //移动图元
    //---------
    bool _isDrawCtrlPos; //是否绘制控制点，默认绘制
    QPointF _leftTop; //左上控制点
    QPointF _tempLeftTop;

    QPointF _ellipsePos; //椭圆大小控制点
    QPointF _tempEllipsePos;
    qreal _tempEllipseRadius;
    qreal _ellipsePosRand;

    QPointF _transformPos;//旋转控制点
    QPointF _tempTranLeftTop;

    QPointF _tempTranRightBottom;

    QPointF _rightBottom; //右下控制点
    QPointF _tempRightBottom;

    QPointF _leftBottom; //左下
    QPointF _rightTop; //右上
    //--
    QPointF _pressPos; //鼠标按下
    //---
    bool _isLeftTop;
    bool _isEllipsePos;
    bool _isTransformPos;
    bool _isRightBottom;
    //
    qreal _radius; //点放大成圆半径
    qreal _tranAngle; //旋转角度
    qreal _tempTranAngle;
    qreal _tempBeforeTranAngle; //旋转前角度
    qreal _initRightBottomLeftTopAngle;//初始右下与左上在坐标系中角度
    qreal _tranRightBottomleftTopAngle;//旋转后右下与左上在坐标系中角度
private:
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
};

#endif // RECTELLIPSEITEM_H
