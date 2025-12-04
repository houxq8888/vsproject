#include "hgrunningchartwidget.h"
#include <QGraphicsRectItem>
#include <QButtonGroup>
#include "common.h"

std::vector<std::string> chartContent={"Spectral","FlowProgress","InTimeResult"};
std::vector<curColorStruct> curveType={
    curColorStruct("U-t",QColor(255,0,0)),
    curColorStruct("drift-t",QColor(0,255,0)),
    curColorStruct("water",QColor(0,0,255)),
    curColorStruct("dV-t",QColor(255,255,0))
};


HGRunningChartWidget::HGRunningChartWidget(std::string lang,QWidget *parent) : QWidget(parent),
    m_lang(lang),
    m_chartView(new MyChartView()),
    m_chart(new QChart()),
    m_displayTypeLabel(new HGQLabel(false,getPath("/resources/V1/@1xze-bars 1.png"))),
    inTimeDisplayLabel(new HGQLabel(false,getPath("/resources/V1/@1xze-apps-o 1.png"))),
    m_dragDialog(nullptr),
    m_displayDialog(nullptr)
{
    RWDb::writeAuditTrailLog(loadTranslation(m_lang,"Enter")+loadTranslation(m_lang,"RunningStatus"));
    m_layout=new QGridLayout();
    m_layout->setVerticalSpacing(0);

    m_flowGroup=new QGroupBox();
    m_chartGroup=new QGroupBox();
    m_flowLayout=new QGridLayout();
    m_chartLayout=new QGridLayout();
    m_flowGroup->setLayout(m_flowLayout);
    m_chartGroup->setLayout(m_chartLayout);
    m_horSplitter=new QSplitter(Qt::Vertical);
    m_horSplitter->setStyleSheet("QSplitter::handle{background-color:lightgray;}");

    m_chart->legend()->setBorderColor(QColor(0, 0, 85, 70));
    m_chart->legend()->setAlignment(Qt::AlignBottom);
    m_chart->legend()->setShowToolTips(true);
    m_chart->setMargins(QMargins(10, 10, 10, 50));
    m_chart->setBackgroundRoundness(Qt::NoBrush);
    m_chartView->setChart(m_chart);
    m_chartView->setRubberBand(QChartView::NoRubberBand);

    connect(m_displayTypeLabel, &HGQLabel::leftClicked, this, &HGRunningChartWidget::slotDisplayType);
    connect(inTimeDisplayLabel, &HGQLabel::leftClicked, this, &HGRunningChartWidget::slotInTimeDisplayType);

    for (const auto& name : chartContent) {
        m_displayLabels[name] = recordLabelWithSeries1();
        m_displayLabels[name].seriesLine = new QLineSeries();
        m_displayLabels[name].seriesLine->setName(QString::fromStdString(name));
        m_displayLabels[name].seriesLine->setPointsVisible(true);
        m_displayLabels[name].seriesBar = new QBarSeries();
        m_displayLabels[name].seriesBar->setName(QString::fromStdString(name));
    }
    for (const auto& name : curveType) {
        m_recordLabels[name.curType] = curLabel();
    }

    m_chartLayout->addWidget(m_displayTypeLabel,0,7,1,1);
    m_chartLayout->addWidget(inTimeDisplayLabel,0,8,1,1);
    m_chartLayout->addWidget(m_chartView,1,0,1,9);

    m_horSplitter->addWidget(m_flowGroup);
    m_horSplitter->addWidget(m_chartGroup);

    m_layout->addWidget(m_horSplitter);
    this->setLayout(m_layout);
}

bool HGRunningChartWidget::closeWindow()
{
    return true;
}
HGRunningChartWidget::~HGRunningChartWidget()
{

}
void HGRunningChartWidget::slotDisplayType(){
    if (m_dragDialog!=NULL) {
        SAFE_DELETE(m_dragDialog);
        return;
    }
    m_dragDialog=new DraggableDialog(this);

    QVBoxLayout* layout=new QVBoxLayout(m_dragDialog);

    HGQLabel* nameLabel=new HGQLabel(false,getPath("/resources/V1/@1xze-bars 1.png"));

    for (auto name:chartContent){
        if (m_displayLabels[name].label.flag)
            m_displayLabels[name].label.label=new LabelWithImg(IMGLEFT,12,getPath("/resources/V1/@1xze-certificate 1.png"),loadTranslation(m_lang,name));
        else m_displayLabels[name].label.label=new LabelWithImg(IMGLEFT,12,getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"),loadTranslation(m_lang,name));
    }

    layout->addWidget(nameLabel);
    for (auto it:m_displayLabels){
        connect(m_displayLabels[it.first].label.label,SIGNAL(clickLeftName(std::string)),this,SLOT(slotLabelClicked(std::string)));
        layout->addWidget(it.second.label.label);
    }

    m_dragDialog->setLayout(layout);
    m_dragDialog->show();
}
void HGRunningChartWidget::slotInTimeDisplayType(){
    if (m_displayDialog!=NULL) {
        SAFE_DELETE(m_displayDialog);
        return;
    }
    m_displayDialog=new DraggableDialog(this);

    QVBoxLayout* layout=new QVBoxLayout(m_displayDialog);

    HGQLabel* nameLabel=new HGQLabel(false,getPath("/resources/V1/@1xze-apps-o 1.png"));
 
    for (auto name:curveType){
        m_recordLabels[name.curType].label=new HGCurveWidget(m_recordLabels[name.curType].flag,
            m_lang,name.curType,name.color);
    }

    layout->addWidget(nameLabel);
    for (auto it:m_recordLabels){
        connect(m_recordLabels[it.first].label,SIGNAL(clickLeftName(std::string)),this,SLOT(slotLabelClicked(std::string)));
        layout->addWidget(it.second.label);
    }

    QLabel* intervalLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"CaptureInterval")));
    QComboBox* intervalCombo=new QComboBox();
    intervalCombo->addItem("100 s");
    layout->addWidget(intervalLabel);
    layout->addWidget(intervalCombo);
    
    m_displayDialog->setLayout(layout);
    m_displayDialog->show();
}
void HGRunningChartWidget::slotLabelClicked(std::string name){
    std::string keyname=findTranslationKey(m_lang,name);
    if (m_recordLabels.find(keyname) != m_recordLabels.end())
    {
        m_recordLabels[keyname].flag = !m_recordLabels[keyname].flag;
        if (!m_recordLabels[keyname].flag)
            m_recordLabels[keyname].label->setImg(getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"));
        else
            m_recordLabels[keyname].label->setImg(getPath("/resources/V1/@1xze-certificate 1.png"));
    }
    else if (m_displayLabels.find(keyname) != m_displayLabels.end())
    {
        m_displayLabels[keyname].label.flag = !m_displayLabels[keyname].label.flag;
        if (!m_displayLabels[keyname].label.flag){
            m_displayLabels[keyname].label.label->setImg(getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"));
            // fnUpdateDisplay(false,keyname);
        }
        else{
            m_displayLabels[keyname].label.label->setImg(getPath("/resources/V1/@1xze-certificate 1.png"));
            // fnUpdateDisplay(true,keyname);
            // slotUpdateChart();
        }
    }
}