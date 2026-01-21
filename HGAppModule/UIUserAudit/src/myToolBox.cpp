#include "myToolBox.h"
#include "common.h"
#include "warningwidget.h"
#include "hgxml.h"

using namespace HGMACHINE;

MyToolBox::MyToolBox(QWidget *parent) :
    QWidget(parent),
    m_pContentVBoxLayout(nullptr)
{
    QWidget *widget = new QWidget();
    m_pContentVBoxLayout = new QVBoxLayout;
    m_pContentVBoxLayout->setContentsMargins(0, 0, 0, 0);
    m_pContentVBoxLayout->setSpacing(2);
    widget->setLayout(m_pContentVBoxLayout);

    m_scrollArea=new QScrollArea();
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setWidget(widget);

    QVBoxLayout *vBoxLayout = new QVBoxLayout();
    vBoxLayout->addWidget(m_scrollArea);
    this->setLayout(vBoxLayout);
}

MyToolBox::~MyToolBox()
{
    
}
bool MyToolBox::closeWindow()
{
    if (m_pContentVBoxLayout==nullptr) return true;
    int count=m_pContentVBoxLayout->count();
    for(int i=0;i<count;i++)
    {
        ToolPage *page=dynamic_cast<ToolPage*>(m_pContentVBoxLayout->itemAt(i)->widget());
        if(page)
        {
            page->closeWindow();
        }
    }
    return true;
}
#include <QDebug>
BaseWidget* MyToolBox::getWidget(const std::string& title) {
    if (!m_pContentVBoxLayout) {
        qDebug() << "Layout is null.";
        return nullptr;
    }

    QString qTitle = QString::fromStdString(title);

    for (int i = 0; i < m_pContentVBoxLayout->count(); ++i) {
        QLayoutItem* layoutItem = m_pContentVBoxLayout->itemAt(i);
        if (!layoutItem) {
            continue;
        }

        ToolPage* widget = qobject_cast<ToolPage*>(layoutItem->widget());
        if (!widget) {
            qDebug() << "Item at index" << i << "is not a widget.";
            continue;
        }

        if (widget->objectName() == qTitle) {
            // qDebug() << "className:"<<widget->getWidget()->metaObject()->className();
            return qobject_cast<BaseWidget*>(widget->getWidget());
        }
    }

    qDebug() << "No widget found with the specified title.";
    return nullptr;
}
void MyToolBox::setLanguage(std::string lang){
    for (int i = 0; i < m_pContentVBoxLayout->count(); ++i) {
        QLayoutItem* layoutItem = m_pContentVBoxLayout->itemAt(i);
        if (!layoutItem) {
            continue;
        }

        ToolPage* widget = qobject_cast<ToolPage*>(layoutItem->widget());
        if (!widget) {
            qDebug() << "Item at index" << i << "is not a widget.";
            continue;
        }

        widget->setLanguage(lang);
    }
}
void MyToolBox::addWidget(const std::string& lang, const std::string &title, BaseWidget *widget)
{
    ToolPage *page = new ToolPage(this);
    page->addWidget(lang,title, widget);
    page->setObjectName(QString::fromStdString(title));
    connect(page,&ToolPage::signalSoundVolumn,[=](int volumn){
        emit signalSoundVolumn(volumn);
    });
    connect(page,&ToolPage::updateLanguage,[=](std::string lang){
        emit updateLanguage(lang);
    });
    m_pContentVBoxLayout->addWidget(page);
}

