/*
    @file   rectellipseitem.cpp
    @brief  Implementation file
    @author XiaoQin.Hou
*/
#include "rectellipseitem.h"
#include <QStyleOption>

//中点坐标
 QPointF midPos(const QPointF &p1,const QPointF &p2)
{
    return QPointF((p1.x()+p2.x())/2.0,(p1.y()+p2.y())/2.0);
}
 qreal twoPosLength(const QPointF &p1, const QPointF &p2)
{
    return sqrt((p2.x() - p1.x())*(p2.x() - p1.x()) + (p2.y() - p1.y())*(p2.y() - p1.y()));
}
//有bug
//static qreal ajustAngle(qreal angle)
//{
//    if(angle > 360){
//        return angle - 360;
//    }
//    if(angle <-360){
//        return angle + 360;
//    }
//}
//参数（起点坐标，角度，斜边长（距离）） 这是一个基本的三角函数应用
static QPointF getTrianglePoint(QPointF pointB,qreal angle, qreal bevel)
{
    //顺时针角度为正，逆时针角度为负
    //将角度转为弧度
    qreal radian = angle * M_PI / 180;
    qreal xMargin = cos(radian) * bevel;
    qreal yMargin = sin(radian) * bevel;
    return  QPointF(pointB.x() + xMargin,pointB.y() + yMargin);
}
static void vRotationTransform(double dX, double dY, double dAngle, double &dbNewX, double &dbNewY)
{
    dbNewX = dX * cos(dAngle) - dY * sin(dAngle);
    dbNewY = dX * sin(dAngle) + dY * cos(dAngle);
}
static QPointF vRotationTransform(const QPointF &pos, double dAngle)
{
    double dbNewX = pos.x() * cos(dAngle) - pos.y() * sin(dAngle);
    double dbNewY = pos.x() * sin(dAngle) + pos.y() * cos(dAngle);
    return QPointF(dbNewX,dbNewY);
}

RectEllipseItem::RectEllipseItem(QGraphicsItem *parent)
    :QGraphicsItem(parent)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setAcceptHoverEvents(true);
    _isInit = false;
    _radius = 5.0; //控制点尺寸px
    _isLeftTop = false;
    _isEllipsePos = false;
    _isTransformPos = false;
    _isRightBottom = false;
    _isDrawCtrlPos = false;
    _tranAngle = 0;
    _isMove = false;
    _isMoveItem = false;
    _ellipsePosRand = 0;
}

