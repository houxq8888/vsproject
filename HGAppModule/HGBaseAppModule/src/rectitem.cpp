/*
    @file   rectitem.cpp
    @brief  Implementation file
    @author XiaoQin.Hou
*/
#include "rectitem.h"

//中点坐标
static QPointF midPos(const QPointF &p1,const QPointF &p2)
{
    return QPointF((p1.x()+p2.x())/2.0,(p1.y()+p2.y())/2.0);
}
static qreal twoPosLength(const QPointF &p1, const QPointF &p2)
{
    return sqrt((p2.x() - p1.x())*(p2.x() - p1.x()) + (p2.y() - p1.y())*(p2.y() - p1.y()));
}
TextItem::TextItem(QGraphicsItem *parent_item):
    QGraphicsSimpleTextItem(parent_item)
{
    setBrush(QBrush(Qt::yellow));
}
TextItem::~TextItem()
{

}
QRectF TextItem::boundingRect() const
{
    if (shape().isEmpty()) return QRectF();
    else return shape().boundingRect();
}
QPainterPath TextItem::shape() const
{
    QPainterPath path;
    QFontMetrics fm(this->font());
    int w = fm.horizontalAdvance(this->text());
    int h = fm.height();
    path.addRect(QRectF(1,1,w+18,h+6));
    return path;
}
void TextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    double scalerFator = painter->transform().m11();

    painter->setPen(QColor(QColor(0,0,0)));
    painter->setBrush(QColor(Qt::yellow));

    if(option->state & QStyle::State_MouseOver){
        painter->setPen(QColor(Qt::yellow));
        painter->setBrush(QColor(255,0,255,200));
    }
    painter->drawRect(this->boundingRect());
    painter->drawText(this->boundingRect(),this->text(),QTextOption(Qt::AlignCenter));
    painter->scale(scalerFator,scalerFator);
}
//////////////////////////////////////////////////////
/// \brief ControlPosItem::ControlPosItem
/// \param index
/// \param parent_item
///
ControlPosItem::ControlPosItem(int index,QGraphicsItem *parent_item):
    QGraphicsItem(parent_item)
{
    //忽略缩放
    setFlag(ItemIgnoresTransformations);
    _radius = 5.0;
    setIndex(index);
    if(0 == index){
        setCursor(QCursor(Qt::SizeFDiagCursor));
    }else if(1 == index){
        setCursor(QCursor(Qt::SizeBDiagCursor));
    }else if(2 == index){
        setCursor(QCursor(Qt::SizeFDiagCursor));
    }else if(3 == index){
        setCursor(QCursor(Qt::SizeBDiagCursor));
    }else if(4 == index){
        setCursor(QCursor(Qt::SizeVerCursor));
    }else if(5 == index){
        setCursor(QCursor(Qt::SizeHorCursor));
    }else if(6 == index){
        setCursor(QCursor(Qt::SizeVerCursor));
    }else if(7 == index){
        setCursor(QCursor(Qt::SizeHorCursor));
    }
    setVisible(false);
}

QRectF ControlPosItem::boundingRect() const
{
    return shape().boundingRect();
}

QPainterPath ControlPosItem::shape() const
{
    QPainterPath path;
    path.addRoundedRect(QRectF(-_radius,-_radius,_radius*2,_radius*2),25,25);
    return path;
}

void ControlPosItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    double scalerFactor = painter->transform().m11();
    painter->setPen(QColor(0,0,0,200));
    painter->setBrush(QColor(255,0,255));
    if(option->state & QStyle::State_MouseOver){
        painter->setBrush(QColor(255,0,255,200));
    }
    painter->drawRoundedRect(QRectF(-_radius,-_radius,_radius*2,_radius*2),25,25);
    painter->scale(scalerFactor,scalerFactor);
}

void ControlPosItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(parentItem()){
        RectItem * rectItem = dynamic_cast<RectItem*>(parentItem());
        if(!rectItem->adjustArea(_index,event->scenePos())){
            update();
            return;
        }
    }
    update();
}

void ControlPosItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(parentItem()){
        RectItem * rectItem = dynamic_cast<RectItem*>(parentItem());
        rectItem->ctrlPosPressed(_index);
    }
    update();
//    QGraphicsItem::mousePressEvent(event);
}

void ControlPosItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(parentItem()){
        RectItem * rectItem = dynamic_cast<RectItem*>(parentItem());
        rectItem->ctrlPosRelease(_index);
    }
    emit ControlPosItemchang();
    update();
//    QGraphicsItem::mouseReleaseEvent(event);
}

int ControlPosItem::index() const
{
    return _index;
}

void ControlPosItem::setIndex(int newIndex)
{
    _index = newIndex;
}
////////////////
/// \brief RectItem::RectItem
/// \param parent
RectItem::RectItem(const QPointF &start, const QPointF &end,const QString &name,QGraphicsItem *parent)
    :QGraphicsItem(parent),
    _textItem(new TextItem(this)),
    m_currentColor(Qt::green),
    m_currentcolorchang(Qt::green),
    brushflag(false)
{
    setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
    QList<QPointF> pointf;
    //初始的时候需要view转scence
    QPointF leftTop = start;
    QPointF rightTop = QPointF(end.x(),start.y());
    QPointF rightBottom= end;
    QPointF leftBottom = QPointF(start.x(),end.y());
    pointf<<leftTop<<rightTop<<rightBottom<<leftBottom;
    QColor qcolor = QColor(255, 0, 0);
    init(name,pointf,qcolor);
}

RectItem::~RectItem()
{

}

void RectItem::setrectbrush(bool b){
    brushflag = b;
}
void RectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    _index = -1;
    returnpos();
 //   update();
}
QString RectItem::GetName() {
    return _textItem->text();
}
void RectItem::init(const QString &text,const QList<QPointF> &pointFs,QColor &color)
{
    m_currentColor = color;
    m_currentcolorchang = m_currentColor;
    _ctrlItems.clear();
    _oldCtrlPosList.clear();
    _ctrlMidItems.clear();
    _textItem->setText(text);
    //4个控制点(左上，右上，右下，左下)
    for(int i=0; i<pointFs.size(); ++i){
        auto pPos = pointFs.at(i);
        auto cPos = QPointF(pPos.x(),pPos.y());
        auto posItem = new ControlPosItem(i,this);
        posItem->setPos(this->mapFromScene(cPos));
        _ctrlItems.append(posItem);
        _oldCtrlPosList.append(posItem->pos());
        connect(posItem,SIGNAL(ControlPosItemchang()),this,SLOT(returnposslot()));
    }
    //文本位置
    _textItem->setPos(_ctrlItems.at(0)->pos());
    //4个控制点中间的控制点
    {//4
        auto cPos = midPos(_ctrlItems.at(0)->pos(),_ctrlItems.at(1)->pos());
        auto posItem = new ControlPosItem(4,this);
        posItem->setPos(cPos);
        connect(posItem,SIGNAL(ControlPosItemchang()),this,SLOT(returnposslot()));
        _ctrlMidItems.append(posItem);
    }
    {//5
        auto cPos = midPos(_ctrlItems.at(1)->pos(),_ctrlItems.at(2)->pos());
        auto posItem = new ControlPosItem(5,this);
        posItem->setPos(cPos);
        connect(posItem,SIGNAL(ControlPosItemchang()),this,SLOT(returnposslot()));
        _ctrlMidItems.append(posItem);
    }
    {//6
        auto cPos = midPos(_ctrlItems.at(2)->pos(),_ctrlItems.at(3)->pos());
        auto posItem = new ControlPosItem(6,this);
        posItem->setPos(cPos);
        connect(posItem,SIGNAL(ControlPosItemchang()),this,SLOT(returnposslot()));
        _ctrlMidItems.append(posItem);
    }
    {//7
        auto cPos = midPos(_ctrlItems.at(3)->pos(),_ctrlItems.at(0)->pos());
        auto posItem = new ControlPosItem(7,this);
        posItem->setPos(cPos);
        connect(posItem,SIGNAL(ControlPosItemchang()),this,SLOT(returnposslot()));
        _ctrlMidItems.append(posItem);
    }
}
QRectF RectItem::boundingRect() const
{
    return shape().boundingRect();
}
QPainterPath RectItem::shape() const
{
    QPolygonF plf;
    QPainterPath path;
    if(_ctrlItems.size() > 0){
        for(int i=0; i<_ctrlItems.size(); ++i){
            plf<<_ctrlItems.at(i)->pos();
        }
        plf.append(_ctrlItems.first()->pos());
        path.addPolygon(plf);
    }
    return path;
}
QVariant RectItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(change == ItemSelectedHasChanged ){
        //显示隐藏控制点
        foreach (auto &var, _ctrlItems) {
            if(var->isVisible() == !value.toBool()){
                var->setVisible(value.toBool());
            }
        }
        foreach (auto &var, _ctrlMidItems) {
            if(var->isVisible() == !value.toBool()){
                var->setVisible(value.toBool());
            }
        }
        if(m_currentcolorchang == QColor(Qt::yellow)){
            m_currentcolorchang = m_currentColor;
        }else{
            m_currentcolorchang = QColor(Qt::yellow);
        }
    }
    return QGraphicsItem::itemChange(change,value);
}
void RectItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverEnterEvent(event);
}
void RectItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverLeaveEvent(event);
}

