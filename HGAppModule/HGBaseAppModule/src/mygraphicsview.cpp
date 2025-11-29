/*
    @file   mygraphicsview.cpp
    @brief  Implementation file
    @author XiaoQin.Hou
*/
#include "mygraphicsview.h"
#include <QApplication>
#include <iostream>
#include "common.h"
#include <QDateTime>


MyGraphicsView::MyGraphicsView(int wpIndex, QWidget *parent):QGraphicsView(parent)
{
    isDrawing=false;
    currentRectItem=nullptr;
    centerLab = NULL;
    bottomLab = NULL;
    actionZoom=NULL;
    actiongroup=NULL;
    start_btn=NULL;
    simulate_btn=NULL;
    Pointer=NULL;
    Pan=NULL;
    Zoomin=NULL;
    Zoomout=NULL;
    Fitimage=NULL;
    Zoom=NULL;
    pixelgrid=NULL;
    SubpixelGrid=NULL;
    m_wpIndex = wpIndex;
    menu = NULL;
    m_fullScreenFlag=false;
    centerAnchor=QPointF(0.0,.0);
    posAnchor=QPointF(.0,.0);
    isMousePressed=false;
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    init();
    setDragMode(QGraphicsView::RubberBandDrag);
    setRenderHint(QPainter::Antialiasing);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setCacheMode(QGraphicsView::CacheBackground);

    QString basePath = qApp->applicationDirPath();
    pixzoomin = QPixmap(basePath + "/image/zoomin.png");
    QSize size(20,20);
    //设置图片大小
    pixzoomin = pixzoomin.scaled(size);
    pixzoomout = QPixmap(basePath + "/image/zoomout.png");
    pixzoomout = pixzoomout.scaled(size);

    m_imgPointLabel=new QLabel(this);
    m_imgPointLabel->setGeometry(0,0,600,50);
    m_imgPointLabel->move(0,0);
    m_imgPointLabel->setStyleSheet("color:red;");
    m_imgPointLabel->setAlignment(Qt::AlignLeft|Qt::AlignTop);
}
MyGraphicsView::~MyGraphicsView()
{
    SAFE_DELETE(start_btn);
    SAFE_DELETE(simulate_btn);
    SAFE_DELETE(centerLab);
    SAFE_DELETE(bottomLab);
    SAFE_DELETE(actiongroup);
    SAFE_DELETE(actionZoom);
    SAFE_DELETE(Pointer);
    SAFE_DELETE(Pan);
    SAFE_DELETE(Zoomin);
    SAFE_DELETE(Zoomout);
    SAFE_DELETE(Fitimage);
    SAFE_DELETE(Zoom);
    SAFE_DELETE(pixelgrid);
    SAFE_DELETE(SubpixelGrid);
    SAFE_DELETE(menu);
}
void MyGraphicsView::init(){
    zoomflag = 0;
    m_pixelgridFlag = false;
    m_subpixelGridFlag=false;
    start_btn =nullptr;
    simulate_btn=nullptr;
    Pointer = new QAction(this);
    Pointer->setObjectName(QString::fromUtf8("Pointer"));
    Pointer->setText("Pointer");
    Pointer->setCheckable(true);
    Pan = new QAction(this);
    Pan->setObjectName(QString::fromUtf8("Pan"));
    Pan->setText("Pan");
    Pan->setCheckable(true);
    Zoomin = new QAction(this);
    Zoomin->setObjectName(QString::fromUtf8("Zoomin"));
    Zoomin->setText("Zoomin");
    Zoomin->setCheckable(true);
    Zoomout = new QAction(this);
    Zoomout->setObjectName(QString::fromUtf8("Zoomout"));
    Zoomout->setText("Zoomout");
    Zoomout->setCheckable(true);

    Fitimage= new QAction(this);
    Fitimage->setObjectName(QString::fromUtf8("Fitimage"));
    Fitimage->setText("Fitimage");
    Zoom = new QAction(this);
    Zoom->setObjectName(QString::fromUtf8("Zoom"));
    Zoom->setText("Zoom(100%)");

    pixelgrid= new QAction(this);
    pixelgrid->setObjectName(QString::fromUtf8("pixelgrid"));
    pixelgrid->setText("pixelgrid");
    pixelgrid->setCheckable(true);
    SubpixelGrid= new QAction(this);
    SubpixelGrid->setObjectName(QString::fromUtf8("SubpixelGrid"));
    SubpixelGrid->setText("SubpixelGrid");
    SubpixelGrid->setCheckable(true);
    connect(pixelgrid,SIGNAL(triggered()),this,SLOT(pixelgrid_slot()));
    connect(SubpixelGrid,SIGNAL(triggered()),this,SLOT(SubpixelGrid_slot()));
    connect(Pointer,SIGNAL(triggered()),this,SLOT(Pointer_slot()));
    connect(Pan,SIGNAL(triggered()),this,SLOT(Pan_slot()));
    connect(Zoom,SIGNAL(triggered()),this,SLOT(Zoom_slot()));
    connect(Fitimage,SIGNAL(triggered()),this,SLOT(Fitimage_slot()));
    connect(Zoomin,SIGNAL(triggered()),this,SLOT(Zoomin_slot()));
    connect(Zoomout,SIGNAL(triggered()),this,SLOT(Zoomout_slot()));
}
void MyGraphicsView::addstartbtn(){
    start_btn = new QPushButton(this);
    start_btn->setStyleSheet("QPushButton{border:none;background:transparent;}");//
    start_btn->setText(("运行"));
    start_btn->setGeometry(width()-80,height()-30,80,30);
    start_btn->show();
    connect(start_btn,&QPushButton::clicked,[=](){
        if(start_btn->text()=="运行"){
            emit startsignal(m_wpIndex, true);
        }else if(start_btn->text()=="暂停"){
            emit startsignal(m_wpIndex, false);
        }
    });
}
void MyGraphicsView::addsimulatebtn()
{
    simulate_btn = new QPushButton(this);
    simulate_btn->setStyleSheet("QPushButton{border:none;background:transparent;}");//
    simulate_btn->setText(("仿真"));
    simulate_btn->setGeometry(width()-80,height()-60,80,30);
    simulate_btn->show();
    connect(simulate_btn,&QPushButton::clicked,[=](){
        if(simulate_btn->text()=="仿真"){
            emit simulatesignal(m_wpIndex, true);
        }else if(simulate_btn->text()=="暂停"){
            emit simulatesignal(m_wpIndex, false);
        }
    });
}
void MyGraphicsView::setstartbtntext(QString str){
    if(start_btn!=nullptr)
        start_btn->setText(str);
}
void MyGraphicsView::setsimulatebtntext(QString str){
    if (simulate_btn!=nullptr)
        simulate_btn->setText(str);
}
QString MyGraphicsView::getsimulatebtntext()
{
    if (simulate_btn!=nullptr)
        return simulate_btn->text();
    else return QString("");
}
QString MyGraphicsView::getstartbtntext()
{
    if (start_btn!=nullptr)
        return start_btn->text();
    else return QString("");
}
void MyGraphicsView::setstartbtnvisible(bool flag)
{
    if (start_btn!=nullptr) {
        start_btn->setVisible(flag);
        start_btn->setEnabled(flag);
    }
}
void MyGraphicsView::setsimulatebtnvisible(bool flag)
{
    if (simulate_btn!=nullptr) {
        simulate_btn->setVisible(flag);
        simulate_btn->setEnabled(flag);
    }
}
void MyGraphicsView::addLabel()
{
    centerLab = new QLabel(this);
    bottomLab = new QLabel(this);
    centerLab->setGeometry((width()>>1) - 50,0,100,30);
    centerLab->setAlignment(Qt::AlignCenter);
    bottomLab->setGeometry(0,height()-30,width()-50,30);
    bottomLab->setAlignment(Qt::AlignLeft);
}

