#ifndef MYPIXMAPITEM_H
#define MYPIXMAPITEM_H

#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>
#include <QCursor>

class MyPixmapItem : public QGraphicsPixmapItem {
public:
    MyPixmapItem(const std::string& lang,const QPixmap &pixmap);

protected:
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

private:
    bool isResizing;
    QPointF resizeStartPos;
    QPixmap originalPixmap;
    std::string m_lang;
};

#endif
