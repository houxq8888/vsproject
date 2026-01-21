#include "displayandlightwidget.h"
#include "common.h"
#include <QToolTip>


DisplayAndLightWidget::DisplayAndLightWidget(std::string lang,QWidget *parent) : BaseWidget(parent),
    m_autoRectifyLightFlag(true),
    m_deviceName(""),
    m_lang(lang)
{
    m_resolutionLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"Resolution")));//"显示器分辨率");
    m_autoRectifyLightLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"AutoLight")));//"自动调整亮度");
    m_lightLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"Brightness")));//"亮度");

    m_resolutionComboBox=new QComboBox();//(推荐)
    std::vector<std::string> resolutionList=listResolutions(m_deviceName);
    m_resolutionComboBox->addItem("1920x1080");
    m_resolutionComboBox->addItem("2560x1600");
    m_resolutionComboBox->addItem("1920x1440");
    m_resolutionComboBox->addItem("1856x1392");
    m_resolutionComboBox->addItem("1792x1344");
    m_resolutionComboBox->addItem("1920x1200");
    m_resolutionComboBox->addItem("1600x1200");
    m_resolutionComboBox->addItem("1680x1050");
    m_resolutionComboBox->addItem("1400x1050");
    m_resolutionComboBox->addItem("1280x1024");
    m_resolutionComboBox->addItem("1400x900");
    m_resolutionComboBox->addItem("1280x960");
    m_resolutionComboBox->addItem("1360x768");
    m_resolutionComboBox->addItem("1280x768");
    m_resolutionComboBox->addItem("1024x768");
    m_resolutionComboBox->addItem("800x600");
    if (resolutionList.size()>0) m_resolutionComboBox->setCurrentText(QString::fromStdString(resolutionList[0]));
    connect(m_resolutionComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(slotResolutionChanged(int)));

    m_autoRectifyLightImg=new LabelWithImg(IMGRIGHT,12,getPath("/resources/V1/@1xIOS开关_enable.png"),
        loadTranslation(m_lang,"On"));//"开");
    connect(m_autoRectifyLightImg,SIGNAL(clickImgLabel()),this,SLOT(clickEnableAutoRectifyLight()));
    m_lightSlider=new QSlider(Qt::Horizontal);
    m_lightSlider->setRange(0,100);
    connect(m_lightSlider,SIGNAL(valueChanged(int)),this,SLOT(slotLightChanged(int)));

    // 连接 QSlider 的 valueChanged 信号，在滑动时显示 QToolTip
    connect(m_lightSlider, &QSlider::sliderMoved, this, [=](int value) {
            // 获取滑块的位置
        QPoint pos = m_lightSlider->mapToGlobal(QPoint(m_lightSlider->width() * value / m_lightSlider->maximum(), 0));
        QToolTip::showText(pos, QString::number(value));
    });

    m_layout=new QGridLayout();
    this->setLayout(m_layout);
    m_layout->addWidget(m_resolutionLabel,0,0);
    m_layout->addWidget(m_resolutionComboBox,0,1);
    m_layout->addWidget(m_autoRectifyLightLabel,1,0);
    m_layout->addWidget(m_autoRectifyLightImg,1,1);
    m_layout->addWidget(m_lightLabel,2,0);
    m_layout->addWidget(m_lightSlider,2,1);
    fnReadDB();
}

DisplayAndLightWidget::~DisplayAndLightWidget(){
    closeWindow();
}
bool DisplayAndLightWidget::closeWindow()
{
    fnWriteDB();
    return true;
}
void DisplayAndLightWidget::fnReadDB()
{
    if (GlobalSingleton::instance().getSystemInfo("显示器分辨率")!=""){
        m_resolutionComboBox->setCurrentText(QString::fromStdString(GlobalSingleton::instance().getSystemInfo("显示器分辨率")));
    } else m_resolutionComboBox->setCurrentIndex(-1);
    m_autoRectifyLightFlag=(GlobalSingleton::instance().getSystemInfo("自动调整亮度")=="true"?true:false);

    if (GlobalSingleton::instance().getSystemInfo("亮度")!="") 
        m_lightSlider->setValue(std::stoi(GlobalSingleton::instance().getSystemInfo("亮度")));
    else m_lightSlider->setValue(0);
    setControlStatus();
}
void DisplayAndLightWidget::fnWriteDB()
{
    GlobalSingleton::instance().setSystemInfo("显示器分辨率",m_resolutionComboBox->currentText().toStdString());
    GlobalSingleton::instance().setSystemInfo("自动调整亮度",m_autoRectifyLightFlag?"true":"false");
    GlobalSingleton::instance().setSystemInfo("亮度",std::to_string(m_lightSlider->value()));
}
void DisplayAndLightWidget::setControlStatus()
{
    if (!m_autoRectifyLightFlag){
        m_autoRectifyLightImg->setImg(getPath("/resources/V1/@1xIOS开关.png"));
        m_autoRectifyLightImg->getTextLabel()->setText(QString::fromStdString(loadTranslation(m_lang,"Off")));//"关");
    } else {
        m_autoRectifyLightImg->setImg(getPath("/resources/V1/@1xIOS开关_enable.png"));
        m_autoRectifyLightImg->getTextLabel()->setText(QString::fromStdString(loadTranslation(m_lang,"On")));//"开");
    }
    m_lightSlider->setEnabled(!m_autoRectifyLightFlag);
}
void DisplayAndLightWidget::slotLightChanged(int value)
{
    if (m_autoRectifyLightFlag) return;
    int light=value;
     // 设置亮度为 50%
    std::string command = "xbacklight -set "+std::to_string(value);
    
    // 执行命令
    int result = system(command.c_str());

    if (result == 0) {
        printf("Brightness set successfully!\n");
        RWDb::writeAuditTrailLog("setLight:["+std::to_string(value)+"] successfully");
    } else {
        printf("Failed to set brightness.\n");
        RWDb::writeAuditTrailLog("setLight:["+std::to_string(value)+"] failed");
    }
}
void DisplayAndLightWidget::slotResolutionChanged(int index)
{
    if (index==-1) {
        return;
    }
    std::string resolution=m_resolutionComboBox->currentText().toStdString();
    // 设置分辨率为 1920x1080
    std::string command = "xrandr --output "+m_deviceName+" --mode "+resolution;
    
    // 执行命令
    int result = system(command.c_str());

    if (result == 0) {
        printf("Resolution changed successfully!\n");
        RWDb::writeAuditTrailLog("setResolution:["+resolution+"] successfully");
    } else {
        printf("Failed to change resolution.\n");
        RWDb::writeAuditTrailLog("setResolution:["+resolution+"] failed");
    }
}

void DisplayAndLightWidget::clickEnableAutoRectifyLight()
{
    m_autoRectifyLightFlag=!m_autoRectifyLightFlag;
    RWDb::writeAuditTrailLog(m_autoRectifyLightFlag?("打开"+m_autoRectifyLightLabel->text().toStdString()): \
        "关闭"+m_autoRectifyLightLabel->text().toStdString());
    setControlStatus();
}
void DisplayAndLightWidget::setLanguage(std::string lang){
    m_lang=lang;
    m_resolutionLabel->setText(QString::fromStdString(loadTranslation(m_lang,"Resolution")));//"显示器分辨率");
    m_autoRectifyLightLabel->setText(QString::fromStdString(loadTranslation(m_lang,"AutoLight")));//"自动调整亮度");
    m_lightLabel->setText(QString::fromStdString(loadTranslation(m_lang,"Brightness")));//"亮度");

}