void MyGraphicsView::setLabeltextcolor(QString str)
{
    if(centerLab!=nullptr){
        if (str.contains("初始")) centerLab->setStyleSheet("QLabel{background-color:rgb(245,240,0);border:none;color::black;}");
        else if (str.contains("良品")) centerLab->setStyleSheet("QLabel{background-color:rgb(0,255,0);border:none;color::black;}");
        else if (str.contains("劣品")) centerLab->setStyleSheet("QLabel{background-color:rgb(245,0,0);border:none;color::black;}");
        else if (str.contains("失败")) centerLab->setStyleSheet("QLabel{background-color:rgb(195,195,195);border:none;color::black;}");
        else if (str.contains("单向上")) centerLab->setStyleSheet("QLabel{background-color:rgb(245,0,0);border:none;color::black;}");	// 单向(上/下/左/右)
        else if (str.contains("单向下")) centerLab->setStyleSheet("QLabel{background-color:rgb(245,0,0);border:none;color::black;}");
        else if (str.contains("单向左")) centerLab->setStyleSheet("QLabel{background-color:rgb(245,0,0);border:none;color::black;}");
        else if (str.contains("单向右")) centerLab->setStyleSheet("QLabel{background-color:rgb(245,0,0);border:none;color::black;}");
        else if (str.contains("双向左上")) centerLab->setStyleSheet("QLabel{background-color:rgb(245,0,0);border:none;color::black;}") ;	// 双向(左上/左下/右上/右下)
        else if (str.contains("双向左下")) centerLab->setStyleSheet("QLabel{background-color:rgb(245,0,0);border:none;color::black;}");
        else if (str.contains("双向右上")) centerLab->setStyleSheet("QLabel{background-color:rgb(245,0,0);border:none;color::black;}");
        else if (str.contains("双向右下")) centerLab->setStyleSheet("QLabel{background-color:rgb(245,0,0);border:none;color::black;}");
        else centerLab->setStyleSheet("QLabel{background-color:rgb(245,240,0);border:none;color::black;}");
        centerLab->setText(str);
    }
}

