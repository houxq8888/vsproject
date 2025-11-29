#include "hgprintwidget.h"
#include <QHeaderView>
#include "common.h"
#include "mypixmapitem.h"
#include <QGraphicsOpacityEffect>
#include <QPrinter>
#include <QMessageBox>
#include "hgcomwithssh.h"
#include "PrinterManager.h"


HGPrintWidget::HGPrintWidget(std::string lang,std::string printTitle,std::vector<std::string> printOptions,QWidget *parent) : QWidget(parent),
    m_lang(lang),
    m_handWritingWidget(nullptr),
    m_clearBtn(nullptr),
    m_okBtn(nullptr),
    m_printTitle(printTitle)
{
    // for (int i=0;i<int(printOptions.size());i++){
    //     PrintOption po;
    //     po.key=printOptions[i];
    //     po.optionBox=new QCheckBox(QString::fromStdString(po.key));
    //     po.optionBox->setObjectName(QString::fromStdString(po.key));
    //     m_printOptions[po.key]=po;
    //     m_printOptions[po.key].optionBox->setChecked(true);
    //     connect(m_printOptions[po.key].optionBox, &QCheckBox::toggled, [this](bool checked) {
    //         fnPaintImage();
    //     });
    // }
    m_layout=new QGridLayout();
    this->setLayout(m_layout);

    m_printGroup=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"Print")));
    m_printGroup->setStyleSheet("QGroupBox { font-size: 12pt; font-weight:bold;}");
    m_printLayout=new QGridLayout();

    m_backBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Back")));

    m_numberLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"PageNumber")));
    m_pageNumberSpinBox=new QSpinBox();
    m_pageNumberSpinBox->setRange(1,999);
    m_printerLabel=new HGQLabel();
    QPixmap pixmap(QString::fromStdString(getPath("/resources/V1/@print.png")));
    m_printerLabel->setPixmap(pixmap.scaled(80,80,Qt::KeepAspectRatio));
    connect(m_printerLabel,SIGNAL(leftClicked()),this,SLOT(slotPrintClicked()));


    m_printerGroup=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"Printer")));
    m_printInfoGroup=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"PrintInfo")));
    m_printerLayout=new QGridLayout();
    m_printerGroup->setLayout(m_printerLayout);
    m_printInfoLayout=new QGridLayout();
    m_printInfoGroup->setLayout(m_printInfoLayout);

    m_paperSizeLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"PaperSize")));
    m_eSignCheckBox=new QCheckBox(QString::fromStdString(loadTranslation(m_lang,"EleSign")));
    m_printerComboBox=new QComboBox();
    std::vector<std::string> printerList=getPrinerList();
    printerList.push_back("网络");
    for (int i=0;i<int(printerList.size());i++){
        m_printerComboBox->addItem(QIcon(QString::fromStdString(getPath("/resources/V1/@print_ready.png"))),
            QString::fromStdString(printerList[i]));
    }
    m_printerLayout->addWidget(m_printerComboBox,0,0);

    m_paperSizeComboBox=new QComboBox();
    m_paperSizeComboBox->addItems({"A4","Letter","B5","A5","B6","A6","Executive","16K","自定义尺寸"});

    m_reportTemplateLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"ReportTemplate")));
    m_reportTemplateComboBox=new QComboBox();
    m_reportTemplateComboBox->addItems({QString::fromStdString(loadTranslation(m_lang,"Default"))});

    m_drawChartTypeLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"DrawChartType")));
    m_drawChartTypeComboBox=new QComboBox();
    m_drawChartTypeComboBox->addItems({QString::fromStdString(loadTranslation(m_lang,"Mesh"))});
    m_chartScaleLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"ChartScale")));
    m_chartScaleComboBox=new QComboBox();
    m_chartScaleComboBox->addItems({"Auto"});
   
    connect(m_eSignCheckBox, &QCheckBox::toggled, [this](bool checked) {
        if (checked){
            m_handWritingWidget=new HGHandWritingWidget();
            m_clearBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Clear")));
            m_okBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Ok")));
            m_printLayout->addWidget(m_handWritingWidget,5,1,1,1);
            m_printLayout->addWidget(m_clearBtn,5,2,1,1);
            m_printLayout->addWidget(m_okBtn,6,2,1,1);
            connect(m_clearBtn, &QPushButton::clicked, [this]() {
                m_handWritingWidget->clearPoints();
            });
            connect(m_okBtn, &QPushButton::clicked, [this]() {
                QPixmap pixmap = m_handWritingWidget->getPixmap();
                if (!pixmap.isNull())
                {
                    MyPixmapItem *overlayItem = new MyPixmapItem(m_lang, pixmap);
                    overlayItem->setZValue(1);
                    // 设置透明度
                    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect;
                    effect->setOpacity(1.0); // 50% 透明
                    overlayItem->setGraphicsEffect(effect);
                    scene->addItem(overlayItem);
                    RWDb::writeAuditTrailLog("add eSign");
                    // emit signalESignPix(pixmap);
                }           
            });
        } else {
            if (m_handWritingWidget!=nullptr){
                m_printLayout->removeWidget(m_handWritingWidget);
                delete m_handWritingWidget;
                m_handWritingWidget=nullptr;
            }
            if (m_clearBtn!=nullptr){
                m_printLayout->removeWidget(m_clearBtn);
                SAFE_DELETE(m_clearBtn);
            }
            if (m_okBtn!=nullptr){
                m_printLayout->removeWidget(m_okBtn);
                SAFE_DELETE(m_okBtn);
            }
        }
    });

    m_printInfoLayout->addWidget(m_reportTemplateLabel,0,0);
    m_printInfoLayout->addWidget(m_reportTemplateComboBox,0,1);
    // int i=0;
    // for (auto option : m_printOptions){
    //     m_printInfoLayout->addWidget(option.second.optionBox,m_printInfoLayout->rowCount()-(i%2),(i%2));
    //     i++;
    // }
    m_printInfoLayout->addWidget(m_drawChartTypeLabel,m_printInfoLayout->rowCount(),0);
    m_printInfoLayout->addWidget(m_drawChartTypeComboBox,m_printInfoLayout->rowCount()-1,1);
    m_printInfoLayout->addWidget(m_chartScaleLabel,m_printInfoLayout->rowCount(),0);
    m_printInfoLayout->addWidget(m_chartScaleComboBox,m_printInfoLayout->rowCount()-1,1);

    m_printLayout->addWidget(m_backBtn,0,1);
    m_printLayout->addWidget(m_numberLabel,1,0);
    m_printLayout->addWidget(m_pageNumberSpinBox,1,1);
    m_printLayout->addWidget(m_printerLabel,0,2,2,2);
    m_printLayout->addWidget(m_printerGroup,2,0,1,4);
    m_printLayout->addWidget(m_printInfoGroup,3,0,1,4);
    m_printLayout->addWidget(m_paperSizeLabel,4,0,1,2);
    m_printLayout->addWidget(m_paperSizeComboBox,4,1,1,2);
    m_printLayout->addWidget(m_eSignCheckBox,5,0,1,1);
    
    m_printGroup->setLayout(m_printLayout);

    view = new QGraphicsView;
    view->setStyleSheet("background: transparent");
    view->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    scene = new QGraphicsScene;
    view->setScene(scene);
    // m_qimg=QImage(594,892,QImage::Format_ARGB32);
    // pixmap=QPixmap::fromImage(m_qimg);
    // pixmapItem=scene->addPixmap(pixmap);
    // view->fitInView(pixmapItem,Qt::KeepAspectRatio);
    view->viewport()->update();

    m_layout->addWidget(m_printGroup,0,1);
    m_layout->addWidget(view,0,2);
    connect(m_backBtn,SIGNAL(clicked()),this,SLOT(slotBackClicked()));
}
bool HGPrintWidget::closeWindow()
{
    if (m_handWritingWidget!=nullptr){
        if (m_handWritingWidget->closeWindow()){
            SAFE_DELETE(m_handWritingWidget);
        }
    }
    return true;
}
HGPrintWidget::~HGPrintWidget()
{
    
}
void HGPrintWidget::setPrintData(const std::vector<InPrintOption> &printData){
    m_printOptions.clear();
    m_printOptions.resize(printData.size());
    for (int i=0;i<int(printData.size());i++){
        m_printOptions[i].printTitle=printData[i].printTitle;
        m_printOptions[i].printElements.resize(printData[i].inPrintElements.size());
        for (int j=0;j<int(printData[i].inPrintElements.size());j++){
            m_printOptions[i].printElements[j].optionBox=NULL;
            m_printOptions[i].printElements[j].inPrintElement.key=printData[i].inPrintElements[j].key;
            m_printOptions[i].printElements[j].inPrintElement.value=printData[i].inPrintElements[j].value;
        }
    }
    fnPaintImage();
}
void HGPrintWidget::fnPaintImage()
{
    scene->clear();
    // ======== 字体设置 ========
    QFont labelFont("Arial", 12, QFont::Bold);
    QFont valueFont("Arial", 12);
    QFont titleFont("Arial", 20, QFont::Bold);
    QFontMetrics valueMetrics(valueFont);
    QFontMetrics labelMetrics(labelFont);

    // ======== 页面参数（A4 72dpi） ========
    const qreal pageWidth = 595;
    const qreal pageHeight = 842;
    const qreal margin = 20;// 页面间距
    const qreal bottomMargin = 40;

    // ======== 表格布局参数 ========
    int leftMargin  = 20;
    int rightMargin = 20;
    int gap         = 10; // label 和 value 之间的间距
    int labelWidth = 180;
    int valueWidth = int(pageWidth) - leftMargin - rightMargin - labelWidth - gap;
    int rowSpacing = 2;
    int titleHeight = 80;

    m_qimgs.clear();

    for (int i = 0; i < int(m_printOptions.size()); ++i)
    {
        // ======== 新建第一页 ========
        QImage img(pageWidth, pageHeight, QImage::Format_ARGB32);
        img.fill(Qt::white);
        QPainter painter(&img);
        painter.setRenderHint(QPainter::Antialiasing);
        // ======== 绘制标题 ========
        painter.setFont(titleFont);
        painter.setPen(Qt::black);
        painter.drawText(QRect(0, 20, int(pageWidth), 50), 
            Qt::AlignCenter, QString::fromStdString(m_printOptions[i].printTitle));
        // ======== 绘制表格 ========
        int inity = titleHeight;

        // ======== 绘制表格（分页逻辑） ========
        for (int j=0;j<int(m_printOptions[i].printElements.size());j++)
        {
            // if (!data.second.optionBox->isChecked())
                // continue;
            std::string value = m_printOptions[i].printElements[j].inPrintElement.value;
            std::string key = m_printOptions[i].printElements[j].inPrintElement.key;

            // ======== 计算 key/value 高度 ========
            QRect keyRectCalc(0,0,labelWidth,1000);
            QRect keyBounding = labelMetrics.boundingRect(
                keyRectCalc, Qt::TextWordWrap, 
                QString::fromStdString(key)
            );

            QRect valueRectCalc(0, 0, valueWidth, 1000);
            QRect valueBounding = valueMetrics.boundingRect(valueRectCalc, Qt::TextWordWrap, 
                QString::fromStdString(value));

            int rowHeight = std::max(40, std::max(keyBounding.height(),valueBounding.height()) + 20);
            // 如果当前行放不下，分页
            if (inity + rowHeight > pageHeight - bottomMargin){
                painter.end();
                m_qimgs.push_back(img);
                
                inity = titleHeight/2;
                // new page
                img=QImage(pageWidth, pageHeight, QImage::Format_ARGB32);
                img.fill(Qt::white);
                painter.begin(&img);
                painter.setRenderHint(QPainter::Antialiasing);

                // // ======== 绘制标题 ========
                // painter.setFont(titleFont);
                // painter.setPen(Qt::black);
                // painter.drawText(QRect(0, 20, int(pageWidth), 50), 
                //         Qt::AlignCenter, QString::fromStdString(m_printOptions[i].printTitle));
                // inity = titleHeight;
            }
            // ======== 绘制单元格 ========
            QRect labelRect(leftMargin, inity, labelWidth, rowHeight);
            QRect valueRect(leftMargin + labelWidth + gap, inity, valueWidth, rowHeight);

            // 边框
            QPen borderPen(QColor(180, 180, 180));
            borderPen.setWidth(1);
            painter.setPen(borderPen);
            painter.drawRect(labelRect);
            painter.drawRect(valueRect);

            // key（支持换行）
            painter.setFont(labelFont);
            painter.setPen(Qt::black);
            painter.drawText(labelRect.adjusted(5, 5, -5, -5), 
                Qt::AlignLeft | Qt::TextWordWrap, 
                QString::fromStdString(key));

            // value（支持换行）
            painter.setFont(valueFont);
            painter.drawText(valueRect.adjusted(5, 5, -5, -5), 
                Qt::AlignLeft | Qt::TextWordWrap, 
                QString::fromStdString(value));

            inity += rowHeight + rowSpacing;
        }
        // 最后一页
        painter.end();
        m_qimgs.push_back(img);
    }
    // ======== 把所有页加到 Scene ========
    for (int p=0;p<m_qimgs.size();++p){
        // 页面在 Scene 中的位置
        qreal x = 0;
        qreal y = p * (pageHeight + margin);

        // ======== 将页面添加到 Scene ========
        QGraphicsPixmapItem* pageItem = scene->addPixmap(QPixmap::fromImage(m_qimgs[p]));
        pageItem->setPos(x, y);
        // 页面边框
        scene->addRect(QRectF(x, y, pageWidth, pageHeight),QPen(Qt::black, 2));
    }
    // ======== 调整视图 ========
    view->setRenderHint(QPainter::Antialiasing);
    view->setSceneRect(scene->itemsBoundingRect());
}

