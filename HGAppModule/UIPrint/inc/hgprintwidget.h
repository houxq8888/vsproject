#ifndef HGPRINTWIDGET_H
#define HGPRINTWIDGET_H

#include <QWidget>
#include "hgqlabel.h"
#include <QSplitter>
#include <QPushButton>
#include <QTableWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QTextEdit>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "hghandwritingwidget.h"

typedef struct tagInPrintElement{
    std::string key;
    std::string value;
    tagInPrintElement(){
        key = "";
        value = "";
    }
    tagInPrintElement(std::string _key,std::string _val){
        key = _key;
        value = _val;
    }
} InPrintElement;
typedef struct tagInPrintOption{
    std::vector<InPrintElement> inPrintElements;
    std::string printTitle;
} InPrintOption;
typedef struct tagPrintElement{
    InPrintElement inPrintElement;
    QCheckBox* optionBox;
} PrintElement;
typedef struct tagPrintOption{
    std::vector<PrintElement> printElements;
    std::string printTitle;
} PrintOption;

class HGPrintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGPrintWidget(std::string,std::string,std::vector<std::string> printOptions,QWidget *parent = nullptr);
    bool closeWindow();
    ~HGPrintWidget();
    void setPrintData(const std::vector<InPrintOption> &printData);

signals:
    void signalBackClicked();

private slots:
    void slotBackClicked();
    void slotPrintClicked();
private:
    void fnPaintImage();

private:
    std::string m_printTitle;
    QLabel *m_numberLabel;
    QGroupBox *m_printerGroup,*m_printInfoGroup;
    QGridLayout* m_printerLayout,*m_printInfoLayout;
    HGQLabel* m_printerLabel;

    QLabel* m_paperSizeLabel;
    QGroupBox* m_printGroup;
    QGridLayout* m_layout, *m_printLayout;
    std::string m_lang;
    QPushButton* m_backBtn,*m_clearBtn,*m_okBtn;
    QSpinBox* m_pageNumberSpinBox;
    QComboBox* m_paperSizeComboBox,*m_printerComboBox;
    QCheckBox* m_eSignCheckBox;

    QLabel* m_reportTemplateLabel,*m_drawChartTypeLabel,*m_chartScaleLabel;
    QComboBox* m_reportTemplateComboBox,*m_drawChartTypeComboBox,*m_chartScaleComboBox;
    std::vector<PrintOption> m_printOptions;
    std::vector<QImage> m_qimgs;


    HGHandWritingWidget* m_handWritingWidget;
    QGraphicsView* view;
    QGraphicsScene* scene;
    QGraphicsPixmapItem* pixmapItem;

};

#endif // HGPRINTWIDGET_H
