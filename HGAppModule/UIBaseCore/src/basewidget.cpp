#include "basewidget.h"


BaseWidget::BaseWidget(QWidget *parent) : QWidget(parent)
{

}
BaseWidget::~BaseWidget(){
    
}
BaseWidget& BaseWidget::operator=(const BaseWidget& obj){
    fnCopy(obj);
    return (*this);
}
void BaseWidget::fnCopy(const BaseWidget& obj){

}
void BaseWidget::setLanguage(std::string lang){
    
}


