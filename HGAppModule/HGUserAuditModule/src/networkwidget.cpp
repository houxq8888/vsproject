#include "networkwidget.h"
#include "common.h"
#include <QDebug>
#include <iostream>


NetworkWidget::NetworkWidget(std::string lang,QWidget *parent) : BaseWidget(parent),
    m_isEnableWifi(true),
    m_listWidget(NULL),
    m_lang(lang)
{
    m_layout=new QGridLayout();
    this->setLayout(m_layout);
    
    m_wifiLabel=new HGQLabel(false,getPath("/resources/V1/@1xiconPark-wifi 1.png"));
    m_wifiImg=new LabelWithImg(IMGRIGHT,12,getPath("/resources/V1/@1xIOS开关_enable.png"),"WLAN");
    connect(m_wifiImg,SIGNAL(clickImgLabel()),this,SLOT(clickEnableWifi()));

// 已连接安全
// 断开连接
// 连接
    m_listWidget=new QListWidget();
    m_listWidget->setResizeMode(QListWidget::Adjust); // 允许调整大小
    m_listWidget->setUniformItemSizes(false);        // 允许不同 Item 高度
    m_layout->addWidget(m_wifiLabel,0,0,1,1);
    m_layout->addWidget(m_wifiImg,0,1,1,1);
    m_layout->addWidget(m_listWidget,1,0,1,2);
    fnReadDB();
    
    connect(m_listWidget, SIGNAL(itemPressed(QListWidgetItem*)),this,SLOT(clickListItem(QListWidgetItem*)));
}

NetworkWidget::~NetworkWidget(){
    closeWindow();
}
bool NetworkWidget::closeWindow()
{
    fnWriteDB();
    fnDestroyWifiWidget();
    return true;
}
void NetworkWidget::fnInitWifiInfo(){
    std::vector<WifiNetwork> networks = scanWifiNetworks();
    std::map<std::string,int> wifiNames;
    // 打印出网络列表
    for (const auto& network : networks) {
        // std::cout << "SSID: " << network.ssid
        //           << ", Signal Strength: " << network.signal_strength
        //           << ", Security: " << network.security << std::endl;
        if (network.ssid=="") continue;
        if (wifiNames.find(network.ssid)!=wifiNames.end()) continue;

        wifiNames[network.ssid]++;

        // 创建自定义 QWidget
        m_wifiWs[network.ssid].wifiW = new WifiWidget(m_lang,network);

        // 创建 QListWidgetItem
        m_wifiWs[network.ssid].wifiItem = new QListWidgetItem();
        m_wifiWs[network.ssid].wifiItem->setToolTip(QString::fromStdString(network.ssid));
        // 设置 item 的大小（可选）
        m_wifiWs[network.ssid].wifiItem->setSizeHint(m_wifiWs[network.ssid].wifiW->sizeHint());
        // 将 item 添加到 QListWidget
        m_listWidget->addItem(m_wifiWs[network.ssid].wifiItem);

        // 将自定义 QWidget 设置为 item 的 widget
        m_listWidget->setItemWidget(m_wifiWs[network.ssid].wifiItem, m_wifiWs[network.ssid].wifiW);
    }
}
void NetworkWidget::fnReadDB()
{
    m_isEnableWifi=(GlobalSingleton::instance().getSystemInfo("WLAN")=="true"?true:false);
    setControlStatus();
}
void NetworkWidget::fnWriteDB()
{
    GlobalSingleton::instance().setSystemInfo("WLAN", m_isEnableWifi?"true":"false");
}
void NetworkWidget::setControlStatus(){
    if (!m_isEnableWifi){
        m_wifiImg->setImg(getPath("/resources/V1/@1xIOS开关.png"));
        fnDestroyWifiWidget();
        m_listWidget->clear();
    } else {
        m_wifiImg->setImg(getPath("/resources/V1/@1xIOS开关_enable.png"));
        fnInitWifiInfo();
    }
}
void NetworkWidget::fnDestroyWifiWidget()
{
    for (auto wifi : m_wifiWs)
    {
        if (m_wifiWs[wifi.first].wifiW != NULL)
        {
            if (m_wifiWs[wifi.first].wifiW->closeWindow())
                SAFE_DELETE(m_wifiWs[wifi.first].wifiW);
        }
        SAFE_DELETE(m_wifiWs[wifi.first].wifiItem);
    }
    m_wifiWs.clear();
}
void NetworkWidget::clickEnableWifi()
{
    m_isEnableWifi=!m_isEnableWifi;
    RWDb::writeAuditTrailLog(m_isEnableWifi?("打开"+m_wifiLabel->text().toStdString()): \
        "关闭"+m_wifiLabel->text().toStdString());
    setControlStatus();
}
void NetworkWidget::clickListItem(QListWidgetItem *item)
{
    // 检查 currentItem 是否为空
    if (!m_listWidget->currentItem())
    {
        qDebug() << "当前选中项为空";
        return;
    }

    // 获取当前选中项的文本
    QString textQt = m_listWidget->currentItem()->toolTip();
    // qDebug() << textQt;
    std::string text = textQt.toStdString();

    // // 确保 m_wifiWs 的访问是线程安全的（假设 m_wifiWsMutex 是一个互斥锁）
    // std::lock_guard<std::mutex> lock(m_wifiWsMutex);

    // 检查 m_wifiWs 中是否存在对应的 key
    auto it = m_wifiWs.find(text);
    if (it != m_wifiWs.end())
    {
        it->second.wifiW->changeStatus();
    }
    else
    {
        // 处理不存在的情况，例如记录日志
        qDebug() << "Key not found: " << textQt;
    }
}
void NetworkWidget::setLanguage(std::string lang){
    m_lang=lang;


}