RectEllipseItem::~RectEllipseItem()
{
}
QRectF RectEllipseItem::boundingRect() const
{
    return shape().boundingRect();
}
QPainterPath RectEllipseItem::shape() const
{
    QPainterPath path;
    QPolygonF plygonF;
    QRectF tRectF(0,0,_radius,_radius);
    tRectF.moveCenter(_leftTop);
    auto tempTopLeft = tRectF.topLeft();
    plygonF.append(tempTopLeft);

    tRectF.moveCenter(_leftBottom);
    plygonF.append(tRectF.bottomLeft());

    tRectF.moveCenter(_rightBottom);
    plygonF.append(tRectF.bottomRight());

    tRectF.moveCenter(_rightTop);
    plygonF.append(tRectF.topRight());
    plygonF.append(tempTopLeft);
    path.addPolygon(plygonF);
    return path;
}
qreal RectEllipseItem::height() const
{
    return this->boundingRect().height();
}
qreal RectEllipseItem::width() const
{
    return this->boundingRect().width();
}
void RectEllipseItem::init()
{
    QRectF tempRectF(0,0,600,600);
    _leftTop = tempRectF.topLeft(); //左上控制点
    _leftBottom = tempRectF.bottomLeft(); //左下

    _rightTop = tempRectF.topRight(); //右上
    _rightBottom = tempRectF.bottomRight(); //右下控制点

    _transformPos = midPos(_rightTop,_rightBottom); //旋转控制点
    QLineF lineF(_rightBottom,_leftTop);
    _initRightBottomLeftTopAngle = lineF.angle();
    //qDebug()<<__FUNCTION__<<__LINE__<<"_initRightBottomLeftTopAngle:"<<_initRightBottomLeftTopAngle;
    _tempTranLeftTop = _leftTop;
    _tempTranRightBottom = _rightBottom;
    _tranAngle = 0;
    QRectF rect;
    rect.setTopLeft(_leftTop);
    rect.setBottomRight(_rightBottom);
    _ellipsePos = rect.center(); //椭圆控制点
}
void RectEllipseItem::init(qreal tranAngle,
                           const QPointF &leftTop,
                           const QPointF &leftBottom,
                           const QPointF &rightTop,
                           const QPointF &rightBottom,
                           const QPointF &ellipsePos,
                           const QPointF &transformPos,
                           const QString &text)
{
    _leftTop = leftTop; //左上控制点
    _leftBottom = leftBottom; //左下

    _rightTop = rightTop; //右上
    _rightBottom = rightBottom; //右下控制点

    _transformPos = transformPos; //旋转控制点
    _ellipsePos = ellipsePos;
    _tranAngle = tranAngle;

    QRectF rect = resetRect(leftTop,rightBottom,_tranAngle);
    _tempTranLeftTop = rect.topLeft();
    _tempTranRightBottom = rect.bottomRight();
    _text = text;
}
void RectEllipseItem::init(const QString &text,
                           qreal tranAngle,
                           const QPointF &centers,
                           qreal halfWidth,
                           qreal halfHeight,
                           const QPointF &ellipsePoss)
{//传入scence坐标
    QPointF center = this->mapFromScene(centers);
    QPointF ellipsePos = this->mapFromScene(ellipsePoss);
    _tranAngle = tranAngle;
    _ellipsePos = ellipsePos;

    QLineF lineF0(QPointF(0,10),QPointF(10,10));
    QLineF lineF;
    lineF.setP1(center);
    lineF.setLength(halfWidth);
    lineF.setAngle(tranAngle+lineF0.angle());
    _transformPos = lineF.p2();
    lineF.setAngle(lineF0.angle()+tranAngle+180.0);
    auto leftMid = lineF.p2();

    QLineF lineF1(leftMid,center);
    lineF1.setAngle(lineF1.angle()+90.0);
    lineF1.setLength(halfHeight);
    _leftTop = lineF1.p2();

    lineF1.setAngle(lineF1.angle()+180.0);
    _leftBottom = lineF1.p2();

    QLineF lineF2(_transformPos,center);
    lineF2.setLength(halfHeight);
    lineF2.setAngle(lineF2.angle()-90.0);
    _rightTop = lineF2.p2();

    lineF2.setAngle(lineF2.angle()+180.0);
    _rightBottom = lineF2.p2();

    QRectF rect = resetRect(_leftTop,_rightBottom,_tranAngle);
    _tempTranLeftTop = rect.topLeft();
    _tempTranRightBottom = rect.bottomRight();
    _text = text;
    //TOD:
    center = midPos(_leftTop,_rightBottom);
    leftMid = midPos(_leftTop,_leftBottom);
    auto l = twoPosLength(center,leftMid);
    auto length = twoPosLength(center,_ellipsePos);
    QPainterPath path;
    auto l2 = 0;
    if (fabs(l)>0.0001) l2=length/l;
    if(l2>l)
        l2 = 1.0;
    _ellipsePosRand = l2;
}

