#include "hghandwritingwidget.h"
#include <QPainter>

HGHandWritingWidget::HGHandWritingWidget(QWidget *parent) : QWidget(parent) {
    setAttribute(Qt::WA_StaticContents);
    setAttribute(Qt::WA_StyledBackground);
    setStyleSheet("background-color: white;");
    setMinimumSize(400, 300);  // 设定窗口大小
}

HGHandWritingWidget::~HGHandWritingWidget(){

}
bool HGHandWritingWidget::closeWindow()
{
    return true;
}
QPixmap HGHandWritingWidget::getPixmap()
{
    QPixmap pixmap;
    if (points.size()>0) pixmap = this->grab();
    // if (pixmap.isNull()){
    //     printf("pixmap is null");
    // } else {
    //     printf("pixmap is not null");
    // }
    return pixmap;
}
void HGHandWritingWidget::mousePressEvent(QMouseEvent *event) 
{
    points.resize(points.size()+1);             
    points[points.size()-1].append(event->pos()); // 记录按下点
    update();
}

void HGHandWritingWidget::mouseMoveEvent(QMouseEvent *event) 
{
    if (event->buttons() & Qt::LeftButton)
    {
        points[points.size()-1].append(event->pos()); // 记录移动路径
        update();
    }
}
void HGHandWritingWidget::clearPoints()
{
    points.clear();// 清除之前的轨迹
    update();
}   
void HGHandWritingWidget::paintEvent(QPaintEvent *event) 
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // 抗锯齿
    painter.setPen(QPen(Qt::black, 3));            // 设置笔颜色和粗细

    for (int i = 0; i < points.size(); i++)
    {
        for (int j = 1; j < points[i].size(); ++j)
        {
            painter.drawLine(points[i][j - 1], points[i][j]); // 画出轨迹
        }
    }
}