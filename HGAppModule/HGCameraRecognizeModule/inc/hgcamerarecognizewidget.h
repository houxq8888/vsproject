#ifndef HGCAMERARECOGNIZEWIDGET_H
#define HGCAMERARECOGNIZEWIDGET_H

#include <QWidget>
#include "hgqlabel.h"
#include <QSplitter>
#include <QPushButton>
#include <QTableWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QTextEdit>
#include "mykeyboard.h"
#include "common.h"
#include "rectitem.h"
#include <QGraphicsScene>
#include "mygraphicsview.h"
#include "opencv2/opencv.hpp"


class HGCameraRecognizeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGCameraRecognizeWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();
    ~HGCameraRecognizeWidget();

signals:
  

private slots:
    void updateScenePos(int x,int y);

private:
    bool eventFilter(QObject* obj,QEvent* event);
    void fnChangeParam();
    
private:
    QGridLayout* m_layout;
    std::string m_lang;

    QGroupBox* m_setGroup, *m_manipulateGroup;
    QGridLayout* m_setLayout, *m_manipulateLayout;

    QLabel* m_deviceNameLabel,*m_interfaceLabel;
    QComboBox* m_deviceNameComboBox,*m_interfaceComboBox;

    QGraphicsScene* m_scene;
    MyGraphicsView* m_view;
    QGraphicsPixmapItem *m_pix;

    HGImg2D m_img;
    cv::Mat m_mat;
    QPushButton* m_captureBtn,*m_selectBtn,*m_saveTemplateBtn,*m_recognizeBtn;
};

#endif // HGCAMERARECOGNIZEWIDGET_H
