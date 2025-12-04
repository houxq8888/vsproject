#ifndef HGCOMDEBUGWIDGET_H
#define HGCOMDEBUGWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QLabel>

class HGComDebugWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HGComDebugWidget(QWidget *parent = nullptr);
    virtual ~HGComDebugWidget();

private slots:
    void fnSlotListenTimerOut();

private:
    void openSerial();

private:
    bool m_valid;
    QTimer *m_listenTimer;
    QLabel *m_elapsedLabel;
};
#endif // HGCOMDEBUGWIDGET_H
