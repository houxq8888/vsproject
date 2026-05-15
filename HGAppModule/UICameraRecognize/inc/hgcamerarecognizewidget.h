#ifndef HGCAMERARECOGNIZEWIDGET_H
#define HGCAMERARECOGNIZEWIDGET_H

#include <QObject>
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
#include "HGCommonTypes.h"
#include "CameraRecognizeInterface.h"



class HGCameraRecognizeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGCameraRecognizeWidget(std::string,bool outDB=false,QWidget *parent = nullptr);
    bool closeWindow();
    ~HGCameraRecognizeWidget();
    void outEnableDb();
    HGMACHINE::CameraRecognizeInfo getCameraRecognizeInfo();
    bool getCameraExist();

signals:
    void emitCameraInfo(std::string,std::string);
    void emitTemplateName(std::string,float);
    void emitROI(HGMACHINE::HGRect2D);
  

private slots:
    void onTempEditClicked();

private:
    bool eventFilter(QObject* obj,QEvent* event);
    void fnChangeParam();
    void fnReFetchCameraList();
    void fnReadDB();
    void fnWriteDB();
    void setChildrenEnabled(QWidget* parent,bool enabled);
    
private:
    QGroupBox* m_wholeGroup;
    QGridLayout* m_wholeLayout;
    QGridLayout* m_layout;
    std::string m_lang;

    QGroupBox* m_step1Group,*m_step2Group,*m_step3Group,*m_step4Group;
    QGridLayout* m_step1Layout,*m_step2Layout,*m_step3Layout,*m_step4Layout;
    QGroupBox* m_setGroup, *m_manipulateGroup;
    QGridLayout* m_setLayout, *m_manipulateLayout;

    QLabel* m_deviceNameLabel,*m_interfaceLabel;
    QComboBox* m_deviceNameComboBox,*m_interfaceComboBox;

    QGraphicsScene* m_scene;
    MyGraphicsView* m_view;
    QGraphicsPixmapItem *m_pix;

    QPushButton* m_saveToROIBtn,*m_saveBtn;
    QPushButton* m_captureBtn,*m_selectBtn,*m_saveTemplateBtn,*m_recognizeBtn;
    cv::Mat image;

    QLabel* m_scoreLabel,*m_tempLabel;
    QLineEdit* m_scoreEdit,*m_tempEdit;
    HGMACHINE::HGRect2D m_roiRect;
    
    HGMACHINE::CameraRecognizeInterface* m_cameraRecognize;
    
    std::string m_currentCameraType;
    std::string m_currentCameraName;
};

#endif // HGCAMERARECOGNIZEWIDGET_H
