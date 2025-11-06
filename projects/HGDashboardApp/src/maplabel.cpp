#include "maplabel.h"
#include <QToolTip>
#include <QDoubleValidator>
#include <QMessageBox>
#include "dashboardRWDB.h"
#include "HGExactTime.h"
#include <QHeaderView>
#include <QTextDocument>
#include <QDebug>

using namespace HGMACHINE;

MapLabel::MapLabel(const int& dataType,QWidget* parent)
    : QLabel(parent),
      zoomFactor(1.0),
      m_dataType(dataType)
{
    m_deviceMapInfo=DashboardRWDB::readDeviceMapInfo();
    setMouseTracking(true);
    m_layout=new QGridLayout();
    this->setLayout(m_layout);
    // setScaledContents(false);
    m_inputDeviceUserGroup=new QGroupBox("");
    m_inputDeviceUserGroup->setStyleSheet("QGroupBox { color: white; border: 2px solid yellow; font-size: 32px;}");
    m_inputDeviceUserLayout=new QGridLayout();
    m_inputDeviceUserGroup->setLayout(m_inputDeviceUserLayout);
    m_inputDeviceUserLatitudeLabel=new QLabel("纬度");
    m_inputDeviceUserLatitudeEdit=new QLineEdit();
    // 创建 QIntValidator 对象，设置最小值和最大值
    QDoubleValidator *validatorLongitude = new QDoubleValidator(minLon, maxLon, 4,this);  // 
    QDoubleValidator *validatorLatitude = new QDoubleValidator(minLat, maxLat, 4,this);

    m_inputDeviceUserLongitudeLabel=new QLabel("经度");
    m_inputDeviceUserLongitudeEdit=new QLineEdit();
    m_inputDeviceUserButton=new QPushButton("录入");
    m_inputDeviceCityLabel=new QLabel("城市");
    m_inputOperatorLabel=new QLabel("录入员");
    m_inputOperatorEdit=new QLineEdit();
    m_inputDeviceCityEdit=new QLineEdit();
    m_inputDeviceUserLatitudeEdit->setValidator(validatorLatitude);
    m_inputDeviceUserLongitudeEdit->setValidator(validatorLongitude);

    m_inputDeviceCityLabel->setStyleSheet("QLabel { color: white; font-size: 24px;}");
    m_inputDeviceUserLongitudeLabel->setStyleSheet("QLabel { color: white;font-size: 24px; }");
    m_inputDeviceUserLatitudeLabel->setStyleSheet("QLabel { color: white; font-size: 24px;}");
    m_inputOperatorLabel->setStyleSheet("QLabel { color: white; font-size: 24px;}");
    m_inputDeviceUserButton->setStyleSheet("QPushButton { color: white;  background-color: rgb(0,176,240);font-size: 24px;}");
    m_bkImgLabel=new QLabel();
    m_bkImgLabel->setScaledContents(false);

    QStringList headers=QStringList{"录入时间", "经度", "纬度", "城市", "录入员"};
    m_tableW=new QTableWidget(0,headers.size()); 
    m_tableW->setHorizontalHeaderLabels(headers); 
    // m_tableW->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_tableW->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableW->verticalHeader()->setVisible(false);
    // m_tableW->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // m_tableW->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    m_tableW->setMouseTracking(true);
    m_tableW->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color:rgb(0,176,240);color:rgb(0,0,0);}");
    connect(m_tableW, &QTableWidget::cellEntered, this, [this](int row, int column)
            {
                qDebug()<<"cellEntered, row:"<<row<<", column:"<<column;
    QTableWidgetItem *item = m_tableW->item(row, column);
    if (item) {
        QToolTip::showText(QCursor::pos(), item->text(), m_tableW);
    } });

    m_inputDeviceUserLayout->addWidget(m_inputDeviceUserLongitudeLabel,m_inputDeviceUserLayout->rowCount(),0);
    m_inputDeviceUserLayout->addWidget(m_inputDeviceUserLongitudeEdit,m_inputDeviceUserLayout->rowCount()-1,1);
    m_inputDeviceUserLayout->addWidget(m_inputDeviceUserLatitudeLabel,m_inputDeviceUserLayout->rowCount(),0);
    m_inputDeviceUserLayout->addWidget(m_inputDeviceUserLatitudeEdit,m_inputDeviceUserLayout->rowCount()-1,1);
    m_inputDeviceUserLayout->addWidget(m_inputDeviceCityLabel,m_inputDeviceUserLayout->rowCount(),0);
    m_inputDeviceUserLayout->addWidget(m_inputDeviceCityEdit,m_inputDeviceUserLayout->rowCount()-1,1);
    m_inputDeviceUserLayout->addWidget(m_inputOperatorLabel,m_inputDeviceUserLayout->rowCount(),0);
    m_inputDeviceUserLayout->addWidget(m_inputOperatorEdit,m_inputDeviceUserLayout->rowCount()-1,1);
    m_inputDeviceUserLayout->addWidget(m_inputDeviceUserButton,m_inputDeviceUserLayout->rowCount(),1);
    m_inputDeviceUserLayout->addWidget(m_tableW, m_inputDeviceUserLayout->rowCount(),0,1,2);

    m_layout->addWidget(m_inputDeviceUserGroup,0,0);
    m_layout->addWidget(m_bkImgLabel,0,1);

    connect(m_inputDeviceUserButton,&QPushButton::clicked,this,&MapLabel::onInputDeviceUserButtonClicked);
    updateMap();


}