void MyGraphicsView::setbottomtext(QString bottomtext)
{
    if(bottomLab!=nullptr)
        bottomLab->setText(bottomtext);
}
void MyGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
    if (isMousePressed && currentRectItem){
        // update the rectangle as the mouse moves
        QPointF currentPos = mapToScene(event->pos());
        QRectF rect(drawStartPos,currentPos);
        m_selectROI=rect;
        currentRectItem->update(rect);
    }
}
void MyGraphicsView::showImageGray(int x,int y,int gray)
{
    m_imgPointLabel->setText(QString("ImagePos:(%1,%2) ImageGray(%3)").arg(x).arg(y).arg(gray));
}
QRectF MyGraphicsView::getSelectROI()
{
    if (currentRectItem){
        m_selectROI=currentRectItem->mapToScene(currentRectItem->boundingRect()).boundingRect();
    }
    return m_selectROI; 
}
void MyGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
    if (event->button()==Qt::LeftButton){
        if (isMousePressed){
            isMousePressed=false;
            isDrawing=false;
        }
    }
}
void MyGraphicsView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
    if (this->scene()==nullptr) {
        return ;
    }
    //鼠标左键按下事件
    if (event->button() == Qt::LeftButton)
    {
        if (!isDrawing) return;
        // setViewZoom(event);
        // start drawing a new rectangle
        isMousePressed=true;
        drawStartPos=mapToScene(event->pos());
        currentRectItem=new RectItem(drawStartPos,QPointF(drawStartPos.x()+50,drawStartPos.y()+50),"NewTemplate");
        this->scene()->addItem(currentRectItem);
    }
    else if (event->button() == Qt::RightButton)
    {
        setmenu();
    }
}

void MyGraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{ //鼠标双击事件
    QGraphicsView::mouseDoubleClickEvent(event);
    if(event->button() == Qt::LeftButton)
        m_fullScreenFlag=!m_fullScreenFlag;
        emit leftdoubsignal(m_wpIndex,m_fullScreenFlag);
}

