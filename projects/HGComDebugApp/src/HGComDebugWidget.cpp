#include "HGComDebugWidget.h"
#include "hgserial.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include "hglog4cplus.h"
#include "hgcommonutility.h"

using namespace HGMACHINE;

#define COM_HEAD                        0xEE                // head
#define COM_CODE_SWITCH_PAGE            0xB100              // 切换画面指令
#define COM_CODE_READ_PAGE              0xB101              // 读取画面
#define COM_CODE_FOCUS                  0xB102              // 设置光标焦点
#define COM_CODE_HIDE_CONTROL           0xB103              // 屏蔽/隐藏控件
#define COM_CODE_ENABLE_CONTROL         0xB104              // 禁止/使能控件
#define COM_CODE_SWITCH_PAGE_AVI        0xB105              // 使用动画效果切换画面
#define COM_CODE_TEXT_FORMAT            0xB107              // 格式化文本显示
#define COM_CODE_SEND                   0xB110              // 发送的组态控件指令
#define COM_CODE_UPLOAD                 0xB111              // 上传／接收的组态控件指令
#define COM_CODE_BATCH_UPDATE_VALUE     0xB112              // 批量更新控件数值
#define COM_CODE_TEXT_BLINK_CYCLE       0xB115              // 设置文本控件闪烁周期
#define COM_CODE_TEXT_ROLL_SPEED        0xB116              // 设置文本控件滚动速度
#define COM_CODE_TEXT_BK_TRANSPARENT    0xB117              // 设置文本背景透明
#define COM_CODE_TEXT_BK_COLOR          0xB118              // 设置控件背景色
#define COM_CODE_TEXT_FR_COLOR          0xB119              // 设置控件前景色
#define COM_CODE_CONTROL_START_AVI      0xB120              // 启动动画播放控件
#define COM_CODE_CONTROL_STOP_AVI       0xB121              // 停止动画播放控件
#define COM_CODE_CONTROL_PAUSE_AVI      0xB122
#define COM_CODE_ICON                   0xB123              // 图标控件指令
#define COM_TAIL1                       0xFF
#define COM_TAIL2                       0xFC
#define COM_TAIL3                       0xFF
#define COM_TAIL4                       0xFF

HGComDebugWidget::HGComDebugWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(800,800);

    m_elapsedLabel = new QLabel(this);
    m_elapsedLabel->move(20,500);
    m_elapsedLabel->setText("elapsed:");

    this->show();
    m_valid=false;
    openSerial();
    m_listenTimer=NULL;
    m_listenTimer=new QTimer();
    connect(m_listenTimer,SIGNAL(timeout()),this,SLOT(fnSlotListenTimerOut()));

    if (m_valid) m_listenTimer->start(1000);


}

HGComDebugWidget::~HGComDebugWidget()
{
    if (m_listenTimer==NULL){
        delete m_listenTimer;
        m_listenTimer=NULL;
    }
    // HGSerial::close();

}
void HGComDebugWidget::openSerial()
{
//     if (0==HGSerial::open("/dev/ttyUSB0")) m_valid=true;
//     else return;
//     int stop=1;
//     int bits=8;
//     char event='N';
//     unsigned int speed=115200;
//     HGSerial::set(speed,bits,event,stop);
//     HGSERIAL_STANDARD_INFO hgserial_info;
//     hgserial_info.head.push_back(COM_HEAD);
//     hgserial_info.tail.push_back(COM_TAIL1);
//     hgserial_info.tail.push_back(COM_TAIL2);
//     hgserial_info.tail.push_back(COM_TAIL3);
//     hgserial_info.tail.push_back(COM_TAIL4);
//     hgserial_info.function["SwitchPage"]        ={0xB1,0x00};
//     hgserial_info.function["ReadPage"]          ={0xB1,0x01};
//     hgserial_info.function["SetFocus"]          ={0xB1,0x02};              // 设置光标焦点
//     hgserial_info.function["HideControl"]       ={0xB1,0x03};              // 屏蔽/隐藏控件
//     hgserial_info.function["EnableControl"]     ={0xB1,0x04};              // 禁止/使能控件
//     hgserial_info.function["SwitchPageWAVI"]    ={0xB1,0x05};              // 使用动画效果切换画面
//     hgserial_info.function["FormatText"]        ={0xB1,0x07};             // 格式化文本显示
//     hgserial_info.function["Send"]              ={0xB1,0x10};             // 发送的组态控件指令
//     hgserial_info.function["Recv"]              ={0xB1,0x11};             // 上传／接收的组态控件指令
//     hgserial_info.function["BatchUpdateValue"]  ={0xB1,0x12};             // 批量更新控件数值
// //    hgserial_info.function[""]={TEXT_BLINK_CYCLE       0xB1,0x15};             // 设置文本控件闪烁周期
// //    hgserial_info.function[""]={TEXT_ROLL_SPEED        0xB1,0x16};             // 设置文本控件滚动速度
// //    hgserial_info.function[""]={TEXT_BK_TRANSPARENT    0xB1,0x17};             // 设置文本背景透明
// //    hgserial_info.function[""]={TEXT_BK_COLOR          0xB1,0x18};             // 设置控件背景色
// //    hgserial_info.function[""]={TEXT_FR_COLOR          0xB1,0x19};             // 设置控件前景色
// //    hgserial_info.function[""]={CONTROL_START_AVI      0xB1,0x20};             // 启动动画播放控件
// //    hgserial_info.function[""]={CONTROL_STOP_AVI       0xB1,0x21};             // 停止动画播放控件
// //    hgserial_info.function[""]={CONTROL_PAUSE_AVI      0xB1,0x22};
// //#define COM_CODE_ICON                   0xB123              // 图标控件指令
//     HGSerial::setStandardInfo(hgserial_info);
}
void HGComDebugWidget::fnSlotListenTimerOut()
{
    int timeout = 1000;
    std::vector<uint8_t> contents;

    // HGExactTime start=HGGetTime();
    // HGExactTime end;
    // int ret = HGSerial::read(contents,timeout);
    // end = HGGetTime();
    // std::ostringstream ss;
    // ss<<"read time:"<<HGCalTimeElapsed(start,end);
    // printf("%s ms\n",ss.str().c_str());
    // if (ret < 0){
    //     printf("Error reading serial port.\n");
    // } else {
//        std::stringstream ss;
//        ss<<std::hex;
//        printf("len:%d\n",int(contents.size()));
////        HGLog4Cplus::getLogInstance(LOG_PATH)->logout(std::to_string(contents.size()),LOGINFO);
//        for (int i=0;i<int(contents.size());i++){
//            ss<<std::setw(2)<<std::setfill('0')<<(int)contents[i]<<"  ";
//        }
//        std::string hex_str=ss.str();
//        printf("\n[hex]:%s\n",hex_str.c_str());
////        HGLog4Cplus::getLogInstance(LOG_PATH)->logout(hex_str,LOGINFO);
        // start=HGGetTime();
        // HGSerial::deSerialize(contents);
        // end=HGGetTime();
        // double timeElapsed=HGCalTimeElapsed(start,end);
        // ss.str("");
        // ss<<"elpased:"<<timeElapsed<<" ms";
        // printf("time:%s\n",ss.str().c_str());
    // }

    // m_elapsedLabel->setText(QString::fromStdString(ss.str()));
}

