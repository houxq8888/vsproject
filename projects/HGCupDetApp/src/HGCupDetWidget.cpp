#include "HGCupDetWidget.h"
#include <QDebug>
#include <QRegularExpression>
#include <QFileDialog>
#include <QMessageBox>
#include "hglog4cplus.h"
#include "hgsaveimgtolocaldisk.h"
#include "hgcommonutility.h"
#include "hgsavedatatodb.h"
#include "HGCupDetInterface.h"
// #include "hgonnxdetect.h"
#include "hgdetectcircle.h"

using namespace HGMACHINE;

HGCupDetWidget::HGCupDetWidget(QWidget *parent)
    : QWidget(parent)
{
    // m_saveDataToDB=new HGSaveDataToDB();
    // m_saveDataToDB->openDB("./cupdet.db");
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
    // closeUSBCamera();
    m_capture2DFromUSB.close();
    // m_saveDataToDB->closeDB();
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

    m_capture2DFromUSB.open(0);
    // openUSBCamera("/dev/media0");

    int ret=HGMkDir(SAVE_IMG_PATH(m_basePath.toStdString()));
    if (ret == 0) {
        std::ostringstream name;
        name << SAVE_IMG_PATH(m_basePath.toStdString()) << "mkdir success\n";
//        printf(name.str().c_str());
//        HGLog4Cplus::getLogInstance(LOG_PATH)->logout(name.str(),LOGINFO);
    } else {
        std::ostringstream name;
        name << SAVE_IMG_PATH(m_basePath.toStdString()) << "mkdir failed\n";
        printf(name.str().c_str());
    }

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

    std::vector<std::string> fileLists;
    HGGetFiles(m_scanPath.toStdString(),".jpg",fileLists);
    for (int i=0;i<int(fileLists.size());i++) {
        m_fileLists.push_back(fileLists[i]);
    }
    fileLists.clear();
    HGGetFiles(m_scanPath.toStdString(),".png",fileLists);
    for (int i=0;i<int(fileLists.size());i++) {
        m_fileLists.push_back(fileLists[i]);
    }
    fileLists.clear();
    HGGetFiles(m_scanPath.toStdString(),".bmp",fileLists);
    for (int i=0;i<int(fileLists.size());i++) {
        m_fileLists.push_back(fileLists[i]);
    }

    qDebug()<<"size:"<<m_fileLists.size();

}
void HGCupDetWidget::DetImg(cv::Mat &mat)
{
    // HGOnnxDetect onnxDet;
    // onnxDet.detect(mat, cv::Rect(0,0,0,0));
    // return ;
    
    HGImg2D img;
    img.width=mat.cols;
    img.height=mat.rows;
    img.type=mat.type();
    img.data=mat.data;
    HGRect2D roi;
    roi.x1=0;
    roi.y1=0;
    roi.x2=mat.cols;
    roi.y2=mat.rows;
    HGExactTime start=HGGetTime();
    detCircle(img,roi);
    HGExactTime end=HGGetTime();
    double timeElapsed=HGCalTimeElapsed(start,end);
    std::ostringstream content;
    content<<"elpased:"<<timeElapsed<<" ms";
    printf("elpased: %.2f ms, content:%s\n", timeElapsed,content.str().c_str());
    m_elapsedLabel->setText(QString::fromStdString(content.str()));

    bool flag=getAbsenseFlag();
    if (flag){
        m_absenseLabel->setPixmap(QPixmap(m_basePath+"/resources/ok.png"));
    } else {
        m_absenseLabel->setPixmap(QPixmap(m_basePath+"/resources/ng.png"));
    }
}
void HGCupDetWidget::DisplayImg(cv::Mat &mat)
{
    HGImg2D img=getDst();
    // cv::Mat image(img.height, img.width, img.type, img.data);
    cv::Mat image(img.height, img.width, img.type, const_cast<uchar*>(img.data)); 

    QImage qimg;

    if (image.channels() == 1)
    {
        qimg = QImage((const uchar*)image.data, image.cols, image.rows, image.step, QImage::Format_Grayscale8);
    }
    else if (image.channels() == 3)
    {
        qimg = QImage((const uchar*)image.data, image.cols, image.rows, image.step, QImage::Format_BGR888);
    }
    else{}
    QPixmap pixmap = QPixmap::fromImage(qimg);
    m_pic_label->setPixmap(pixmap);
}

void HGCupDetWidget::UpdateImg()
{
    cv::Mat mat;
    m_capture2DFromUSB.getFrameOne(mat);
    if (mat.empty()) {
        // qDebug()<<"mat is empty";
        return;
    } else {
        std::ostringstream str;
        str << HGCUPDETNAME << mat.cols << "," << mat.rows;
        HGLog4Cplus::getLogInstance(LOG_PATH)->logout(str.str(),LOGINFO);
        HGSaveImgToLocalDisk::getSaveImgInstance()->save(mat,SAVE_IMG_PATH(m_basePath.toStdString()));
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
