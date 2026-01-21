#ifndef HGCUPDETWIDGET_H
#define HGCUPDETWIDGET_H


#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QMouseEvent>
#include <QGridLayout>
#include <QGroupBox>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QWidget>
#include <QLineEdit>
#include "hgcapture2dfromusb.h"
#include "HGMacroData.h"
#include "hgsavedatatodb.h"

using namespace HGMACHINE;


#define HGCUPDETNAME "[HGCupDet]"

#define SAVE_IMG_PATH(base) base+"/img"

class HGCupDetWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HGCupDetWidget(QWidget *parent = nullptr);
    virtual ~HGCupDetWidget();

private slots:
    void fnSlotListenTimerOut();
    void clickPre();
    void clickNext();
    void clickScan();
    void clickPause();
    void clickStart();
    void fnSlotScaleEditTextChanged(QString str);

private:
    int OnInitial();
    void UpdateImg();
    void DetImg(cv::Mat &mat);
    void DisplayImg(cv::Mat &mat);


private:
    int m_width;
    int m_height;

    QLabel* m_absenseLabel;
    QLabel *m_pic_label;
    QLabel *m_elapsedLabel;
    QPushButton *m_prebtn;
    QPushButton *m_nextbtn;
    QPushButton *m_searchbtn;
    QPushButton *m_pauseBtn, *m_startBtn;
    QTimer * m_listentimer;

    QLabel* m_moveDistanceLabel;
    QLabel* m_scaleLabel;
    QLineEdit* m_scaleEdit;

    HGCapture2DFromUSB m_capture2DFromUSB;

    QString m_basePath;
    QString m_scanPath;

    std::vector<std::string> m_fileLists;

    HGSaveDataToDB *m_saveDataToDB;

    int m_index;

};

#endif // HGCUPDETWIDGET_H