int RectEllipseItem::setMousePos(const QPointF &pos)
{
    QRectF rect(0,0,_radius,_radius);
    rect.moveCenter(_leftTop);
    if(rect.contains(pos)){
        this->setCursor(Qt::CrossCursor);
        return 1;
    }
    //椭圆控制点
    rect.moveCenter(_ellipsePos);
    if(rect.contains(pos)){
        this->setCursor(Qt::CrossCursor);
        return 2;
    }
    //旋转控制点
    rect.moveCenter(_transformPos);
    if(rect.contains(pos)){
        this->setCursor(Qt::CrossCursor);
        return 3;
    }

    rect.moveCenter(_rightBottom);
    if(rect.contains(pos)){
        this->setCursor(Qt::CrossCursor);
        return 4;
    }
    return 0;
}
void RectEllipseItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverEnterEvent(event);
}
void RectEllipseItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverLeaveEvent(event);
}
QVariant RectEllipseItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(change == ItemSelectedHasChanged ){
        //显示隐藏控制点
        _isDrawCtrlPos = value.toBool();
        this->update();
    }
    return QGraphicsItem::itemChange(change,value);
}
void RectEllipseItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    auto pos = event->pos();
    int index = setMousePos(pos);
    _isMoveItem = false;
    if(0 == index){
        this->setCursor(Qt::SizeAllCursor);
        _isMoveItem = true;
        QGraphicsItem::mousePressEvent(event);
        return;
    }
    _pressPos = pos;
    auto center = midPos(_leftTop,_rightBottom);
    if(1 == index){
        _isLeftTop = true;
        //        _isDrawCtrlPos = true;
    }else if(2 == index){
        _isEllipsePos = true;
        //        _isDrawCtrlPos = true;

    }else if(3 == index){
        QLineF lineF(center,_transformPos);
        _tempBeforeTranAngle = lineF.angle();
//        qDebug()<<"_tempBeforeTranAngle:"<<_tempBeforeTranAngle;
        _isTransformPos = true;
        //        _isDrawCtrlPos = true;

    }else if(4 == index){
        _isRightBottom = true;
        //        _isDrawCtrlPos = true;

    }else if(5 == index){
        //                _isDrawCtrlPos = true;
    }else if(6 == index){
        //                _isDrawCtrlPos = true;
    }else if(7 == index){
        //        _isMove = true;
        //        _isDrawCtrlPos = true;
    }else{
        //        _isDrawCtrlPos = false;
    }
    {//重置
        auto center = midPos(_leftTop,_rightBottom);
        QRectF rect = resetRect(_leftTop,_rightBottom,_tranAngle);

        _tempLeftTop = rect.topLeft();
        _tempRightBottom = rect.bottomRight();

        QLineF lineF5(center,rect.bottomLeft());
        //        lineF5.setAngle(ajustAngle(lineF5.angle()+_tranAngle));
        lineF5.setAngle(lineF5.angle()+_tranAngle);
        auto leftBottom = lineF5.p2();

        auto leftMid = midPos(_leftTop,leftBottom);
        QPainterPath path;
        path.moveTo(center);
        path.lineTo(leftMid);

        _tempEllipsePos = path.pointAtPercent(_ellipsePosRand);
        _tempEllipseRadius = twoPosLength(leftMid,_tempEllipsePos);
    }
    this->update();
}
void RectEllipseItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverMoveEvent(event);
    int index = setMousePos(event->pos());
    if(0 == index){
        this->unsetCursor();
    }
}
void RectEllipseItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{

    if(_isMoveItem){
        QGraphicsItem::mouseMoveEvent(event);
        return;
    }
    auto pos = event->pos();
    int index = setMousePos(pos);


    if(_isMove){
        auto tPos = pos - _pressPos;
        _pressPos = pos;
        _tempTranLeftTop = _tempTranLeftTop + tPos;
        _tempTranRightBottom = _tempTranRightBottom + tPos;

        _tempLeftTop = _leftTop = _leftTop + tPos; //左上控制点
        _leftBottom = _leftBottom + tPos; //左下
        _rightTop = _rightTop + tPos; //右上
        _tempRightBottom = _rightBottom = _rightBottom + tPos; //右下控制点

        _transformPos = midPos(_rightTop,_rightBottom); //旋转控制点
        _tempEllipsePos = _tempEllipsePos + tPos;
        _ellipsePos = _ellipsePos + tPos;
        this->update();
    }
    if(_isLeftTop){
        _tempLeftTop = pos;
        auto center = midPos(pos,_rightBottom);
        QRectF rect = resetRect(pos,_rightBottom,_tranAngle);
        _tempLeftTop = rect.topLeft();
        _tempRightBottom = rect.bottomRight();

        QLineF lineF5(center,rect.bottomLeft());
        //          lineF5.setAngle(ajustAngle(lineF5.angle()+_tranAngle));
        lineF5.setAngle(lineF5.angle()+_tranAngle);
        auto leftBottom = lineF5.p2();

        auto leftMid = midPos(_leftTop,leftBottom);
        QPainterPath path;
        path.moveTo(center);
        path.lineTo(leftMid);
        _tempEllipsePos = path.pointAtPercent(_ellipsePosRand);
        _tempEllipseRadius = twoPosLength(leftMid,_tempEllipsePos);
        this->update();

    }else if(_isEllipsePos){
        QPointF center = midPos(_leftTop,_rightBottom);
        auto leftMid = midPos(_leftTop,_leftBottom);
        auto l = twoPosLength(center,leftMid);
        auto length = twoPosLength(center,pos);
        QPainterPath path;
        if (fabs(l)>0.0001) l = length/l;
        if(l>1){
            l = 1;
        }
        path.moveTo(center);
        path.lineTo(leftMid);
        _ellipsePos = path.pointAtPercent(l);
        this->update();

    }else if(_isTransformPos){
        QPointF center = midPos(_leftTop,_rightBottom);
        qreal width = twoPosLength(_leftTop,_rightTop);
        QLineF lineF(center,pos);
        lineF.setLength(width/2.0);
        _transformPos = lineF.p2();
        QLineF lineF2(center,_transformPos);
        QRectF rect = resetRect(_leftTop,_rightBottom,_tranAngle);
        _tempTranAngle = lineF2.angle();
        //        qDebug()<<"_tempTranAngle:"<<_tempTranAngle;
        _tempTranLeftTop = rect.topLeft();
        _tempTranRightBottom = rect.bottomRight();

        QLineF lineF5(center,rect.bottomLeft());
        //        lineF5.setAngle(ajustAngle(lineF5.angle()+_tranAngle));
        lineF5.setAngle(lineF5.angle()+_tranAngle);
        auto leftBottom = lineF5.p2();
        auto leftMid = midPos(_leftTop,leftBottom);
        QPainterPath path;
        path.moveTo(center);
        path.lineTo(leftMid);
        _tempEllipsePos = path.pointAtPercent(_ellipsePosRand);
        _tempEllipseRadius = twoPosLength(leftMid,_tempEllipsePos);
        this->update();

    }else if(_isRightBottom){
        _tempRightBottom = pos;
        auto center = midPos(_leftTop,pos);
        auto rect = resetRect(_leftTop,pos,_tranAngle);
        _tempLeftTop = rect.topLeft();
        _tempRightBottom = rect.bottomRight();

        QLineF lineF5(center,rect.bottomLeft());
        //          lineF5.setAngle(ajustAngle(lineF5.angle()+_tranAngle));
        lineF5.setAngle(lineF5.angle()+_tranAngle);
        auto leftBottom = lineF5.p2();

        auto leftMid = midPos(_leftTop,leftBottom);
        QPainterPath path;
        path.moveTo(center);
        path.lineTo(leftMid);
        _tempEllipsePos = path.pointAtPercent(_ellipsePosRand);
        _tempEllipseRadius = twoPosLength(leftMid,_tempEllipsePos);
        this->update();
    }
}

void RectEllipseItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

    if(_isMoveItem){
        QGraphicsItem::mouseReleaseEvent(event);
        _isMoveItem = false;
        this->setCursor(Qt::ArrowCursor);
//        qDebug()<<"===============move item ======================";
//        qDebug()<<"item.pos:"<<this->pos()<<this->mapToScene(this->pos());
//        qDebug()<<"_tranAngle:"<<_tranAngle;
//        qDebug()<<"_leftTop:"<<_leftTop<<this->mapToScene(_leftTop);
//        qDebug()<<"_leftBottom:"<<_leftBottom<<this->mapToScene(_leftBottom);
//        qDebug()<<"_rightTop:"<<_rightTop<<this->mapToScene(_rightTop);
//        qDebug()<<"_rightBottom:"<<_rightBottom <<this->mapToScene(_rightBottom);
//        qDebug()<<"_ellipsePos:"<<_ellipsePos<<this->mapToScene(_ellipsePos);
//        qDebug()<<"_transformPos:"<<_transformPos<<this->mapToScene(_transformPos);
//        qDebug()<<"_xRnd:"<<_xRnd<<"_yRnd:"<<_yRnd;
//        qDebug()<<"--------ROI------------";
//        qDebug()<<"_tranAngle:"<<_tranAngle;
        auto center = midPos(_leftTop,_rightBottom);
//        qDebug()<<"center:"<<this->mapToScene(center);
//        qDebug()<<"halfWidth:"<<twoPosLength(_leftTop,_rightTop)/2.0;
//        qDebug()<<"halfHeight:"<<twoPosLength(_leftTop,_leftBottom)/2.0;
//        qDebug()<<"round:"<<this->mapToScene(_ellipsePos);

        emit senddatasignal(_text,_tranAngle,this->mapToScene(center),twoPosLength(_leftTop,_rightTop)/2.0,\
                      twoPosLength(_leftTop,_leftBottom)/2.0,this->mapToScene(_ellipsePos));
        return;
    }
    auto pos = event->pos();

    if(_isLeftTop){
        _leftTop = pos;
        auto center = midPos(pos,_rightBottom);
        auto rect = resetRect(pos,_rightBottom,_tranAngle);
        _tempTranLeftTop = rect.topLeft();
        _tempTranRightBottom = rect.bottomRight();

        QLineF lineF5(center,rect.bottomLeft());
        lineF5.setAngle(lineF5.angle()+_tranAngle);
        _leftBottom = lineF5.p2();

        QLineF lineF6(center,rect.topRight());
        lineF6.setAngle(lineF6.angle()+_tranAngle);
        _rightTop = lineF6.p2();
        //重置旋转控制点位置
        _transformPos = midPos(_rightTop,_rightBottom);
        //重置椭圆控制点位置
        auto leftMid = midPos(_leftTop,_leftBottom);
        QPainterPath path;
        path.moveTo(center);
        path.lineTo(leftMid);
        _ellipsePos = path.pointAtPercent(_ellipsePosRand);

    }else if(_isEllipsePos){
        auto center = midPos(_leftTop,_rightBottom);
        auto leftMid = midPos(_leftTop,_leftBottom);
        auto l = twoPosLength(center,leftMid);
        auto length = twoPosLength(center,pos);
        QPainterPath path;
        if (fabs(l)>0.0001) l = length/l;
        if(l>1){
            l = 1;
        }
        path.moveTo(center);
        path.lineTo(leftMid);
        _ellipsePosRand = l;
        _ellipsePos = path.pointAtPercent(l);

    }else if(_isTransformPos){
        auto center = midPos(_leftTop,_rightBottom);

        QLineF lineF(center,pos);
        auto rect = resetRect(_leftTop,_rightBottom,_tranAngle);
//        qDebug()<<__FUNCTION__<<__LINE__<<rect;
        _tranAngle = lineF.angle();
        _tempTranLeftTop = rect.topLeft();
        _tempTranRightBottom = rect.bottomRight();

        QLineF lineF3(center,rect.topLeft());
        lineF3.setAngle(lineF3.angle()+_tranAngle);
        _leftTop = lineF3.p2();
        QLineF lineF4(center,rect.bottomRight());
        lineF4.setAngle(lineF4.angle()+_tranAngle);
        _rightBottom = lineF4.p2();

        QLineF lineF5(center,rect.bottomLeft());
        lineF5.setAngle(lineF5.angle()+_tranAngle);
        _leftBottom = lineF5.p2();

        QLineF lineF6(center,rect.topRight());
        lineF6.setAngle(lineF6.angle()+_tranAngle);
        _rightTop = lineF6.p2();
        //重置旋转控制点位置
        _transformPos = midPos(_rightTop,_rightBottom);
        //重置椭圆控制点位置
        auto leftMid = midPos(_leftTop,_leftBottom);
        QPainterPath path;
        path.moveTo(center);
        path.lineTo(leftMid);
        _ellipsePos = path.pointAtPercent(_ellipsePosRand);

    }else if(_isRightBottom){

        _rightBottom = pos;
//        qDebug()<<__FUNCTION__<<__LINE__<<"_tranAngle:"<<_tranAngle;
        auto center = midPos(_leftTop,pos);
        auto rect = resetRect(_leftTop,_rightBottom,_tranAngle);
        _tempTranLeftTop = rect.topLeft();
        _tempTranRightBottom = rect.bottomRight();
        QLineF lineF5(center,rect.bottomLeft());
        lineF5.setAngle(lineF5.angle()+_tranAngle);
        _leftBottom = lineF5.p2();

        QLineF lineF6(center,rect.topRight());
        lineF6.setAngle(lineF6.angle()+_tranAngle);
        _rightTop = lineF6.p2();
        //重置旋转控制点位置
        _transformPos = midPos(_rightTop,_rightBottom);
        //重置椭圆控制点位置
        auto leftMid = midPos(_leftTop,_leftBottom);
        QPainterPath path;
        path.moveTo(center);
        path.lineTo(leftMid);
        _ellipsePos = path.pointAtPercent(_ellipsePosRand);
    }
    _isLeftTop = false;
    _isEllipsePos = false;
    _isTransformPos = false;
    _isRightBottom = false;
    _isMove = false;
    _isMoveItem = false;
