#include "testwidget.h"

TestWidget::TestWidget(QWidget *parent) : QWidget(parent)
{
    layout = new QVBoxLayout(this);
    label = new QLabel("Test Widget", this);
    button = new QPushButton("Click Me", this);
    layout->addWidget(label);
    layout->addWidget(button);
    setLayout(layout);
}
