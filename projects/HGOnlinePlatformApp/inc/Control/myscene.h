#ifndef MYSCENE_H
#define MYSCENE_H


#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QApplication>
#include <QGraphicsView>

class MyScene : public QGraphicsScene {
public:
    MyScene(QObject *parent = nullptr);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    bool drawing;
    QPainterPath path;
    QGraphicsPathItem *pathItem;
};

#endif // MYSCENE_H