void RectItem::returnposslot()
{
    returnpos();
}

void RectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::RightButton){
        emit rightsignal(this);
    }
    update();
}
void RectItem::returnpos()
{
    emit returnpossignal();
}
void RectItem::ctrlPosPressed(int index)
{
    _index = index;
    _oldCtrlPosList.clear();
    for(int i=0; i<_ctrlItems.size(); ++i){
        _oldCtrlPosList.append(_ctrlItems.at(i)->scenePos());
    }
}
void RectItem::ctrlPosRelease(int index)
{
    _index = -1;
    _oldCtrlPosList.clear();
    this->update();
}
bool RectItem::adjustArea(int index,const QPointF& scenePos)
{
    //对角点
    int dIndex = -1;
    //index上一个点
    int p1 = -1;
    //index下一个点
    int p2 = -1;
    if(0 == index){
        p1 = 3;
        dIndex = 2;
        p2 = 1;
    }else if(1 == index){
        p1 = 2;
        dIndex = 3;
        p2 = 0;
    }else if(2 == index){
        p1 = 1;
        dIndex = 0;
        p2 = 3;
    }else if(3 == index){
        p1 = 0;
        dIndex = 1;
        p2 = 2;
    }
    QPointF currentPos = this->mapFromScene(scenePos);
    if(index <4){
        QPointF point1 = QPointF(currentPos.x(),_ctrlItems.at(dIndex)->pos().y());
        QPointF point2 = QPointF(_ctrlItems.at(dIndex)->pos().x(),currentPos.y());
        _ctrlItems.at(index)->setPos(currentPos);
        _ctrlItems.at(p1)->setPos(point1);
        _ctrlItems.at(p2)->setPos(point2);
    }else{//中间控制点
        if(4 == index){
            _ctrlItems.at(0)->setPos(QPointF(_ctrlItems.at(0)->x(),currentPos.y()));
            _ctrlItems.at(1)->setPos(QPointF(_ctrlItems.at(1)->x(),currentPos.y()));
            _ctrlMidItems.at(0)->setPos(currentPos);
        }else if(5 == index){
            _ctrlItems.at(1)->setPos(QPointF(currentPos.x(),_ctrlItems.at(1)->y()));
            _ctrlItems.at(2)->setPos(QPointF(currentPos.x(),_ctrlItems.at(2)->y()));
            _ctrlMidItems.at(1)->setPos(currentPos);
        }else if(6 == index){
            _ctrlItems.at(2)->setPos(QPointF(_ctrlItems.at(2)->x(),currentPos.y()));
            _ctrlItems.at(3)->setPos(QPointF(_ctrlItems.at(3)->x(),currentPos.y()));
            _ctrlMidItems.at(2)->setPos(currentPos);
        }else if(7 == index){
            _ctrlItems.at(3)->setPos(QPointF(currentPos.x(),_ctrlItems.at(3)->y()));
            _ctrlItems.at(0)->setPos(QPointF(currentPos.x(),_ctrlItems.at(0)->y()));
            _ctrlMidItems.at(3)->setPos(currentPos);
        }
    }
    resetRect();
    return true;
}

