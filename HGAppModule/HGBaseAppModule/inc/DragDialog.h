#ifndef DRAGGDIALOG_H
#define DRAGGDIALOG_H

#include <QDialog>
#include <QMouseEvent>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineSeries>
#include "labelwithimg.h"

struct recordLabel{
    LabelWithImg* label;
    bool flag;
    recordLabel(){
        Clear();
    }
    void Clear(){
        label=NULL;
        flag=false;
    }
};


class DraggableDialog : public QDialog {
    Q_OBJECT

public:
    explicit DraggableDialog(QWidget *parent = nullptr) : QDialog(parent) {
        // 去掉标题栏
        setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    }

protected:
    // 捕获鼠标按下事件
    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            dragStartPosition = event->globalPos() - frameGeometry().topLeft();
            dragging = true;
            event->accept();
        }
    }

    // 捕获鼠标移动事件
    void mouseMoveEvent(QMouseEvent *event) override {
        if (dragging && (event->buttons() & Qt::LeftButton)) {
            move(event->globalPos() - dragStartPosition);
            event->accept();
        }
    }

    // 捕获鼠标释放事件
    void mouseReleaseEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            dragging = false;
            event->accept();
        }
    }

private:
    QPoint dragStartPosition; // 记录鼠标拖动起始点
    bool dragging = false;    // 标志是否正在拖动
};

#endif