void MapLabel::onInputDeviceUserButtonClicked()
{
    bool latOk, lonOk;
    double latitude = m_inputDeviceUserLatitudeEdit->text().toDouble(&latOk);
    double longitude = m_inputDeviceUserLongitudeEdit->text().toDouble(&lonOk);
    // 限制小数点后四位
    latitude = QString::number(latitude, 'f', 4).toDouble();
    longitude = QString::number(longitude, 'f', 4).toDouble();
    if (!lonOk || longitude < minLon || longitude > maxLon) {
        // 如果转换失败或值超出限制
        QMessageBox::warning(this, m_inputDeviceUserLongitudeLabel->text()+"输入无效", "请输入一个介于 "+QString::number(minLon)+" 和 "+QString::number(maxLon)+" 之间的浮点数!");
        return;
    } 
    if (!latOk || latitude < minLat || latitude > maxLat) {
        // 如果转换失败或值超出限制
        QMessageBox::warning(this, m_inputDeviceUserLatitudeLabel->text()+"输入无效", "请输入一个介于 "+QString::number(minLat)+" 和 "+QString::number(maxLat)+" 之间的浮点数!");
        return;
    } 
    if (m_inputOperatorEdit->text().isEmpty()){
        QMessageBox::warning(this, m_inputOperatorLabel->text()+"输入无效", "请输入录入员!");
        return;
    }
    if (latOk && lonOk) {
        // 将新的经纬度添加到 
        std::map<std::string,std::string> info{
            {"longtitude", std::to_string(longitude)},
            {"latitude", std::to_string(latitude)},
            {"inputTime", HGExactTime::currentTime().toStringFromYearToSec()},
            {"city", m_inputDeviceCityEdit->text().toStdString()},
            {"deviceType", std::to_string(m_dataType)},// 0--tuzhuang,1--diding,2--shuifen
            {"inputOperator", m_inputOperatorEdit->text().toStdString()},
            {"reserved1",""},
            {"reserved2",""},
            {"reserved3",""},
            {"reserved4",""}
        };

        DashboardRWDB::writeDeviceMapInfo(info);
        m_deviceMapInfo.push_back(info);

        // 更新地图，显示所有的标记
        updateMap();
    }
}
void MapLabel::resizeEvent(QResizeEvent* event)
{
    // QLabel::resizeEvent(event);
    // if (!baseMap.isNull() && width() > 0 && height() > 0)
    //     updateMap();
}