//    qDebug()<<"============move ctrl pos=========================";
//    qDebug()<<"_tranAngle:"<<_tranAngle;
//    qDebug()<<"_leftTop:"<<_leftTop<<this->mapToScene(_leftTop);
//    qDebug()<<"_leftBottom:"<<_leftBottom<<this->mapToScene(_leftBottom);
//    qDebug()<<"_rightTop:"<<_rightTop<<this->mapToScene(_rightTop);
//    qDebug()<<"_rightBottom:"<<_rightBottom <<this->mapToScene(_rightBottom);
//    qDebug()<<"_ellipsePos:"<<_ellipsePos<<this->mapToScene(_ellipsePos);
//    qDebug()<<"_transformPos:"<<_transformPos<<this->mapToScene(_transformPos);
//    qDebug()<<"_xRnd:"<<_xRnd<<"_yRnd:"<<_yRnd;
    auto center = midPos(_leftTop,_rightBottom);
    emit senddatasignal(_text,_tranAngle,this->mapToScene(center),twoPosLength(_leftTop,_rightTop)/2.0,\
                  twoPosLength(_leftTop,_leftBottom)/2.0,this->mapToScene(_ellipsePos));
    this->setCursor(Qt::ArrowCursor);
    this->update();
}
QRectF RectEllipseItem::resetRect(const QPointF &rectLeftTop,
                                  const QPointF &rectRightBottom,
                                  qreal tranAngle) const
{
    QPointF center = midPos(rectLeftTop,rectRightBottom);
    QLineF lineF3(center,rectLeftTop);
    auto a3 = lineF3.angle()-tranAngle;
    //    qDebug()<<"lineF3.angle():"<<lineF3.angle();
    lineF3.setAngle(a3);
    auto leftTop = lineF3.p2();

    QLineF lineF4(center,rectRightBottom);
    //    qDebug()<<"lineF4.angle():"<<lineF4.angle();
    auto a4 = lineF4.angle()-tranAngle;
    lineF4.setAngle(a4);
    auto rightBottom = lineF4.p2();

    QRectF rect;
    rect.setTopLeft(leftTop);
    rect.setBottomRight(rightBottom);
    return rect;
}

