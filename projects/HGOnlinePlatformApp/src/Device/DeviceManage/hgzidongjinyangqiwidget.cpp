#include "hgzidongjinyangqiwidget.h"
#include "common.h"
#include <sstream>
#include <QMessageBox>
#include "HGOnlinePlatformInterface.h"


HGZidongjinyangqiWidget::HGZidongjinyangqiWidget(std::string lang,std::string name,QWidget *parent) : QWidget(parent),
m_lang(lang)
{
    m_layout=new QGridLayout();
    this->setLayout(m_layout);
    m_name=name;
    m_nameLabel=new QLabel(QString::fromStdString(name));
    QFont font=m_nameLabel->font();
    font.setPointSize(18);
    m_nameLabel->setFont(font);
      
// 当前温度：32.9 .C
// 加热温度：160 。C
// 气流速度：0 mL/m
// 风扇状态：关
// 搅拌档位： 13
    int airSpeed = 0;
    int stirLevel = 13;
    QString strstatus = QString(
        "气流速度：%1 mL/m\n"
        "搅拌档位：%2")
        .arg(airSpeed)
        .arg(stirLevel);
    m_dataInfoLabel=new QLabel(strstatus);

    m_pumpInfoGroup=new QGroupBox("气泵");
    m_swirlInfoGroup=new QGroupBox("搅拌");
    m_pumpInfoLayout=new QGridLayout();
    m_swirlInfoLayout=new QGridLayout();
    m_pumpInfoGroup->setLayout(m_pumpInfoLayout);
    m_swirlInfoGroup->setLayout(m_swirlInfoLayout);
       
    m_punctureInfoGroup=new QGroupBox("取样针");
    m_punctureInfoLayout=new QGridLayout();
    m_punctureInfoGroup->setLayout(m_punctureInfoLayout);
    m_heightGroup=new QGroupBox("距离瓶底高度选择：");
    m_heightLayout=new QGridLayout();
    m_heightGroup->setLayout(m_heightLayout);
    m_tallHeightRadio=new QRadioButton("高 19.07mm");
    m_middleHeightRadio=new QRadioButton("中 4.97mm");
    m_shortHeightRadio=new QRadioButton("低 3.81mm");
    
    connect(m_tallHeightRadio, &QRadioButton::toggled, this, [=](bool checked) {
        if (checked) {
            setPuncturePosIndex(0);
            // qDebug() << "RadioButton 被选中";
        } 
    });
    connect(m_middleHeightRadio, &QRadioButton::toggled, this, [=](bool checked) {
        if (checked) {
            setPuncturePosIndex(1);
            // qDebug() << "RadioButton 被选中";
        } 
    });
    connect(m_shortHeightRadio, &QRadioButton::toggled, this, [=](bool checked) {
        if (checked) {
            setPuncturePosIndex(2);
            // qDebug() << "RadioButton 被选中";
        } 
    });

    m_adjustModeGroup=new QGroupBox("调整方式选择：");
    m_adjustModeLayout=new QGridLayout();
    m_adjustModeGroup->setLayout(m_adjustModeLayout);   

    m_longPressRadio=new QRadioButton("长按");
    m_arriveLimitRadio=new QRadioButton("到限位");
    m_stepArriveRadio=new QRadioButton("步进");
    m_stepCombo=new QComboBox();
    m_stepCombo->addItems({"0.4mm","0.8mm","1.2mm","1.6mm","2.0mm","4.0mm"});

    m_circleGroup=new QGroupBox("转盘");
    m_circleLayout=new QGridLayout();
    m_circleGroup->setLayout(m_circleLayout);
    m_circleNumberSpin=new QSpinBox();
    m_circleNumberSpin->setRange(0,36);

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QObject::connect(m_circleNumberSpin, &QSpinBox::valueChanged,
                     [](int value)
    {
        setCircleNumber(value);
    });
#else
    QObject::connect(m_circleNumberSpin, QOverload<int>::of(&QSpinBox::valueChanged),
                     [](int value)
    {
        setCircleNumber(value);
    });
#endif

    m_chuisaoBtn=new QPushButton("吹扫位");
    m_circleExecuteBtn=new QPushButton("执行");
    m_releaseBtn=new QPushButton("抬起");
    m_punctureBtn=new QPushButton("穿刺");
    m_punctureBtn->installEventFilter(this);
    m_arrivalBtn=new QPushButton("到位置");
    m_onBtn=new QPushButton("开启");
    m_offBtn=new QPushButton("关闭");
    m_ascendBtn=new QPushButton("+");
    m_descendBtn=new QPushButton("-");
    connect(m_onBtn,SIGNAL(clicked()),this,SLOT(slotOnClick()));
    connect(m_offBtn,SIGNAL(clicked()),this,SLOT(slotOffClick()));
    connect(m_ascendBtn,SIGNAL(clicked()),this,SLOT(slotAscendClick()));
    connect(m_descendBtn,SIGNAL(clicked()),this,SLOT(slotDescendClick()));
    connect(m_arrivalBtn,SIGNAL(clicked()),this,SLOT(slotArrivalClick()));
    connect(m_releaseBtn,SIGNAL(clicked()),this,SLOT(slotReleaseClick()));
    connect(m_punctureBtn,SIGNAL(clicked()),this,SLOT(slotPunctureClick()));
    connect(m_chuisaoBtn,SIGNAL(clicked()),this,SLOT(slotChuisaoClick()));
    connect(m_circleExecuteBtn,SIGNAL(clicked()),this,SLOT(slotCircleExecuteClick()));

    m_circleLayout->addWidget(m_circleNumberSpin,0,0);
    m_circleLayout->addWidget(m_chuisaoBtn,1,0);
    m_circleLayout->addWidget(m_circleExecuteBtn,1,1);

    m_heightLayout->addWidget(m_tallHeightRadio,0,0);
    m_heightLayout->addWidget(m_middleHeightRadio,1,0);
    m_heightLayout->addWidget(m_shortHeightRadio,2,0);

    m_adjustModeLayout->addWidget(m_longPressRadio,0,0);
    m_adjustModeLayout->addWidget(m_arriveLimitRadio,1,0);
    m_adjustModeLayout->addWidget(m_stepArriveRadio,2,0);
    m_adjustModeLayout->addWidget(m_stepCombo,2,1);
    m_adjustModeLayout->addWidget(m_releaseBtn,3,0);
    m_adjustModeLayout->addWidget(m_punctureBtn,3,1);

    m_curNeedlePosLabel=new QLabel("距离瓶底：");
    m_updateBtn=new QPushButton("更新");
    m_stopBtn=new QPushButton("停止");
    m_savePosBtn=new QPushButton("保存位置");
    connect(m_updateBtn,SIGNAL(clicked()),this,SLOT(slotUpdateClick()));
    connect(m_stopBtn,SIGNAL(clicked()),this,SLOT(slotStopClick()));
    connect(m_savePosBtn,SIGNAL(clicked()),this,SLOT(slotSavePosClick()));

    m_punctureInfoLayout->addWidget(m_heightGroup,0,0);
    m_punctureInfoLayout->addWidget(m_arrivalBtn,1,0);
    m_punctureInfoLayout->addWidget(m_adjustModeGroup,2,0);
    m_punctureInfoLayout->addWidget(m_curNeedlePosLabel,3,0);
    m_punctureInfoLayout->addWidget(m_updateBtn,3,1);
    m_punctureInfoLayout->addWidget(m_stopBtn,4,0);
    m_punctureInfoLayout->addWidget(m_savePosBtn,4,1);

    m_pumpInfoLayout->addWidget(m_onBtn,0,0);
    m_pumpInfoLayout->addWidget(m_offBtn,0,1);

    m_swirlInfoLayout->addWidget(m_ascendBtn,0,0);
    m_swirlInfoLayout->addWidget(m_descendBtn,0,1);

    m_layout->addWidget(m_nameLabel,0,0);
    m_layout->addWidget(m_punctureInfoGroup,1,0);
    m_layout->addWidget(m_circleGroup,2,0);
    m_layout->addWidget(m_pumpInfoGroup,3,0);
    m_layout->addWidget(m_swirlInfoGroup,4,0);
    m_layout->addWidget(m_dataInfoLabel,5,0);    

    m_timer=new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(slotUpdateDataInfo()));
    m_timer->start(100);
}
bool HGZidongjinyangqiWidget::closeWindow(){
    return true;
}

