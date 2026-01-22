#include "labelwithimg.h"
#include <QPainter>


LabelWithImg::LabelWithImg(int imgPos,int fontSize,const std::string& imgPath, const std::string& text, QWidget *parent) : QWidget(parent)
{
    m_name=text;
    m_imgPath=imgPath;
    m_painterFlag=-1;
    m_fillFlag=false;
    m_isDragging=false;
    m_imgLabel=new HGQLabel;
    if (imgPath != "")
    {
        QPixmap pixmap(QString::fromStdString(imgPath));
        if (pixmap.width() / pixmap.height() == 1)
        {
            m_imgLabel->setPixmap(pixmap.scaled(QSize(35, 35), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
        else
        {
            m_imgLabel->setPixmap(pixmap);
        }
    }
    connect(m_imgLabel,SIGNAL(leftClicked()),this,SLOT(clickImg()));
    // m_imgLabel->setScaledContents(true);

    m_textLabel=new QLabel(QString::fromStdString(text));
    m_textLabel->installEventFilter(this);
    // m_textLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    m_textLabel->setAlignment(Qt::AlignCenter);

    QFont font=m_textLabel->font();
    font.setPointSize(fontSize);
    m_textLabel->setFont(font);

    m_layout=new QGridLayout();
    switch (imgPos){
        case IMGTOP:{
            m_layout->addWidget(m_imgLabel,0,0);
            m_layout->addWidget(m_textLabel,1,0);
            break;
        }
        case IMGLEFT:{
            m_layout->addWidget(m_imgLabel,0,0);
            m_layout->addWidget(m_textLabel,0,1);
            break;
        }
        case IMGRIGHT:{
            m_layout->addWidget(m_textLabel,0,0);
            m_layout->addWidget(m_imgLabel,0,1);
            break;
        }
        case IMGRIGHTBOTTOM:{
            m_layout->addWidget(m_textLabel,0,0);
            m_layout->addWidget(m_imgLabel,1,1);
            break;
        }
        case NOIMG:{
            m_layout->addWidget(m_textLabel,0,0);
            break;
        }
    }

    this->setLayout(m_layout);
}
bool LabelWithImg::eventFilter(QObject* obj,QEvent* event){
    if (event->type()==QEvent::MouseButtonPress){
        QMouseEvent* mouseEvent=static_cast<QMouseEvent*>(event);
        if (mouseEvent->button()==Qt::LeftButton){
            if (obj==m_textLabel){
                emit clickLeftName(m_name);
            }
        }
    } else {
        return QWidget::eventFilter(obj,event);
    }
    return true;
}
void LabelWithImg::mousePressEvent(QMouseEvent *event){
    if (event->button()==Qt::LeftButton){
        m_isDragging=true;
        m_lastMousePosition=event->pos();
        // m_currentRect=QRect(event->pos()-QPoint(25,25),QSize(50,50));
    }
    QWidget::mousePressEvent(event);
}
void LabelWithImg::clickImg()
{
    emit clickLeftName(m_name);
    emit clickImgLabel();
}
void LabelWithImg::setImg(const std::string& imgPath){
    QPixmap pixmap(QString::fromStdString(imgPath));
    m_imgLabel->setPixmap(pixmap);
    m_imgPath=imgPath;
}
void LabelWithImg::setLabelText(const std::string& text){
    m_textLabel->setText(QString::fromStdString(text));
}
void LabelWithImg::paintEvent(QPaintEvent* event){
    if (m_painterFlag<0) return;
    QPainter painter(this);
    if (m_painterFlag==0){
        painter.setBrush(Qt::gray);
    } else if (m_painterFlag==1){
        painter.setPen(QPen(Qt::white,2));
    }
    painter.setPen(QPen(Qt::black,2));
    painter.drawRect(0,0,this->width()-1,this->height()-1);

    if (m_fillFlag){
        painter.fillRect(QRect(0,0,this->width()-1,this->height()-1),QColor(147,210,243));
    }
    if (m_isDragging){
        painter.setBrush(Qt::blue);
        // painter.drawRect(m_currentRect);
    }
}
void LabelWithImg::setFillFlag(bool flag) 
{
    if (m_painterFlag<0) return;
    m_fillFlag=flag;
    update();
}
void LabelWithImg::mouseMoveEvent(QMouseEvent* event){
    if (m_isDragging){
        int dx=event->pos().x() - m_lastMousePosition.x();
        int dy=event->pos().y() - m_lastMousePosition.y();
        m_releaseMousePosition=event->pos();
        // m_currentRect.translate(dx,dy);
        // m_lastMousePosition=event->pos();
        update();
    }
}
void LabelWithImg::mouseReleaseEvent(QMouseEvent* event){
    if (event->button() == Qt::LeftButton){
        m_isDragging=false;
        int dx = m_releaseMousePosition.x()-m_lastMousePosition.x();
        if (dx >= this->width()) {
            m_releaseMousePosition.setX(0);
            m_releaseMousePosition.setY(0);
            emit createModule(m_name);
        }
        update();
    }
}