void MyGraphicsView::dragEnterEvent(QDragEnterEvent *event)
{
    QGraphicsView::dragEnterEvent(event);
    this->update();

}

void MyGraphicsView::dragMoveEvent(QDragMoveEvent *event)
{
    QGraphicsView::dragMoveEvent(event);
    this->update();
}

void MyGraphicsView::drawBackground(QPainter *painter, const QRectF &rect)
{
//    Q_UNUSED(rect);
//    QPen pen;

//    qreal left=rect.left();
//    qreal right=rect.right();
//    qreal top=rect.top();
//    qreal bottom=rect.bottom();
//    //边界值调整
//    left = (left/128)*128;
//    right = (right/128)*128;
//    top = (top/128)*128;
//    bottom = (bottom/128)*128;

////    pen.setColor(QColor(0,0,0));
////    pen.setWidth(2);
////    pen.setStyle(Qt::SolidLine);
////    painter->setPen(pen);
////    //绘制坐标轴
////    painter->drawLine(left,0,right,0);
////    painter->drawLine(0,top,0,bottom);

//    pen.setColor(QColor(60,60,60));
//    pen.setWidth(0);
//    pen.setStyle(Qt::DashLine);
//    painter->setPen(pen);
//    //绘制横线
//    //绘制Y轴正半轴
//    for(int i=0;i>=top;i-=128)
//    {
//        painter->drawLine(left,i,right,i);
//    }
//    //绘制Y轴负半轴
//    for(int i=0;i<=bottom;i+=128)
//    {
//        painter->drawLine(left,i,right,i);
//    }

//    //绘制竖线
//    //绘制X轴正半轴
//    for(int i=0;i<=right;i+=128)
//    {
//        painter->drawLine(i,top,i,bottom);
//    }
//    //绘制X轴负半轴
//    for(int i=0;i>=left;i-=128)
//    {
//        painter->drawLine(i,top,i,bottom);
//    }
}


void MyGraphicsView::wheelEvent(QWheelEvent *event)
{ 
    QPoint delta = event->angleDelta();
        if (delta.y() == 0){
            event->ignore();
            return;
        }
        this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        double const d = delta.y() / std::abs(delta.y());
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        if(d < 0 && this->transform().m11() < 0.000001){
            this->scale(1, 1);
            return;
        }
#else
        if(d < 0 && this->matrix().m11() < 0.000001){
            this->scale(1, 1);
            return;
        }
#endif
    d>0?Zoomflag(true):Zoomflag(false);

    emit pixgridsignal(m_pixelgridFlag,zoomflag);
    emit subpixgridsignal(m_subpixelGridFlag,zoomflag);
}

void MyGraphicsView::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
}

