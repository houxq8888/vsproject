#include "scannerwidget.h"
#include "common.h"
#include <QHeaderView>
#include "systemusage.h"
#include "config.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

ScannerWidget::ScannerWidget(std::string path,QWidget *parent) : QWidget(parent)
{ 
    uint16_t vendorID = FileConfig::getScannerVendorID();//0xAF99;
    uint16_t productID = FileConfig::getScannerProductID();//0x8002;
    // 检查USB设备是否存在
    if (!SystemUsage::getUSBDevices(vendorID, productID)){
        std::cerr << "Target USB device not found" << std::endl;
        // return;
    }
    scannerInfo=RWDb::readScannerInfo();

    m_lang = path;
    m_layout = new QGridLayout(this);
    m_contentLayout = new QGridLayout();
    m_contentGroupBox = new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"Content")));
    m_sampleNameCheckBox = new QCheckBox(QString::fromStdString(loadTranslation(m_lang,"Sname")));
    m_batchCheckBox = new QCheckBox(QString::fromStdString(loadTranslation(m_lang,"Sbatch")));
    m_serialCheckBox = new QCheckBox(QString::fromStdString(loadTranslation(m_lang,"Sserial")));
    m_weightCheckBox = new QCheckBox(QString::fromStdString(loadTranslation(m_lang,"Samount")));
    m_sampleNameCheckBox->setObjectName(m_sampleNameCheckBox->text());
    m_batchCheckBox->setObjectName(m_batchCheckBox->text());
    m_serialCheckBox->setObjectName(m_serialCheckBox->text());
    m_weightCheckBox->setObjectName(m_weightCheckBox->text());

    m_separatorLayout=new QGridLayout();
    m_separatorGroupBox=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"SeparatorSet")));
    m_separatorGroupBox->setLayout(m_separatorLayout);

    m_realDisplayLayout=new QGridLayout();
    m_realDisplayGroupBox=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"RealPreview")));
    m_realDisplayGroupBox->setLayout(m_realDisplayLayout);
    m_realDisplayRegionLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"Preview"))+":");
   
    m_separatorLineEdit = new QLineEdit();
    m_separatorLineEdit->setPlaceholderText("请输入分隔符（如逗号、空格等）");
    

    m_saveBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Save")));
    m_resetBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Reset")));

    m_contentGroupBox->setLayout(m_contentLayout);
    m_sampleNameCheckBox->setChecked(true);
    m_contentLayout->addWidget(m_sampleNameCheckBox,0,0);
    m_contentLayout->addWidget(m_batchCheckBox,0,1);
    m_contentLayout->addWidget(m_serialCheckBox,0,2);
    m_contentLayout->addWidget(m_weightCheckBox,1,0);

    m_separatorLayout->addWidget(new QLabel(QString::fromStdString(loadTranslation(m_lang,"InputSeparator"))+":"));
    m_separatorLayout->addWidget(m_separatorLineEdit);

    m_testTable = new QTableWidget(m_contentLayout->count(),2);
    m_testTable->verticalHeader()->setVisible(false);
    m_testTable->horizontalHeader()->setVisible(false);
    m_testTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_testTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_testTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for (int i=0;i<m_contentLayout->count();i++){
        int row=i;
        int col=0;
        QWidget *widget = m_contentLayout->itemAt(i)->widget();
    
        if (widget) {
            // 只处理 QCheckBox 类型的控件
            QCheckBox *checkBox = qobject_cast<QCheckBox *>(widget);
            if (checkBox) {
                QTableWidgetItem *item = new QTableWidgetItem(checkBox->text());
                m_testTable->setItem(row, col, item);
            }
        }
    }
     
    m_testGroupBox=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"Test")));
    m_testLayout = new QGridLayout(m_testGroupBox);
    m_testLineEdit = new ScanInputLineEdit();
    m_testLineEdit->setPlaceholderText(QString::fromStdString(loadTranslation(m_lang,"PleaseInputTestBarcode")));
    m_testStatusLabel = new QLabel();
    m_testLayout->addWidget(new QLabel(QString::fromStdString(loadTranslation(m_lang,"TestBarcode"))+":"));
    m_testLayout->addWidget(m_testLineEdit);
    m_testLayout->addWidget(m_testTable,0,1,2,1);
    m_testLayout->addWidget(m_testStatusLabel,m_testLayout->rowCount(),0,1,2);


    m_realDisplayLayout->addWidget(m_realDisplayRegionLabel,0,0);
    m_layout->addWidget(m_contentGroupBox,0,0,1,2);
    m_layout->addWidget(m_separatorGroupBox,m_layout->rowCount(),0,1,2);
    m_layout->addWidget(m_realDisplayGroupBox,m_layout->rowCount(),0,1,2);
    m_layout->addWidget(m_saveBtn,m_layout->rowCount(),0);
    m_layout->addWidget(m_resetBtn,m_layout->rowCount()-1,1);
    m_layout->addWidget(m_testGroupBox,m_layout->rowCount(),0,1,2);

    connect(m_sampleNameCheckBox, &QCheckBox::toggled, this, &ScannerWidget::updatePreview);
    connect(m_batchCheckBox, &QCheckBox::toggled, this, &ScannerWidget::updatePreview);
    connect(m_serialCheckBox, &QCheckBox::toggled, this, &ScannerWidget::updatePreview);
    connect(m_weightCheckBox, &QCheckBox::toggled, this, &ScannerWidget::updatePreview);
    connect(m_separatorLineEdit, &QLineEdit::textChanged, this, &ScannerWidget::updatePreview);
    connect(m_saveBtn, &QPushButton::clicked, this, &ScannerWidget::saveSettings);
    connect(m_resetBtn, &QPushButton::clicked, this, &ScannerWidget::resetSettings);
    connect(m_testLineEdit, &QLineEdit::textChanged, this, &ScannerWidget::onTextChanged);

    if (scannerInfo["Sname"]=="true"){
        m_sampleNameCheckBox->setChecked(true);
    } else m_sampleNameCheckBox->setChecked(false);
    if (scannerInfo["Sbatch"]=="true"){
        m_batchCheckBox->setChecked(true);
    } else m_batchCheckBox->setChecked(false);
    if (scannerInfo["Sserial"]=="true"){
        m_serialCheckBox->setChecked(true);
    } else m_serialCheckBox->setChecked(false);
    if (scannerInfo["Samount"]=="true"){
        m_weightCheckBox->setChecked(true);
    } else m_weightCheckBox->setChecked(false);
    m_separatorLineEdit->setText(QString::fromStdString(scannerInfo["Separator"]));

    updatePreview();

}
ScannerWidget::~ScannerWidget()
{
}
bool ScannerWidget::closeWindow()
{
    return true;
}