QRectF RectEllipseItem::resetRect()  const
{
    auto rectF = resetRect(_leftTop,_rightBottom,_tranAngle);
    return rectF;
}

void RectEllipseItem::setText(QString text)
{
    _text = text;
}
void RectEllipseItem::drawArrow(QPainter *painter,const QLineF &lineF1)
{
    //绘制箭头
    qreal arrowSize = 15.0;//15.0
    double angle = std::atan2(-lineF1.dy(), lineF1.dx());
    QPointF destArrowP1 = lineF1.p2() + QPointF(sin(angle - M_PI / 3) * arrowSize,
                                                cos(angle - M_PI / 3) * arrowSize);
    QPointF destArrowP2 = lineF1.p2() + QPointF(sin(angle - M_PI + M_PI / 3) * arrowSize,
                                                cos(angle - M_PI + M_PI / 3) * arrowSize);
    painter->setBrush(Qt::green);
    //只绘制单箭头
    painter->drawPolygon(QPolygonF() << lineF1.p2() << destArrowP1 << destArrowP2);
}


// 绕矩形的中心旋转矩形
void RectEllipseItem::rotateAndPaintRect(QPainter *painter, const QRectF &rect, qreal angle,int xRnd,int yRnd)
{
    QRectF rotatedRect(-rect.width()/2.0, -rect.height()/2.0, rect.width(), rect.height());
    qreal cx = rect.x() + rect.width() / 2.0;
    qreal cy = rect.y() + rect.height() / 2.0;

    painter->save();
    painter->translate(cx, cy);
    painter->rotate(angle);
    painter->drawRoundedRect(rotatedRect,xRnd,yRnd);
    painter->restore();
}