void HGPrintWidget::slotBackClicked(){
    emit signalBackClicked();
}
void HGPrintWidget::slotPrintClicked()
{
    std::string printerName=m_printerComboBox->currentText().toStdString();
    int copyNum = m_pageNumberSpinBox->value();
    std::string paperSize = m_paperSizeComboBox->currentText().toStdString();

    std::string reportPath = FileConfig::getDirPath() + "/report/";
    HGMkDir(reportPath);
    HGExactTime curTime = HGExactTime::currentTime();
    std::string syncslice = curTime.toStringFromYearToSec();
    std::string filename = reportPath + syncslice + ".pdf";

    // 设置 PDF 打印机
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(QString::fromStdString(filename));

    // 设置页面大小 (A4)
    if (paperSize == "A4") printer.setPageSize(QPageSize(QPageSize::A4));
    else if (paperSize == "A5") printer.setPageSize(QPageSize(QPageSize::A5));
    else if (paperSize == "B5") printer.setPageSize(QPageSize(QPageSize::B5));
    else if (paperSize == "B6") printer.setPageSize(QPageSize(QPageSize::B6));
    else if (paperSize == "A6") printer.setPageSize(QPageSize(QPageSize::A6));
    else if (paperSize == "Letter") printer.setPageSize(QPageSize(QPageSize::Letter));
    else if (paperSize == "Executive") printer.setPageSize(QPageSize(QPageSize::Executive));
    else if (paperSize == "16K" || paperSize == ""){
        QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),
                             QString::fromStdString(loadTranslation(m_lang, "PaperSizeError")));
        return;
    }
    printer.setFullPage(true);

    // 创建 QPainter 进行渲染
    QPainter painter(&printer);
    for (int i=0;i<int(m_qimgs.size());++i){
        if (i>0){
            printer.newPage();
        }
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        QRectF pageRect = printer.pageRect(QPrinter::Millimeter);
        QSize imgSize = m_qimgs[i].size();
        qreal scale = qMin(pageRect.width() / qreal(imgSize.width()), 
            pageRect.height() / qreal(imgSize.height()));
        
        int w = imgSize.width() * scale;
        int h = imgSize.height() * scale;
        int x = pageRect.x() + (pageRect.width() - w)/2;
        int y = pageRect.y() + (pageRect.height() - h)/2;
#else
        QRect pageRect = printer.pageRect();
        QSize imgSize = m_qimgs[i].size();
        qreal scale = qMin(pageRect.width() / qreal(imgSize.width()), 
            pageRect.height() / qreal(imgSize.height()));
        
        int w = imgSize.width() * scale;
        int h = imgSize.height() * scale;
        int x = pageRect.x() + (pageRect.width() - w)/2;
        int y = pageRect.y() + (pageRect.height() - h)/2;
#endif

        painter.drawImage(QRect(x, y, w, h), m_qimgs[i]);
    }
    painter.end();
    if (printerName == "网络")
    {
        SshUploader uploader(GlobalSingleton::instance().getSystemInfo("NetworkIP1") + "." +
                                 GlobalSingleton::instance().getSystemInfo("NetworkIP2") + "." +
                                 GlobalSingleton::instance().getSystemInfo("NetworkIP3") + "." +
                                 GlobalSingleton::instance().getSystemInfo("NetworkIP4"),
                             22, GlobalSingleton::instance().getSystemInfo("NetworkLoginName"),
                             GlobalSingleton::instance().getSystemInfo("NetworkLoginPasswd"));
        if (!uploader.connect())
        {
            std::cerr << "failed to connect SSH server\n";
            return;
        }
        if (!uploader.uploadFile(filename, GlobalSingleton::instance().getSystemInfo("NetworkSavePath") + syncslice + ".pdf"))
        {
            std::cerr << "file upload failed\n";
            return;
        }
        std::cout << "upload sucess\n";
    }
    else
    {
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        PrinterManager::setDefaultPrinter(printerName);
        if (PrinterManager::printPdfWithPDFtoPrinter(filename))
        {
            QMessageBox::information(this, QString::fromStdString(HG_DEVICE_NAME),
                                     QString::fromStdString(loadTranslation(m_lang, "PrintSuccess")));
        }
        else
        {
            QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),
                                 QString::fromStdString(loadTranslation(m_lang, "PrintError")));
        }
#else
        PrintInfo printInfo;
        printInfo.copyNum = copyNum;
        printInfo.filePath = filename;
        printInfo.printerName = printerName;
        printInfo.pageSize = paperSize;
        int ret = printFile(printInfo);
        if (ret < 0)
        {
            QMessageBox::warning(this, QString::fromStdString(HG_DEVICE_NAME),
                                 QString::fromStdString(loadTranslation(m_lang, "PrintError")));
        }
#endif
    }
    RWDb::writeAuditTrailLog("print " + filename);
}