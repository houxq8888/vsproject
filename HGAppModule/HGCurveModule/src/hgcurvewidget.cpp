#include "hgcurvewidget.h"
#include "common.h"
#include "hgxml.h"

using namespace HGMACHINE;

std::map<std::string,std::vector<std::string>> units={
    {"U-t",     {"mv","v"}},
    {"drift-t", {"ug/min"}},
    {"water",   {"ug"}},
    {"dV-t",    {"mv"}}
};
HGCurveWidget::HGCurveWidget(bool flag,const std::string &lang,const std::string &name,
    const QColor& color,QWidget *parent) : QWidget(parent),
    m_lang(lang)
{
    m_layout=new QHBoxLayout(this);
    m_layout->setMargin(0);

    m_label=new LabelWithImg(IMGLEFT,12,getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"),name);
    connect(m_label,SIGNAL(clickLeftName(std::string)),this,SLOT(slotLabelClicked(std::string)));

    m_label->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    m_unitCombo=new QComboBox();
    m_unitCombo->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    for (int i=0;i<int(units[name].size());i++){
        std::string unit=units[name].at(i);
        m_unitCombo->addItem(QString::fromStdString(unit));
    }
    m_colorLabel=new QLabel();
    m_colorLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_colorLabel->setStyleSheet("background-color: "+color.name()+";");
    m_colorLabel->installEventFilter(this);

    m_layout->addWidget(m_label,2);
    m_layout->addWidget(m_unitCombo,1);
    m_layout->addWidget(m_colorLabel,1);
}
bool HGCurveWidget::closeWindow(){
    return true;
}
void HGCurveWidget::setImg(std::string imgpath){
    m_label->setImg(imgpath);
}
void HGCurveWidget::slotLabelClicked(std::string name){
    emit clickLeftName(name);
}
bool HGCurveWidget::eventFilter(QObject *obj, QEvent *event) {
    if (obj == m_colorLabel && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent->button() == Qt::LeftButton)
        {
            QColor color = QColorDialog::getColor(Qt::white, this, 
                QString::fromStdString(loadTranslation(m_lang,"ChooseColor")));
            if (color.isValid())
            {
                m_colorLabel->setStyleSheet("background-color: " + color.name() + "; color: white;");
            }
            return true;  // 事件已处理
        }
    }
    return QWidget::eventFilter(obj, event);
}