std::string HGZidongjinyangqiWidget::getParamStr(){
    std::ostringstream ss;
    // ss<<m_maxSpeedLabel->text().toStdString()<<":" \
    //   <<m_maxSpeedEdit->text().toStdString()<<"," \
    //   <<m_distanceLabel->text().toStdString()<<":" \
    //   <<m_distanceEdit->text().toStdString()<<"," \
    //   <<m_portLabel->text().toStdString()<<":" \
    //   <<m_portEdit->text().toStdString()<<"," \
    //   <<m_dataLabel->text().toStdString()<<":" \
    //   <<m_dataEdit->text().toStdString()<<"," \
    //   <<m_adjustLabel->text().toStdString()<<":" \
    //   <<m_adjustEdit->text().toStdString();
    return ss.str();
}

void HGZidongjinyangqiWidget::setParamStr(std::string param){
    // setControlText(m_pumpInfoLayout,param);
    // setControlText(m_swirlInfoLayout,param);
}
void HGZidongjinyangqiWidget::slotOnClick(){
    setPumpOn();
}
void HGZidongjinyangqiWidget::slotOffClick(){
    setPumpOff();
}
void HGZidongjinyangqiWidget::slotAscendClick(){
    setSwirlAscend();
}
void HGZidongjinyangqiWidget::slotDescendClick(){
    setSwirlDescend();
}