QPoint MapLabel::mapToImageCoords(const QPoint& widgetPos) const
{
    if (baseMap.isNull())
        return QPoint(-1, -1);

    QSize pixSize = baseMap.size() * zoomFactor;
    QSize lblSize = size();
    int offsetX = (lblSize.width() - pixSize.width()) / 2;
    int offsetY = (lblSize.height() - pixSize.height()) / 2;

    // printf("offset (%d,%d)\n",offsetX,offsetY);
    QPoint imgPos = (widgetPos - QPoint(offsetX, offsetY)) / zoomFactor;
    if (imgPos.x() < 0 || imgPos.y() < 0 || imgPos.x() >= baseMap.width() || imgPos.y() >= baseMap.height())
        return QPoint(-1, -1);

    return imgPos;
}

void MapLabel::mouseMoveEvent(QMouseEvent* event)
{
    if (baseMap.isNull()) {
        return;
    }

    // 获取鼠标在控件中的位置
    QPoint widgetPos = event->pos();
    QPoint imgPos = mapToImageCoords(widgetPos);  // 将控件位置映射到图片坐标

    // 如果鼠标位置超出地图范围，则不做任何处理
    if (imgPos.x() < 0 || imgPos.y() < 0 || imgPos.x() >= baseMap.width() || imgPos.y() >= baseMap.height()) {
        QToolTip::hideText();
        return;
    }

    // 根据图像的大小和经纬度的范围，计算鼠标位置对应的经纬度
    double longitude = minLon + (imgPos.x() / double(baseMap.width())) * (maxLon - minLon);
    double latitude = maxLat - (imgPos.y() / double(baseMap.height())) * (maxLat - minLat);

    // 创建提示文本显示鼠标对应的经纬度
    QString toolTipText = QString("经度: %1\n纬度: %2").arg(longitude, 0, 'f', 4).arg(latitude, 0, 'f', 4);
    
    // 显示提示框
    QToolTip::showText(event->globalPos(), toolTipText, this);
}


void MapLabel::mousePressEvent(QMouseEvent* event)
{
    if (baseMap.isNull()) {
        return;
    }

    // 获取鼠标在控件中的位置
    QPoint widgetPos = event->pos();
    QPoint imgPos = mapToImageCoords(widgetPos);  // 将控件位置映射到图片坐标

    // 如果鼠标位置超出地图范围，则不做任何处理
    if (imgPos.x() < 0 || imgPos.y() < 0 || imgPos.x() >= baseMap.width() || imgPos.y() >= baseMap.height()) {
        return;
    }

    // 根据图像的大小和经纬度的范围，计算鼠标点击位置对应的经纬度
    double longitude = minLon + (imgPos.x() / double(baseMap.width())) * (maxLon - minLon);
    double latitude = maxLat - (imgPos.y() / double(baseMap.height())) * (maxLat - minLat);

    // 显示经纬度信息的弹出框
    QString msg = QString("你点击的位置:\n经度: %1\n纬度: %2")
                  .arg(longitude, 0, 'f', 4)
                  .arg(latitude, 0, 'f', 4);

    QMessageBox::information(this, "当前位置", msg);
}

void MapLabel::wheelEvent(QWheelEvent* event)
{
    // if (baseMap.isNull())
    //     return;

    // if (event->angleDelta().y() > 0)
    //     zoomFactor *= 1.1;
    // else
    //     zoomFactor *= 0.9;

    // // 限制缩放范围
    // zoomFactor = qBound(0.5, zoomFactor, 5.0);

    // updateMap();
}