void RectItem::resetRect()
{
    //求宽
    QPointF p0 = this->mapToParent(_ctrlItems.at(0)->pos());
    QPointF p1 = this->mapToParent(_ctrlItems.at(1)->pos());
    QPointF p2 = this->mapToParent(_ctrlItems.at(2)->pos());
    QPointF p3 = this->mapToParent(_ctrlItems.at(3)->pos());
    qreal w = twoPosLength(p0,p1);
    qreal h = twoPosLength(p0,p3);

    QPointF centerPos = (p0 + p2)/2.0;
    this->setPos(centerPos);
    w /= 2;
    h /= 2;
    _ctrlItems.at(3)->setPos(QPointF(-w,h));
    _ctrlItems.at(2)->setPos(QPointF(w,h));
    _ctrlItems.at(1)->setPos(QPointF(w,-h));
    _ctrlItems.at(0)->setPos(QPointF(-w,-h));
    //文本位置
        _textItem->setPos(_ctrlItems.at(0)->pos());
    {//4
        auto cPos = midPos(_ctrlItems.at(0)->pos(),_ctrlItems.at(1)->pos());
        _ctrlMidItems.at(0)->setPos(cPos);
    }
    {//5
        auto cPos = midPos(_ctrlItems.at(1)->pos(),_ctrlItems.at(2)->pos());
        _ctrlMidItems.at(1)->setPos(cPos);
    }
    {//6
        auto cPos = midPos(_ctrlItems.at(2)->pos(),_ctrlItems.at(3)->pos());
        _ctrlMidItems.at(2)->setPos(cPos);
    }
    {//7
        auto cPos = midPos(_ctrlItems.at(3)->pos(),_ctrlItems.at(0)->pos());
        _ctrlMidItems.at(3)->setPos(cPos);
    }
}

void RectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    double scaleFactor = painter->transform().m11();
    if(brushflag){
//        painter->setBrush(QColor(226, 222, 222));
        painter->setBrush(QColor(96, 119, 136));
    }else{
        painter->setBrush(Qt::NoBrush);
    }
    if(_index >= 0){
        painter->setPen(QPen(Qt::magenta,2));
        QPolygonF plf;
        QPainterPath path;
        if(_oldCtrlPosList.size() > 0){
            for(int i=0; i<_oldCtrlPosList.size(); ++i){
                plf<<this->mapFromScene(_oldCtrlPosList.at(i));
            }
            plf.append(mapFromScene(_oldCtrlPosList.first()));
            path.addPolygon(plf);
        }
        painter->drawPath(path);
    }
    QPen pen;
    pen.setColor(m_currentcolorchang);
    if (fabs(scaleFactor)>0.0001) pen.setWidth(2/scaleFactor);
    else pen.setWidth(1);

//    painter->setPen(QPen(m_currentcolorchang,2));
    painter->setPen(pen);
    painter->drawPath(shape());
}

void RectItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        emit rectitemdoubleclicke(_textItem->text());
    }
    update();
}
const QList<ControlPosItem *> &RectItem::ctrlItems() const
{
    return _ctrlItems;
}