void HGZidongjinyangqiWidget::slotArrivalClick(){
    setNeedleArrivalTargetPos();
}
void HGZidongjinyangqiWidget::slotReleaseClick(){
    setNeedleRelease();
}
void HGZidongjinyangqiWidget::slotUpdateClick(){
    double pos = getNeedlePunctureCurPos();
    QString strstatus = QString("距离瓶底：%1 mm\n")
        .arg(pos, 0, 'f', 1) ; // 保留1位小数;
    m_curNeedlePosLabel->setText(strstatus);
}
void HGZidongjinyangqiWidget::slotStopClick(){
    setNeedlePunctureRelease();
}
void HGZidongjinyangqiWidget::slotSavePosClick(){
    if (QMessageBox::Ok==QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),
         "请先检查当前穿刺位置是否符合，再确认保存！", 
         QMessageBox::Ok | QMessageBox::Cancel))
    {

        // saveNeedleCurPos();
    }
}
bool HGZidongjinyangqiWidget::eventFilter(QObject *obj, QEvent *event) {
    if (m_longPressRadio->isChecked())
    {
        if (obj == m_punctureBtn)
        {
            if (event->type() == QEvent::MouseButtonPress)
            {
                setNeedleArrivalLimitPos();
                // qDebug() << "按钮按下";
                // 启动一个定时器判断是否长按
            }
            else if (event->type() == QEvent::MouseButtonRelease)
            {
                setNeedlePunctureRelease();
                // qDebug() << "按钮放开";
                // 判断是否达成长按时间
            }
        }
    }
    return QObject::eventFilter(obj, event);
}
void HGZidongjinyangqiWidget::slotPunctureClick(){
    if (m_arriveLimitRadio->isChecked())
        setNeedleArrivalLimitPos();
    else {
        if (m_stepArriveRadio->isChecked())
        {
            QRegExp rx("(\\d+\\.?\\d*)"); // 匹配整数或小数

            if (rx.indexIn(m_stepCombo->currentText()) != -1)
            {
                QString numStr = rx.cap(1);
                double value = numStr.toDouble();
                setNeedlePunctureStep(value);
            } 
        }
    }
}
void HGZidongjinyangqiWidget::slotChuisaoClick(){
    setCircleNumber(0);
    setCircleExecute();
}
void HGZidongjinyangqiWidget::slotCircleExecuteClick(){
    setCircleExecute();
}
void HGZidongjinyangqiWidget::slotUpdateDataInfo(){
    setRequestData();
    std::map<std::string,std::string> values = getCAS6RawValues();
    if (values.size() < 4) return; 
    double airSpeed = values.find("气流速度")!=values.end()?std::atof(values["气流速度"].c_str()):0.0;
    int stirLevel = 13;
    QString strstatus = QString(
        "气流速度：%1 mL/m\n"
        "搅拌档位：%2")
        .arg(airSpeed)
        .arg(stirLevel);
    m_dataInfoLabel->setText(strstatus);
}