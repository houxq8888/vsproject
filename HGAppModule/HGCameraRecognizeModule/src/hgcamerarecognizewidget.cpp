#include "hgcamerarecognizewidget.h"
#include "CameraRecognizeInterface.h"
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    connect(m_interfaceComboBox,&QComboBox::currentIndexChanged,this,&HGCameraRecognizeWidget::fnChangeParam);
#else
    connect(m_interfaceComboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this, &HGCameraRecognizeWidget::fnChangeParam);
#endif
    m_view=new MyGraphicsView(0);
    m_scene=new QGraphicsScene();
    m_view->setScene(m_scene);
    QPixmap pixmap(QString::fromStdString(getPath("/resources/V1/@print.png")));
    m_pix = m_scene->addPixmap(pixmap);
    m_view->fitInView(m_pix,Qt::KeepAspectRatio);

    m_captureBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Capture")));
    m_selectBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Select")));
    m_saveTemplateBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"SaveTemplate")));
    m_recognizeBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Recognize")));

    static int index=0;
    connect(m_captureBtn,&QPushButton::clicked,this,[=](){
        qDebug()<<"capture index:"<<index++;
        m_view->setSelectROI(false);
        m_view->clearRectItem();
        
        // 关闭之前打开的摄像头（如果有）
        if (!m_currentCameraType.empty() && !m_currentCameraName.empty()) {
            closeCamera(m_currentCameraType, m_currentCameraName);
            qDebug() << "Closed previous camera:" << QString::fromStdString(m_currentCameraType) 
                     << "-" << QString::fromStdString(m_currentCameraName);
        }
        
        // 打开摄像头
        std::string cameraType = m_interfaceComboBox->currentText().toStdString();
        std::string cameraName = m_deviceNameComboBox->currentText().toStdString();
        openCamera(cameraType, cameraName);
        
        // 更新当前摄像头状态
        m_currentCameraType = cameraType;
        m_currentCameraName = cameraName;
        
        // 获取图像
        image=getImgOneShotMat(m_interfaceComboBox->currentText().toStdString(),
            m_deviceNameComboBox->currentText().toStdString());

        // 检查图像是否获取成功
        if (image.empty()) {
            QMessageBox::warning(this, "Warning", 
                QString("Failed to capture image from %1 camera: %2")
                .arg(QString::fromStdString(cameraType))
                .arg(QString::fromStdString(cameraName)));
            qDebug() << "Failed to capture image from camera:" << QString::fromStdString(cameraType) 
                     << "-" << QString::fromStdString(cameraName);
            return;
        }

        QImage qimg;
        
        // 检查图像数据是否有效
        if (image.data && image.cols > 0 && image.rows > 0)
        {
            // 根据通道数创建QImage
            if (image.channels() == 1)
            {
                qimg = QImage((const uchar *)image.data, image.cols, image.rows, image.step, QImage::Format_Grayscale8);
            }
            else if (image.channels() == 3)
            {
                // OpenCV使用BGR格式，Qt使用RGB格式，需要转换
                qimg = QImage((const uchar *)image.data, image.cols, image.rows, image.step, QImage::Format_BGR888);
            }
            else
            {
                qDebug()<<"Unsupported image format with"<<image.channels()<<"channels";
                return;
            }
        }
        else
        {
            qDebug()<<"Failed to get image from camera";
            // 创建一个默认的黑色图像
            qimg = QImage(640, 480, QImage::Format_RGB888);
            qimg.fill(Qt::black);
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
        
        // 设置图像并调整视图
        m_pix->setPixmap(pixmap);
        m_view->fitInView(m_pix, Qt::KeepAspectRatio);
        
        qDebug()<<"Image captured successfully, size:"<<pixmap.size();
    });
    connect(m_selectBtn, &QPushButton::clicked, this, [=]()
    {
        m_view->clearRectItem();
        m_view->setSelectROI(true);
    });
    connect(m_saveTemplateBtn,&QPushButton::clicked,this,[=](){
        if (!(image.data && image.cols > 0 && image.rows > 0)){
            QMessageBox::warning(this,"Warning","No image available");
            return;
        }
        QRectF selectROI=m_view->getSelectROI();
        qDebug()<<"x1:"<<selectROI.left()<<"y1:"<<selectROI.top()<<"x2:"<<selectROI.right()<<"y2:"<<selectROI.bottom();
        
        // 检查ROI框是否有效
        if (selectROI.isEmpty() || selectROI.width() <= 0 || selectROI.height() <= 0) {
            QMessageBox::warning(this,"Warning","Invalid ROI selection");
            return;
        }
        
        HGRect2D roiRect(0,0,0,0);
        roiRect.x1=selectROI.left();
        roiRect.y1=selectROI.top();
        roiRect.x2=selectROI.right();
        roiRect.y2=selectROI.bottom();
        
        // 边界检查并调整
        if (roiRect.x1<0) roiRect.x1=0;
        if (roiRect.y1<0) roiRect.y1=0;
        if (roiRect.x2>image.cols) roiRect.x2=image.cols;
        if (roiRect.y2>image.rows) roiRect.y2=image.rows;
        
        // 检查调整后的ROI是否有效
        if (roiRect.x1 >= roiRect.x2 || roiRect.y1 >= roiRect.y2) {
            QMessageBox::warning(this,"Warning","ROI selection is outside image boundaries");
            return;
        }
        
        std::string templateName=saveCameraTemplateMat(image,roiRect);
        if (templateName=="failed"){
            QMessageBox::warning(this,"Warning","Failed to save template");
        } else {
            QMessageBox::information(this,"Information","Template saved as:"+QString::fromStdString(templateName));
        }
        // m_view->clearRectItem();
    });
    connect(m_recognizeBtn,&QPushButton::clicked,this,[=](){
        // 清除ROI框
        m_view->clearRectItem();
        m_view->setSelectROI(false);
        
        if (image.empty()){
            QMessageBox::warning(this,"Warning","No image available");
            return;
        }
        QString filePath=QFileDialog::getOpenFileName(nullptr,"OpenFile",QDir::homePath(),
            "image file(*.png *.jpg *.jpeg *.bmp *.gif);;"
            "txt file(*.txt *.csv);;"
            "all file(*.*)");
        HGImg2D img;
        img.data = image.data;
        img.width=image.cols;
        img.height=image.rows;
        img.type=image.type();
        MatchResult2D matchResult=recognizeCameraTemplate(img,filePath.toStdString());
        if (!matchResult.flag){
            QMessageBox::warning(this,"Warning","Failed to recognize template");
            return;
        }
        cv::imwrite("dst.jpg",matchResult.dstMat);
        cv::Mat dst;
        if (matchResult.dstMat.empty()){
            dst=cv::Mat(matchResult.dst.height,matchResult.dst.width,matchResult.dst.type,
                const_cast<uchar*>(matchResult.dst.data));
        } else {
            matchResult.dstMat.copyTo(dst);
        }
        QImage qimg;
        if (dst.channels() == 1)
        {
            qimg = QImage((const uchar *)dst.data, dst.cols, dst.rows, dst.step, QImage::Format_Grayscale8);
        }
        else if (dst.channels() == 3)
        {
            // OpenCV使用BGR格式，Qt使用RGB格式，需要转换
            qimg = QImage((const uchar *)dst.data, dst.cols, dst.rows, dst.step, QImage::Format_BGR888);
        }
        else
        {
            qDebug()<<"Unsupported image format with"<<dst.channels()<<"channels";
            return;
        }
        
        // 将识别结果显示到图像控件
        if (qimg.isNull()){
            qDebug()<<"failed to create QImage from recognition result";
            return;
        }
        
        QPixmap pixmap = QPixmap::fromImage(qimg);
        if (pixmap.isNull()){
            qDebug()<<"failed to convert to QPixmap";
            return;
        }
        
        // 设置识别结果图像并调整视图
        m_pix->setPixmap(pixmap);
        m_view->fitInView(m_pix, Qt::KeepAspectRatio);
        
        qDebug()<<"Recognition result displayed successfully, size:"<<pixmap.size();
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
    // 关闭当前打开的摄像头（如果有）
    if (!m_currentCameraType.empty() && !m_currentCameraName.empty()) {
        closeCamera(m_currentCameraType, m_currentCameraName);
        qDebug() << "Closed camera in destructor:" << QString::fromStdString(m_currentCameraType) 
                 << "-" << QString::fromStdString(m_currentCameraName);
    }
}
bool HGCameraRecognizeWidget::closeWindow(){
    return true;
}
void HGCameraRecognizeWidget::fnChangeParam(){
    // 关闭当前打开的摄像头（如果有）
    if (!m_currentCameraType.empty() && !m_currentCameraName.empty()) {
        closeCamera(m_currentCameraType, m_currentCameraName);
        qDebug() << "Closed previous camera:" << QString::fromStdString(m_currentCameraType) 
                 << "-" << QString::fromStdString(m_currentCameraName);
    }
    
    m_deviceNameComboBox->clear();
    std::vector<std::string> listCameras=getCameraList(m_interfaceComboBox->currentText().toStdString());
    for (auto item:listCameras){
        m_deviceNameComboBox->addItem(QString::fromStdString(item));
    }
    
    // 重置当前摄像头状态
    m_currentCameraType = "";
    m_currentCameraName = "";
    
    qDebug() << "Interface changed to:" << m_interfaceComboBox->currentText() 
             << "Available cameras:" << listCameras.size();
}
bool HGCameraRecognizeWidget::eventFilter(QObject* obj,QEvent* event){
    return QObject::eventFilter(obj,event);
}