void MyGraphicsView::setmenu(){
    menu = new QMenu(this);
    actiongroup = new QActionGroup(this);
    actiongroup->setExclusive(true);
    menu->addAction(actiongroup->addAction(Pointer));
    menu->addAction(actiongroup->addAction(Pan));
    menu->addAction(actiongroup->addAction(Zoomin));
    menu->addAction(actiongroup->addAction(Zoomout));
    menu->addSeparator();
    actionZoom=new QActionGroup(this);
    actionZoom->setExclusive(true);
    menu->addAction(actionZoom->addAction(Fitimage));
    menu->addAction(actionZoom->addAction(Zoom));
    menu->addSeparator();
    menu->addAction(pixelgrid);
    menu->addAction(SubpixelGrid);
    menu->exec(QCursor::pos());
}
void MyGraphicsView::Pointer_slot(){
    isitemmove(true);
    setDragMode(QGraphicsView::NoDrag);
    menuflag = POINTER;
    this->setCursor(Qt::ArrowCursor);
}
void MyGraphicsView::Pan_slot(){
    isitemmove(false);
    setDragMode(QGraphicsView::ScrollHandDrag);
    menuflag = PAN;
    this->setCursor(Qt::OpenHandCursor);
}
void MyGraphicsView::Zoomin_slot(){
    isitemmove(false);
    setDragMode(QGraphicsView::NoDrag);
    menuflag = ZOOMIN;
    this->setCursor(QCursor(pixzoomin));
}
void MyGraphicsView::Zoomout_slot(){
    isitemmove(false);
    setDragMode(QGraphicsView::NoDrag);
    menuflag = ZOOMOUT;
    this->setCursor(QCursor(pixzoomout));
}
void MyGraphicsView::Fitimage_slot(){
    setDragMode(QGraphicsView::NoDrag);

}
void MyGraphicsView::Zoom_slot(){
    setDragMode(QGraphicsView::NoDrag);
  /*  this->fitInView(Qt::IgnoreAspectRatio);
    if(zoomflag>=0){
        for(int i = 0;i!=zoomflag; --zoomflag){
            double const step   = 1.2;
            double const factor = std::pow(step, -1.0);
            scale(factor, factor);
        }
    }else if(zoomflag<0){
        for(int i = 0;i!=zoomflag; ++zoomflag){
            double const step   = 1.2;
            double const factor = std::pow(step, 1.0);
            scale(factor, factor);
        }
    }*/

    emit changezoom(m_pixelgridFlag,zoomflag);
}

void MyGraphicsView::pixelgrid_slot(){
    m_pixelgridFlag = !m_pixelgridFlag;
    emit pixgridsignal(m_pixelgridFlag,zoomflag);
}
void MyGraphicsView::SubpixelGrid_slot(){
    m_subpixelGridFlag = !m_subpixelGridFlag;
    emit subpixgridsignal(m_subpixelGridFlag,zoomflag);
}

void MyGraphicsView::isitemmove(bool b)
{
    QList<QGraphicsItem*> items = this->scene()->items();
    for(int i=0;i<items.size();i++){
        RectEllipseItem* node = dynamic_cast<RectEllipseItem*>(items.at(i));
        if(node != NULL){
            node ->setFlag(QGraphicsItem::ItemIsMovable,b);
            node ->setFlag(QGraphicsItem::ItemIsSelectable,b);
        }
        RectItem *item = dynamic_cast<RectItem* >(items.at(i));
        if(item !=NULL){
            item->setFlag(QGraphicsItem::ItemIsMovable,b);
            item->setFlag(QGraphicsItem::ItemIsSelectable,b);
        }
    }
}
void MyGraphicsView::setViewZoom(QMouseEvent* event){
    menuflag=PAN;
    switch (menuflag) {
        case POINTER:
            break;
        case PAN:
        {
            // 记录鼠标按下时的中心点坐标
            centerAnchor=mapToScene(event->pos())-event->pos()+QPointF(width()>>1,height()>>1);
            // 记录当前鼠标在view中的位置，用来在mouseMove中计算偏移
            // 此处不将view坐标转换为scene坐标，原因是优化性能，在move过程中会产生抖动
            posAnchor=event->pos();
            isMousePressed=true;
            break;
        }
        case ZOOMIN:
            Zoomflag(true);
            break;
        case ZOOMOUT:
            Zoomflag(false);
            break;
        case FITIMAGE:
            break;
        case ZOOM:
            break;
        case PXIELGRID:
            break;
        case SUBPIXELGRID:
            break;
        default:
            break;
    }
}
void MyGraphicsView::scaleToView()
{
    Zoomflag(false);
}
void MyGraphicsView::Zoomflag(bool b)
{  
    if(b){
        zoomflag+=1;
        double const step   = 2.0;
        double const factor = std::pow(step, 1.0);
        scale(factor, factor);
    }else{
        zoomflag-=1;
        double const step   = 2.0;
        double const factor = std::pow(step, -1.0);
        scale(factor, factor);
    }
    emit pixgridsignal(m_pixelgridFlag,zoomflag);
    emit subpixgridsignal(m_subpixelGridFlag,zoomflag);
}
