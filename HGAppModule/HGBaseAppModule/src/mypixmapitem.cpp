#include "mypixmapitem.h"
#include "hgxml.h"

using namespace HGMACHINE;

MyPixmapItem::MyPixmapItem(const std::string &lang,const QPixmap &pixmap) : QGraphicsPixmapItem(pixmap) {
    m_lang = lang;
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    setAcceptHoverEvents(true);
    isResizing = false;
}

void MyPixmapItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
    // 判断鼠标是否靠近右下角，进入调整大小模式
    if (boundingRect().contains(event->pos()) &&
        event->pos().x() > boundingRect().width() - 10 &&
        event->pos().y() > boundingRect().height() - 10) {
        setCursor(Qt::SizeFDiagCursor);  // 右下角调整大小光标
    } else {
        setCursor(Qt::OpenHandCursor);  // 普通拖动光标
    }
    QGraphicsPixmapItem::hoverMoveEvent(event);
}

void MyPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (cursor().shape() == Qt::SizeFDiagCursor) {
        isResizing = true;  // 进入调整大小模式
        resizeStartPos = event->scenePos();
        originalPixmap = pixmap();
    } else {
        isResizing = false;
        setCursor(Qt::ClosedHandCursor);
    }
    QGraphicsPixmapItem::mousePressEvent(event);
}

void MyPixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (isResizing) {
        // 计算缩放比例
        QPointF delta = event->scenePos() - resizeStartPos;
        int newWidth = originalPixmap.width() + delta.x();
        int newHeight = originalPixmap.height() + delta.y();

        if (newWidth > 10 && newHeight > 10) {  // 防止缩放过小
            setPixmap(originalPixmap.scaled(newWidth, newHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    } else {
        QGraphicsPixmapItem::mouseMoveEvent(event);
    }
}

void MyPixmapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    setCursor(Qt::OpenHandCursor);
    isResizing = false;
    QGraphicsPixmapItem::mouseReleaseEvent(event);
}
void MyPixmapItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
    QMenu menu;

    QAction *deleteAction = menu.addAction(QString::fromStdString(loadTranslation(m_lang,"Delete")));
    // QAction *resizeAction = menu.addAction("调整大小");
    // QAction *infoAction = menu.addAction("查看信息");

    QAction *selectedAction = menu.exec(event->screenPos());

    if (selectedAction == deleteAction) {
        delete this;
    } /*else if (selectedAction == resizeAction) {
        QMessageBox::information(nullptr, "提示", "调整大小功能待实现...");
    } else if (selectedAction == infoAction) {
        QMessageBox::information(nullptr, "图片信息", 
            QString("图片大小: %1 x %2").arg(pixmap().width()).arg(pixmap().height()));
    }*/
}