#include "hgmethodlistwidget.h"
#include "common.h"

HGMethodListWidget::HGMethodListWidget(std::string lang,QWidget *parent) : QWidget(parent),
m_lang(lang)
{
    m_dianweiLabel=new LabelWithImg(IMGRIGHTBOTTOM,12,getPath("/resources/V1/@1xze-filter-o 1.png"),"电位滴定");
    m_colorDDLabel=new LabelWithImg(IMGRIGHTBOTTOM,12,getPath("/resources/V1/@1xze-eye-o 1.png"),"颜色滴定");
    m_photometerLabel=new LabelWithImg(IMGRIGHTBOTTOM,12,getPath("/resources/V1/@1xiconPark-sun 1.png"),"光度法");
    m_rongliangLabel=new LabelWithImg(IMGRIGHTBOTTOM,12,getPath("/resources/V1/@1xif-beaker 1.png"),"容量法(水分)");
    m_kulunLabel=new LabelWithImg(IMGRIGHTBOTTOM,12,getPath("/resources/V1/@1xarcoDesign-translate 1.png"),"库伦法(水分)");
    m_tempDDLabel=new LabelWithImg(IMGRIGHTBOTTOM,12,getPath("/resources/V1/@1xhaFill-70-thermometer-three-quarters 1.png"),"温度滴定");

    connect(m_dianweiLabel,SIGNAL(clickLeftButton()),this,SLOT(slotClickDianWei()));
    connect(m_colorDDLabel,SIGNAL(clickLeftButton()),this,SLOT(slotClickColorDD()));
    connect(m_photometerLabel,SIGNAL(clickLeftButton()),this,SLOT(slotClickPhotometer()));
    connect(m_rongliangLabel,SIGNAL(clickLeftButton()),this,SLOT(slotClickRongLiang()));
    connect(m_kulunLabel,SIGNAL(clickLeftButton()),this,SLOT(slotClickKulun()));
    connect(m_tempDDLabel,SIGNAL(clickLeftButton()),this,SLOT(slotClickTempDD()));

    connect(m_dianweiLabel,SIGNAL(createModule(std::string)),this,SLOT(slotCreateModule(std::string)));
    connect(m_colorDDLabel,SIGNAL(createModule(std::string)),this,SLOT(slotCreateModule(std::string)));
    connect(m_photometerLabel,SIGNAL(createModule(std::string)),this,SLOT(slotCreateModule(std::string)));
    connect(m_rongliangLabel,SIGNAL(createModule(std::string)),this,SLOT(slotCreateModule(std::string)));
    connect(m_kulunLabel,SIGNAL(createModule(std::string)),this,SLOT(slotCreateModule(std::string)));
    connect(m_tempDDLabel,SIGNAL(createModule(std::string)),this,SLOT(slotCreateModule(std::string)));
   
    m_dianweiLabel->setRectPainter(true);
    m_colorDDLabel->setRectPainter(true);
    m_photometerLabel->setRectPainter(true);
    m_rongliangLabel->setRectPainter(true);
    m_kulunLabel->setRectPainter(true);
    m_tempDDLabel->setRectPainter(true);

    m_contentWidget= new QWidget();
    // m_contentWidget->setFixedWidth(this->width()/4);
    m_layout=new QVBoxLayout();
    m_contentWidget->setLayout(m_layout);
    

    m_layout->addWidget(m_dianweiLabel);
    m_layout->addWidget(m_colorDDLabel);
    m_layout->addWidget(m_photometerLabel);
    m_layout->addWidget(m_rongliangLabel);
    m_layout->addWidget(m_kulunLabel);
    m_layout->addWidget(m_tempDDLabel);
    
    m_scrollArea = new QScrollArea();
    m_scrollArea->setWidgetResizable(true); // 自动调整内容大小
    m_scrollArea->setWidget(m_contentWidget); // 设置滚动区域的内容

    m_mainLayout=new QVBoxLayout();
    m_mainLayout->addWidget(m_scrollArea);
    this->setLayout(m_mainLayout);
}
void HGMethodListWidget::slotCreateModule(std::string name){
    emit createModule(name);
}
bool HGMethodListWidget::closeWindow(){
    return true;
}
std::vector<std::string> HGMethodListWidget::getMethods()
{
    std::vector<std::string> methods;
    for (int r = 0; r < m_layout->count(); ++r)
    {
        QLayoutItem *item = m_layout->itemAt(r);
        if (item && item->widget())
        {
            if (LabelWithImg *label = qobject_cast<LabelWithImg *>(item->widget()))
            {
                methods.push_back(label->getTextLabel()->text().toStdString());
            }
        }
    }
    return methods;
}
void HGMethodListWidget::lightMethod(std::string type){
    setAllFalse();
    for (int r = 0; r < m_layout->count(); ++r)
    {
        QLayoutItem *item = m_layout->itemAt(r);
        if (item && item->widget())
        {
            if (LabelWithImg *label = qobject_cast<LabelWithImg *>(item->widget()))
            {
                if (label->getTextLabel()->text().toStdString()==type)
                    label->setFillFlag(true);
            }
        }
    }
}
void HGMethodListWidget::setAllFalse(){
    m_dianweiLabel->setFillFlag(false);
    m_colorDDLabel->setFillFlag(false);
    m_photometerLabel->setFillFlag(false);
    m_rongliangLabel->setFillFlag(false);
    m_kulunLabel->setFillFlag(false);
    m_tempDDLabel->setFillFlag(false);
}
void HGMethodListWidget::slotClickDianWei(){

    setAllFalse();
    m_dianweiLabel->setFillFlag(true);
}
void HGMethodListWidget::slotClickColorDD(){

    setAllFalse();
    m_colorDDLabel->setFillFlag(true);
}
void HGMethodListWidget::slotClickPhotometer(){

    setAllFalse();
    m_photometerLabel->setFillFlag(true);
}
void HGMethodListWidget::slotClickRongLiang(){

    setAllFalse();
    m_rongliangLabel->setFillFlag(true);
}
void HGMethodListWidget::slotClickKulun(){

    setAllFalse();
    m_kulunLabel->setFillFlag(true);
}
void HGMethodListWidget::slotClickTempDD(){

    setAllFalse();
    m_tempDDLabel->setFillFlag(true);
}