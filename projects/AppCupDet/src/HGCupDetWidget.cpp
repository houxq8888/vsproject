#include "HGCupDetWidget.h"
#include <QDebug>
#include <QRegularExpression>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include "loginterface.h"
#include "SvcFactory.h"

using namespace HGMACHINE;

HGCupDetWidget::HGCupDetWidget(QWidget *parent)
    : QWidget(parent)
{
    m_cameraControl = new CameraControlInterface();
    m_cupDet = new HGCupDetInterface();
    m_timeService = SvcFactory::CreateTimeService().get();
    m_basePath = qApp->applicationDirPath();
    this->setFixedSize(640*800/480, 800);
    OnInitial();

    this->show();
    m_listentimer = NULL;
    m_listentimer = new QTimer();
    connect(m_listentimer,SIGNAL(timeout()),this,SLOT(fnSlotListenTimerOut()));
    m_listentimer->start(200);
}

HGCupDetWidget::~HGCupDetWidget()
{
    if (m_listentimer == NULL) {
        delete m_listentimer;
        m_listentimer = NULL;
    }
    m_cameraControl->closeCamera();
    delete m_cameraControl;
    delete m_cupDet;
    delete m_timeService;
}

int HGCupDetWidget::OnInitial()
{
    m_index = 0;
    m_pic_label = new QLabel(this);
    m_pic_label->setFixedSize(640,480);
    m_pic_label->move(10,10);
    m_pic_label->setText("hello world");
    m_pic_label->setScaledContents(true);
    m_pic_label->show();

    m_prebtn = new QPushButton(this);
    m_prebtn->move(10, 500);
    m_prebtn->setText("pre");
    connect(m_prebtn,        SIGNAL(clicked()),this,SLOT(clickPre()));

    m_nextbtn = new QPushButton(this);
    m_nextbtn->move(20+m_prebtn->width(),500);
    m_nextbtn->setText("next");
    connect(m_nextbtn,        SIGNAL(clicked()),this,SLOT(clickNext()));

    m_searchbtn = new QPushButton(this);
    m_searchbtn->move(20+m_prebtn->width()+m_nextbtn->width(),500);
    m_searchbtn->setText("scan");
    connect(m_searchbtn,      SIGNAL(clicked()),this,SLOT(clickScan()));

    m_pauseBtn=new QPushButton(this);
    m_pauseBtn->move(20+m_prebtn->width()+m_nextbtn->width()+m_searchbtn->width(),500);
    m_pauseBtn->setText("pause");
    connect(m_pauseBtn,       SIGNAL(clicked()),this,SLOT(clickPause()));

    m_startBtn = new QPushButton(this);
    m_startBtn->move(20+m_prebtn->width()+m_nextbtn->width()+m_searchbtn->width()+m_pauseBtn->width(),500);
    m_startBtn->setText("start");
    connect(m_startBtn,       SIGNAL(clicked()),this,SLOT(clickStart()));

    m_elapsedLabel = new QLabel(this);
    m_elapsedLabel->move(20+m_prebtn->width()+m_nextbtn->width()+m_searchbtn->width()+m_pauseBtn->width()+m_startBtn->width(),500);
    m_elapsedLabel->setText("elapsed:");
    m_elapsedLabel->setFixedSize(200,20);

    m_scaleLabel=new QLabel(this);
    m_scaleLabel->setText("请输入比例：");
    m_scaleEdit=new QLineEdit(this);
    m_scaleEdit->setPlaceholderText("请输入比例......");
    m_scaleEdit->setEnabled(true);
    m_scaleEdit->setReadOnly(false);
    m_scaleEdit->setFocusPolicy(Qt::StrongFocus);
    connect(m_scaleEdit,SIGNAL(textChanged(QString)),this,SLOT(fnSlotScaleEditTextChanged(QString)));

    m_scaleLabel->move(m_pic_label->width()+20,20);
    m_scaleLabel->setFixedSize(100,20);

    m_scaleEdit->move(m_pic_label->width()+20+m_scaleLabel->width(),20);
    m_scaleEdit->setFixedSize(200,20);
    m_scaleEdit->setText("0.3");

    m_moveDistanceLabel = new QLabel(this);
    m_moveDistanceLabel->move(m_pic_label->width()+20,m_scaleLabel->height());
    m_moveDistanceLabel->setText("假设以图像中心为对位标准\n现瓶子卡槽位置为%d，\n像素距离%d，\n换算为毫米为%.2f mm.\n");
    m_moveDistanceLabel->setFixedSize(200,200);

    m_absenseLabel=new QLabel(this);
    // m_absenseLabel->setPixmap(QPixmap(":/images/absense.png"));
    m_absenseLabel->move(m_pic_label->width()+20,m_scaleLabel->height()+m_moveDistanceLabel->height()+20);
    m_absenseLabel->setFixedSize(200,200);

    m_cameraControl->openCamera("USB", "video=0");
    SvcFactory::CreateCommonService()->CreateDirectory(SAVE_IMG_PATH(m_basePath.toStdString()));

    UpdateImg();
    return 0;
}
void HGCupDetWidget::fnSlotScaleEditTextChanged(QString str)
{
    // QRegularExpression re("^\\d+$"); // 纯数字（整数）
    // if (re.match(str).hasMatch())
    // {
    //     qDebug() << "是纯数字";
    // }
    // else
    // {
    //     QMessageBox::warning(this,"提示","请输入纯数字");
    //     qDebug() << "是文字或混合内容";
    //     m_scaleEdit->setText("");
    // }
    bool ok;
    int number = str.toInt(&ok);

    if (ok)
    {
        qDebug() << "是数字：" << number;
    }
    else
    {
        double value = str.toDouble(&ok);
        if (ok)
        {
            qDebug() << "是浮点数：" << value;
        }
        else
        {
            qDebug() << "不是数字，是文字";
            QMessageBox::warning(this, "提示", "请输入纯数字");
            m_scaleEdit->setText("");
        }
    }
}
void HGCupDetWidget::clickPause()
{
    m_listentimer->stop();
}
void HGCupDetWidget::clickStart()
{
    m_listentimer->start(200);
}
void HGCupDetWidget::clickPre()
{
    if (m_fileLists.size() <= 0) return;
    m_index--;
    if (m_index<0) m_index=int(m_fileLists.size())-1;
    cv::Mat mat = cv::imread(m_fileLists[m_index],cv::IMREAD_UNCHANGED);
    if (mat.empty()) {
        // qDebug()<<"pre mat is empty";
        return;
    }

    // HGImg2D img;
    // img.width=mat.cols;
    // img.height=mat.rows;
    // img.channels=mat.channels();
    // img.data=mat.data;
    DetImg(mat);
    DisplayImg(mat);
}
void HGCupDetWidget::clickNext()
{
    if (m_fileLists.size() <= 0) return;
    m_index++;
    if (m_index>=int(m_fileLists.size())) m_index=0;
    cv::Mat mat = cv::imread(m_fileLists[m_index],cv::IMREAD_UNCHANGED);
    if (mat.empty()) {
        // qDebug()<<"next mat is empty";
        return;
    }
    // HGImg2D img;
    // img.width=mat.cols;
    // img.height=mat.rows;
    // img.channels=mat.channels();
    // img.data=mat.data;
    DetImg(mat);
    DisplayImg(mat);

    // m_saveDataToDB->setStr("key1","str1");
}
void HGCupDetWidget::clickScan()
{
    m_fileLists.clear();
    QString file_path = QFileDialog::getExistingDirectory(this,tr("Chosse Source Directory"),"~/",QFileDialog::ShowDirsOnly);
    if (file_path.isEmpty())
        return;
    else {
        m_scanPath = file_path;
    }
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    if (file_path.contains(QRegExp("[\\x4e00-\\x9fa5]+"))) {
        
        return;
    }
#endif
    m_scanPath+="/";

    QDir dir(m_scanPath);
    QStringList filters;
    filters << "*.jpg" << "*.png" << "*.bmp";
    dir.setNameFilters(filters);
    QFileInfoList fileList = dir.entryInfoList(QDir::Files);
    
    for (const QFileInfo& fileInfo : fileList) {
        m_fileLists.push_back(fileInfo.absoluteFilePath().toStdString());
    }

    qDebug()<<"size:"<<m_fileLists.size();

}
void HGCupDetWidget::DetImg(cv::Mat &mat)
{
    TimeInfo start = m_timeService->GetCurrentTime();
    m_cupDet->detCircle(mat, 0, 0, mat.cols, mat.rows);
    TimeInfo end = m_timeService->GetCurrentTime();
    
    double timeElapsed = m_timeService->GetElapsedMilliseconds(start, end);
    std::ostringstream content;
    content<<"elpased:"<<timeElapsed<<" ms";
    printf("elpased: %lld ms, content:%s\n", timeElapsed, content.str().c_str());
    m_elapsedLabel->setText(QString::fromStdString(content.str()));

    bool flag=m_cupDet->getAbsenseFlag();
    if (flag){
        m_absenseLabel->setPixmap(QPixmap(m_basePath+"/resources/ok.png"));
    } else {
        m_absenseLabel->setPixmap(QPixmap(m_basePath+"/resources/ng.png"));
    }
}
void HGCupDetWidget::DisplayImg(cv::Mat &mat)
{
    cv::Mat dst = m_cupDet->getDst();
    if (dst.empty()) {
        dst = mat;
    }

    QImage qimg;

    if (dst.channels() == 1)
    {
        qimg = QImage((const uchar*)dst.data, dst.cols, dst.rows, dst.step, QImage::Format_Grayscale8);
    }
    else if (dst.channels() == 3)
    {
        qimg = QImage((const uchar*)dst.data, dst.cols, dst.rows, dst.step, QImage::Format_BGR888);
    }
    else{}
    QPixmap pixmap = QPixmap::fromImage(qimg);
    m_pic_label->setPixmap(pixmap);
}

void HGCupDetWidget::UpdateImg()
{
    cv::Mat mat = m_cameraControl->getImgOneShot("USB", "video=0");
    if (mat.empty()) {
        // qDebug()<<"mat is empty";
        return;
    } else {
        std::ostringstream str;
        str << HGCUPDETNAME << mat.cols << "," << mat.rows;
        LOG_IF.logInfo(str.str());
        HGImg2D img;
        img.width=mat.cols;
        img.height=mat.rows;
        img.type=mat.type();
        img.data=mat.data;
        SvcFactory::CreateSaveService()->SaveImage(img,SAVE_IMG_PATH(m_basePath.toStdString()));
    }

    // HGImg2D img=getImgFromUSBOneShot();
    // detect image
    DetImg(mat);
    DisplayImg(mat);
    m_index++;
}
void HGCupDetWidget::fnSlotListenTimerOut()
{
    UpdateImg();
}