void ScannerWidget::saveSettings() {
    // 保存用户设置的代码
    RWDb::writeScannerInfo(std::map<std::string,std::string>{
        {"Sname", m_sampleNameCheckBox->isChecked() ? "true" : "false"},  // 使用 isChecked() 获取勾选状态
        {"Sbatch", m_batchCheckBox->isChecked() ? "true" : "false"},
        {"Sserial", m_serialCheckBox->isChecked() ? "true" : "false"},
        {"Samount", m_weightCheckBox->isChecked() ? "true" : "false"},
        {"Separator", m_separatorLineEdit->text().toStdString()}  // 获取分隔符文本
    });
}
void ScannerWidget::resetSettings() {
    m_sampleNameCheckBox->setChecked(true);
    m_batchCheckBox->setChecked(false);
    m_serialCheckBox->setChecked(false);
    m_weightCheckBox->setChecked(false);
    m_separatorLineEdit->clear();
    m_realDisplayRegionLabel->setText(QString::fromStdString(loadTranslation(m_lang,"Preview"))+":");
    updatePreview();
}
void ScannerWidget::updatePreview() {
    QStringList selectedOptions;
    if (m_sampleNameCheckBox->isChecked()) selectedOptions.append(m_sampleNameCheckBox->text());
    if (m_batchCheckBox->isChecked()) selectedOptions.append(m_batchCheckBox->text());
    if (m_serialCheckBox->isChecked()) selectedOptions.append(m_serialCheckBox->text());
    if (m_weightCheckBox->isChecked()) selectedOptions.append(m_weightCheckBox->text());
    QString separator = m_separatorLineEdit->text();
    m_realDisplayRegionLabel->setText(QString::fromStdString(loadTranslation(m_lang,"Preview"))+":\n" + selectedOptions.join(separator));
}
int ScannerWidget::decodeInput(const std::string &inputStr,std::map<std::string,std::string> &parseList)
{
    QString separator = QString::fromStdString(scannerInfo["Separator"]);
    if (separator.isEmpty()){
        return -1;
    } 
    QStringList dataList = QString::fromStdString(inputStr).split(separator, Qt::SkipEmptyParts);
    if (dataList.size() == m_contentLayout->count()){
        for (int i=0;i<m_contentLayout->count();i++){
            QWidget *widget = m_contentLayout->itemAt(i)->widget();
            if (widget) {
                // 只处理 QCheckBox 类型的控件
                QCheckBox *checkBox = qobject_cast<QCheckBox *>(widget);
                if (checkBox) {
                    if (checkBox->isChecked()) {
                        parseList[checkBox->text().toStdString()]=dataList[i].toStdString();
                    } else {
                        parseList[checkBox->text().toStdString()]="";;
                    }
                }
            }
        }
    } else {
        return -2;
    }
    return 0;
}
void ScannerWidget::onTextChanged(const QString &newText)
{
    if (!m_testLineEdit->isScannerInput()){
        printf("not scanner input\n");
        return;
    }
        
    // 获取用户设置的分隔符
    QString separator = m_separatorLineEdit->text(); 
    // 如果没有设置分隔符，直接返回
    if (separator.isEmpty()) {
        m_testStatusLabel->setText("没有设置分隔符");
        return;
    } else if (separator.trimmed().isEmpty()){
        m_testStatusLabel->setText("分隔符空格");
    }
    // 检查是否已经有数据输入
    if (!newText.isEmpty()) {
        // 解析输入的数据
        QStringList dataList = newText.split(separator, Qt::SkipEmptyParts);  // 根据分隔符分割文本

        // 处理数据
        parseData(dataList);
    }
}
void ScannerWidget::parseData(const QStringList &dataList)
{
    // clear
    for (int i=0;i<m_testTable->rowCount();i++){
        QTableWidgetItem *item = new QTableWidgetItem("");
        m_testTable->setItem(i, 1, item);
    }
    // 假设你有四个数据项
    if (dataList.size() == m_contentLayout->count()) {
        for (int i=0;i<m_contentLayout->count();i++){
            QWidget *widget = m_contentLayout->itemAt(i)->widget();
    
            if (widget) {
                // 只处理 QCheckBox 类型的控件
                QCheckBox *checkBox = qobject_cast<QCheckBox *>(widget);
                if (checkBox) {
                    if (checkBox->isChecked()) {
                        QString qstr = dataList[i];

                        QTableWidgetItem *item = new QTableWidgetItem(qstr);
                        m_testTable->setItem(i, 1, item);
                    }
                }
            }
        }
        m_testStatusLabel->setText("数据解析成功");
    } else {
        if (m_testLineEdit->isScannerOver())
            m_testStatusLabel->setText("二维码数据不符合约定规则");
        printf("Invalid data input.");
    }
}
bool ScannerWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) // 单击
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton)
        {
            // 首次弹出登入框
            // if (obj==m_thresholdEdit)
            // {
            //     QString str=m_thresholdEdit->text();
            //     int threshold=str.toInt();
            //     emit sendMsg2Manager(threshold);
            // }

            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        // pass the event to the parent class
        return QWidget::eventFilter(obj, event);
    }
    return true;
}
