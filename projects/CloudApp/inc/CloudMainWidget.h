#ifndef CLOUDMAINWIDGET_H
#define CLOUDMAINWIDGET_H 

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <pcl/visualization/pcl_visualizer.h>
// #include <pcl/point_type.h>
#include <pcl/io/pcd_io.h>

class CloudMainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CloudMainWidget(QWidget *parent = nullptr);
    virtual ~CloudMainWidget();

private slots:
    void slotLoadCloud();

private:
    void fnSetupUI();

private:
    QVTKOpenGLNativeWidget* m_vtkWidget;
    pcl::visualization::PCLVisualizer::Ptr m_viewer;
    QPushButton* m_loadBtn;
    QGridLayout* m_layout;
};

#endif