void MapLabel::updateMap()
{
    // Cache image paths to avoid repeated computation
    static const QString imgPath = qApp->applicationDirPath() + "/LOGO/ChinaMap.png";
    static const QString pinPath = qApp->applicationDirPath() + "/LOGO/pin.png";

    if (baseMap.isNull()) {
        baseMap = QPixmap(imgPath);
        if (baseMap.isNull()) {
            printf("baseMap is null\n");
            return;
        }
    }

    // 计算缩放后的目标大小
    QSize targetSize = baseMap.size() * zoomFactor;
    QPixmap scaledMap = baseMap.scaled(targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QPainter painter(&scaledMap);
    if (!painter.isActive()) {
        printf("Failed to activate painter\n");
        return;
    }

    // 载入图钉图标
    QPixmap pinIcon(pinPath);
    if (pinIcon.isNull()) {
        printf("pin icon not found! Using fallback red dot.\n");
    }

    // 计算原始地图的宽高
    double mapWidth = baseMap.width();
    double mapHeight = baseMap.height();

    m_tableW->clearContents();
    m_tableW->setRowCount(0);
    int row = m_tableW->rowCount();

    // 对于每个坐标点，计算缩放前的像素坐标
    for (auto deviceInfo : m_deviceMapInfo) {
        if (deviceInfo.count("deviceType") == 0 || 
            deviceInfo.at("deviceType") != std::to_string(m_dataType)) {
            continue;
        }

        m_tableW->insertRow(row);
        auto setItemWithText = [&](int col, const std::string& key) {
            QString value = QString::fromStdString(deviceInfo.count(key) ? deviceInfo.at(key) : "");

            // 将 \n 替换为 <br> 进行换行
            // value = "<html><body>" + value.replace("\n", "<br>") + "</body></html>";

            // 使用 QLabel 显示 HTML 内容
            QLabel *label = new QLabel();
            label->setText(value);  // 设置 HTML 内容
            // label->setTextFormat(Qt::RichText);  // 设置文本格式为富文本
            label->setWordWrap(true);
            label->setAlignment(Qt::AlignTop | Qt::AlignLeft);  // 设置对齐方式

            // 将 QLabel 设置到表格的单元格中
            m_tableW->setCellWidget(row, col, label);
        };
        setItemWithText(0, "inputTime");
        setItemWithText(1, "longtitude");
        setItemWithText(2, "latitude");
        setItemWithText(3, "city");
        setItemWithText(4, "inputOperator");

        bool lonOk, latOk;
        double longitude = QString::fromStdString(deviceInfo.at("longtitude")).toDouble(&lonOk);
        double latitude = QString::fromStdString(deviceInfo.at("latitude")).toDouble(&latOk);

        if (!lonOk || !latOk) {
            printf("Invalid coordinate data for device\n");
            row++;
            continue;
        }

        // 将经纬度映射到原始地图尺寸上的像素坐标
        int x = static_cast<int>((longitude - minLon) / (maxLon - minLon) * mapWidth);
        int y = static_cast<int>((maxLat - latitude) / (maxLat - minLat) * mapHeight);

        QPoint originalCoord(x, y);

        // 根据缩放比例调整图钉位置
        int scaledX = originalCoord.x() * zoomFactor;
        int scaledY = originalCoord.y() * zoomFactor;

        QPoint scaledCoord(scaledX, scaledY);

        if (!pinIcon.isNull()) {
            // 让图钉的尖端对准坐标点
            QPoint pinTopLeft = scaledCoord - QPoint(pinIcon.width() / 2, pinIcon.height());
            painter.drawPixmap(pinTopLeft, pinIcon);
        } else {
            // 如果没找到图标，fallback 用红点
            painter.setBrush(Qt::red);
            painter.setPen(Qt::NoPen);
            painter.drawEllipse(scaledCoord, 5, 5);
        }
        row++;
    }

    m_bkImgLabel->setPixmap(scaledMap);

    // 自动调整行高和列宽
    m_tableW->resizeRowsToContents();
    m_tableW->resizeColumnsToContents();
}

