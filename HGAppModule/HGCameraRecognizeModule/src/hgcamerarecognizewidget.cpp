#include "hgcamerarecognizewidget.h"
#include "CameraRecognizeInterface.h"
#include <QDebug>
#include "opencv2/opencv.hpp"


HGCameraRecognizeWidget::HGCameraRecognizeWidget(std::string name,QWidget *parent)
    : QWidget(parent)
{
    m_lang = name; 
    m_layout=new QGridLayout();
    this->setLayout(m_layout);
    m_setLayout=new QGridLayout();
    m_manipulateLayout=new QGridLayout();
    m_setGroup=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"Set")));
    m_manipulateGroup=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"Display")));
    m_setGroup->setLayout(m_setLayout);
    m_manipulateGroup->setLayout(m_manipulateLayout);

    m_deviceNameLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"DeviceName")));
    m_deviceNameComboBox=new QComboBox();
    
    m_interfaceLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"Interface")));
    m_interfaceComboBox=new QComboBox();
    m_interfaceComboBox->addItems({"USB","IP"});
    m_interfaceComboBox->setCurrentIndex(0);
    fnChangeParam();
    connect(m_interfaceComboBox,QOverload<int>::of(&QComboBox::currentIndexChanged),this,&HGCameraRecognizeWidget::fnChangeParam);

    m_view=new MyGraphicsView(0);
    m_scene=new QGraphicsScene();
    m_view->setScene(m_scene);
    QPixmap pixmap(QString::fromStdString(getPath("/resources/V1/@print.png")));
    connect(m_view,SIGNAL(getScenePos(int,int)),this,SLOT(updateScenePos(int,int)));
    m_pix = m_scene->addPixmap(pixmap);
    m_view->fitInView(m_pix,Qt::KeepAspectRatio);

    m_captureBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Capture")));
    m_selectBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Select")));
    m_saveTemplateBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"SaveTemplate")));
    m_recognizeBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Recognize")));

    connect(m_captureBtn,&QPushButton::clicked,this,[=](){
        openCamera(m_interfaceComboBox->currentText().toStdString(),m_deviceNameComboBox->currentText().toStdString());
        m_mat=getImgOneShotMat(m_interfaceComboBox->currentText().toStdString(),
            m_deviceNameComboBox->currentText().toStdString());

        QImage qimg;
        if (m_mat.data && m_mat.cols > 0 && m_mat.rows > 0)
        {
            if (m_mat.channels() == 1)
            {
                qimg = QImage((const uchar *)m_mat.data, m_mat.cols, m_mat.rows, m_mat.step, QImage::Format_Grayscale8);
            }
            else if (m_mat.channels() == 3)
            {
                qimg = QImage((const uchar *)m_mat.data, m_mat.cols, m_mat.rows, m_mat.step, QImage::Format_BGR888);
            }
            else
            {
            }
        }
        if (qimg.isNull()){
            qDebug()<<"failed to create QImage";
            return;
        }
        QPixmap pixmap = QPixmap::fromImage(qimg);
        if (pixmap.isNull()){
            qDebug()<<"failed to convert to QPixmap";
            return;
        }
        m_pix->setPixmap(pixmap);
    });
    connect(m_selectBtn, &QPushButton::clicked, this, [=]()
    {
        m_view->setSelectROI(true);
    });
    connect(m_saveTemplateBtn,&QPushButton::clicked,this,[=](){
        if (!(m_mat.data && m_mat.cols > 0 && m_mat.rows > 0)){
            qDebug()<<"failed to get image";
            return;
        }
        QRectF rect=m_view->getSelectROI();
        HGRect2D roiRect(rect.left(),rect.top(),rect.right(),rect.bottom());
        if (roiRect.x1<0) roiRect.x1=0;
        if (roiRect.y1<0) roiRect.y1=0;
        if (roiRect.x2>m_mat.cols) roiRect.x2=m_mat.cols;
        if (roiRect.y2>m_mat.rows) roiRect.y2=m_mat.rows;
        saveCameraTemplateMat(m_mat,roiRect);
    });
    connect(m_recognizeBtn,&QPushButton::clicked,this,[=](){
        recognizeCameraTemplate(m_img);
    });
    m_setLayout->addWidget(m_deviceNameLabel,0,0);
    m_setLayout->addWidget(m_deviceNameComboBox,0,1);
    m_setLayout->addWidget(m_interfaceLabel,1,0);
    m_setLayout->addWidget(m_interfaceComboBox,1,1);

    m_manipulateLayout->addWidget(m_view,0,0,1,4);
    m_manipulateLayout->addWidget(m_captureBtn,1,0);
    m_manipulateLayout->addWidget(m_selectBtn,1,1);
    m_manipulateLayout->addWidget(m_saveTemplateBtn,1,2);
    m_manipulateLayout->addWidget(m_recognizeBtn,1,3);

    m_layout->addWidget(m_setGroup,0,0);
    m_layout->addWidget(m_manipulateGroup,1,0);
}
HGCameraRecognizeWidget::~HGCameraRecognizeWidget(){

}
bool HGCameraRecognizeWidget::closeWindow(){
    return true;
}
void HGCameraRecognizeWidget::fnChangeParam(){
    m_deviceNameComboBox->clear();
    std::vector<std::string> listCameras=getCameraList(m_interfaceComboBox->currentText().toStdString());
    for (auto item:listCameras){
        m_deviceNameComboBox->addItem(QString::fromStdString(item));
    }
}
bool HGCameraRecognizeWidget::eventFilter(QObject* obj,QEvent* event){
    return QObject::eventFilter(obj,event);
}

void HGCameraRecognizeWidget::updateScenePos(int x,int y)
{
    QPointF linepoint;
    if(m_pix!=nullptr&&m_pix->boundingRect().width()!=0&&m_pix->boundingRect().height()!=0){
        linepoint.setX(m_pix->mapFromScene(x*m_scene->width()/m_pix->boundingRect().width(),
                                           y*m_scene->height()/m_pix->boundingRect().height()).x());
        linepoint.setY(m_pix->mapFromScene(x*m_scene->width()/m_pix->boundingRect().width(),
                                         y*m_scene->height()/m_pix->boundingRect().height()).y());

        int grayval=-1;

        m_view->showImageGray(linepoint.x(),linepoint.y(),grayval);
    }
}