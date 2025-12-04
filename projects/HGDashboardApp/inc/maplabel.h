#ifndef MAPLABEL_H
#define MAPLABEL_H

#include <QLabel>
#include <QWidget>
#include <QMap>
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>
#include <QGroupBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>

struct CityInfo {
    QPoint coord;     // 地图上的原始坐标
    int totalDevices;
    int deviceByTypeA; // 可扩展
};

class MapLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MapLabel(const int& dataType,QWidget* parent = nullptr);

signals:
    void cityClicked(const QString& name, const CityInfo& info);

private slots:
    void onInputDeviceUserButtonClicked();

protected:
    void resizeEvent(QResizeEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    void updateMap();
    QPoint mapToImageCoords(const QPoint& widgetPos) const;

    std::vector<std::map<std::string,std::string>> m_deviceMapInfo;

    QPixmap baseMap;
    double zoomFactor;

    double minLon = 73.5, maxLon = 135.0;  // 例如：中国的经度范围
    double minLat = 18.0, maxLat = 53.5;   // 例如：中国的纬度范围

    QTableWidget* m_tableW;

    QLabel* m_bkImgLabel;
    QGroupBox* m_inputDeviceUserGroup;
    QGridLayout* m_inputDeviceUserLayout, *m_layout;
    QLabel* m_inputDeviceUserLatitudeLabel, *m_inputDeviceUserLongitudeLabel;
    QLineEdit* m_inputDeviceUserLatitudeEdit, *m_inputDeviceUserLongitudeEdit;
    QLabel* m_inputDeviceCityLabel;
    QLineEdit* m_inputDeviceCityEdit;
    QPushButton* m_inputDeviceUserButton;
    QLabel* m_inputOperatorLabel;
    QLineEdit* m_inputOperatorEdit;

    int m_dataType;
};


#endif