#ifndef HGANALYSISRECORDWIDGET_H
#define HGANALYSISRECORDWIDGET_H

#include <QWidget>
#include "hgqlabel.h"
#include <QSplitter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimer>
#include "hgprintwidget.h"
#include "hgdisplayrecordwidget.h"
#include "hgdisplaychartwidget.h"
#include "hgdisplayrecordinfowidget.h"

class HGAnalysisRecordWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGAnalysisRecordWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();
    ~HGAnalysisRecordWidget();

signals:

private slots:
    void onTimeout();
    void slotShowDataInfo(std::map<std::string,std::string>);
    void slotShowPrinter();

private:
    void fnUpdateDataInfo();

private:
    QSplitter *m_horsplitter,*m_versplitter;
    QVBoxLayout* m_verlayout;
    QHBoxLayout* m_horlayout;
    HGDisplayRecordWidget* m_displayRecordW;
    HGDisplayChartWidget* m_displayChartsW;
    HGDisplayRecordInfoWidget* m_displayRecordInfoW;
    QGroupBox* m_rightBox;
    QTimer* m_timer;
    std::string m_lang;
    HGPrintWidget* m_printW;
};

#endif // HGANALYSISRECORDWIDGET_H
