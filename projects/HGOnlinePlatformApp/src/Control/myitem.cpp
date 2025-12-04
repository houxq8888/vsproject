/*
    @file   myitem.cpp
    @brief  Implementation file
    @author XiaoQin.Hou
*/
#include "myitem.h"
#include <QMenu>
#include "hgxml.h"
#include "common.h"
#include <QToolTip>

using namespace HGMACHINE;

MyHighAccuracyPumpItem::MyHighAccuracyPumpItem(QRectF rect, std::string title,std::string lang,
    QColor fillColor, QColor borderColor, int borderWidth)
        : m_rect(rect), 
        m_lang(lang),
        m_title(title),
        m_fillColor(fillColor),
        m_borderColor(borderColor), 
        m_borderWidth(borderWidth)
{
    setParentItem(nullptr);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);  // 使项可选
    setFlag(QGraphicsItem::ItemIsFocusable);   // 使项可获得焦点
    setFlag(QGraphicsItem::ItemClipsToShape, true);
    setAcceptHoverEvents(true);
    // setOpacity(0.0);
}

void MyHighAccuracyPumpItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, 
    QWidget *widget) 
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);

    // 设置画笔（边框）
    QPen pen(m_borderColor);
    pen.setWidth(m_borderWidth);
    painter->setPen(pen);

    // 设置字体
    QFont font("Arial", 16);  // 使用 Arial 字体，字号为 16
    painter->setFont(font);

    // 设置刷子（填充颜色）
    painter->setBrush(m_fillColor);

    // 绘制矩形
    painter->drawRect(QRectF(0,0,30,20));
    painter->drawLine(QPointF(0+30/2,20),QPointF(0+30/2,40));
    painter->setBrush(QColor(0,255,50,0));
    painter->drawRect(QRectF(0,40,30,50));
    painter->drawLine(QPointF(0+30/2,40+50),QPointF(0+30/2,90+30));
    painter->drawLine(QPointF(0,90+30),QPointF(30,90+30));
}
void MyHighAccuracyPumpItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    QToolTip::showText(event->screenPos(), "Custom High accuracy pump Tooltip");
    QGraphicsItem::hoverEnterEvent(event);
}
void MyAuxiliaryPumpItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    QToolTip::showText(event->screenPos(), "Custom auxiliary pump Tooltip");
    QGraphicsItem::hoverEnterEvent(event);
}
MyAuxiliaryPumpItem::MyAuxiliaryPumpItem(QRectF rect, std::string title,std::string lang,
    QColor fillColor, QColor borderColor, int borderWidth)
        : m_rect(rect), 
        m_lang(lang),
        m_title(title),
        m_fillColor(fillColor),
        m_borderColor(borderColor), 
        m_borderWidth(borderWidth)
{
    setParentItem(nullptr);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);  // 使项可选
    setFlag(QGraphicsItem::ItemIsFocusable);   // 使项可获得焦点
    setFlag(QGraphicsItem::ItemClipsToShape, true);
    setAcceptHoverEvents(true);

    // setOpacity(0.0);
}

void MyAuxiliaryPumpItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, 
    QWidget *widget) 
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);

    // 设置画笔（边框）
    QPen pen(m_borderColor);
    pen.setWidth(m_borderWidth);
    painter->setPen(pen);

    // 设置字体
    QFont font("Arial", 16);  // 使用 Arial 字体，字号为 16
    painter->setFont(font);

    // 设置刷子（填充颜色）
    painter->setBrush(m_fillColor);

    painter->drawLine(QPointF(0,0),QPointF(0,30));
    painter->drawLine(QPointF(0,15),QPointF(30,15));
    painter->drawRect(QRectF(15,2,30,26));
}

MyRectangleItem::MyRectangleItem(QRectF rect, std::string title,std::string lang,
    QColor fillColor, QColor borderColor, int borderWidth)
        : m_rect(rect), 
        m_lang(lang),
        m_title(title),
        m_fillColor(fillColor),
        m_borderColor(borderColor), 
        m_borderWidth(borderWidth)
{
    setParentItem(nullptr);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);  // 使项可选
    setFlag(QGraphicsItem::ItemIsFocusable);   // 使项可获得焦点
    setFlag(QGraphicsItem::ItemClipsToShape, true);
    setAcceptHoverEvents(true);

    // setOpacity(0.0);
}
void MyRectangleItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    QToolTip::showText(event->screenPos(), QString::fromStdString(loadTranslation(m_lang,m_title)));
    QGraphicsItem::hoverEnterEvent(event);
}
void MyRectangleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, 
    QWidget *widget) 
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);

    // 设置画笔（边框）
    QPen pen(m_borderColor);
    pen.setWidth(m_borderWidth);
    painter->setPen(pen);

    // 设置字体
    QFont font("Arial", 16);  // 使用 Arial 字体，字号为 16
    painter->setFont(font);

    // 设置刷子（填充颜色）
    painter->setBrush(m_fillColor);

    // 绘制矩形
    painter->drawRect(boundingRect());

    QString text=QString::fromStdString(loadTranslation(m_lang,m_title));
    // 计算文字的宽度和高度
    int textWidth = painter->fontMetrics().horizontalAdvance(text);
    int textHeight = painter->fontMetrics().height();

    // 计算文本绘制的位置，使其水平居中，垂直位于矩形上方
    int x = (boundingRect().width() - textWidth) / 2;  // 水平居中
    int y = boundingRect().top() + textHeight;  // 位于矩形上方

    // 绘制文字
    painter->drawText(x, y, text);
   
    if (!m_image.isNull()) {
        // 如果图像不为空，绘制图像
        QRectF imageRect((boundingRect().width()-m_image.width())/2,
            (boundingRect().height()-m_image.height())/1,
            m_image.width(), m_image.height());
        painter->drawPixmap(imageRect, m_image,QRectF(0, 0, m_image.width(), m_image.height()));  // 将图像绘制到指定位置
    }
}
void MyRectangleItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    update();
    QGraphicsItem::mousePressEvent(event);
}
// 重写右键菜单事件
void MyRectangleItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
    if (m_title!="ActionPool") return;
    QMenu contextMenu;

    // 添加菜单项
    QAction *action1 = contextMenu.addAction(QString::fromStdString(loadTranslation(m_lang,"Swirl")));

    // 执行用户选择的菜单项
    QAction *selectedAction = contextMenu.exec(event->screenPos());

    if (selectedAction == action1) {
        // add swirl
        showImage();
    } 
}
void MyRectangleItem::showImage() {
    QString picPath=QString::fromStdString(getPath("/resources/V1/@1xiconPark-switch 1.png"));
    m_image.load(picPath);  // 加载图像，可以修改为你的图像路径
    update();  // 触发重绘
}
