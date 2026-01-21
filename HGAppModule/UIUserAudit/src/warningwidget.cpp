#include "warningwidget.h"
#include "common.h"
#include <QDebug>
#include <QToolTip>


WarningWidget::WarningWidget(std::string lang,QWidget *parent) : BaseWidget(parent),
    m_isEnableSoundWarn(true),
    m_isEnableLightWarn(true),
    m_setSoundValueFromMain(false),
    m_lang(lang)
{
    m_soundWarnLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"SoundWarn")));//)"声音报警");
    m_lightWarnLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"LightWarn")));//"灯光报警");
    m_soundSlider=new QSlider(Qt::Horizontal);
    m_soundSlider->setRange(0,100);
    connect(m_soundSlider,SIGNAL(valueChanged(int)),this,SLOT(slotSetSoundWarnImg(int)));
    // 连接 QSlider 的 valueChanged 信号，在滑动时显示 QToolTip
    connect(m_soundSlider, &QSlider::sliderMoved, this, [=](int value) {
            // 获取滑块的位置
        QPoint pos = m_soundSlider->mapToGlobal(QPoint(m_soundSlider->width() * value / m_soundSlider->maximum(), 0));
        QToolTip::showText(pos, QString::number(value));
    });

    m_soundWarnImg=new LabelWithImg(IMGRIGHT,12,getPath("/resources/V1/@1xIOS开关_enable.png"),
        loadTranslation(m_lang,"On"));
    m_lightWarnImg=new LabelWithImg(IMGRIGHT,12,getPath("/resources/V1/@1xIOS开关_enable.png"),
        loadTranslation(m_lang,"On"));
    connect(m_soundWarnImg,SIGNAL(clickImgLabel()),this,SLOT(clickEnableSoundWarn()));
    connect(m_lightWarnImg,SIGNAL(clickImgLabel()),this,SLOT(clickEnableLightWarn()));

    m_soundWarnText=new HGQLabel(false,getPath("/resources/V1/@sound-open.png"));

    m_layout=new QGridLayout();
    this->setLayout(m_layout);
    m_layout->addWidget(m_soundWarnLabel,0,0);
    m_layout->addWidget(m_soundWarnImg,0,1);
    m_layout->addWidget(m_soundWarnText,1,0);
    m_layout->addWidget(m_soundSlider,1,1);
    m_layout->addWidget(m_lightWarnLabel,2,0);
    m_layout->addWidget(m_lightWarnImg,2,1);
    fnReadDB();
}

WarningWidget::~WarningWidget(){
    closeWindow();
}
BaseWidget& WarningWidget::operator=(const BaseWidget& obj){
    BaseWidget::operator=(obj);
    fnCopy((const WarningWidget&)obj);
    return (*this);
}
void WarningWidget::fnCopy(const WarningWidget& obj)
{
    m_isEnableSoundWarn=obj.m_isEnableSoundWarn;
    m_isEnableLightWarn=obj.m_isEnableLightWarn;
    m_soundWarnLabel=obj.m_soundWarnLabel;
    m_lightWarnLabel=obj.m_lightWarnLabel;
    m_soundSlider=obj.m_soundSlider;
    m_soundWarnImg=obj.m_soundWarnImg;
    m_lightWarnImg=obj.m_lightWarnImg;
    m_soundWarnText=obj.m_soundWarnText;
    m_layout=obj.m_layout;
    m_setSoundValueFromMain=obj.m_setSoundValueFromMain;
}
bool WarningWidget::closeWindow()
{
    fnWriteDB();
    return true;
}
void WarningWidget::fnReadDB()
{
    m_isEnableSoundWarn=(GlobalSingleton::instance().getSystemInfo("声音报警")=="true"?true:false);
    m_isEnableLightWarn=(GlobalSingleton::instance().getSystemInfo("灯光报警")=="true"?true:false);

    if (GlobalSingleton::instance().getSystemInfo("声音value")!="")  
        m_soundSlider->setValue(std::stoi(GlobalSingleton::instance().getSystemInfo("声音value")));
    else m_soundSlider->setValue(0);
    setControlStatus();
}
void WarningWidget::fnWriteDB()
{
    GlobalSingleton::instance().setSystemInfo("声音报警", m_isEnableSoundWarn?"true":"false");
    GlobalSingleton::instance().setSystemInfo("灯光报警", m_isEnableLightWarn?"true":"false");
    GlobalSingleton::instance().setSystemInfo("声音value", std::to_string(m_soundSlider->value()));
}
void WarningWidget::setControlStatus()
{
    if (!m_isEnableSoundWarn){
        m_soundWarnImg->setImg(getPath("/resources/V1/@1xIOS开关.png"));
        m_soundWarnImg->getTextLabel()->setText(QString::fromStdString(loadTranslation(m_lang,"Off")));
    } else {
        m_soundWarnImg->setImg(getPath("/resources/V1/@1xIOS开关_enable.png"));
        m_soundWarnImg->getTextLabel()->setText(QString::fromStdString(loadTranslation(m_lang,"On")));
    }

    if (!m_isEnableLightWarn){
        m_lightWarnImg->setImg(getPath("/resources/V1/@1xIOS开关.png"));
        m_lightWarnImg->getTextLabel()->setText(QString::fromStdString(loadTranslation(m_lang,"Off")));
    } else {
        m_lightWarnImg->setImg(getPath("/resources/V1/@1xIOS开关_enable.png"));
        m_lightWarnImg->getTextLabel()->setText(QString::fromStdString(loadTranslation(m_lang,"On")));
    }
    m_soundSlider->setEnabled(m_isEnableSoundWarn);
    if (!m_isEnableSoundWarn) return;
    setVolume(m_soundSlider->value());
    if (m_soundSlider->value()==0){
        m_soundWarnText->changePixmap(getPath("/resources/V1/@sound-close.png"),24);
    } else {
        m_soundWarnText->changePixmap(getPath("/resources/V1/@sound-open.png"),24);
    }
}

