#include "HGCurDataWidget.h"


HGCurDataWidget::HGCurDataWidget(int tickcount,const std::string& name,QWidget *parent) : QWidget(parent)
{
    m_layout=new QGridLayout(this);
    int width = getScreenGeometry().width();
    QFont labelFont;
    labelFont.setPointSize(width * 10/1500);

    m_series=new QLineSeries();
    m_scatterSeries=new QScatterSeries();
    m_scatterSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    m_scatterSeries->setMarkerSize(10);
    m_scatterSeries->setColor(QColor(0,255,0));
 
    m_xAxisDateTime=new QDateTimeAxis();
    // m_xAxisDateTime->setFormat("hh:mm");
    m_xAxisDateTime->setFormat("hh");
    m_xAxisDateTime->setTickCount(tickcount);
    // m_xAxisDateTime->setTitleText("时间");
    // m_xAxisDateTime->setTitleFont(QFont("Arial",width * 25/1500));
    m_xAxisDateTime->setGridLineVisible(false);
    m_xAxisDateTime->setTitleBrush(Qt::white);
    m_xAxisDateTime->setLabelsFont(labelFont);

    m_xAxis=new QValueAxis();
    m_yAxis=new QValueAxis();
    m_xAxis->setGridLineVisible(false);
    m_yAxis->setGridLineVisible(false);
    m_xAxis->setTickCount(10);
    m_xAxis->setRange(0,10);
    m_yAxis->setLabelsFont(labelFont);

    QDateTime now=QDateTime::currentDateTime();
    QDateTime fourtyEightHoursBefore=QDateTime(now.addDays(-2));
    m_xAxisDateTime->setRange(fourtyEightHoursBefore,now);

    QPen axisPen(Qt::white);
    m_yAxis->setLinePen(axisPen);
    // m_xAxisDateTime->setGridLinePen(axisPen);
    m_yAxis->setLabelsColor(QColor(255,255,255));
    m_xAxisDateTime->setLabelsColor(QColor(255,255,255));

    m_series->setName("系列一");                  //设置系列名称
    m_chart = new QChart();                          //创建图标对象
    m_chart->addSeries(m_series);                  //向图表中添加系列
    m_chart->addSeries(m_scatterSeries);
    m_chart->addAxis(m_xAxisDateTime, Qt::AlignBottom);
    // m_chart->addAxis(m_xAxis, Qt::AlignBottom);
    m_chart->addAxis(m_yAxis, Qt::AlignLeft);
    m_series->attachAxis(m_xAxisDateTime);
    m_series->attachAxis(m_yAxis);
    m_scatterSeries->attachAxis(m_yAxis);
    m_chart->setBackgroundBrush(QBrush(QColor(8,11,56,255)));
    m_chart->legend()->hide();  

    
    //隐藏图例，也就是不显示系列名称了
    m_chartView = new MyChartView();
    m_chartView->setChart(m_chart);                    //向chartView中添加图标
    m_chartView->setRenderHints(QPainter::Antialiasing);//消除边缘，看起来平滑一些
    m_chartView->setStyleSheet(
        "border: 2px solid white;"
        "background-color: rgb(8,11,56);"
        "border-radius: 10px;"  // 这里的10px是圆角半径，调大调小看效果
    );
    // m_chartView->setScatterSeries(m_scatterSeries);

    
    QGraphicsTextItem *titleItem = new QGraphicsTextItem(QString::fromStdString(name));
    QRectF rect=titleItem->boundingRect();
    qreal sceneWidth = m_chartView->scene()->width();
    titleItem->setFont(QFont("Arial", width * 25/1800, QFont::Bold)); 
    titleItem->setDefaultTextColor(QColor(255,255,255));
    titleItem->setPos((sceneWidth-rect.width())*35/100,0);  // 设置左上角 (10, 10) 位置
    m_chart->scene()->addItem(titleItem);

    // 创建图像项
    std::string imgPath = qApp->applicationDirPath().toStdString() + "/LOGO/decorder.png";
    QPixmap pixmap = QPixmap(QString::fromStdString(imgPath));
    QPixmap scaledPixmap = pixmap.scaled(width*15/1200, width*15/1200, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QGraphicsPixmapItem *imageItem = new QGraphicsPixmapItem(scaledPixmap);
    // 设置位置为左上角（0,0），稍微偏移避免挡住坐标轴
    imageItem->setPos(5, 5);
    imageItem->setZValue(1000); // 确保在最上层

    // 添加到 chart 的场景中
    m_chart->scene()->addItem(imageItem);

    // m_layout->addWidget(decorderLabel,0,0);
    m_layout->addWidget(m_chartView,1,0);
}
HGCurDataWidget::~HGCurDataWidget(){

}
void HGCurDataWidget::updateYAxis() {
    // 获取数据点
    auto points = m_series->points();
    if (points.isEmpty()) return;

    // 找到最大值和最小值
    double minY = points.first().y();
    double maxY = points.first().y();
    for (const QPointF &point : points) {
        if (point.y() < minY) minY = point.y();
        if (point.y() > maxY) maxY = point.y();
    }

    // 增加一定的缓冲
    double margin = (maxY - minY) * 0.1;
    if (margin < 1) margin = 1;

    m_yAxis->setRange(minY - margin, maxY + margin);
}
void HGCurDataWidget::fnUpdateData(const std::map<std::string, std::string>& msg){
    // append data
    std::string timestamp="";
    float value=0.0;
    for (const auto &kv : msg){
        // printf("msg %s:%s\n",kv.first.c_str(),kv.second.c_str());
        if (kv.first=="ts") timestamp=kv.second;
        else if (kv.first!="deviceNum" && kv.first!="sessionId") {value=std::atof(kv.second.c_str());}
        else {}
    }
    
    QDateTime now = QDateTime::currentDateTime();

    m_series->append(now.toMSecsSinceEpoch(), value);

    QDateTime fourtyEightHoursBefore=QDateTime(now.addDays(-2));
    m_xAxisDateTime->setRange(fourtyEightHoursBefore,now);

    updateYAxis();
}
void HGCurDataWidget::slotPointHoverd(const QPointF &point, bool state)
{
    if (state) {
        m_valueLabel->setText(QString::asprintf("%1.0f%", point.y()));

        QPoint curPos = (QCursor::pos());

        m_valueLabel->move(curPos.x() - m_valueLabel->width() / 2, curPos.y() - m_valueLabel->height() * 1.5);//移动数值

        m_valueLabel->show();//显示出来

        //显示被选中点的边框
        QScatterSeries *series1 = (QScatterSeries *)m_chartView->chart()->series().at(1);
        series1->clear();
        series1->append(point);
        series1->setVisible(true);
    }
    else {
        m_valueLabel->hide();//进行

        QScatterSeries *series1 = (QScatterSeries *)m_chartView->chart()->series().at(1);
        series1->setVisible(false);

    }
}