void RectEllipseItem::initEllipsePosRand()
{
    QPointF center = midPos(_leftTop,_rightBottom);
    auto leftMid = midPos(_leftTop,_leftBottom);
    auto l = twoPosLength(center,leftMid);
    auto length = twoPosLength(center,_ellipsePos);
    QPainterPath path;
    auto l2 = 0;
    if (fabs(l)>0.0001) l2=length/l;
    if(l2>l)
        l2 = 1.0;
    _ellipsePosRand = l2;
}
void RectEllipseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //背景颜色
    //    painter->setBrush(QBrush(QColor(0,0,255,100)));
    //    painter->drawPath(shape());
    double scaleFactor = painter->transform().m11();

    QRectF rect;
    rect.setTopLeft(_tempTranLeftTop);
    rect.setBottomRight(_tempTranRightBottom);
    //中心点
    auto center = midPos(_leftTop,_rightBottom);

    painter->drawEllipse(center,5,5);
    { //绘制椭圆
        painter->setBrush(Qt::NoBrush);
        QPen pen;
        pen.setColor(Qt::yellow);
        if (fabs(scaleFactor)>0.0001) pen.setWidth(2/scaleFactor);
        else pen.setWidth(1);
        painter->setPen(pen);
        auto leftMid = midPos(_leftTop,_leftBottom);
        qreal length = twoPosLength(leftMid,_ellipsePos);
        _xRnd = _yRnd = length;
        rotateAndPaintRect(painter,rect,-_tranAngle,length,length);
    }
    if(_isLeftTop || _isRightBottom){
        painter->setBrush(Qt::NoBrush);
        QPen pen;
        pen.setColor(Qt::magenta);
        if (fabs(scaleFactor)>0.0001) pen.setWidth(2/scaleFactor);
        else pen.setWidth(1);
        painter->setPen(pen);
        QRectF rect;
        rect.setTopLeft(_tempLeftTop);
        rect.setBottomRight(_tempRightBottom);
        rotateAndPaintRect(painter,rect,-_tranAngle,_tempEllipseRadius,_tempEllipseRadius);
    }else if(_isTransformPos){
        painter->setBrush(Qt::NoBrush);
        QPen pen;
        pen.setColor(Qt::magenta);
        if (fabs(scaleFactor)>0.0001) pen.setWidth(2/scaleFactor);
        else pen.setWidth(1);
        painter->setPen(pen);
        rotateAndPaintRect(painter,rect,-_tempTranAngle,_tempEllipseRadius,_tempEllipseRadius);
        painter->drawLine(center,_transformPos);
    }
    //绘制绿色
    {
        painter->setPen(QPen(Qt::red,2));
        //左中
        auto leftMid = midPos(_leftTop,_leftBottom);
        QPen pen;
        pen.setColor(Qt::green);
        if (fabs(scaleFactor)>0.0001) pen.setWidth(2/scaleFactor);
        else pen.setWidth(1);
        painter->setPen(pen);
        QLineF lineF1(leftMid,_leftBottom);
        painter->drawLine(lineF1);
        auto rightMid = midPos(_rightBottom,_rightTop);
        QLineF lineF2(leftMid,rightMid);
        painter->drawLine(lineF2);
        drawArrow(painter,lineF1);
        drawArrow(painter,lineF2);
    }
    //option->state & QStyle::State_MouseOver
    if(_isDrawCtrlPos){
        //绘制控制点
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::magenta);
        //矩形
        QRectF rect(0,0,_radius*2.0,_radius*2.0);
        rect.moveCenter(_leftTop);
        if (fabs(scaleFactor)>0.0001) {
            painter->drawRoundedRect(rect,50/scaleFactor,50/scaleFactor);
            rect.moveCenter(_ellipsePos);
            painter->drawRoundedRect(rect,50/scaleFactor,50/scaleFactor);
            rect.moveCenter(_transformPos);
            painter->drawRoundedRect(rect,50/scaleFactor,50/scaleFactor);
            rect.moveCenter(_rightBottom);
            painter->drawRoundedRect(rect,50/scaleFactor,50/scaleFactor);
        }
        //圆
        painter->setPen(QColor(0,0,0,200));
        painter->setBrush(QColor(255,0,255));
//        painter->scale(scaleFactor,scaleFactor);

        //        if(option->state & QStyle::State_MouseOver){
        //            painter->setBrush(QColor(255,0,255,200));
        //        }

        //        painter->drawEllipse(_leftTop,_radius,_radius);
        //        painter->drawEllipse(_ellipsePos,_radius,_radius);
        //        painter->drawEllipse(_transformPos,_radius,_radius);
        //        painter->drawEllipse(_rightBottom,_radius,_radius);
    }
    painter->setBrush(Qt::yellow);
    painter->drawText(_rightBottom,_text);
}


QString RectEllipseItem::getText()
{
    return _text;
}

qreal RectEllipseItem::tranAngle() const
{
    return _tranAngle;
}
