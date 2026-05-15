#ifndef HGHANDWRITINGWIDGET_H
#define HGHANDWRITINGWIDGET_H

#include <QWidget>
#include <QPoint>
#include <QVector>
#include <QMouseEvent>
#include <QPaintEvent>


class HGHandWritingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGHandWritingWidget(QWidget *parent = nullptr);
    bool closeWindow();
    ~HGHandWritingWidget();
    void clearPoints();
    QPixmap getPixmap();

signals:
private slots:

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<QVector<QPoint>> points;  // 存储手写轨迹点


};

#endif // HGHANDWRITINGWIDGET_H
