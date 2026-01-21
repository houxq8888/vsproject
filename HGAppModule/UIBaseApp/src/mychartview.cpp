/*
**
**	版权:	
**
**	名称:	mychartview.cpp
**
**	作者:	侯 晓 琴
**
**	日期:	2024/10/29
**
**	描述:
**
**	功能:	自定义图表类
**
**	备注:
**
*/
#include "mychartview.h"
#include <QDebug>
#include "HGMacroData.h"
#include <QToolTip>


MyChartView::MyChartView(QWidget* parent)
{
    m_chartType=CHART_LINE;
    m_x_line=new QGraphicsLineItem();
    m_y_line=new QGraphicsLineItem();
    m_x_line->setZValue(5);
    m_y_line->setZValue(5);
    m_coordItem=new QGraphicsSimpleTextItem();
    m_coordItem->setZValue(10);
    // this->scene()->addItem(m_x_line);
    // this->scene()->addItem(m_y_line);
    // this->scene()->addItem(m_coordItem);
}
MyChartView::~MyChartView()
{
    SAFE_DELETE(m_x_line);
    SAFE_DELETE(m_y_line);
    SAFE_DELETE(m_coordItem);
}
void MyChartView::saveAxisRange()
{
    QChart* chart=this->chart();
    if (chart==NULL) return;
    QValueAxis *axisX=dynamic_cast<QValueAxis*>(this->chart()->axisX());
    if (axisX!=NULL) {
        m_xMin=axisX->min();
        m_xMax=axisX->max();
    }
    QValueAxis *axisY=dynamic_cast<QValueAxis*>(this->chart()->axisY());
    if (axisY!=NULL) {
        m_yMin=axisY->min();
        m_yMax=axisY->max();
    }
    // qDebug()<<"xmin:"<<m_xMin<<",xmax:"<<m_xMax<<",ymin:"<<m_yMin<<",ymax:"<<m_yMax;
}
void MyChartView::mousePressEvent(QMouseEvent *event)
{
    if (event->button()==Qt::LeftButton)
    {
        if (this->chart()!=NULL&&this->chart()->axisX()!=NULL&&this->chart()->axisY()!=NULL) {
            m_lastPoint=event->pos();
            m_isPress=true;
        }
        emit chartClicked(event->pos());
    }
    QChartView::mousePressEvent(event);
}
void MyChartView::mouseMoveEvent(QMouseEvent *event)
{
    // 以防父类的实例接收不到mouseMoveEvent事件
    QChartView::mouseMoveEvent(event);//调用父类的重写方法

    QGraphicsItem* item=this->chart()->scene()->itemAt(event->pos(),this->chart()->transform());
    if (!item) return;
    
    
    QGraphicsTextItem* textItem=dynamic_cast<QGraphicsTextItem*>(item);
    if (textItem){
        QString text=textItem->toPlainText();
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        QToolTip::showText(event->globalPos(),text);
#else
        QToolTip::showText(event->globalPosition().toPoint(),text);
#endif
    } else {
        QToolTip::hideText();
    }

    // 显示当前鼠标所在位置的坐标
    const QPoint curpos=event->pos();
    QPointF curChartPos=this->chart()->mapToValue(QPointF(curpos));
    //平移
    if (m_isPress) {
        QPoint offset=curpos-m_lastPoint;
        m_lastPoint=curpos;
        if (!m_alreadySaveRange) {
            this->saveAxisRange();
            m_alreadySaveRange=true;
        }
        if (this->chart()!=NULL)
            this->chart()->scroll(-offset.x(),offset.y());
    }
    if (this->chart()!=NULL&&this->chart()->axisX()!=NULL&&this->chart()->axisY()!=NULL) {
        //  QPointF chartPoint = chart()->mapToValue(event->pos());
        // qDebug() << "Mouse clicked at chart value: (" << chartPoint.x() << "," << chartPoint.y() << ")";
        // QChartView::mousePressEvent(event);
        // m_x_line->setLine(event->x(),0,event->x(),this->height());
        // m_y_line->setLine(0,event->y(),this->width(),event->y());
        // QString coordStr=QString("(X=%1,Y=%2)").arg(curChartPos.x()).arg(curChartPos.y());
        // coordStr+="\n";
        // m_coordItem->setText(coordStr);
        // m_coordItem->setPos(curpos);
    }
}
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
void MyChartView::enterEvent(QEvent *event)
#else
void MyChartView::enterEvent(QEnterEvent *event)
#endif
{
    if (this->chart()!=NULL&&this->chart()->axisX()!=NULL&&this->chart()->axisY()!=NULL) {
        // m_x_line->setVisible(true);
        // m_y_line->setVisible(true);
        // m_coordItem->setVisible(true);
    }
}
void MyChartView::leaveEvent(QEvent *event)
{
    // m_x_line->setVisible(false);
    // m_y_line->setVisible(false);
    // m_coordItem->setVisible(false);
}
void MyChartView::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPress=false;
    //鼠标右键恢复初始化状态
    if (event->button()==Qt::RightButton) {
        if (m_alreadySaveRange) {
            if (this->chart()==NULL) return;
            if (this->chart()->axisX()==NULL) return;
            this->chart()->axisX()->setRange(m_xMin,m_xMax);
            if (this->chart()->axisY()==NULL) return;
            this->chart()->axisY()->setRange(m_yMin,m_yMax);
        }
    }
}
void MyChartView::wheelEvent(QWheelEvent *event)
{
    if (this->chart()==NULL) return;
    QValueAxis *axisY=dynamic_cast<QValueAxis*>(this->chart()->axisY());
    if (axisY==NULL) return;
    QValueAxis *axisX=dynamic_cast<QValueAxis*>(this->chart()->axisX());
    if (axisX==NULL) return;
    
    // Qt6兼容性处理
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    const QPoint curpos=event->pos();
    const int delta=event->delta();
#else
    const QPoint curpos=event->position().toPoint();
    const int delta=event->angleDelta().y();
#endif
    
    QPointF curChartPos=this->chart()->mapToValue(QPointF(curpos));
    if (!m_alreadySaveRange) {
        this->saveAxisRange();
        m_alreadySaveRange=true;
    }
    const double factor=1.1;//缩放比例
    const double yMin=axisY->min();
    const double yMax=axisY->max();
    const double yCentral=curChartPos.y();

    const double xMin=axisX->min();
    const double xMax=axisX->max();
    const double xCentral=curChartPos.x();

    double bottomOffset, topOffset,leftOffset,rightOffset;
    if (delta>0) { // 放大
        bottomOffset=1.0/factor*(yCentral-yMin);
        topOffset=1.0/factor*(yMax-yCentral);
        leftOffset=1.0/factor*(xCentral-xMin);
        rightOffset=1.0/factor*(xMax-xCentral);
    } else { // 缩小
        bottomOffset=1.0*factor*(yCentral-yMin);
        topOffset=1.0*factor*(yMax-yCentral);
        leftOffset=1.0*factor*(xCentral-xMin);
        rightOffset=1.0*factor*(xMax-xCentral);
    }
    this->chart()->axisY()->setRange(yCentral-bottomOffset,yCentral+topOffset);
    this->chart()->axisX()->setRange(xCentral-leftOffset,xCentral+rightOffset);
    
}
void MyChartView::contextMenuEvent(QContextMenuEvent *event)
{
    // 创建菜单
    QMenu contextMenu(this);

    // 创建菜单项
    m_lineAction = contextMenu.addAction("折线图");
    m_barAction = contextMenu.addAction("柱状图");

    m_lineAction->setCheckable(true);
    m_barAction->setCheckable(true);
    switch (m_chartType){
        case CHART_LINE:{
            m_lineAction->setChecked(true);
            m_barAction->setChecked(false);
            break;
        }
        case CHART_BAR:{
            m_lineAction->setChecked(false);
            m_barAction->setChecked(true);
            break;
        }
    }
    // 连接菜单项的信号与槽
    connect(m_lineAction, &QAction::triggered, this, &MyChartView::onAction1Triggered);
    connect(m_barAction, &QAction::triggered, this, &MyChartView::onAction2Triggered);

    // 显示菜单
    contextMenu.exec(event->globalPos());
}
void MyChartView::onAction1Triggered()
{
    m_chartType=CHART_LINE;
    
    // 处理Action 1
    emit createLineChart();
}

void MyChartView::onAction2Triggered()
{
    m_chartType=CHART_BAR;
    // 处理Action 2
    emit createBarChart();
}