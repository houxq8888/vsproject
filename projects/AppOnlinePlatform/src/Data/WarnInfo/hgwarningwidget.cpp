#include "hgwarningwidget.h"

HGWarningWidget::HGWarningWidget(std::string lang,QWidget *parent) : QWidget(parent),
m_lang(lang)
{
    m_label = new HGQLabel();
    m_label->setParent(this);
    m_label->setText("delete this widget");
    m_label->move(100,100);
    m_label->setVisible(true);
}

bool HGWarningWidget::closeWindow()
{
    return true;
}
HGWarningWidget::~HGWarningWidget()
{

}