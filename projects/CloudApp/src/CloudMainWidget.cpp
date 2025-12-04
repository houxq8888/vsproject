#include "CloudMainWidget.h"
#include <QFileDialog>
#include <QMessageBox>


CloudMainWidget::CloudMainWidget(QWidget *parent)
    : QWidget(parent)
{
    fnSetupUI();
}
CloudMainWidget::~CloudMainWidget(){}

void CloudMainWidget::fnSetupUI()
{
    m_layout = new QGridLayout();
    this->setLayout(m_layout);
    m_loadBtn = new QPushButton("Load"); 
    connect(m_loadBtn, &QPushButton::clicked, this, &CloudMainWidget::slotLoadCloud);

    m_vtkWidget = new QVTKOpenGLNativeWidget();
    m_vtkWidget->setFocusPolicy(Qt::StrongFocus);
    m_vtkWidget->setMouseTracking(true);
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    m_vtkWidget->SetRenderWindow(renderWindow);

    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);

    // 用已有的 renderWindow 指针创建 PCLVisualizer，避免创建新窗口
    m_viewer = pcl::visualization::PCLVisualizer::Ptr(
        new pcl::visualization::PCLVisualizer(renderer, renderWindow,"Viewer",false)
    );

    m_viewer->setupInteractor(m_vtkWidget->GetInteractor(),m_vtkWidget->GetRenderWindow());
    m_viewer->setBackgroundColor(0.1,0.1,0.1);
    m_viewer->addCoordinateSystem(1.0);


    m_layout->addWidget(m_vtkWidget,0,0,1,2);
    m_layout->addWidget(m_loadBtn,1,0);
    setMinimumSize(800, 600);  // 添加默认窗口大小

    std::cout << "Widget render window: " << m_vtkWidget->GetRenderWindow() << std::endl;
    std::cout << "Viewer render window: " << m_viewer->getRenderWindow() << std::endl;

}
void CloudMainWidget::slotLoadCloud(){
    QString filename = QFileDialog::getOpenFileName(this, "Open PCD File", "", "*.pcd");
    if (filename.isEmpty()) return;

    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
    if (pcl::io::loadPCDFile<pcl::PointXYZ>(filename.toStdString(), *cloud) == -1) {
        QMessageBox::warning(this,"Error","Failed to load the PCD file.");
        return;
    }

    m_viewer->removeAllPointClouds();
    m_viewer->addPointCloud<pcl::PointXYZ>(cloud, "cloud");
    m_viewer->resetCamera();

    //force update
    m_viewer->getRenderWindow()->Render();
    m_vtkWidget->update();
}