void WarningWidget::setWarnSoundVolumn(int volumn){
    m_setSoundValueFromMain=true;
    setVolume(volumn);
    if (volumn==0){
        m_soundWarnText->changePixmap(getPath("/resources/V1/@sound-close.png"),24);
    } else {
        m_soundWarnText->changePixmap(getPath("/resources/V1/@sound-open.png"),24);
    }
    m_soundSlider->setValue(volumn);
    m_setSoundValueFromMain=false;
}
void WarningWidget::slotSetSoundWarnImg(int value)
{
    if (m_setSoundValueFromMain) return;
    // long volume = 50; // 设置音量值（0 - 100%）
    setVolume(value);
    if (value==0){
        m_soundWarnText->changePixmap(getPath("/resources/V1/@sound-close.png"),24);
    } else {
        m_soundWarnText->changePixmap(getPath("/resources/V1/@sound-open.png"),24);
    }
    RWDb::writeAuditTrailLog("设置音量:["+std::to_string(value)+"]");
    emit signalSoundVolumn(value);
    playSound(getPath("/resources/Alarm03.mp3")); // 播放一段声音文件
}
void WarningWidget::clickEnableSoundWarn()
{
    m_isEnableSoundWarn=!m_isEnableSoundWarn;
    RWDb::writeAuditTrailLog(m_isEnableSoundWarn?("打开"+m_soundWarnLabel->text().toStdString()): \
        "关闭"+m_soundWarnLabel->text().toStdString());
    setControlStatus();
}

void WarningWidget::clickEnableLightWarn()
{
    m_isEnableLightWarn=!m_isEnableLightWarn;
    RWDb::writeAuditTrailLog(m_isEnableLightWarn?("打开"+m_lightWarnLabel->text().toStdString()): \
        "关闭"+m_lightWarnLabel->text().toStdString());
    setControlStatus();
}
void WarningWidget::setLanguage(std::string lang){
    m_lang=lang;
    m_soundWarnLabel->setText(QString::fromStdString(loadTranslation(m_lang,"SoundWarn")));//)"声音报警");
    m_lightWarnLabel->setText(QString::fromStdString(loadTranslation(m_lang,"LightWarn")));
    if (!m_isEnableLightWarn){
        m_lightWarnImg->getTextLabel()->setText(QString::fromStdString(loadTranslation(m_lang,"Off")));
    } else {
        m_lightWarnImg->getTextLabel()->setText(QString::fromStdString(loadTranslation(m_lang,"On")));
    }
    if (!m_isEnableSoundWarn){
        m_soundWarnImg->getTextLabel()->setText(QString::fromStdString(loadTranslation(m_lang,"Off")));
    } else {
        m_soundWarnImg->getTextLabel()->setText(QString::fromStdString(loadTranslation(m_lang,"On")));
    }
}