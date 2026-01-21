#include "hgcamerarecognizewidget.h"
#include "CameraRecognizeInterface.h"
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include "opencv2/opencv.hpp"
#include "config.h"


using namespace HGMACHINE;
HGCameraRecognizeWidget::HGCameraRecognizeWidget(std::string name,bool outDB,QWidget *parent)
    : QWidget(parent),
    m_currentCameraType(""),
    m_currentCameraName("")
{
    if (outDB) outEnableDb();  
    m_lang = name; 
    m_layout=new QGridLayout();
    this->setLayout(m_layout);
    m_wholeGroup=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"Whole")));
    m_wholeGroup->setCheckable(true);
    m_wholeGroup->setChecked(false);
    m_wholeLayout=new QGridLayout(m_wholeGroup);
    m_layout->addWidget(m_wholeGroup);

    m_setLayout=new QGridLayout();
    m_manipulateLayout=new QGridLayout();
    m_setGroup=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"Set")));
    m_manipulateGroup=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"Display")));
    m_setGroup->setLayout(m_setLayout);
    m_manipulateGroup->setLayout(m_manipulateLayout);

    m_deviceNameLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"DeviceName")));
    m_deviceNameComboBox=new QComboBox();
    connect(m_deviceNameComboBox,QOverload<int>::of(&QComboBox::currentIndexChanged),this,&HGCameraRecognizeWidget::fnChangeParam);

    m_interfaceLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"Interface")));
    m_interfaceComboBox=new QComboBox();
    m_interfaceComboBox->addItems({"USB","IP"});
    m_interfaceComboBox->setCurrentIndex(0);
    fnReFetchCameraList();
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
    m_view->setDragMode(QGraphicsView::ScrollHandDrag);
    m_view->setCursor(Qt::OpenHandCursor);

    m_captureBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Capture")));
    m_selectBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Select")));
    m_saveTemplateBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"SaveTemplate")));
    m_recognizeBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Recognize")));
    m_saveToROIBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"SaveToROI")));
    m_saveBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Save")));

    QPixmap templatepixmap(QString::fromStdString(getPath("/resources/@template.png")));
    QPixmap scaledPixmap=templatepixmap.scaled(200,200,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    QGraphicsPixmapItem* imageItem=new QGraphicsPixmapItem(scaledPixmap);
    imageItem->setPos(0,0);
    imageItem->setZValue(1);
    m_scene->addItem(imageItem);

    m_scoreLabel=new QLabel("分数[0-1,漏检，值设低；有过检，值设高]");
    m_tempLabel=new QLabel("模板");
    m_scoreEdit=new QLineEdit("");
    m_scoreEdit->setPlaceholderText("请输入识别分数");
    m_tempEdit=new QLineEdit("");
    m_tempEdit->setPlaceholderText("请加载识别模板");
    m_tempEdit->setReadOnly(true);
    m_tempEdit->installEventFilter(this);

    m_step1Group=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"Step1")));
    m_step1Layout=new QGridLayout(m_step1Group);
    m_step2Group=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"Step2")));
    m_step2Layout=new QGridLayout(m_step2Group);
    m_step3Group=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"Step3")));
    m_step3Layout=new QGridLayout(m_step3Group);
    m_step4Group=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"Step4")));
    m_step4Layout=new QGridLayout(m_step4Group);

    connect(m_captureBtn,&QPushButton::clicked,this,[=](){
        m_view->setSelectROI(false);
        m_view->clearRectItem();
    
        // 打开摄像头
        QString cameraTypeQStr = m_interfaceComboBox->currentText();
        QString cameraNameQStr = m_deviceNameComboBox->currentText();
        
        // 检查字符串是否为空，避免空指针异常
        if (cameraTypeQStr.isEmpty() || cameraNameQStr.isEmpty()) {
            qDebug() << "Camera type or name is empty, cannot proceed";
            QTimer::singleShot(0, this, [this]() {
                QMessageBox::warning(this, "Warning", "Camera type or name is empty. Please select valid camera settings.");
            });
            return;
        }
        
        std::string cameraType = cameraTypeQStr.toStdString();
        std::string cameraName = cameraNameQStr.toStdString();
        openCamera(cameraType, cameraName);
        
        // 更新当前摄像头状态
        m_currentCameraType = cameraType;
        m_currentCameraName = cameraName;
        
        // 获取图像
        image=getImgOneShotMat(cameraType, cameraName);

        // 检查图像是否获取成功
        if (image.empty()) {
            // 使用QTimer::singleShot确保在主线程中安全显示对话框
            QTimer::singleShot(0, this, [this, cameraType, cameraName]() {
                QMessageBox::warning(this, "Warning", 
                    QString("Failed to capture image from %1 camera: %2")
                    .arg(QString::fromStdString(cameraType))
                    .arg(QString::fromStdString(cameraName)));
            });
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
    connect(m_saveBtn,&QPushButton::clicked,this,[=](){
        closeWindow();
        QString templateName = m_tempEdit->text();
        if (templateName.isEmpty()) {
            qDebug() << "Template name is empty";
            return;
        }
        emit emitTemplateName(templateName.toStdString(),m_scoreEdit->text().toFloat());
    });
    connect(m_saveToROIBtn,&QPushButton::clicked,this,[=](){
        QRectF selectROI=m_view->getSelectROI();
        qDebug()<<"x1:"<<selectROI.left()<<"y1:"<<selectROI.top()<<"x2:"<<selectROI.right()<<"y2:"<<selectROI.bottom();
        
        // 检查ROI框是否有效
        if (selectROI.isEmpty() || selectROI.width() <= 0 || selectROI.height() <= 0) {
            // 使用QTimer::singleShot确保在主线程中安全显示对话框
            QTimer::singleShot(0, this, [this]() {
                QMessageBox::warning(this,"Warning","Invalid ROI selection");
            });
            return;
        }
        
        HGRect2D roiRect(0,0,0,0);
        roiRect.x1=selectROI.left();
        roiRect.y1=selectROI.top();
        roiRect.x2=selectROI.right();
        roiRect.y2=selectROI.bottom();
        if (roiRect.x1<0) roiRect.x1=0;
        if (roiRect.y1<0) roiRect.y1=0;
        if (roiRect.x2>image.cols) roiRect.x2=image.cols;
        if (roiRect.y2>image.rows) roiRect.y2=image.rows;
        if (roiRect.x1 >= roiRect.x2 || roiRect.y1 >= roiRect.y2) {
            // 使用QTimer::singleShot确保在主线程中安全显示对话框
            QTimer::singleShot(0, this, [this]() {
                QMessageBox::warning(this,"Warning","ROI selection is outside image boundaries");
            });
            return;
        }
        GlobalSingleton::instance().setSystemInfo("ROIX1",std::to_string(m_roiRect.x1));
        GlobalSingleton::instance().setSystemInfo("ROIY1",std::to_string(m_roiRect.y1));
        GlobalSingleton::instance().setSystemInfo("ROIX2",std::to_string(m_roiRect.x2));
        GlobalSingleton::instance().setSystemInfo("ROIY2",std::to_string(m_roiRect.y2));
    });
    connect(m_saveTemplateBtn,&QPushButton::clicked,this,[=](){
        if (!(image.data && image.cols > 0 && image.rows > 0)){
            // 使用QTimer::singleShot确保在主线程中安全显示对话框
            QTimer::singleShot(0, this, [this]() {
                QMessageBox::warning(this,"Warning","No image available");
            });
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
            // 使用QTimer::singleShot确保在主线程中安全显示对话框
            QTimer::singleShot(0, this, [this]() {
                QMessageBox::warning(this,"Warning","Failed to save template");
            });
        } else {
            // 使用QTimer::singleShot确保在主线程中安全显示对话框
            QTimer::singleShot(0, this, [this, templateName]() {
                QMessageBox::information(this,"Information","Template saved as:"+QString::fromStdString(templateName));
            });
        }
        m_view->clearRectItem();
    });
    connect(m_recognizeBtn,&QPushButton::clicked,this,[=](){
        qDebug() << "HGCameraRecognizeWidget::recognizeBtn clicked - Starting recognition process";
        
        // 先停止绘制模式，再清除ROI框
        qDebug() << "HGCameraRecognizeWidget::recognizeBtn - Stopping drawing mode";
        m_view->setSelectROI(false);
        
        // 清除ROI框
        qDebug() << "HGCameraRecognizeWidget::recognizeBtn - Clearing rect item";
        m_view->clearRectItem();
        
        // 给场景一些时间来更新BSP树
        QApplication::processEvents();
        
        if (image.empty()){
            qDebug() << "HGCameraRecognizeWidget::recognizeBtn - Image is empty, showing warning";
            // 使用QTimer::singleShot确保在主线程中安全显示对话框
            QTimer::singleShot(0, this, [this]() {
                qDebug() << "HGCameraRecognizeWidget::recognizeBtn - Showing 'No image available' dialog";
                QMessageBox::warning(this,"Warning","No image available");
                qDebug() << "HGCameraRecognizeWidget::recognizeBtn - Dialog closed";
            });
            return;
        }

        HGImg2D img;
        img.data = image.data;
        img.width=image.cols;
        img.height=image.rows;
        img.type=image.type();
        QString filePath=m_tempEdit->text();
        qDebug() << "HGCameraRecognizeWidget::recognizeBtn - Template file path:" << filePath;
        if (filePath==""){
            qDebug() << "HGCameraRecognizeWidget::recognizeBtn - No template set, showing warning dialog";
            // 使用QTimer::singleShot确保在主线程中安全显示对话框
            QTimer::singleShot(0, this, [this]() {
                qDebug() << "HGCameraRecognizeWidget::recognizeBtn - Showing '没有设置模板' dialog";
                QMessageBox::warning(this, "提示", "没有设置模板");
                qDebug() << "HGCameraRecognizeWidget::recognizeBtn - '没有设置模板' dialog closed";
            });
            return;
        }
        float score=m_scoreEdit->text().toFloat();
        
        // 检查ROI区域是否有效，如果无效则使用整个图像区域
        HGRect2D roiToUse = m_roiRect;
        if (roiToUse.x1 >= roiToUse.x2 || roiToUse.y1 >= roiToUse.y2 || 
            roiToUse.x2 <= 0 || roiToUse.y2 <= 0) {
            // ROI无效，使用整个图像区域
            roiToUse.x1 = 0;
            roiToUse.y1 = 0;
            roiToUse.x2 = img.width;
            roiToUse.y2 = img.height;
        }
        
        // 边界检查，确保ROI在图像范围内
        if (roiToUse.x1 < 0) roiToUse.x1 = 0;
        if (roiToUse.y1 < 0) roiToUse.y1 = 0;
        if (roiToUse.x2 > img.width) roiToUse.x2 = img.width;
        if (roiToUse.y2 > img.height) roiToUse.y2 = img.height;
        if (roiToUse.x1 >= roiToUse.x2) roiToUse.x2 = roiToUse.x1 + 1;
        if (roiToUse.y1 >= roiToUse.y2) roiToUse.y2 = roiToUse.y1 + 1;
        
        QString filePathQStr = filePath;
        if (filePathQStr.isEmpty()) {
            qDebug() << "File path is empty, cannot proceed with recognition";
            QTimer::singleShot(0, this, [this]() {
                QMessageBox::warning(this, "提示", "模板文件路径为空");
            });
            return;
        }
        MatchResult2D matchResult=recognizeCameraTemplate(img,roiToUse,score,filePathQStr.toStdString());
        if (!matchResult.flag){
            // 使用QTimer::singleShot确保在主线程中安全显示对话框
            QTimer::singleShot(0, this, [this]() {
                QMessageBox::warning(this,"Warning","Failed to recognize template");
            });
            return;
        }

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

    m_manipulateLayout->addWidget(m_view,0,0,1,5);
    m_manipulateLayout->addWidget(m_step1Group,1,0);
    m_manipulateLayout->addWidget(m_step2Group,1,1);
    m_manipulateLayout->addWidget(m_step3Group,1,2);
    m_manipulateLayout->addWidget(m_step4Group,1,3);

    m_step1Layout->addWidget(m_captureBtn,0,0);
    m_step2Layout->addWidget(m_selectBtn,0,0);
    m_step2Layout->addWidget(m_saveToROIBtn,0,1);
    m_step2Layout->addWidget(m_saveTemplateBtn,1,1);
    m_step3Layout->addWidget(m_tempLabel,0,0);
    m_step3Layout->addWidget(m_tempEdit,0,1);
    m_step3Layout->addWidget(m_scoreLabel,1,0);
    m_step3Layout->addWidget(m_scoreEdit,1,1);
    m_step3Layout->addWidget(m_saveBtn,2,0,1,2);
    m_step4Layout->addWidget(m_recognizeBtn,0,0);

    m_wholeLayout->addWidget(m_setGroup,0,0);
    m_wholeLayout->addWidget(m_manipulateGroup,1,0);

    connect(m_wholeGroup,&QGroupBox::toggled,[this](bool checked){
        setChildrenEnabled(m_wholeGroup,checked);
    });
}
void HGCameraRecognizeWidget::setChildrenEnabled(QWidget* parent,bool enabled){
    if (parent==m_wholeGroup){
        QLayout* layout=parent->layout();
        if (layout){
            for (int i=0;i<layout->count();++i){
                QWidget* widget=layout->itemAt(i)->widget();
                if (widget){
                    widget->setEnabled(enabled);
                }
            }
        }
    } else {
        parent->setEnabled(enabled);
    }
}
HGCameraRecognizeWidget::~HGCameraRecognizeWidget(){
    // 关闭当前打开的摄像头（如果有）
    if (!m_currentCameraType.empty() && !m_currentCameraName.empty()) {
        closeCamera(m_currentCameraType, m_currentCameraName);
        qDebug() << "Closed camera in destructor:" << QString::fromStdString(m_currentCameraType) 
                 << "-" << QString::fromStdString(m_currentCameraName);
    }
}
void HGCameraRecognizeWidget::onTempEditClicked(){
    QString filePath=QFileDialog::getOpenFileName(
        nullptr,
        "打开文件",
        qApp->applicationDirPath()+"/template",
        "图像文件(*.png *.jpg *.jpeg *.bmp *.gif);;"
        "文本文件(*.txt *.csv);;"
        "所有文件(*.*)"
    );
    if (filePath=="") return;
    m_tempEdit->setText(filePath);
    QString filePathQStr = filePath;
    if (filePathQStr.isEmpty()) {
        qDebug() << "File path is empty, cannot emit template name";
        return;
    }
    emit emitTemplateName(filePathQStr.toStdString(),m_scoreEdit->text().toFloat());
}

bool HGCameraRecognizeWidget::closeWindow(){
    fnWriteDB();
    return true;
}
void HGCameraRecognizeWidget::outEnableDb(){
    QString basePath=qApp->applicationDirPath();
    std::string dbDir=basePath.toStdString()+"/database";
    HGMkDir(dbDir);

    std::string configPath=basePath.toStdString()+"/config";
    HGMkDir(configPath);
    std::string path=configPath+"/config.xml";
    bool valid=isFileExist(path);
    if (!valid){
        FileConfig::createConfigFile(path,"CameraInterface");
    } else {
        FileConfig::loadConfigFile(path);
    }
    FileConfig::setDirPath(basePath.toStdString());
    FileConfig::saveConfigFile("CameraInterface");
    RWDb::openDB(basePath.toStdString());
}
void HGCameraRecognizeWidget::fnReadDB(){
    m_currentCameraType=GlobalSingleton::instance().getSystemInfo("CameraType");
    m_currentCameraName=GlobalSingleton::instance().getSystemInfo("CameraName");
    m_roiRect.x1=std::atoi(GlobalSingleton::instance().getSystemInfo("ROIX1").c_str());
    m_roiRect.y1=std::atoi(GlobalSingleton::instance().getSystemInfo("ROIY1").c_str());
    m_roiRect.x2=std::atoi(GlobalSingleton::instance().getSystemInfo("ROIX2").c_str());
    m_roiRect.y2=std::atoi(GlobalSingleton::instance().getSystemInfo("ROIY2").c_str());
    m_interfaceComboBox->setCurrentText(QString::fromStdString(m_currentCameraType));
    m_deviceNameComboBox->setCurrentText(QString::fromStdString(m_currentCameraName));
    m_scoreEdit->setText(QString::fromStdString(GlobalSingleton::instance().getSystemInfo("CameraScore")));
    m_tempEdit->setText(QString::fromStdString(GlobalSingleton::instance().getSystemInfo("CameraTemplateName")));
}
CameraRecognizeInfo HGCameraRecognizeWidget::getCameraRecognizeInfo(){
    fnReadDB();
    CameraRecognizeInfo info;
    info.cameraType=m_currentCameraType;
    info.cameraName=m_currentCameraName;
    info.score=std::stof(GlobalSingleton::instance().getSystemInfo("CameraScore"));
    info.templatePath=GlobalSingleton::instance().getSystemInfo("CameraTemplateName");
    info.roi.x1=m_roiRect.x1;
    info.roi.y1=m_roiRect.y1;
    info.roi.x2=m_roiRect.x2;
    info.roi.y2=m_roiRect.y2;
    return info;
}
void HGCameraRecognizeWidget::fnWriteDB(){
    GlobalSingleton::instance().setSystemInfo("CameraType",m_currentCameraType);
    GlobalSingleton::instance().setSystemInfo("CameraName",m_currentCameraName);
    QString scoreStr = m_scoreEdit->text();
    QString templateNameStr = m_tempEdit->text();
    if (!scoreStr.isEmpty()) {
        GlobalSingleton::instance().setSystemInfo("CameraScore",scoreStr.toStdString());
    }
    if (!templateNameStr.isEmpty()) {
        GlobalSingleton::instance().setSystemInfo("CameraTemplateName",templateNameStr.toStdString());
    }
    GlobalSingleton::instance().saveSystemInfo();
}
void HGCameraRecognizeWidget::fnReFetchCameraList(){
    if (!m_currentCameraType.empty() && !m_currentCameraName.empty()) {
        closeCamera(m_currentCameraType, m_currentCameraName);
        qDebug() << "Closed previous camera:" << QString::fromStdString(m_currentCameraType) 
                 << "-" << QString::fromStdString(m_currentCameraName);
    }
    
    m_deviceNameComboBox->clear();
    QString interfaceType = m_interfaceComboBox->currentText();
    if (interfaceType.isEmpty()) {
        qDebug() << "Interface type is empty, cannot get camera list";
        return;
    }
    std::vector<std::string> listCameras=getCameraList(interfaceType.toStdString());
    for (auto item:listCameras){
        m_deviceNameComboBox->addItem(QString::fromStdString(item));
    }
}
bool HGCameraRecognizeWidget::getCameraExist(){
    int index = m_deviceNameComboBox->findText(QString::fromStdString(m_currentCameraName));
    return (index==-1?false:true);
}
void HGCameraRecognizeWidget::fnChangeParam(){
    // 关闭当前打开的摄像头（如果有）
    if (!m_currentCameraType.empty() && !m_currentCameraName.empty()) {
        closeCamera(m_currentCameraType, m_currentCameraName);
        qDebug() << "Closed previous camera:" << QString::fromStdString(m_currentCameraType) 
                 << "-" << QString::fromStdString(m_currentCameraName);
    }
    
    // 重置当前摄像头状态
    QString interfaceType = m_interfaceComboBox->currentText();
    QString cameraName = m_deviceNameComboBox->currentText();
    
    if (interfaceType.isEmpty() || cameraName.isEmpty()) {
        qDebug() << "Camera type or name is empty, cannot reset camera info";
        return;
    }
    
    m_currentCameraType = interfaceType.toStdString();
    m_currentCameraName = cameraName.toStdString();
    emit emitCameraInfo(m_currentCameraType,m_currentCameraName);
}
bool HGCameraRecognizeWidget::eventFilter(QObject* obj,QEvent* event){
    if (event->type()==QEvent::MouseButtonPress){
        QMouseEvent* mouseEvent=static_cast<QMouseEvent*>(event);
        if (mouseEvent->button()==Qt::LeftButton){
            if (obj==m_tempEdit)
                onTempEditClicked();
            qDebug() << "Left mouse button pressed at:" << mouseEvent->pos();
        }
    } else {
        return QObject::eventFilter(obj,event);
    }
    return true;
}
