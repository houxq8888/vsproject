/*
**
**	版权:	
**
**	名称:	mygraphicsview.h
**
**	作者:	侯 晓 琴
**
**	日期:	2022/10/20
**
**	描述:
**
**	功能:	视图类
**
**	备注:
**
*/

#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H
#include <QGraphicsView>
#include <QMouseEvent>
#include <QPoint>
#include <QMenu>
#include <QActionGroup>
#include <QAction>
#include <QLine>
#include <QFrame>
#include <QPixmap>
#include <QSize>
#include <QDebug>
#include <QTimer>
#include <QGraphicsScene>
#include <QRectF>
#include <math.h>
#include "rectellipseitem.h"
#include "rectitem.h"
#include <QPushButton>
#include <QPixmap>
#include <QLabel>

enum MenuFlag{
    POINTER,
    PAN,
    ZOOMIN,
    ZOOMOUT,
    FITIMAGE,
    ZOOM,
    PXIELGRID,
    SUBPIXELGRID
};
class MyGraphicsView:public QGraphicsView
{
    Q_OBJECT
public:
    MyGraphicsView(int wpIndex, QWidget *parent=0);
    ~MyGraphicsView();
    void setSelectROI(bool flag) {isDrawing=flag;};
    QRectF getSelectROI();
    void clearRectItem();
    void init();
    void addstartbtn();
    void setstartbtntext(QString);
    void addsimulatebtn();
    void setsimulatebtntext(QString);
    QString getstartbtntext();
    QString getsimulatebtntext();
    void addLabel();
    void setLabeltextcolor(QString centertext=nullptr);
    void setbottomtext(QString bottomtext=nullptr);
    void setstartbtnvisible(bool flag);
    void setsimulatebtnvisible(bool flag);
    void showImageGray(int,int,int);
    void scaleToView();

protected:
    void mouseMoveEvent(QMouseEvent *event)override;
    void mousePressEvent(QMouseEvent *event)override;
    void mouseDoubleClickEvent(QMouseEvent *event)override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void wheelEvent(QWheelEvent *event) override;
    void showEvent(QShowEvent *event) override;

private:
    void Zoomflag(bool);
    void setViewZoom(QMouseEvent* event);
    void setmenu();
    void isitemmove(bool);
signals:
    void mouseDoubleClick(QPoint);
    void changezoom(bool,int);
    void pixgridsignal(bool,int);
    void subpixgridsignal(bool,int);
    void startsignal(int,bool);
    void simulatesignal(int,bool);
    void leftdoubsignal(int,bool);

    void getScenePos(int x,int y);
protected slots:
    void Pointer_slot();
    void Pan_slot();
    void Zoomin_slot();
    void Zoomout_slot();
    void Fitimage_slot();
    void Zoom_slot();
    void pixelgrid_slot();
    void SubpixelGrid_slot();
private:
    QRectF m_selectROI;
    QMenu *menu;
    QActionGroup *actiongroup;
    QActionGroup *actionZoom;
    QAction *Pointer;
    QAction *Pan;
    QAction *Zoomin;
    QAction* Zoomout;
    QAction* Fitimage;
    QAction *Zoom;
    QAction *pixelgrid;
    QAction *SubpixelGrid;
    MenuFlag menuflag;

    int zoomflag;
    bool m_pixelgridFlag;
    bool m_subpixelGridFlag;
    QPushButton *start_btn,*simulate_btn;

    int m_wpIndex;
    QLabel *centerLab;
    QLabel *bottomLab;

    QPixmap pixzoomin;
    QPixmap pixzoomout;

    bool m_fullScreenFlag;

    QPointF centerAnchor;
    QPointF posAnchor;
    bool isMousePressed;

    QLabel* m_imgPointLabel;

    QPointF drawStartPos;
    bool isDrawing;
    RectItem* currentRectItem;
};

#endif // MYGRAPHICSVIEW_H
