#ifndef TESTWIDGET_H
#define TESTWIDGET_H 

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

class TestWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TestWidget(QWidget *parent = nullptr);

signals:

public slots:

private:
    QVBoxLayout *layout;
    QLabel* label;
    QPushButton* button;
};
#endif // TESTWIDGET_H