ToolPage::ToolPage(QWidget *parent) :
    QWidget(parent),
    m_pPushButtonFold(nullptr),
    m_bIsExpanded(true),
    m_pLabel(nullptr),
    m_pContentVBoxLayout(nullptr)
{
    this->setAttribute(Qt::WA_StyledBackground);

    m_pPushButtonFold=new QPushButton();
    m_pPushButtonFold->setStyleSheet("QPushButton { text-align: left; vertical-align: middle; }");
    m_pLabel = new QLabel();
    m_pLabel->setFixedSize(24,24);
    m_pLabel->setPixmap(QPixmap(QString::fromStdString(getPath("/resources/V1/@1xze-arrow-down 1.png"))).scaled(m_pLabel->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    m_pContentVBoxLayout=new QVBoxLayout();
    this->setLayout(m_pContentVBoxLayout);

    QHBoxLayout *layout = new QHBoxLayout(m_pPushButtonFold);
    layout->setContentsMargins(0, 0, 5, 0);
    layout->addStretch(1);
    layout->addWidget(m_pLabel);

    m_pContentVBoxLayout->addWidget(m_pPushButtonFold);
    connect(m_pPushButtonFold, &QPushButton::clicked, this, &ToolPage::onPushButtonFoldClicked);
}

ToolPage::~ToolPage()
{
    
}
bool ToolPage::closeWindow()
{
    int count=m_pContentVBoxLayout->count();
    for(int i=0;i<count;i++)
    {
        QWidget *widget=m_pContentVBoxLayout->itemAt(0)->widget();
        if(widget)
        {
            delete widget;
            widget=nullptr;
        }
    }
    return true;
}
void ToolPage::addWidget(const std::string &lang, const std::string &title, BaseWidget *widget)
{
    m_title=title;
    m_pPushButtonFold->setText(QString::fromStdString(loadTranslation(lang,title)));
    m_pContentVBoxLayout->addWidget(widget);
    connect(widget,&BaseWidget::signalSoundVolumn,[=](int volumn){
        emit signalSoundVolumn(volumn);
    });
    connect(widget,&BaseWidget::updateLanguage,[=](std::string lang){
        emit updateLanguage(lang);
    });
}
BaseWidget* ToolPage::getWidget()
{
     // 检查 m_pContentVBoxLayout 是否为空
    if (!m_pContentVBoxLayout) {
        qDebug() << "m_pContentVBoxLayout is null.";
        return nullptr;
    }
    int count=m_pContentVBoxLayout->count();
    if (count < 2) return nullptr;
    return qobject_cast<BaseWidget*>(m_pContentVBoxLayout->itemAt(1)->widget());
}
void ToolPage::expand()
{
    if (m_bIsExpanded) {
        return; // 已经展开，无需重复操作
    }

    if (!m_pContentVBoxLayout || !m_pLabel) {
        qWarning("ToolPage::expand: m_pContentVBoxLayout or m_pLabel is null");
        return;
    }
    if (m_pContentVBoxLayout->count() < 2) return ;
    QWidget* widget = m_pContentVBoxLayout->itemAt(1)->widget();
    if (!widget) {
        qWarning("ToolPage::expand: widget at index 1 is null");
        return;
    }

    widget->show();

    QString imagePath = QString::fromStdString(getPath("/resources/V1/@1xze-arrow-down 1.png"));
    if (imagePath.isEmpty()) {
        qWarning("ToolPage::expand: Invalid image path");
        return;
    }

    QPixmap pixmap(imagePath);
    if (pixmap.isNull()) {
        qWarning("ToolPage::expand: Failed to load image from path: %s", qPrintable(imagePath));
        return;
    }

    m_pLabel->setPixmap(pixmap.scaled(m_pLabel->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    m_bIsExpanded = true;
}
void ToolPage::setLanguage(std::string lang){
    m_pPushButtonFold->setText(QString::fromStdString(loadTranslation(lang,m_title)));
}
void ToolPage::collapse()
{
    // 空指针检查
    if (!m_pContentVBoxLayout || !m_pLabel) {
        qWarning() << "m_pContentVBoxLayout or m_pLabel is nullptr";
        return;
    }
    if (m_pContentVBoxLayout->count() < 2) return ;
    // 隐藏内容
    QWidget* widget = m_pContentVBoxLayout->itemAt(1)->widget();
    if (widget) {
        widget->hide();
    } else {
        qWarning() << "Widget at index 1 is nullptr";
    }

    // 设置折叠状态
    m_bIsExpanded = false;

    try {
        // 获取路径
        QString imagePath = QString::fromStdString(getPath("/resources/V1/@1xze-arrow-left 1.png"));
        if (imagePath.isEmpty()) {
            qWarning() << "ImagePath is empty";
            return;
        }

        // 缓存图片
        static QPixmap cachedPixmap;
        if (cachedPixmap.isNull()) {
            cachedPixmap = QPixmap(imagePath);
            if (cachedPixmap.isNull()) {
                throw std::runtime_error("Failed to load image");
            }
        }

        // 设置图片
        m_pLabel->setPixmap(cachedPixmap.scaled(m_pLabel->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    } catch (const std::exception& e) {
        qCritical() << "Exception occurred: " << e.what();
    }
}

void ToolPage::onPushButtonFoldClicked()
{
    if (m_bIsExpanded) {
        collapse();
    } else {
        expand();
    }
}
