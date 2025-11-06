#include "HGDashboardWidget.h"
#include <QDebug>
#include <QFileDialog>
#include "hglog4cplus.h"
#include "hgcommonutility.h"
#include <QDesktopWidget>
#include "hgcomwithmqtt.h"
#include "dashboardRWDB.h"
#include <curl/curl.h>
#include <QApplication>
#include <iomanip>
#include <set>
#include <iostream>
#include <string>
#include <regex>
#include <sstream>
#include "systemusage.h"
// #include <QWebEngineView>
// #include <QWebEngineSettings>
#include "maplabel.h"
//#include <QQuickWidget>
//#include <QQmlContext>
#include "opencv2/opencv.hpp"

#define CAN_IP_TUZHUANG "192.168.4.101"
#define CAN_PORT_TUZHUANG 8881
#define CAN_IP_DIDING "192.168.4.102"
#define CAN_PORT_DIDING 8883
#define CAN_IP_SHUIFEN "192.168.4.103"
#define CAN_PORT_SHUIFEN 8882


struct THData {
    float temperature;
    float humidity;
};
struct StData {
    std::string group;   // 组合号
    std::string code;    // 代号
    std::string subCode; // 子项（如果有）
    // 用于去重比较
    bool operator<(const StData &other) const {
        return std::tie(group, code, subCode) < std::tie(other.group, other.code, other.subCode);
    }
};

// 返回是否成功解析
bool parseTH(const std::string &data, THData &out) {
    // 找到 "th=" 和 "#" 的位置
    std::size_t posStart = data.find("th=");
    std::size_t posEnd = data.find('#', posStart);

    // 检查 "th=" 和 "#" 是否有效
    if (posStart == std::string::npos || posEnd == std::string::npos || posEnd <= posStart + 3) {
        printf("decode TH failed: invalid format or no valid data between 'th=' and '#'\n");
        HGLog4Cplus::getLogInstance(LOG_PATH)->logout("decode TH failed: invalid format or no valid data between 'th=' and '#'\n",LOGERROR);
        return false;
    }

    // 提取 "th=" 后和 "#" 之前的字符串
    std::string values = data.substr(posStart + 3, posEnd - posStart - 3);
    std::stringstream ss(values);
    std::string tempStr;
    std::vector<float> numbers;

    // 逐个读取分隔符为逗号的值并转换为 float
    while (std::getline(ss, tempStr, ',')) {
        try {
            printf("th: %s\n", tempStr.c_str());
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout("decode TH: " + tempStr,LOGINFO);
            numbers.push_back(std::stof(tempStr)); // 转换为 float
        } catch (const std::invalid_argument& e) {
            std::ostringstream errorMsg;
            errorMsg << "decode stof TH failed: " << e.what();
            printf("decode stof TH failed: %s\n", e.what());
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout(errorMsg.str(),LOGERROR);
            return false; // 转换失败
        } catch (const std::out_of_range& e) {
            printf("decode stof TH failed: out of range\n");
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout("decode stof TH failed: out of range",LOGERROR);
            return false; // 转换失败
        }
    }

    // 确保我们解析到了两个数值
    if (numbers.size() != 2) {
        printf("decode TH failed: expected 2 values, got %zu\n", numbers.size());
        HGLog4Cplus::getLogInstance(LOG_PATH)->logout("decode TH failed: expected 2 values, got " + std::to_string(numbers.size()),LOGERROR);
        return false;
    }

    // 填充输出数据
    out.temperature = numbers[0];
    out.humidity = numbers[1];
    return true;
}

bool parseLastC(const std::string &data, std::string &outC) {
    std::size_t posStart = data.find("cha=");
    std::size_t posEnd = data.find('#');

    if (posStart == std::string::npos || posEnd == std::string::npos || posEnd <= posStart + 4) {
        return false; // 格式不对
    }

    // 获取 x,y,a,b,c 字符串
    std::string values = data.substr(posStart + 4, posEnd - posStart - 4);
    std::stringstream ss(values);
    std::string temp;
    std::string lastValue;

    // 分割逗号，取最后一个
    while (std::getline(ss, temp, ',')) {
        lastValue = temp;
    }

    if (lastValue.empty()) return false;

    outC = lastValue;
    return true;
}

// 解析单个 st=...#
bool parseSingleSt(const std::string &segment, StData &result) {
    if (segment.find("st=") != 0 || segment.back() != '#') {
        return false;
    }

    // 去掉 "st=" 和 "#"
    std::string content = segment.substr(3, segment.size() - 4);

    // 按 '*' 拆分
    std::vector<std::string> parts;
    size_t start = 0, pos = 0;
    while ((pos = content.find('*', start)) != std::string::npos) {
        parts.push_back(content.substr(start, pos - start));
        start = pos + 1;
    }
    parts.push_back(content.substr(start));

    if (parts.size() < 2) return false;

    result.group = parts[0];
    result.code  = parts[1];
    result.subCode = (parts.size() > 2) ? parts[2] : "";

    return true;
}

// 从一个大字符串中提取所有 st=...# 并去重
std::vector<StData> parseAllSt(const std::string &input) {
    std::set<StData> uniqueSet;
    std::vector<StData> results;

    size_t pos = 0;
    while (true) {
        size_t start = input.find("st=", pos);
        if (start == std::string::npos) break;

        size_t end = input.find('#', start);
        if (end == std::string::npos) break;

        std::string segment = input.substr(start, end - start + 1);

        StData data;
        if (parseSingleSt(segment, data)) {
            // 插入 set，确保不重复
            if (uniqueSet.insert(data).second) {
                results.push_back(data);
            }
        }

        pos = end + 1;
    }

    return results;
}



using namespace HGMACHINE;

// 回调函数：将返回的数据写入 std::string
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    std::string* buffer = static_cast<std::string*>(userp);
    buffer->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}
void adjustLabelFont(QLabel *label) {
    QFont font = label->font();
    int fontSize = font.pointSize();

    QFontMetrics fm(font);
    QString text = label->text();

    while ((fm.boundingRect(label->rect(), Qt::TextWordWrap, text).height() > label->height()) && fontSize > 1) {
        fontSize--;
        font.setPointSize(fontSize);
        label->setFont(font);
        fm = QFontMetrics(font);
    }
}

QRect getScreenGeometry()
{
    QDesktopWidget* desktopWidget=QApplication::desktop();
    QRect applicationRect=desktopWidget->screenGeometry();
    return applicationRect;
}

