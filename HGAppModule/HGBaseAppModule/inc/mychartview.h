/*
**
**	版权:	
**
**	名称:	mychartview.h
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

#ifndef MYCHARTVIEW_H
#define MYCHARTVIEW_H

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QGridLayout>
#include <QMenu>
#include <QAction>

using namespace QtCharts;

enum {
    CHART_LINE,
    CHART_BAR
};
class MyChartView : public QChartView
{
    Q_OBJECT
public:
    explicit MyChartView(QWidget *parent=nullptr);
    ~MyChartView();
    int getChartType() {return m_chartType;};
    //保存坐标区域，用于复位
    void saveAxisRange();

signals:
    void createLineChart();
    void createBarChart();
    void chartClicked(const QPoint& pos);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent* event);
    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);
    // 重写右键菜单事件
    void contextMenuEvent(QContextMenuEvent *event) override;

private slots:
    void onAction1Triggered();
    void onAction2Triggered();

private:
    QPoint m_lastPoint;
    bool m_isPress=false;
    bool m_alreadySaveRange=false;
    double m_xMin,m_xMax,m_yMin,m_yMax;
    QGraphicsLineItem* m_x_line, *m_y_line;
    QGraphicsSimpleTextItem* m_coordItem;

    QAction *m_lineAction,*m_barAction;
    int m_chartType;
};



#endif // MYCHARTVIEW_H
