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

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
using namespace QtCharts;
#endif

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
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent* event) override;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;
#else
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
#endif
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