HGDashboardWidget::HGDashboardWidget(QWidget *parent)
    : QWidget(parent)
{
    m_playVideoFlag=false;
    m_weatherStatus="";
    m_temperature="";
    m_dataType=TUZHUANG;
    m_funcType=REAL_DATA;
    m_lastFuncType=REAL_DATA;

    m_Alkalinity1Range.min=0;
    m_Alkalinity1Range.max=14;
    m_didingChannel1Range.min=0;
    m_didingChannel1Range.max=14;
    m_didingChannel2Range.min=0;
    m_didingChannel2Range.max=40;
    m_didingChannel3Range.min=0;
    m_didingChannel3Range.max=14;
    m_didingChannel4Range.min=0;
    m_didingChannel4Range.max=40;
    m_shuifenChannel1Range.min=0;
    m_shuifenChannel1Range.max=99999;
    m_shuifenChannel2Range.min=0;
    m_shuifenChannel2Range.max=99999;
    m_Alkalinity2Range.min=0;
    m_Alkalinity2Range.max=14;
    m_FreeAcidityRange.min=0.6;
    m_FreeAcidityRange.max=1.5;
    m_TotalAcidityRange.min=0;
    m_TotalAcidityRange.max=40;
    m_ElectricalConductivity1Range.min=0;
    m_ElectricalConductivity1Range.max=2000;
    m_ElectricalConductivity2Range.min=0;
    m_ElectricalConductivity2Range.max=14;
    m_vlRange.min=0.01;
    m_vlRange.max=999.99;
    std::ostringstream logtext;
    std::vector<std::map<std::string,std::string>> ipLists=getWirelessIP(); // 获取 wlo1 的 IP
    std::string ip="192.168.1.131";
    for (int i=0;i<int(ipLists.size());i++){
        for (const auto &kv : ipLists[i]) {
            logtext<<"face: "<<kv.first<<" : "<<kv.second<<std::endl;
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout(logtext.str(),LOGINFO);
            std::cout<<"face: "<<kv.first<<" : "<<kv.second<<std::endl;
            if (kv.first == "ens33" || kv.first == "wlo1") ip=kv.second;
        }
    }
    logtext.str("");
    logtext<<"ip: "<<ip;
    HGLog4Cplus::getLogInstance(LOG_PATH)->logout(logtext.str(),LOGINFO);
    std::cout<<"ip: "<<ip<<std::endl;
    // m_mqttclient=new HgComWithLibMQTT(ip,"jwp9Gz8CQhr4DLiCPURT",1883);
    m_mqttclient=new HgComWithLibMQTT("5.tcp.cpolar.cn","jwp9Gz8CQhr4DLiCPURT",14519);
    m_mqttclient->start();

    m_socketServer = new HgComWithSocket(SOCKET_Mode::SERVER, CAN_IP_TUZHUANG, CAN_PORT_TUZHUANG);
    m_socketServerDiding = new HgComWithSocket(SOCKET_Mode::SERVER, CAN_IP_DIDING, CAN_PORT_DIDING);
    m_socketServerShuifen = new HgComWithSocket(SOCKET_Mode::SERVER, CAN_IP_SHUIFEN, CAN_PORT_SHUIFEN);

    DashboardRWDB::openDB();

    m_width = getScreenGeometry().width();
    m_height = getScreenGeometry().height();
    font.setBold(true);
    font.setPointSize(m_width*20/1800);
    qDebug()<<m_width<<","<<m_height;
    this->setFixedSize(m_width,m_height);
    this->setWindowFlag(Qt::Window);
    m_basePath = qApp->applicationDirPath().toStdString();
    std::string imgPath=m_basePath+"/LOGO/涂装.png";
    // 创建图像控件
    m_bkLabel = new QLabel();
    m_bkLabel->setStyleSheet(        
        "border-top: 2px solid yellow;"
        "border-left: 2px solid yellow;"
        "border-right: 2px solid yellow;"
        "border-bottom: none;");
    m_bkLabel->setFixedSize(m_width*0.96,m_height*0.135);
    // 加载并显示图像
    QPixmap pixmap(QString::fromStdString(imgPath)); // 使用资源文件路径

    // 设置图像到Label
    m_bkLabel->setPixmap(pixmap);

    // 可选：保持图像比例
    m_bkLabel->setScaledContents(true);
    m_bkLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    double cpuusage = SystemUsage::getCpuUsage();
    long memoryusage = SystemUsage::getMemoryUsage();
    double networkUsage = SystemUsage::getNetworkUsage();
    double diskIO = SystemUsage::getDiskIO();

    m_titleLabel=new QLabel(QString::fromStdString("CPU:"+std::to_string(cpuusage) + 
        " % ,Memory:" + std::to_string(memoryusage) + " MB, Network(kb/s):" + std::to_string(networkUsage) +",Disk IO(kb/s):"+std::to_string(diskIO)));
    m_titleLabel->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    m_titleLabel->setStyleSheet("QLabel{color:rgb(205,221,230);font-size:30px;font-weight:bold;font-family:黑体;}");
    HGExactTime currentTime = HGExactTime::currentTime();
    std::ostringstream timestr;
    timestr << currentTime.tm_year << "-" << currentTime.tm_mon << "-" << currentTime.tm_mday;
    m_timeLabel=new QLabel(QString::fromStdString(timestr.str()));
    m_timeLabel->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    m_timeLabel->setStyleSheet("QLabel{color:white;font-weight:bold;font-family:黑体;}");
    m_timeLabel->setFont(font);

    timestr.str("");
    timestr << "XX °C, XX %";
    m_rightTimeLabel=new QLabel(QString::fromStdString(timestr.str()));
    m_rightTimeLabel->setAlignment(Qt::AlignCenter);
    m_rightTimeLabel->setStyleSheet("QLabel{color:white;font-weight:bold;font-family:黑体;}");
    m_rightTimeLabel->setFont(font);

    for (int i=0;i<3;i++){
        // m_deviceStatusLabel[i]=new QLabel("deviceStatus"+QString::number(i));
        m_deviceStatusLabel[i]=new QLabel("");
        m_deviceStatusLabel[i]->setAlignment(Qt::AlignLeft);
        m_deviceStatusLabel[i]->setStyleSheet("QLabel{color:white;font-weight:bold;font-family:黑体;}"); // border: 10px solid white;
        m_deviceStatusLabel[i]->setFont(font);
        m_deviceStatusLabel[i]->setFixedSize(m_width * 0.5,m_height*0.08);
        m_deviceStatusLabel[i]->setWordWrap(true);
        // m_deviceStatusLabel->setText("仪器状态 仪器状态 仪器状态 仪器状态 仪器状态 仪器状态 仪器状态 仪器状态 仪器状态 仪器状态 仪器状态 仪器状态 仪器状态 仪器状态 仪器状态 仪器状态 仪器状态 仪器状态 仪器状态 仪器状态 仪器状态 仪器状态 仪器状态 仪器状态 仪器状态 仪器状态 仪器状态 仪器状态 仪器状态 仪器状态 仪器状态 仪器状态 仪器状态 ");
        adjustLabelFont(m_deviceStatusLabel[i]);
    }

    QFont font1;
    font1.setPointSize(m_width*18/1800);
    for (int i=0;i<3;i++){
        m_errStatusLabel[i]=new QLabel(this);
        // m_errStatusLabel[i]->setText("errStatus"+QString::number(i));
        m_errStatusLabel[i]->setText("");
        m_errStatusLabel[i]->setAlignment(Qt::AlignLeft);
        m_errStatusLabel[i]->setStyleSheet("QLabel{color:red;font-family:黑体;}");
        m_errStatusLabel[i]->setFont(font1);
    }
    m_tuzhuangLabel=new QLabel();
    m_tuzhuangLabel->setFixedSize(m_height * 0.09,m_height*0.09);
    m_tuzhuangLabel->setPixmap(QPixmap(QString::fromStdString(m_basePath+"/LOGO/红-涂装.png")));
    m_tuzhuangLabel->setScaledContents(true);
    m_tuzhuangLabel->installEventFilter(this);

    m_didingLabel=new QLabel();
    m_didingLabel->setFixedSize(m_height * 0.09,m_height*0.09);
    m_didingLabel->setPixmap(QPixmap(QString::fromStdString(m_basePath+"/LOGO/蓝-滴定.png")));
    m_didingLabel->setScaledContents(true);
    m_didingLabel->installEventFilter(this);

    m_shuifenLabel=new QLabel();
    m_shuifenLabel->setFixedSize(m_height * 0.09,m_height*0.09);
    m_shuifenLabel->setPixmap(QPixmap(QString::fromStdString(m_basePath+"/LOGO/蓝-水分.png")));
    m_shuifenLabel->setScaledContents(true);
    m_shuifenLabel->installEventFilter(this);

    m_realDataLabel=new QLabel(this);
    m_realDataLabel->setFixedSize(m_width * 0.2,m_height*0.22);
    m_realDataLabel->setPixmap(QPixmap(QString::fromStdString(m_basePath+"/LOGO/红-实时数据.png")).scaled(m_realDataLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    m_realDataLabel->installEventFilter(this);

    m_maintenanceLabel=new QLabel(this);
    m_maintenanceLabel->setFixedSize(m_width * 0.2,m_height*0.22);
    m_maintenanceLabel->setPixmap(QPixmap(QString::fromStdString(m_basePath+"/LOGO/蓝-维护保养.png")).scaled(m_maintenanceLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    m_maintenanceLabel->installEventFilter(this);

    m_mapLabel=new QLabel(this);
    m_mapLabel->setFixedSize(m_width * 0.2,m_height*0.22);
    m_mapLabel->setPixmap(QPixmap(QString::fromStdString(m_basePath+"/LOGO/蓝-用户分布.png")).scaled(m_mapLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    m_mapLabel->installEventFilter(this);

    m_monitorLabel=new QLabel(this);
    m_monitorLabel->setFixedSize(m_width * 0.2,m_height*0.22);
    m_monitorLabel->setPixmap(QPixmap(QString::fromStdString(m_basePath+"/LOGO/蓝-现场监控.png")).scaled(m_monitorLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    m_monitorLabel->installEventFilter(this);

    std::vector<std::string> firstColumnNames={"通道1","通道2","电导率","PH","加药量",}; // {"脱脂1","脱脂2","总酸度","游离酸","电导率1","电导率2"};
    std::vector<std::string> secondColumnNames={"0-14","0-40","≤2000","0-14","0.01-999.99"} ;//{"12-14","12-14","25-40","0.6-1.5","≤500","≤500"};

    QStringList headers=QStringList{"参数名称", "工艺范围", "实时值", "状态", "检测时间"};
    for (int i=0;i<3;i++){
        m_tableW[i]=new QTableWidget(int(firstColumnNames.size()),headers.size()); 
        m_tableW[i]->setHorizontalHeaderLabels(headers); 
        m_tableW[i]->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        m_tableW[i]->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        m_tableW[i]->verticalHeader()->setVisible(false);
        m_tableW[i]->horizontalHeader()->setVisible(true);
        m_tableW[i]->setShowGrid(false);
        m_tableW[i]->setEditTriggers(QAbstractItemView::NoEditTriggers);
        m_tableW[i]->setSelectionMode(QAbstractItemView::NoSelection);
        m_tableW[i]->setFocusPolicy(Qt::NoFocus);
        m_tableW[i]->horizontalHeader()->setStyleSheet("QHeaderView::section{color:rgb(0,176,240);}");
        m_tableW[i]->setFont(font);
        m_tableW[i]->horizontalHeader()->setFont(font);
        m_tableW[i]->setFixedSize(m_width * 0.9,m_height*0.3);

        // 设置背景透明
        m_tableW[i]->setStyleSheet("QTableWidget { background: transparent; }"
                         "QHeaderView::section { background-color: transparent; }");
        for (int j = 0; j < m_tableW[i]->rowCount(); ++j) {
            m_tableW[i]->setRowHeight(j, m_height*0.1);
        }
        int total = 4*1 + 8;  // 总权重 = 4
        int tableWidth = m_tableW[i]->viewport()->width();

        m_tableW[i]->setColumnWidth(0, tableWidth * 1 / total);
        m_tableW[i]->setColumnWidth(1, tableWidth * 1 / total);
        m_tableW[i]->setColumnWidth(2, tableWidth * 1 / total);
        m_tableW[i]->setColumnWidth(3, tableWidth * 1 / total);
        m_tableW[i]->setColumnWidth(4, tableWidth * 8 / total);

        if (i==0){
            firstColumnNames={"通道1","通道2","电导率","PH","加药量"}; // {"脱脂1","脱脂2","总酸度","游离酸","电导率1","电导率2"};
            secondColumnNames={"0-14","0-40","≤2000","0-14","0.01-999.99"} ;//{"12-14","12-14","25-40","0.6-1.5","≤500","≤500"};
        } else if (i==1){
            firstColumnNames={"通道1-pH","通道1-总胺","通道2-pH","通道2-总胺"/*,"加药量"*/}; 
            secondColumnNames={"0-14","0-40","0-14","0-40"/*,"0.01-999.99"*/} ;
        } else if (i==2){
            firstColumnNames={"通道1","通道2"/*,"加药量"*/}; 
            secondColumnNames={"0-99999","0-99999"/*,"0.01-999.99"*/} ;
        }
        if (firstColumnNames.size() > 0 && secondColumnNames.size() > 0) 
            fillTableData(i,firstColumnNames,secondColumnNames);
    }

    // 设置窗口背景颜色
    QPalette palette = this->palette();
    palette.setColor(QPalette::Background, QColor(0,50,126));  // 设置为 RGB 颜色
    this->setAutoFillBackground(true);  // 自动填充背景
    this->setPalette(palette);

    // 1. 创建一个内容区 QWidget（承载你已有的所有控件）
    m_mainContent = new QWidget(this);
    m_mainContent->setObjectName("mainContent");
    m_mainContent->setStyleSheet("#mainContent { border: 2px solid yellow;background-color: rgb(8,11,56); }");
    m_mainContent->setFixedSize(m_width*0.96,m_height*0.94);
    // 2. 原来的布局绑定到这个内容区，而不是 this
    m_gridLayout = new QGridLayout(m_mainContent);
    m_gridLayout->setContentsMargins(0, 0, 0, 0); // 内部控件不要留边
    // ....后续 addWidget 全部绑定 m_gridLayout 不变

    for (int i=0;i<VIDEO_THREAD_NUM+1;i++){
        m_watchLabel[i] = new QLabel(/*"watchLabel"+QString::number(i)*/);
        m_watchLabel[i]->setStyleSheet(
            "QLabel {"
            "   color: white;"
            "   font-family: 黑体;"
            "   background-color: rgba(0,0,0,0);" // 可选，半透明背景
            "   border: none; margin: 10px;"
            "}"
        );// 设置样式表以添加 margin
        m_watchLabel[i]->setFixedSize(m_width*0.15,m_height*0.15);
        m_watchLabel[i]->setScaledContents(true);
    }

    m_mainContent->move(m_width*0.02,m_height*0.02);
    m_errStatusLabel[TUZHUANG]->move(m_width*0.04,m_height*0.96);
    m_errStatusLabel[DIDING]->move(m_width*0.04,m_height*0.96);
    m_errStatusLabel[SHUIFEN]->move(m_width*0.04,m_height*0.96);
    m_errStatusLabel[DIDING]->hide();
    m_errStatusLabel[SHUIFEN]->hide();
    m_realDataLabel->move(m_width*0.95,m_height*0.05);
    m_maintenanceLabel->move(m_width*0.95,m_height*0.2);
    m_mapLabel->move(m_width*0.95,m_height*0.35);
    m_monitorLabel->move(m_width*0.95,m_height*0.5);

    m_timeLabel->setParent(m_mainContent);
    m_rightTimeLabel->setParent(m_mainContent);
    m_tuzhuangLabel->setParent(m_mainContent);
    m_didingLabel->setParent(m_mainContent);
    m_shuifenLabel->setParent(m_mainContent);
    m_watchLabel[TUZHUANG]->setParent(m_mainContent);

    m_titleLabel->setParent(this);

    m_hor1=new QFrame();
    m_hor1->setFrameShape(QFrame::HLine);
    m_hor1->setFrameShadow(QFrame::Sunken);
    m_hor2=new QFrame();
    m_hor2->setFrameShape(QFrame::HLine);
    m_hor2->setFrameShadow(QFrame::Sunken);

    m_ElectricalConductivity1Bar=new QRoundProgressBar("电导率");
    m_ElectricalConductivity2Bar=new QRoundProgressBar("pH");

    m_Alkalinity1Curve=new HGCurDataWidget(12,"通道1"); // 脱脂1");
    m_Alkalinity2Curve=new HGCurDataWidget(12,"通道2"); // 脱脂2");
    m_TotalAcidityCurve=new HGCurDataWidget(12,"电导率"); // 总酸度");
    m_FreeAcidityCurve=new HGCurDataWidget(12,"pH"); // 游离酸");
    m_didingChannel1Curve=new HGCurDataWidget(12,"通道1-pH");
    m_didingChannel2Curve=new HGCurDataWidget(12,"通道1-总胺");
    m_didingChannel3Curve=new HGCurDataWidget(12,"通道2-pH");
    m_didingChannel4Curve=new HGCurDataWidget(12,"通道2-总胺");
    m_shuifenChannel1Curve=new HGCurDataWidget(24,"通道1");
    m_shuifenChannel2Curve=new HGCurDataWidget(24,"通道2");


    m_gridLayout->addWidget(m_bkLabel,0,0,1,5);
    m_tuzhuangLabel->move(m_width*0.76,m_height*0.038);
    m_tuzhuangLabel->raise();
    m_didingLabel->move(m_width*0.808,m_height*0.038);
    m_didingLabel->raise();
    m_shuifenLabel->move(m_width*0.855,m_height*0.038);
    m_shuifenLabel->raise();
    m_timeLabel->move(m_width* 0.12,-m_height*0.13);
    m_timeLabel->raise();
    m_rightTimeLabel->move(m_width*0.02,-m_height*0.16);
    m_rightTimeLabel->raise();
    m_watchLabel[TUZHUANG]->move(m_width*0.755,m_height*0.138);
    m_watchLabel[TUZHUANG]->raise();

    m_titleLabel->move(m_width*0.02,m_height*0.01);
    m_titleLabel->raise();
    // m_tuzhuangLabel->setVisible(false);
    // m_didingLabel->setVisible(false);
    // m_shuifenLabel->setVisible(false);

    displayRealData();

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(fnSlotListenTimerOut()));
    m_timer->start(1000);

    m_warnTimer=new QTimer(this);
    connect(m_warnTimer, SIGNAL(timeout()), this, SLOT(fnSlotListenWarnTimerOut()));
    m_warnTimer->start(2000);

    m_systemusageTimer=new QTimer(this);
    connect(m_systemusageTimer, SIGNAL(timeout()), this, SLOT(fnSlotListenSystemUsageTimerOut()));
    m_systemusageTimer->start(1000);

    fnSlotListenTimerOut();

    // tuzhuang
    std::thread([this]
                {
        if (m_socketServer->startServer())
        {
            std::ostringstream logtext;
            logtext << "socket tuzhuang start error\n";
            printf(logtext.str().c_str());
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout(logtext.str(),LOGERROR);
        } 
    }).detach();
    // diding
    std::thread([this]
    {
        if (m_socketServerDiding->startServer()){
            std::ostringstream logtext;
            logtext << "socket diding start error\n";
            printf(logtext.str().c_str());
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout(logtext.str(),LOGERROR);
        }
    }).detach();
    // shuifen
    std::thread([this]
    {
        if (m_socketServerShuifen->startServer()){
            std::ostringstream logtext;
            logtext << "socket shuifen start error\n";
            printf(logtext.str().c_str());
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout(logtext.str(),LOGERROR);
        }
    }).detach();

    // 连接信号更新 GUI
    connect(this, SIGNAL(signalNewFrame(const QImage&,int)),this, SLOT(onNewFrame(const QImage &,int)));

    for(int i=0;i<VIDEO_THREAD_NUM;i++){
        m_videoRunning[i] = true;
        m_receiver[i] = nullptr;
    }
    HGMkDir(m_basePath + "/Video");

    m_videoThread[TUZHUANG] = std::thread([this]
                                {
            m_receiver[TUZHUANG] = new TCPVideoReceiver("rtmp://192.168.1.207:1935/live/ch01.264",TCPVideoReceiver::Mode::Client);
             m_receiver[TUZHUANG]->start([this](uint8_t* data,int width, int height){
	            // 转成 QImage 并发信号
                QImage img(data, width, height, 3*width, QImage::Format_RGB888);
                emit signalNewFrame(img,TUZHUANG);
             });
             while (m_videoRunning[TUZHUANG].load()){
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
             }

             m_receiver[TUZHUANG]->stop();
             delete m_receiver[TUZHUANG];
             m_receiver[TUZHUANG] = nullptr; 
    });
    // m_videoThread[DIDING] = std::thread([this]
    //                             {
    //         m_receiver[DIDING] = new TCPVideoReceiver("rtmp://192.168.1.208:1935/live/ch01.264",TCPVideoReceiver::Mode::Client);
    //         m_receiver[DIDING]->start([this](uint8_t* data,int width, int height){
	//             // 转成 QImage 并发信号
    //             QImage img(data, width, height, 3*width, QImage::Format_RGB888);
    //             emit signalNewFrame(img,DIDING);
    //          });
    //          while (m_videoRunning[DIDING].load()){
    //             std::this_thread::sleep_for(std::chrono::milliseconds(100));
    //          }

    //          m_receiver[DIDING]->stop();
    //          delete m_receiver[DIDING];
    //          m_receiver[DIDING] = nullptr; 
    // });
    // m_videoThread[SHUIFEN] = std::thread([this]
    //                             {
    //         m_receiver[SHUIFEN] = new TCPVideoReceiver("rtmp://192.168.1.209:1935/live/ch01.264",TCPVideoReceiver::Mode::Client);
    //          m_receiver[SHUIFEN]->start([this](uint8_t* data,int width, int height){
	//             // 转成 QImage 并发信号
    //             QImage img(data, width, height, 3*width, QImage::Format_RGB888);
    //             emit signalNewFrame(img,SHUIFEN);
    //          });
    //          while (m_videoRunning[SHUIFEN].load()){
    //             std::this_thread::sleep_for(std::chrono::milliseconds(100));
    //          }

    //          m_receiver[SHUIFEN]->stop();
    //          delete m_receiver[SHUIFEN];
    //          m_receiver[SHUIFEN] = nullptr; 
    // });
    this->showFullScreen();
}

HGDashboardWidget::~HGDashboardWidget()
{
    DashboardRWDB::closeDB(); 
    m_socketServer->stop();
    m_socketServerDiding->stop();
    m_socketServerShuifen->stop();
    deThread();
}
void HGDashboardWidget::onNewFrame(const QImage &img, int index)
{
    if (img.isNull()) {
        qDebug() << "Empty image!";
        return;
    }

    // GUI 更新必须在主线程
    QSize labelSize = m_watchLabel[index]->size();
    if (labelSize.isEmpty()) {
        labelSize = QSize(100, 100); // 默认大小
    }

    // 将文字绘制到图片上
    QPixmap pixmap = QPixmap::fromImage(img).scaled(
        labelSize, Qt::KeepAspectRatio, Qt::SmoothTransformation
    );
    
    // 使用 QPainter 将文字绘制到 pixmap 上
    QPainter painter(&pixmap);
    painter.setPen(Qt::red);  // 设置文字颜色
    painter.setFont(QFont("Arial", 20));  // 设置字体和大小
    if (m_dataType==TUZHUANG)
        painter.drawText(pixmap.rect(), Qt::AlignTop | Qt::AlignHCenter, "TU ZHUANG");
    else if (m_dataType == DIDING)
        painter.drawText(pixmap.rect(), Qt::AlignTop | Qt::AlignHCenter, "DI DING");
    else if (m_dataType == SHUIFEN)
        painter.drawText(pixmap.rect(), Qt::AlignTop | Qt::AlignHCenter, "SHUI FEN");

    // 设置合成后的图片
    m_watchLabel[index]->setPixmap(pixmap);
}


void HGDashboardWidget::closeEvent(QCloseEvent *event)
{
    deThread();
    QWidget::closeEvent(event);
}
bool HGDashboardWidget::fnInitCurlWeather()
{
    const std::string city = "南阳";
    const std::string appkey = "3bfb525001ee05b07014e204a958e525";  // 替换成你的聚合数据 AppKey
    std::string response;

    CURL* curl = curl_easy_init();
    if (curl) {
        char* encodedCity = curl_easy_escape(curl,city.c_str(),city.length());
        std::string url = "http://apis.juhe.cn/simpleWeather/query?city=" + std::string(encodedCity) + "&key=" + appkey;
        curl_free(encodedCity);

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.88.0");

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            std::cout << "响应内容: " << response << std::endl;
            try {
                auto jsonData = json::parse(response);
                if (jsonData["reason"] == "查询成功!") {
                    auto result = jsonData["result"];
                    std::string cityName = result["city"];
                    std::string temperature = result["realtime"]["temperature"];
                    std::string info = result["realtime"]["info"];

                    std::cout << "城市: " << cityName << std::endl;
                    std::cout << "温度: " << temperature << "°C" << std::endl;
                    m_temperature=temperature+"°C";
                    m_weatherStatus=info;
                    std::cout << "天气: " << info << std::endl;
                } else {
                    std::cerr << "查询失败: " << jsonData["reason"] << std::endl;
                }
            } catch (json::parse_error& e) {
                std::cerr << "JSON 解析错误: " << e.what() << std::endl;
            }
        }

        curl_easy_cleanup(curl);
    } else {
        std::cerr << "无法初始化 curl" << std::endl;
    }
    return true;
}
void HGDashboardWidget::deThread(){
    for (int i=0;i<VIDEO_THREAD_NUM;i++){
        m_videoRunning[i] = false;  // 设置停止标志
        if (m_videoThread[i].joinable())
            m_videoThread[i].join();
    }
}
void HGDashboardWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->key()==Qt::Key_Escape){
        deThread();
        this->close();
    } else {
        QWidget::keyPressEvent(event);
    }
}
/*message={"dataList":
	[
		{
			"dataLogTime":1743582455795,
			"dataName":"温度",
			"dataUnit":"℃",
			"dataValue":"-10.1",
			"sensorId":"2"
		},
		{
			"dataLogTime":1743582455795,
			"dataName":"电导率",
			"dataUnit":"μS/cm",
			"dataValue":"145.58",
			"sensorId":"1"
		}
	],
	"deviceNum":"A865661073106802",
	"sessionId":"d87749b1-b90b-45db-9c5c-ffc5c966f843"
}*/



void HGDashboardWidget::fnSlotListenWarnTimerOut(){
    if (m_playVideoFlag)
    {
        // play video
        std::string videoPath = "paplay " + m_basePath + "/LOGO/Alarm06.wav";
        system(videoPath.c_str());
        m_playVideoFlag = false;
    }
}
// 功能函数：把 uint8_t 数据转换为 ASCII 字符串
std::string bytesToAscii(const uint8_t* data, size_t len) {
    std::string result;
    result.reserve(len);

    for (size_t i = 0; i < len; ++i) {
        // 只将不可打印字符替换为 '.'，但保留 0x2E 作为 '.'
        if (data[i] == 0x2E) {
            result += '.';  // 如果是 '.' 字符 (0x2E)，直接添加
        } else if (!std::isprint(data[i])) {
            // result += '.';  // 其他不可打印字符，替换为 '.'
        } else {
            result += static_cast<char>(data[i]);  // 可打印字符，直接添加
        }
    }

    return result;
}
// 查找字典中匹配的元素
#include <unordered_set>
std::vector<std::string> findMatches(const std::string &data, const std::vector<std::string> &dict) {
    std::unordered_set<std::string> seen;  // 去重用
    std::vector<std::string> matches;

    for (const auto &key : dict) {
        if (data.find(key) != std::string::npos) {
            if (seen.insert(key).second) {  // 如果是第一次插入
                matches.push_back(key);
            }
        }
    }

    return matches;
}
std::vector<int> parseStirSpeeds(const std::string &data) {
    std::set<int> uniqueSpeeds;   // 用 set 自动去重
    const std::string key = "stir=";

    size_t pos = 0;
    while ((pos = data.find(key, pos)) != std::string::npos) {
        size_t start = pos + key.size();
        size_t end = data.find('#', start);
        if (end == std::string::npos) {
            break; // 没有结束符 #
        }

        std::string valueStr = data.substr(start, end - start);
        try {
            int speed = std::stoi(valueStr);
            uniqueSpeeds.insert(speed);
        } catch (...) {
            // 忽略非法数字
        }

        pos = end + 1; // 继续往后找
    }

    // 把去重后的结果放回 vector
    return std::vector<int>(uniqueSpeeds.begin(), uniqueSpeeds.end());
}
std::vector<std::string> parseErrorInfo(const std::string &data) {
    std::vector<std::string> results;

    // 正则表达式匹配 "er=任意字符#"
    std::regex pattern(R"(er=(.*?)#)");
    std::smatch matches;

    // 错误代码和对应的描述
    std::map<std::string, std::string> errorMap = {
        {"4111", "4111+漏液报警1"},
        {"4121", "4121+漏液报警2"},
        {"4131", "4131+试剂1超限"},
        {"4141", "4141+试剂2超限"},
        {"4151", "4151+试剂3超限"},
        {"4161", "4161+试剂4超限"},
        {"4171", "4171+试剂5超限"},
        {"4181", "4181+试剂6超限"},
        {"4191", "4191+滴定杯1超限"},
        {"41A1", "41A1+滴定杯2超限"},
        {"41B1", "41B1+断流报警1"},
        {"41C1", "41C1+断流报警2"}
    };

    // 使用迭代器遍历 data 字符串，查找所有匹配
    std::string::const_iterator searchStart(data.cbegin());
    while (std::regex_search(searchStart, data.cend(), matches, pattern)) {
        // 获取匹配的编号部分
        std::string errorCode = matches[1].str();

        // 检查该编号是否存在于 errorMap 中
        auto it = errorMap.find(errorCode);
        if (it != errorMap.end()) {
            // 如果存在，加入结果
            results.push_back(it->second);
        } else {
            // 如果不存在，可以选择加入默认信息，或者跳过
            results.push_back("Unknown error code: " + errorCode); // 默认信息
        }

        // 更新搜索位置，跳到下一个可能的匹配
        searchStart = matches.suffix().first;
    }

    return results;
}
std::vector<std::vector<int>> parseAValuesUnique(const std::string &data) {
    std::vector<std::vector<int>> results;
    std::set<std::string> seen; // 用来存储已出现过的字符串，去重

    const std::string key = "a=";
    size_t pos = 0;

    while ((pos = data.find(key, pos)) != std::string::npos) {
        size_t start = pos + key.size();
        size_t end = data.find('#', start);
        if (end == std::string::npos) {
            break; // 没有找到结束符
        }

        std::string valueStr = data.substr(start, end - start);

        // 去重：如果这个串已经处理过，就跳过
        if (seen.find(valueStr) != seen.end()) {
            pos = end + 1;
            continue;
        }
        seen.insert(valueStr);

        // 转换成数字序列
        std::vector<int> values;
        for (char c : valueStr) {
            if (isdigit(c)) {
                values.push_back(c - '0');
            }
        }

        results.push_back(values);

        pos = end + 1; // 继续查找下一个 a=...#
    }

    return results;
}
std::vector<float> parseVLUnique(const std::string &data) {
    std::vector<float> results;
    std::set<float> seen; // 用来去重

    const std::string key = "vl=";
    size_t pos = 0;

    while ((pos = data.find(key, pos)) != std::string::npos) {
        size_t start = pos + key.size();
        size_t end = data.find('#', start);
        if (end == std::string::npos) break; // 没有找到结束符

        std::string valueStr = data.substr(start, end - start);
        std::stringstream ss(valueStr);
        float value;
        ss >> value;

        if (seen.find(value) == seen.end()) {
            seen.insert(value);
            results.push_back(value);
        }

        pos = end + 1; // 继续查找下一个 vl=
    }

    return results;
}
typedef struct tagMResultValue {
    std::string time;
    float value;
    std::string name;
} MResultValue;
std::string formatTimestamp(const std::string& timestamp) {
    if (timestamp.size() != 12) {
        return "Invalid timestamp length";  // 确保是12位数字
    }

    // 提取日期和时间部分
    std::string dateStr = timestamp.substr(0, 6);  // "250829"
    std::string timeStr = timestamp.substr(6, 6);  // "135145"

    // 从日期部分提取年、月、日
    std::string yearStr = "20" + dateStr.substr(0, 2);  // "2025"
    std::string monthStr = dateStr.substr(2, 2);        // "08"
    std::string dayStr = dateStr.substr(4, 2);          // "29"

    // 从时间部分提取时、分、秒
    std::string hourStr = timeStr.substr(0, 2);         // "13"
    std::string minuteStr = timeStr.substr(2, 2);       // "51"
    std::string secondStr = timeStr.substr(4, 2);       // "45"

    // 拼接成 "YYYY-MM-DD HH:MM:SS.000" 格式
    std::stringstream formattedTime;
    formattedTime << yearStr << "-" << monthStr << "-" << dayStr << " "
                  << hourStr << ":" << minuteStr << ":" << secondStr;

    return formattedTime.str();
}
std::vector<MResultValue> parseMResultValue(const std::string &data, const std::string &delStr = "") {
    std::vector<MResultValue> results;
    std::string input = data;

    // 如果有delStr，移除字符串中的所有delStr中的字符
    if (!delStr.empty()) {
        for (char ch : delStr) {
            // 删除所有出现的delStr中的字符
            input.erase(std::remove(input.begin(), input.end(), ch), input.end());
        }
    }

    // 调整正则表达式来匹配 mXX=value*time 格式的字符串，允许前后有任意字符
    std::regex pattern(R"(m(\d{2})=([+-]?\d+(\.\d+)?)\*(\d+))");
    std::smatch matches;

    // 使用正则表达式来匹配并解析字符串
    std::string::const_iterator searchStart(input.cbegin());
    while (std::regex_search(searchStart, input.cend(), matches, pattern)) {
        MResultValue resultValue;
        
        // 提取 mXX 的数字部分，并存入 name 字段
        resultValue.name = "m" + matches[1].str(); // "mXX" 格式

        // 提取 value（浮动点数）并存入 value 字段
        resultValue.value = std::stof(matches[2].str()); // 将 value 转换为 float

        // 提取 time（字符串）并存入 time 字段
        resultValue.time = matches[4].str(); // time 部分保持为字符串
        resultValue.time = formatTimestamp(resultValue.time);
        // 输出结构体的内容
        std::cout << "name = " << resultValue.name << std::endl;
        std::cout << "value = " << resultValue.value << std::endl;
        std::cout << "time = " << resultValue.time << std::endl;

        HGLog4Cplus::getLogInstance(LOG_PATH)->logout("name = " + resultValue.name,LOGINFO);
        HGLog4Cplus::getLogInstance(LOG_PATH)->logout("value = " + std::to_string(resultValue.value),LOGINFO);
        HGLog4Cplus::getLogInstance(LOG_PATH)->logout("time = " + resultValue.time,LOGINFO);
        results.push_back(resultValue);

        // 更新搜索位置，跳到下一个可能的匹配
        searchStart = matches.suffix().first;
    }

    if (results.empty()) {
        std::cout << "No match found for input: " << input << std::endl;
        HGLog4Cplus::getLogInstance(LOG_PATH)->logout("No match found for input: " + input,LOGERROR);
    }

    return results;
}

void HGDashboardWidget::setTableData(const int& type,const int &row, const DashboardDataInfo &dataInfo)
{
    // if (m_dataType!=TUZHUANG){
    //     if (row==2||row==3) return;
    // }
    if (row < 0) return;
    QTableWidgetItem *item = new QTableWidgetItem(QString("(%1,%2)").arg(row).arg(2));
    item->setText(QString::fromStdString(dataInfo.value));
    item->setTextAlignment(Qt::AlignCenter);
    item->setForeground(QBrush(dataInfo.color));
    m_tableW[type]->setItem(row, 2, item);

    item = new QTableWidgetItem(QString("(%1,%2)").arg(row).arg(4));
    item->setText(QString::fromStdString(dataInfo.ts));
    item->setTextAlignment(Qt::AlignCenter);
    item->setForeground(QBrush(dataInfo.color));

    QFont font123 = item->font();
    QFontMetrics fm(font123);

    // 判断文本是否超过单元格宽度
    int cellWidth = m_tableW[type]->columnWidth(4);
    int textWidth = fm.horizontalAdvance(item->text());

    if (textWidth > cellWidth) {
        font123.setPointSize(m_width*18/1800); 
        item->setFont(font123);
    }

    m_tableW[type]->setItem(row, 4, item);

    item = new QTableWidgetItem();
    item->setText(QString::fromStdString(dataInfo.status));
    item->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    if (dataInfo.status == "✔️合格")
    {
        item->setForeground(QBrush(Qt::green));
    }
    else
    {
        item->setText("");
        item->setForeground(QBrush(Qt::red));
        m_playVideoFlag = true;
        // // play video
        // std::string videoPath="paplay " + m_basePath+"/LOGO/Alarm06.wav";
        // system(videoPath.c_str());
    }
    m_tableW[type]->setItem(row, 3, item);
    m_tableW[type]->setIconSize(QSize(16, 16));
}
bool isNumber(const std::string &code) {
    if (code.empty()) return false;
    for (char c : code) {
        if (!std::isdigit(c)) return false;
    }
    return true;
}
void HGDashboardWidget::fnSlotListenSystemUsageTimerOut(){
    double cpuusage = SystemUsage::getCpuUsage();
    long memoryusage = SystemUsage::getMemoryUsage();
    double networkUsage = SystemUsage::getNetworkUsage();
    double diskIO = SystemUsage::getDiskIO();

    m_titleLabel->setText(QString::fromStdString("CPU:"+std::to_string(cpuusage) + 
        " % ,Memory:" + std::to_string(memoryusage) + " MB, Network(kb/s):" + std::to_string(networkUsage) +",Disk IO(kb/s):"+std::to_string(diskIO)));
    
}
void HGDashboardWidget::fnSlotListenTimerOut()
{
    // m_rightTimeLabel->setText("");
    // m_timeLabel->setText("");
    // return;

    std::vector<std::string> wdays={"星期日", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六"};
    std::map<std::string,std::string> dict = {
        {"purge#","定量"},
        {"volume#","进样"},
        // {"dir=f#","方向向上"},
        {"dir=b#","方向向下"},
        {"valve=on#","控制阀开"},
        {"valve=off#","控制阀关"},
        {"am=on#","报警灯开"},
        {"am=off#","报警灯关"},
        {"d=on#","光源开"},
        {"d=off#","光源关"},
        {"bz=0#","空闲"},
        {"bz=1#","忙碌中"},
        {"wash#","清洗计量管"}
    };
    // socket data
    for (int i = 0; i < 3; i++)
    {
        int dataType = i;
        std::vector<uint8_t> reply;
        std::string asciiStr;
        int ret = -1;
        switch (dataType)
        {
        case TUZHUANG:
            ret = m_socketServer->recvData(reply,asciiStr);
            break;
        case DIDING:
            ret = m_socketServerDiding->recvData(reply,asciiStr);
            break;
        case SHUIFEN:
            ret = m_socketServerShuifen->recvData(reply,asciiStr);
            break;
        default:
            break;
        }
        if (ret > 0)
        {
            std::cout << "Server reply ["<<i<<"]: ";
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout("Server reply ["+std::to_string(i)+"]: ",LOGINFO);
            for (int i = 0; i < int(reply.size()); i++)
            {
                std::cout << std::hex << std::setw(2) << std::setfill('0')
                          << (static_cast<unsigned int>(reply[i])) << " ";
            }
            std::cout << std::dec << std::endl;
            std::string asciiStr = bytesToAscii(reply.data(), reply.size());
            std::cout << "ASCII output ["<<i<<"]: " << asciiStr << std::endl;
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout("ASCII output ["+std::to_string(i)+"]: " + asciiStr,LOGINFO);

            THData th;
            std::string cValue;
            // 温度湿度
            if (parseTH(asciiStr, th))
            {

                printf("parseth temp and humidity: %.2f,%.2f\n",th.temperature,th.humidity);
                HGLog4Cplus::getLogInstance(LOG_PATH)->logout("parseth temp and humidity: "+std::to_string(th.temperature)+","+std::to_string(th.humidity),LOGINFO);
                std::ostringstream ss;
                ss << th.temperature << "°C, " << th.humidity << "%";
                m_rightTimeLabel->setText(QString::fromStdString(ss.str()));
            } else {
                printf("parse TH error\n");
                HGLog4Cplus::getLogInstance(LOG_PATH)->logout("parse TH error",LOGERROR);
            }
            // 仪器状态
            std::vector<std::string> dictkey; // 字典
            dictkey.reserve(dict.size());     // 预分配空间，提高效率

            for (const auto &pair : dict)
            {
                dictkey.push_back(pair.first); // 保存 key
            }
            auto matches = findMatches(asciiStr, dictkey);

            // std::cout << "Matched elements:" << std::endl;
            std::ostringstream ss;
            for (const auto &m : matches)
            {
                ss << dict[m] << " ";
            }

            auto parsed = parseAllSt(asciiStr);

            for (auto &d : parsed)
            {
                std::cout << "组合号: " << d.group
                          << " 代号: " << d.code;
                if (isNumber(d.code))
                {
                    ss << "项目" << d.group << "正在";
                    if (d.code == "1")
                        ss << "预清洗";
                    else if (d.code == "2")
                        ss << "进样";
                    else if (d.code == "3")
                        ss << "分析";
                    else if (d.code == "4")
                        ss << "清洗";
                    else if (d.code == "5")
                        ss << "休息";
                }
                else
                {
                    ss << "组合" << std::atoi(d.group.c_str());
                    if (!d.subCode.empty())
                    {
                        // std::cout << " 子项: " << d.subCode;
                        if (d.subCode == "l")
                            ss << "溶剂" << std::atoi(d.code.c_str()) << "液位超限";
                    }
                    else
                    {
                        if (d.code == "r")
                            ss << "清除报警";
                        else if (d.code == "d")
                            ss << "看门狗报警";
                        else if (d.code == "o")
                            ss << "结果超限";
                        else if (d.code == "w")
                            ss << "正在分析";
                        else if (d.code == "c")
                            ss << "寿命报警";
                    }
                }
                ss << " ";
            }

            auto speeds = parseStirSpeeds(asciiStr);
            for (size_t i = 0; i < speeds.size(); ++i)
            {
                ss << "搅拌速度" << speeds[i] << "->";
            }
            if (ss.str() != "")
            {
                m_deviceStatusLabel[dataType]->setText(QString::fromStdString(ss.str()));
                adjustLabelFont(m_deviceStatusLabel[dataType]);
            }

            // 报警信息
            std::ostringstream oss;
            auto results = parseAValuesUnique(asciiStr);
            for (const auto &seq : results)
            {
                int mm = 0;
                for (int v : seq)
                {
                    if (v == 1)
                    {
                        oss << "液位" << mm + 1 << "报警";
                    }
                    mm++;
                }
            }
            // other warning info
            auto resultsWarn = parseErrorInfo(asciiStr);
            for (const auto &seq : resultsWarn){
                oss << seq;
            }
            // 一次性设置 label
            m_errStatusLabel[dataType]->setText(QString::fromStdString(oss.str()));

            // 加药量
            auto values = parseVLUnique(asciiStr);
            for (float v : values)
            {
                int row = 0;
                if (dataType==TUZHUANG) row=4;
                else break;
                DashboardDataInfo dataInfo;
                dataInfo.ts = getStandardCurTime();
                dataInfo.value = std::to_string(v);
                dataInfo.color = QColor(179, 255, 251);
                if (v > m_vlRange.max || v < m_vlRange.min)
                {
                    dataInfo.status = "❌不合格";
                    dataInfo.iconpath = m_basePath + "/LOGO/ng.png";
                }
                else
                {
                    dataInfo.status = "✔️合格";
                    dataInfo.iconpath = m_basePath + "/LOGO/ok.png";
                }
                setTableData(i,row, dataInfo);
            }

            // 
            std::string delStr="";
            if (dataType==TUZHUANG) delStr="";
            else if (dataType==DIDING) delStr="Px}@^l";
            else if (dataType==SHUIFEN) delStr="Px}@";
            auto mvalues = parseMResultValue(asciiStr,delStr);
            int mresultIndex=0;
            for (MResultValue v : mvalues){
                DashboardDataInfo dataInfo;
                dataInfo.ts = v.time;
                dataInfo.value = std::to_string(v.value);
                dataInfo.color = QColor(179,255,251);
                std::map<std::string,std::string> msg;
                msg["ts"]=v.time;
                msg["value"]=std::to_string(v.value);
                if (dataType==TUZHUANG){
                    if (v.name=="m31"){
                        if (v.value > m_Alkalinity1Range.max || v.value < m_Alkalinity1Range.min){
                            dataInfo.status = "❌不合格";
                            dataInfo.iconpath = m_basePath + "/LOGO/ng.png";
                        } else {
                            dataInfo.status = "✔️合格";
                            dataInfo.iconpath = m_basePath + "/LOGO/ok.png";
                        }
                        m_Alkalinity1Curve->fnUpdateData(msg);
                        mresultIndex=0;
                    } else if (v.name=="m61"){
                        if (v.value > m_Alkalinity2Range.max || v.value < m_Alkalinity2Range.min){
                            dataInfo.status = "❌不合格";
                            dataInfo.iconpath = m_basePath + "/LOGO/ng.png";
                        } else {
                            dataInfo.status = "✔️合格";
                            dataInfo.iconpath = m_basePath + "/LOGO/ok.png";
                        }
                        m_Alkalinity2Curve->fnUpdateData(msg);
                        mresultIndex=1;
                    }
                } else if (dataType==SHUIFEN){
                    if (v.name=="m31"){
                        if (v.value > m_shuifenChannel1Range.max || v.value < m_shuifenChannel1Range.min){
                            dataInfo.status = "❌不合格";
                            dataInfo.iconpath = m_basePath + "/LOGO/ng.png";
                        } else {
                            dataInfo.status = "✔️合格";
                            dataInfo.iconpath = m_basePath + "/LOGO/ok.png";
                        }
                        m_shuifenChannel1Curve->fnUpdateData(msg);
                        mresultIndex=0;
                    } else if (v.name=="m61"){
                        if (v.value > m_shuifenChannel2Range.max || v.value < m_shuifenChannel2Range.min){
                            dataInfo.status = "❌不合格";
                            dataInfo.iconpath = m_basePath + "/LOGO/ng.png";
                        } else {
                            dataInfo.status = "✔️合格";
                            dataInfo.iconpath = m_basePath + "/LOGO/ok.png";
                        }
                        m_shuifenChannel2Curve->fnUpdateData(msg);
                        mresultIndex=1;
                    }
                } else if (dataType==DIDING){
                    if (v.name=="m11"){
                        if (v.value > m_didingChannel1Range.max || v.value < m_didingChannel1Range.min){
                            dataInfo.status = "❌不合格";
                            dataInfo.iconpath = m_basePath + "/LOGO/ng.png";
                        } else {
                            dataInfo.status = "✔️合格";
                            dataInfo.iconpath = m_basePath + "/LOGO/ok.png";
                        }
                        m_didingChannel1Curve->fnUpdateData(msg);
                        mresultIndex=0;
                    } else if (v.name=="m31"){
                        if (v.value > m_didingChannel2Range.max || v.value < m_didingChannel2Range.min){
                            dataInfo.status = "❌不合格";
                            dataInfo.iconpath = m_basePath + "/LOGO/ng.png";
                        } else {
                            dataInfo.status = "✔️合格";
                            dataInfo.iconpath = m_basePath + "/LOGO/ok.png";
                        }
                        m_didingChannel2Curve->fnUpdateData(msg);
                        mresultIndex=1;
                    } else if (v.name=="m41"){
                        if (v.value > m_didingChannel3Range.max || v.value < m_didingChannel3Range.min){
                            dataInfo.status = "❌不合格";
                            dataInfo.iconpath = m_basePath + "/LOGO/ng.png";
                        } else {
                            dataInfo.status = "✔️合格";
                            dataInfo.iconpath = m_basePath + "/LOGO/ok.png";
                        }
                        m_didingChannel3Curve->fnUpdateData(msg);
                        mresultIndex=2;
                    } else if (v.name=="m61"){
                        if (v.value > m_didingChannel4Range.max || v.value < m_didingChannel4Range.min){
                            dataInfo.status = "❌不合格";
                            dataInfo.iconpath = m_basePath + "/LOGO/ng.png";
                        } else {
                            dataInfo.status = "✔️合格";
                            dataInfo.iconpath = m_basePath + "/LOGO/ok.png";
                        }
                        m_didingChannel4Curve->fnUpdateData(msg);
                        mresultIndex=3;
                    }
                }
                setTableData(i,mresultIndex, dataInfo);
            }
        }
    }
    //     // play video
    // std::string videoPath = "paplay " + m_basePath + "/LOGO/Alarm06.wav";
    // system(videoPath.c_str());

    
    std::ostringstream logtext;
    HGExactTime currentTime = HGExactTime::currentTime();
    std::ostringstream timestr1;
    timestr1<<currentTime.tm_year << "年" << currentTime.tm_mon << "月" << currentTime.tm_mday << "日"<<wdays[currentTime.tm_wday];
    m_timeLabel->setText(QString::fromStdString(timestr1.str()));
    // m_timeLabel->setFont(font);

    std::vector<std::map<std::string, std::string>> msgs;
    msgs = m_mqttclient->recvMQTTMessage();
    // printf("msg count:%d\n",msgs.size());
    // for (int i=0;i<int(msgs.size());i++){
    //     for (const auto &kv : msgs[i]){
    //         printf("msg key:%s,value:%s\n",kv.first.c_str(),kv.second.c_str());
    //     }
    // }
    // TODO msg是解析之后的数据
    if (!msgs.empty())
    {
        for (int i = 0; i < int(msgs.size()); i++)
        {
            std::map<std::string, std::string> msg = msgs[i];
            if (msg.empty())
                continue;
            
            std::map<std::string,std::string> dbmsg;
            if (msg.find("ts") == msg.end()) continue;
            if (msg.at("ts")=="copy") continue;
            std::ostringstream timestr;
            timestr << DASHBOARDDBNAME << "_" << currentTime.tm_year << currentTime.tm_mon << currentTime.tm_mday;
            

            dbmsg["ts"]=msg.at("ts");
            std::ostringstream recvTimeStr;
            recvTimeStr << currentTime.tm_year << "-" << currentTime.tm_mon << "-" << currentTime.tm_mday << " " << currentTime.tm_hour << ":" << currentTime.tm_min << ":" << currentTime.tm_sec << "." << currentTime.tm_millisec;

            dbmsg["recTs"]=recvTimeStr.str();
            for (const auto &kv : msg)
            {
                if (kv.first != "ts") {
                    dbmsg["key"]=kv.first;
                    dbmsg["value"]=kv.second;
                }
                int row=-1;
                DashboardDataInfo dataInfo;
                if (kv.first == "Alkalinity1")
                {
                    float value = std::atof(kv.second.c_str());
                    row = 0;
                    dataInfo.ts = dbmsg.at("ts");
                    dataInfo.value = kv.second;
                    dataInfo.color = QColor(179, 255, 251);
                    if (value > m_Alkalinity1Range.max || value < m_Alkalinity1Range.min)
                    {
                        dataInfo.status = "❌不合格";
                        dataInfo.iconpath = m_basePath + "/LOGO/ng.png";
                    }
                    else
                    {
                        dataInfo.status = "✔️合格";
                        dataInfo.iconpath = m_basePath + "/LOGO/ok.png";
                    }
                    m_Alkalinity1Curve->fnUpdateData(msg);
                }
                else if (kv.first == "Alkalinity2")
                {
                    float value = std::atof(kv.second.c_str());
                    row = 1;
                    dataInfo.ts = dbmsg.at("ts");
                    dataInfo.value = kv.second;
                    dataInfo.color = QColor(179, 255, 251);
                    if (value > m_Alkalinity2Range.max || value < m_Alkalinity2Range.min)
                    {
                        dataInfo.status = "❌不合格";
                        dataInfo.iconpath = m_basePath + "/LOGO/ng.png";
                    }
                    else
                    {
                        dataInfo.status = "✔️合格";
                        dataInfo.iconpath = m_basePath + "/LOGO/ok.png";
                    }

                    m_Alkalinity2Curve->fnUpdateData(msg);
                }
                // else if (kv.first == "FreeAcidity")
                // {
                //     float value = std::atof(kv.second.c_str());
                //     row = 3;
                //     dataInfo.ts = dbmsg.at("ts");
                //     dataInfo.value = kv.second;
                //     dataInfo.color = QColor(179, 255, 251);
                //     if (value > m_FreeAcidityRange.max || value < m_FreeAcidityRange.min)
                //     {
                //         dataInfo.status = "❌不合格";
                //         dataInfo.iconpath = m_basePath + "/LOGO/ng.png";
                //     }
                //     else
                //     {
                //         dataInfo.status = "✔️合格";
                //         dataInfo.iconpath = m_basePath + "/LOGO/ok.png";
                //     }
                //     m_FreeAcidityCurve->fnUpdateData(msg);
                // }
                // else if (kv.first == "TotalAcidity")
                // {
                //     float value = std::atof(kv.second.c_str());
                //     row = 2;
                //     dataInfo.ts = dbmsg.at("ts");
                //     dataInfo.value = kv.second;
                //     dataInfo.color = QColor(179, 255, 251);
                //     if (value > m_TotalAcidityRange.max || value < m_TotalAcidityRange.min)
                //     {
                //         dataInfo.status = "❌不合格";
                //         dataInfo.iconpath = m_basePath + "/LOGO/ng.png";       
                //     }
                //     else
                //     {
                //         dataInfo.status = "✔️合格";
                //         dataInfo.iconpath = m_basePath + "/LOGO/ok.png";
                //     }
                //     m_TotalAcidityCurve->fnUpdateData(msg);
                // }
                // hogon
                /* else */if (kv.first == "电导率")
                {
                    float value = std::atof(kv.second.c_str());
                    m_ElectricalConductivity1Bar->setObjectName(QString::fromStdString(msg["deviceNum"]));
                    m_ElectricalConductivity1Bar->setValue(std::atof(kv.second.c_str()));

                    row = 2;
                    dataInfo.ts = dbmsg.at("ts");
                    dataInfo.value = kv.second;
                    dataInfo.color = QColor(179, 255, 251);
                    if (value > m_ElectricalConductivity1Range.max || value < m_ElectricalConductivity1Range.min)
                    {
                        dataInfo.status = "❌不合格";
                        dataInfo.iconpath = m_basePath + "/LOGO/ng.png";
                    }
                    else
                    {
                        dataInfo.status = "✔️合格";
                        dataInfo.iconpath = m_basePath + "/LOGO/ok.png";
                    }
                    m_TotalAcidityCurve->fnUpdateData(msg);
                }
                else if (kv.first == "pH")
                {
                    float value = std::atof(kv.second.c_str());
                    m_ElectricalConductivity2Bar->setObjectName(QString::fromStdString(msg["deviceNum"]));
                    m_ElectricalConductivity2Bar->setValue(std::atof(kv.second.c_str()));

                    row = 3;
                    dataInfo.ts = dbmsg.at("ts");
                    dataInfo.value = kv.second;
                    dataInfo.color = QColor(179, 255, 251);
                    if (value > m_ElectricalConductivity2Range.max || value < m_ElectricalConductivity2Range.min)
                    {
                        dataInfo.status = "❌不合格";
                        dataInfo.iconpath = m_basePath + "/LOGO/ng.png";
                    }
                    else
                    {
                        dataInfo.status = "✔️合格";
                        dataInfo.iconpath = m_basePath + "/LOGO/ok.png";
                    }
                    m_FreeAcidityCurve->fnUpdateData(msg);
                }
  
                // nanyang
                // else if (kv.first == "电导率")
                // {
                //     if (m_ElectricalConductivity1Bar->objectName().isEmpty())
                //     {
                //         float value = std::atof(kv.second.c_str());
                //         m_ElectricalConductivity1Bar->setObjectName(QString::fromStdString(msg["deviceNum"]));
                //         m_ElectricalConductivity1Bar->setValue(std::atof(kv.second.c_str()));

                //         row = 4;
                //         dataInfo.ts = dbmsg.at("ts");
                //         dataInfo.value = kv.second;
                //         dataInfo.color = QColor(179, 255, 251);
                //         if (value > m_ElectricalConductivity1Range.max || value < m_ElectricalConductivity1Range.min)
                //         {
                //             dataInfo.status = "❌不合格";
                //             dataInfo.iconpath = m_basePath + "/LOGO/ng.png";
                //         }
                //         else
                //         {
                //             dataInfo.status = "✔️合格";
                //             dataInfo.iconpath = m_basePath + "/LOGO/ok.png";
                //         }
                //     }
                //     else
                //     {
                //         if (m_ElectricalConductivity1Bar->objectName() == QString::fromStdString(msg["deviceNum"]))
                //         {
                //             float value = std::atof(kv.second.c_str());
                //             m_ElectricalConductivity1Bar->setValue(std::atof(kv.second.c_str()));
                //             row = 4;
                //             dataInfo.ts = dbmsg.at("ts");
                //             dataInfo.value = kv.second;
                //             dataInfo.color = QColor(179, 255, 251);
                //             if (value > m_ElectricalConductivity1Range.max || value < m_ElectricalConductivity1Range.min)
                //             {
                //                 dataInfo.status = "❌不合格";
                //                 dataInfo.iconpath = m_basePath + "/LOGO/ng.png";
                //             }
                //             else
                //             {
                //                 dataInfo.status = "✔️合格";
                //                 dataInfo.iconpath = m_basePath + "/LOGO/ok.png";
                //             }
                //         }
                //         else
                //         {
                //             float value = std::atof(kv.second.c_str());
                //             m_ElectricalConductivity2Bar->setObjectName(QString::fromStdString(msg["deviceNum"]));
                //             m_ElectricalConductivity2Bar->setValue(std::atof(kv.second.c_str()));

                //             row = 5;
                //             dataInfo.ts = dbmsg.at("ts");
                //             dataInfo.value = kv.second;
                //             dataInfo.color = QColor(179, 255, 251);
                //             if (value > m_ElectricalConductivity2Range.max || value < m_ElectricalConductivity2Range.min)
                //             {
                //                 dataInfo.status = "❌不合格";
                //                 dataInfo.iconpath = m_basePath + "/LOGO/ng.png";
                //             }
                //             else
                //             {
                //                 dataInfo.status = "✔️合格";
                //                 dataInfo.iconpath = m_basePath + "/LOGO/ok.png";
                //             }
                //         }
                //     }
                // }

                if (row >= 0) {
                    setTableData(TUZHUANG,row,dataInfo);
                }

                logtext<<"msg key:"<<kv.first<<",value:"<<kv.second<<",";
                HGLog4Cplus::getLogInstance(LOG_PATH)->logout(logtext.str(),LOGINFO);
                // std::cout << kv.first << " : " << kv.second << ",";
            }
            // std::cout<<std::endl;
            
            std::string tableName=timestr.str();
            DashboardRWDB::writeDashboardDataInfo(tableName,dbmsg);
        }
    }
}
void HGDashboardWidget::fillTableData(int type, const std::vector<std::string> &firstColumnNames,
    const std::vector<std::string> &secondColumnNames)
{
    for (int row = 0; row < int(firstColumnNames.size()); ++row)
    {
        for (int col = 0; col < 2; ++col)
        {
            QTableWidgetItem *item = new QTableWidgetItem(QString("(%1,%2)").arg(row).arg(col));
            QColor color;
            if (col==0){
                item->setText(QString::fromStdString(firstColumnNames[row]));
                item->setTextAlignment(Qt::AlignCenter);
                color=QColor(0,176,240);
            } else if (col==1){
                item->setText(QString::fromStdString(secondColumnNames[row]));
                item->setTextAlignment(Qt::AlignCenter);
                color=QColor(247,199,187);
            }
            item->setForeground(QBrush(color));
            m_tableW[type]->setItem(row, col, item);
        }
    }
}
void removeAllWidgetsFromGridLayout(QGridLayout* layout,bool isAll=false)
{
    // Get the number of rows and columns in the layout
    int rowCount = layout->rowCount();
    int colCount = layout->columnCount();
    int rowStart;
    if (!isAll) rowStart = 1;
    else rowStart = 0;
    // Loop through all the cells in the grid layout
    for (int row = rowStart; row < rowCount; ++row) {
        for (int col = 0; col < colCount; ++col) {
            // Get the item at the given position
            QLayoutItem* item = layout->itemAtPosition(row, col);
            if (item) {
                // Check if the item is a widget
                QWidget* widget = item->widget();
                if (widget) {
                    // Remove widget from layout
                    layout->removeWidget(widget);
                    // Optionally delete the widget
                    widget->hide();
                    // delete widget;
                }
                // If the item is not a widget, it could be a spacer or another layout item.
                else {
                    layout->removeItem(item);
                }
            }
        }
    }
}
void showAllWidgetsFromGridLayout(QGridLayout* layout)
{
    // Get the number of rows and columns in the layout
    int rowCount = layout->rowCount();
    int colCount = layout->columnCount();

    // Loop through all the cells in the grid layout
    for (int row = 1; row < rowCount; ++row) {
        for (int col = 0; col < colCount; ++col) {
            // Get the item at the given position
            QLayoutItem* item = layout->itemAtPosition(row, col);
            if (item) {
                // Check if the item is a widget
                QWidget* widget = item->widget();
                if (widget) {
                    widget->show();
                }
            }
        }
    }
}
void adjustRowHeight(QTableWidget* tableWidget)
{
    // 获取 QTableWidget 的总高度
    int tableHeight = tableWidget->height();

    // 计算一半的高度
    int halfHeight = tableHeight / 2;

    // 获取当前行数
    int rowCount = tableWidget->rowCount();

    if (rowCount > 0)
    {
        // 计算每行应有的高度
        int rowHeight = halfHeight / rowCount;

        // 设置每一行的高度
        for (int row = 0; row < rowCount; ++row)
        {
            tableWidget->setRowHeight(row, rowHeight);
        }
    }
}
void HGDashboardWidget::displayRealData(){
    removeAllWidgetsFromGridLayout(m_gridLayout);
    m_watchLabel[TUZHUANG]->setParent(m_mainContent);
    m_watchLabel[TUZHUANG]->setFixedSize(m_width*0.15,m_height*0.15);
    m_watchLabel[TUZHUANG]->move(m_width*0.755,m_height*0.138);
    m_watchLabel[TUZHUANG]->raise();
    m_watchLabel[TUZHUANG]->show();
    m_watchLabel[DIDING]->setFixedSize(m_width*0.15,m_height*0.15);
    if (m_funcType == REAL_DATA)
    {
        m_gridLayout->addWidget(m_deviceStatusLabel[m_dataType], 1, 0, 1, 4);
        m_gridLayout->addWidget(m_watchLabel[DIDING], 1, 3, 1, 1);
        m_gridLayout->setAlignment(m_watchLabel[DIDING], Qt::AlignCenter); // 让该控件靠对齐
        m_gridLayout->addWidget(m_hor1, 2, 0, 1, 4);
        m_gridLayout->addWidget(m_tableW[m_dataType], m_gridLayout->rowCount(), 0, 1, 4);
        m_gridLayout->addWidget(m_hor2, m_gridLayout->rowCount(), 0, 1, 4);
    }
    switch (m_dataType){
        case TUZHUANG:
        {
            switch (m_funcType){
                case REAL_DATA:
                m_gridLayout->addWidget(m_Alkalinity1Curve,m_gridLayout->rowCount(),0);
                m_gridLayout->addWidget(m_Alkalinity2Curve,m_gridLayout->rowCount()-1,1);
                m_gridLayout->addWidget(m_TotalAcidityCurve,m_gridLayout->rowCount()-1,2);
                m_gridLayout->addWidget(m_FreeAcidityCurve,m_gridLayout->rowCount()-1,3);
                break;
                case USER_DATA:
                {
                // 创建 MapLabel 并设置城市数据
                MapLabel* mapLabelWidget = new MapLabel(m_dataType);
                mapLabelWidget->setFixedSize(m_width*0.952,m_height*0.81);
                m_gridLayout->addWidget(mapLabelWidget,1,1);
                // 连接点击信号
                connect(mapLabelWidget, &MapLabel::cityClicked, this, [&](const QString& name, const CityInfo& info){
                    QMessageBox::information(this, "城市信息",
                                             QString("城市: %1\n总设备: %2").arg(name).arg(info.totalDevices));
                });
                break;
                }
                case MAINTENANCE:
                {
                QTableWidget* maintenaceTableW = new QTableWidget();

                std::vector<std::string> firstColumnNames={"加药桶换水清理", 
                    "药桶换水清理",
                    "加液桶换水清理",
                    "液桶换水清理",
                    "取样桶1换水清理",
                    "取样桶2换水清理",
                    "PH桶换水清理",
                    "电导桶换水清理","蠕动泵头","蠕动泵管","检测电极","电解电极","计量管活塞1","计量管活塞2","计量管活塞3"}; 
                std::vector<std::string> secondColumnNames={"2周","2周","2周","2周","2周","2周","2周","2周","6个月","3个月",
                    "2年","2年","1年","1年","1年"};
                std::vector<int> mainanceCycleWithDays = {14,14,14,14,14,14,14,14,180,90,365*2,365*2,365,365,365};
                QStringList headers=QStringList{"设备项目","维护周期","维护人员", "维护倒计时"};
                maintenaceTableW=new QTableWidget(int(firstColumnNames.size()),headers.size()); 
                maintenaceTableW->setHorizontalHeaderLabels(headers); 
                maintenaceTableW->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
                maintenaceTableW->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
                maintenaceTableW->verticalHeader()->setVisible(false);
                maintenaceTableW->horizontalHeader()->setVisible(true);
                maintenaceTableW->setShowGrid(true);
                maintenaceTableW->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color:rgb(0,176,240);color:rgb(0,0,0);}");
                maintenaceTableW->setFixedSize(width()*0.98,height()*0.8);
                QFont font = maintenaceTableW->horizontalHeader()->font();  // 获取水平表头字体
                font.setPointSize(36);  // 设置字体大小为14
                maintenaceTableW->horizontalHeader()->setFont(font);  // 应用到水平表头
                font = maintenaceTableW->font();
                font.setPointSize(18);  // 设置字体大小为12
                maintenaceTableW->setFont(font);
                std::map<std::string,std::string> mainMsg;
                mainMsg=DashboardRWDB::readMaintenanceInfo(m_dataType);

                for (int row = 0; row < int(firstColumnNames.size()); ++row)
                {
                    for (int col = 0; col < 4; ++col)
                    {
                        QTableWidgetItem *item = new QTableWidgetItem(QString("(%1,%2)").arg(row).arg(col));
                        QColor color;
                        if (col==0){
                            item->setText(QString::fromStdString(firstColumnNames[row]));
                            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                            color=QColor(131,201,98);
                        } else if (col==1){
                            item->setText(QString::fromStdString(secondColumnNames[row]));
                            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                            color=QColor(255,255,255);
                        } else if (col==2){
                            item->setText("");
                            item->setFlags(item->flags() | Qt::ItemIsEditable);
                            color=QColor(255,255,255);
                        } else if (col==3){
                            std::string timeStr = mainMsg[maintenaceTableW->item(row,0)->text().toStdString()];
                            HGExactTime curTime=HGExactTime::currentTime();
                            int countsecond = curTime.fasterThanThirtyMimutes(timeStr);
                            QString resultText;
                            if (countsecond < 0)
                            {
                                resultText = "X天/超时X天";
                            }
                            else
                            {
                                int countday = countsecond / 86400;
                                int maxDays = mainanceCycleWithDays[row];
                                int overdueDays = countday - maxDays;
                                int recountdays = maxDays - countday;
                                resultText = QString("%1天/超时%2天").arg(recountdays > 0?recountdays:0).arg(overdueDays > 0 ? overdueDays : 0);
                            }

                            item->setText(resultText);
                            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                            color=QColor(255,255,255);
                        }
                        item->setBackground(QBrush(color));
                        item->setTextAlignment(Qt::AlignCenter);
                        maintenaceTableW->setItem(row, col, item);
                    }
                }

                m_gridLayout->addWidget(maintenaceTableW,m_gridLayout->rowCount(),0,1,4);
                // 初始调整行高
                // adjustRowHeight(maintenaceTableW);
                connect(maintenaceTableW, &QTableWidget::cellClicked, [=](int row, int col){
                    // 如果点击的是最后一列 (索引为 3)
                    if (col == 3) {
                        if (QMessageBox::Yes==QMessageBox::question(this,QString::fromStdString("维保"),
                            "确定维保"+maintenaceTableW->item(row,0)->text()+"?",QMessageBox::Yes | QMessageBox::No)){
                            maintenaceTableW->item(row,3)->setText(QString::number(mainanceCycleWithDays[row])+"天/超时0天");
                            std::map<std::string,std::string> tempMsg;
                            HGExactTime curTime=HGExactTime::currentTime();
                            tempMsg[maintenaceTableW->item(row,0)->text().toStdString()]=curTime.toStringFromYearToSec();
                            DashboardRWDB::writeMaintenanceInfo(tempMsg,m_dataType);
                        }
                    } 
                });
                break;
                }
                case MONITOR:{
                m_watchLabel[TUZHUANG]->setFixedSize(m_width*0.952,m_height*0.81);
                m_gridLayout->addWidget(m_watchLabel[TUZHUANG],1,1);
                break;
                }
                default: break;
            }
        break;
        }
        case DIDING:
        {
            switch (m_funcType){
                case REAL_DATA:
                m_gridLayout->addWidget(m_didingChannel1Curve,m_gridLayout->rowCount(),0);
                m_gridLayout->addWidget(m_didingChannel2Curve,m_gridLayout->rowCount()-1,1);
                m_gridLayout->addWidget(m_didingChannel3Curve,m_gridLayout->rowCount()-1,2);
                m_gridLayout->addWidget(m_didingChannel4Curve,m_gridLayout->rowCount()-1,3);
                break;
                case USER_DATA:{
                // 创建 MapLabel 并设置城市数据
                MapLabel* mapLabelWidget = new MapLabel(m_dataType);
                mapLabelWidget->setFixedSize(m_width*0.952,m_height*0.81);
                m_gridLayout->addWidget(mapLabelWidget,1,1);
                // 连接点击信号
                connect(mapLabelWidget, &MapLabel::cityClicked, this, [&](const QString& name, const CityInfo& info){
                    QMessageBox::information(this, "城市信息",
                                             QString("城市: %1\n总设备: %2").arg(name).arg(info.totalDevices));
                });
                break;
                }
                case MAINTENANCE:{
                QTableWidget* maintenaceTableW = new QTableWidget();

                std::vector<std::string> firstColumnNames={"蠕动泵头","蠕动泵管","检测电极","电解电极","计量管活塞1","计量管活塞2"}; 
                std::vector<std::string> secondColumnNames={"6个月","3个月","2年","2年","1年","1年"};
                std::vector<int> mainanceCycleWithDays = {180,90,365*2,365*2,365,365};
                QStringList headers=QStringList{"设备项目","维护周期","维护人员", "维护倒计时"};
                maintenaceTableW=new QTableWidget(int(firstColumnNames.size()),headers.size()); 
                maintenaceTableW->setHorizontalHeaderLabels(headers); 
                maintenaceTableW->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
                // maintenaceTableW->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
                maintenaceTableW->verticalHeader()->setVisible(false);
                maintenaceTableW->horizontalHeader()->setVisible(true);
                maintenaceTableW->setShowGrid(true);
                maintenaceTableW->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color:rgb(0,176,240);color:rgb(0,0,0);}");
                maintenaceTableW->setFixedSize(width()*0.98,height()*0.8);

                QFont font = maintenaceTableW->horizontalHeader()->font();  // 获取水平表头字体
                font.setPointSize(36);  // 设置字体大小为14
                maintenaceTableW->horizontalHeader()->setFont(font);  // 应用到水平表头
                font = maintenaceTableW->font();
                font.setPointSize(18);  // 设置字体大小为12
                maintenaceTableW->setFont(font);
                std::map<std::string,std::string> mainMsg;
                mainMsg=DashboardRWDB::readMaintenanceInfo(m_dataType);

                for (int row = 0; row < int(firstColumnNames.size()); ++row)
                {
                    for (int col = 0; col < 4; ++col)
                    {
                        QTableWidgetItem *item = new QTableWidgetItem(QString("(%1,%2)").arg(row).arg(col));
                        QColor color;
                        if (col==0){
                            item->setText(QString::fromStdString(firstColumnNames[row]));
                            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                            color=QColor(131,201,98);
                        } else if (col==1){
                            item->setText(QString::fromStdString(secondColumnNames[row]));
                            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                            color=QColor(255,255,255);
                        } else if (col==2){
                            item->setText("");
                            item->setFlags(item->flags() | Qt::ItemIsEditable);
                            color=QColor(255,255,255);
                        } else if (col==3){
                            std::string timeStr = mainMsg[maintenaceTableW->item(row,0)->text().toStdString()];
                            HGExactTime curTime=HGExactTime::currentTime();
                            int countsecond = curTime.fasterThanThirtyMimutes(timeStr);
                            QString resultText;
                            if (countsecond < 0)
                            {
                                resultText = "X天/超时X天";
                            }
                            else
                            {
                                int countday = countsecond / 86400;
                                int maxDays = mainanceCycleWithDays[row];
                                int overdueDays = countday - maxDays;
                                int recountdays = maxDays - countday;
                                resultText = QString("%1天/超时%2天").arg(recountdays > 0?recountdays:0).arg(overdueDays > 0 ? overdueDays : 0);
                            }

                            item->setText(resultText);
                            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                            color=QColor(255,255,255);
                        }
                        item->setBackground(QBrush(color));
                        item->setTextAlignment(Qt::AlignCenter);
                        maintenaceTableW->setItem(row, col, item);
                    }
                }
                m_gridLayout->addWidget(maintenaceTableW,m_gridLayout->rowCount(),0,1,4);
                // 初始调整行高
                adjustRowHeight(maintenaceTableW);
                connect(maintenaceTableW, &QTableWidget::cellClicked, [=](int row, int col){
                    // 如果点击的是最后一列 (索引为 3)
                    if (col == 3) {
                        if (QMessageBox::Yes==QMessageBox::question(this,QString::fromStdString("维保"),
                            "确定维保"+maintenaceTableW->item(row,0)->text()+"?",QMessageBox::Yes | QMessageBox::No)){
                            maintenaceTableW->item(row,3)->setText(QString::number(mainanceCycleWithDays[row])+ "天/超时0天");
                            std::map<std::string,std::string> tempMsg;
                            HGExactTime curTime=HGExactTime::currentTime();
                            tempMsg[maintenaceTableW->item(row,0)->text().toStdString()]=curTime.toStringFromYearToSec();
                            DashboardRWDB::writeMaintenanceInfo(tempMsg,m_dataType);
                        }
                    } 
                });
                break;
            }
                case MONITOR:
                m_watchLabel[TUZHUANG]->setFixedSize(m_width*0.952,m_height*0.81);
                m_gridLayout->addWidget(m_watchLabel[TUZHUANG],1,1);
                break;
                default: break;
            }
        }
        break;
        case SHUIFEN:
        {
            switch (m_funcType){
                case REAL_DATA:
                m_gridLayout->addWidget(m_shuifenChannel1Curve,m_gridLayout->rowCount(),0,1,2);
                m_gridLayout->addWidget(m_shuifenChannel2Curve,m_gridLayout->rowCount()-1,2,1,2);
                break;
                case USER_DATA:{
                // 创建 MapLabel 并设置城市数据
                MapLabel* mapLabelWidget = new MapLabel(m_dataType);
                mapLabelWidget->setFixedSize(m_width*0.952,m_height*0.81);
                m_gridLayout->addWidget(mapLabelWidget,1,1);
                // 连接点击信号
                connect(mapLabelWidget, &MapLabel::cityClicked, this, [&](const QString& name, const CityInfo& info){
                    QMessageBox::information(this, "城市信息",
                                             QString("城市: %1\n总设备: %2").arg(name).arg(info.totalDevices));
                });
                break;
            }
                case MAINTENANCE:{
                QTableWidget* maintenaceTableW = new QTableWidget();

                std::vector<std::string> firstColumnNames={"蠕动泵头","蠕动泵管","手工全柜换气","干燥剂"}; 
                std::vector<std::string> secondColumnNames={"6个月","3个月","2周","1周"};
                std::vector<int> mainanceCycleWithDays = {180,90,14,7};
                QStringList headers=QStringList{"设备项目","维护周期","维护人员", "维护倒计时"};
                maintenaceTableW=new QTableWidget(int(firstColumnNames.size()),headers.size()); 
                maintenaceTableW->setHorizontalHeaderLabels(headers); 
                maintenaceTableW->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
                // maintenaceTableW->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
                maintenaceTableW->verticalHeader()->setVisible(false);
                maintenaceTableW->horizontalHeader()->setVisible(true);
                maintenaceTableW->setShowGrid(true);
                maintenaceTableW->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color:rgb(0,176,240);color:rgb(0,0,0);}");
                maintenaceTableW->setFixedSize(width()*0.98,height()*0.8);
                QFont font = maintenaceTableW->horizontalHeader()->font();  // 获取水平表头字体
                font.setPointSize(36);  // 设置字体大小为14
                maintenaceTableW->horizontalHeader()->setFont(font);  // 应用到水平表头
                font = maintenaceTableW->font();
                font.setPointSize(18);  // 设置字体大小为12
                maintenaceTableW->setFont(font);
                std::map<std::string,std::string> mainMsg;
                mainMsg=DashboardRWDB::readMaintenanceInfo(m_dataType);

                for (int row = 0; row < int(firstColumnNames.size()); ++row)
                {
                    for (int col = 0; col < 4; ++col)
                    {
                        QTableWidgetItem *item = new QTableWidgetItem(QString("(%1,%2)").arg(row).arg(col));
                        QColor color;
                        if (col==0){
                            item->setText(QString::fromStdString(firstColumnNames[row]));
                            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                            color=QColor(131,201,98);
                        } else if (col==1){
                            item->setText(QString::fromStdString(secondColumnNames[row]));
                            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                            color=QColor(255,255,255);
                        } else if (col==2){
                            item->setText("");
                            item->setFlags(item->flags() | Qt::ItemIsEditable);
                            color=QColor(255,255,255);
                        } else if (col==3){
                            std::string timeStr = mainMsg[maintenaceTableW->item(row,0)->text().toStdString()];
                            HGExactTime curTime=HGExactTime::currentTime();
                            int countsecond = curTime.fasterThanThirtyMimutes(timeStr);
                            QString resultText;
                            if (countsecond < 0)
                            {
                                resultText = "X天/超时X天";
                            }
                            else
                            {
                                int countday = countsecond / 86400;
                                int maxDays = mainanceCycleWithDays[row];
                                int overdueDays = countday - maxDays;
                                int recountdays = maxDays - countday;
                                resultText = QString("%1天/超时%2天").arg(recountdays > 0?recountdays:0).arg(overdueDays > 0 ? overdueDays : 0);
                            }

                            item->setText(resultText);
                            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                            color=QColor(255,255,255);
                        }
                        item->setBackground(QBrush(color));
                        item->setTextAlignment(Qt::AlignCenter);
                        maintenaceTableW->setItem(row, col, item);
                    }
                }
                m_gridLayout->addWidget(maintenaceTableW,m_gridLayout->rowCount(),0,1,4);

                // 初始调整行高
                adjustRowHeight(maintenaceTableW);
                connect(maintenaceTableW, &QTableWidget::cellClicked, [=](int row, int col){
                    // 如果点击的是最后一列 (索引为 3)
                    if (col == 3) {
                        if (QMessageBox::Yes==QMessageBox::question(this,QString::fromStdString("维保"),
                            "确定维保"+maintenaceTableW->item(row,0)->text()+"?",QMessageBox::Yes | QMessageBox::No)){
                            maintenaceTableW->item(row,3)->setText(QString::number(mainanceCycleWithDays[row])+ "天/超时0天");
                            std::map<std::string,std::string> tempMsg;
                            HGExactTime curTime=HGExactTime::currentTime();
                            tempMsg[maintenaceTableW->item(row,0)->text().toStdString()]=curTime.toStringFromYearToSec();
                            DashboardRWDB::writeMaintenanceInfo(tempMsg,m_dataType);
                        }
                    } 
                });
                break;
            }
                case MONITOR:
                m_watchLabel[TUZHUANG]->setFixedSize(m_width*0.952,m_height*0.81);
                m_gridLayout->addWidget(m_watchLabel[TUZHUANG],1,1);
                break;
                default: break;
            }
        }
        break;
        default: break;
    }
    
    // 设置行列的拉伸因子，使每个控件占据相同的空间
    m_gridLayout->setRowStretch(0, 2);  // 第一行的行拉伸因子
    m_gridLayout->setRowStretch(1, 1);  // 第二行的行拉伸因子
    m_gridLayout->setRowStretch(2, 1);
    m_gridLayout->setRowStretch(3, 5);
    m_gridLayout->setRowStretch(4, 1);
    m_gridLayout->setRowStretch(5, 5);
    m_gridLayout->setColumnStretch(0, 1);  // 第一列的列拉伸因子
    m_gridLayout->setColumnStretch(1, 1);  // 第二列的列拉伸因子
    m_gridLayout->setColumnStretch(2, 1); 
    m_gridLayout->setColumnStretch(3, 1); 
    for (int i=0;i<3;i++){
        if (i==m_dataType && m_funcType==REAL_DATA) {
            m_deviceStatusLabel[i]->show();
            m_errStatusLabel[i]->show();
        } else {
            m_deviceStatusLabel[i]->hide();
            m_errStatusLabel[i]->hide();
        }
    }
    for (int i=1;i<VIDEO_THREAD_NUM+1;i++){
        if (m_funcType==REAL_DATA) {
            m_watchLabel[i]->show();
        } else {
            m_watchLabel[i]->hide();
        }
    }
    showAllWidgetsFromGridLayout(m_gridLayout);
}
bool HGDashboardWidget::eventFilter(QObject* obj,QEvent* event){
    if (event->type()==QEvent::MouseButtonPress){
        QMouseEvent* mouseEvent=static_cast<QMouseEvent*>(event);
        if (mouseEvent->button()==Qt::LeftButton){
            if (obj==m_maintenanceLabel){
                m_realDataLabel->setPixmap(QPixmap(QString::fromStdString(m_basePath+"/LOGO/蓝-实时数据.png")).scaled(m_realDataLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
                m_mapLabel->setPixmap(QPixmap(QString::fromStdString(m_basePath+"/LOGO/蓝-用户分布.png")).scaled(m_mapLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
                m_maintenanceLabel->setPixmap(QPixmap(QString::fromStdString(m_basePath+"/LOGO/红-维护保养.png")).scaled(m_maintenanceLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
                m_monitorLabel->setPixmap(QPixmap(QString::fromStdString(m_basePath+"/LOGO/蓝-现场监控.png")).scaled(m_monitorLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
                m_lastFuncType=m_funcType;
                m_funcType=MAINTENANCE;
                displayRealData();
            } else if (obj==m_mapLabel){
                m_realDataLabel->setPixmap(QPixmap(QString::fromStdString(m_basePath+"/LOGO/蓝-实时数据.png")).scaled(m_realDataLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
                m_mapLabel->setPixmap(QPixmap(QString::fromStdString(m_basePath+"/LOGO/红-用户分布.png")).scaled(m_mapLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
                m_maintenanceLabel->setPixmap(QPixmap(QString::fromStdString(m_basePath+"/LOGO/蓝-维护保养.png")).scaled(m_maintenanceLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
                m_monitorLabel->setPixmap(QPixmap(QString::fromStdString(m_basePath+"/LOGO/蓝-现场监控.png")).scaled(m_monitorLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
                m_lastFuncType=m_funcType;
                m_funcType=USER_DATA;
                displayRealData();
            } else if (obj==m_monitorLabel){
                m_lastFuncType=m_funcType;
                m_funcType=MONITOR;
                displayRealData();
                m_realDataLabel->setPixmap(QPixmap(QString::fromStdString(m_basePath+"/LOGO/蓝-实时数据.png")).scaled(m_realDataLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
                m_mapLabel->setPixmap(QPixmap(QString::fromStdString(m_basePath+"/LOGO/蓝-用户分布.png")).scaled(m_mapLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
                m_maintenanceLabel->setPixmap(QPixmap(QString::fromStdString(m_basePath+"/LOGO/蓝-维护保养.png")).scaled(m_maintenanceLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
                m_monitorLabel->setPixmap(QPixmap(QString::fromStdString(m_basePath+"/LOGO/红-现场监控.png")).scaled(m_monitorLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            } else if (obj==m_realDataLabel){
                m_lastFuncType=m_funcType;
                m_funcType=REAL_DATA;
                displayRealData();
                m_realDataLabel->setPixmap(QPixmap(QString::fromStdString(m_basePath+"/LOGO/红-实时数据.png")).scaled(m_realDataLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
                m_mapLabel->setPixmap(QPixmap(QString::fromStdString(m_basePath+"/LOGO/蓝-用户分布.png")).scaled(m_mapLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
                m_maintenanceLabel->setPixmap(QPixmap(QString::fromStdString(m_basePath+"/LOGO/蓝-维护保养.png")).scaled(m_maintenanceLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
                m_monitorLabel->setPixmap(QPixmap(QString::fromStdString(m_basePath+"/LOGO/蓝-现场监控.png")).scaled(m_monitorLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

            } else if (obj==m_tuzhuangLabel){
                m_dataType=TUZHUANG;
                m_tuzhuangLabel->setPixmap(QPixmap(QString::fromStdString(m_basePath+"/LOGO/红-涂装.png")));
                m_didingLabel->setPixmap(QPixmap(QString::fromStdString(m_basePath+"/LOGO/蓝-滴定.png")));
                m_shuifenLabel->setPixmap(QPixmap(QString::fromStdString(m_basePath+"/LOGO/蓝-水分.png")));
                m_bkLabel->setPixmap(QPixmap(QString::fromStdString(m_basePath+"/LOGO/涂装.png")));
                displayRealData();
            } else if (obj==m_didingLabel){
                m_dataType=DIDING;
                m_tuzhuangLabel->setPixmap(QPixmap(QString::fromStdString(m_basePath+"/LOGO/蓝-涂装.png")));
                m_didingLabel->setPixmap(QPixmap(QString::fromStdString(m_basePath+"/LOGO/红-滴定.png")));
                m_shuifenLabel->setPixmap(QPixmap(QString::fromStdString(m_basePath+"/LOGO/蓝-水分.png")));
                m_bkLabel->setPixmap(QPixmap(QString::fromStdString(m_basePath+"/LOGO/滴定.png")));
                displayRealData();
            } else if (obj==m_shuifenLabel){
                m_dataType=SHUIFEN;
                m_tuzhuangLabel->setPixmap(QPixmap(QString::fromStdString(m_basePath+"/LOGO/蓝-涂装.png")));
                m_didingLabel->setPixmap(QPixmap(QString::fromStdString(m_basePath+"/LOGO/蓝-滴定.png")));
                m_shuifenLabel->setPixmap(QPixmap(QString::fromStdString(m_basePath+"/LOGO/红-水分.png")));
                m_bkLabel->setPixmap(QPixmap(QString::fromStdString(m_basePath+"/LOGO/水分.png")));
                displayRealData();
            } 
        }
    }
    return QWidget::eventFilter(obj,event);
}