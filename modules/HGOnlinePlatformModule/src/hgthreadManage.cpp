#include "hglog4cplus.h"
#include "hgcommonutility.h"
#include "hgthreadManage.h"
#include <string.h>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#else
#include <sys/wait.h>
#include <spawn.h>
#include "hgserial.h"
#endif
#include <iomanip>
#include <iostream>
#include "hgsavedatatodb.h"
#include "hgonlinerwDB.h"
#include <sys/types.h>
#include <cstdlib>
#include <fcntl.h>
#include "structUpperPcToLowerPC.h"
#include "HGMacroData.h"


using namespace HGMACHINE;

namespace HGMACHINE{
    #ifdef ENABLE_CAS6
    std::vector<std::string> gCAS6MCUResult={"","未就绪","错误","等待","检测中","已就绪","异常"};
    std::vector<std::string> gCAS6MCUResultInfo={
        "",
        "已准备就绪，等待%1秒测试漂移值 ！","延时等待，%1秒开始电解","进样倒计时%1秒","进样间隔倒计时%1秒","", 
        "准备进样%1秒","","","","",
        "请平衡仪器","载气流速偏低","温度超过极限温度","温度未稳定","请稍候，未平衡......","开始自动平衡(微量)......",
        "请稍候，开始自动平衡(High)","请稍候，开始自动平衡(Low)","正在自动平衡(微量)......","正在自动平衡(High)......",
        "正在自动平衡(Low)......","正在自动平衡(过碘)......","请稍候，正在分析样品(微量)","请稍候，正在分析样品(High)","请稍候，正在分析样品(Low)",
        "请稍候，正在分析样品","请稍候，分析计算中","请稍候，接近终点......","过水，请稍候......","请稍候，正在分析样品.......","请按换试剂键更换试剂",
        "过碘，请等待或注入少量水后按平衡开始!","请按开始键后，样品测量!","过水，请稍候......","分析完成，等待漂移计算",
        "样品分析完成!","固定时长分析样品完成!","仪器到达强制停止时间","请按平衡开始....","电解电极断路......","测量电极断路......",
        "测量电极短路......","正在自检","加热板连接失败","自动进样器连接失败","加热板连接成功","自动进样器连接成功","流量传感器错误","温度传感器错误","自检结束",
        "正在打开气源","气源已打开","正在吹扫，等待温度稳定","正在加热","取样针上抬",
        "取样针已到顶部","取样针下穿","取样针已到底部","正在转动盘位","磁力搅拌自检失败","细针自检失败","粗针自检失败","细针下限位检测失败","粗针下限位检测失败","圆盘零位自检失败",
        "细针正在自检","粗针正在自检","圆盘正在自检","粗针下穿超时","转盘自动校正中","","","","",""

    };
    std::map<std::string,std::vector<uint8_t>> gMapSendMethod={
        {"终点电位",{0x00, 0x05}},
        {"终点漂移",{0x00, 0x03}},
        {"起始漂移",{0x00, 0x28}},
        {"终点延时",{0x00, 0x32}},
        {"校正因子",{0x00, 0x06}},
        {"最短时长",{0x00, 0x02}},
        {"最长时长",{0x00, 0x18}},
        {"萃取时间",{0x00, 0x16}},
        {"终止时间",{0x00, 0x0e}},
        {"电解电极",{0x00, 0x0b}},
        {"风扇"   ,{0x01, 0x11}},
        {"加热伴管",{0x01, 0x12}},
        {"初始温度",{0x01, 0x0e}},
        {"恒温时间",{0x01, 0x06}},
        {"程序升温",{0x01, 0x13}},
        {"速率1"  ,{0x01, 0x0b}},
        {"速率2"  ,{0x01, 0x0c}},
        {"速率3"  ,{0x01, 0x0d}},
        {"温度1"  ,{0x01, 0x03}},
        {"温度2"  ,{0x01, 0x04}},
        {"温度3"  ,{0x01, 0x05}},
        {"保温1"  ,{0x01, 0x08}},
        {"保温2"  ,{0x01, 0x09}},
        {"保温3"  ,{0x01, 0x0a}}
    };
    std::map<std::string,std::vector<uint8_t>> gMapRecvMethod={
        {"风扇状态",{0x00, 0x1d}},
        {"加热温度",{0x00, 0x24}},
        {"当前温度",{0x00, 0x25}},
        {"气流速度",{0x00, 0x37}},
        {"电压",{0x00, 0x11}},
        {"电解速度",{0x00, 0x28}},
        {"时间",{0x00, 0x10}},
        {"水质量",{0x00, 0x0f}},
        {"状态结果",{0x00, 0x1b}},
        {"标志",{0x00, 0x36}},
        {"漂移",{0x00, 0x1e}},
        {"空白",{0x01, 0x0e}}//,
        // {"恒温时间",{0x01, 0x06}},
        // {"程序升温",{0x01, 0x13}},
        // {"速率1"  ,{0x01, 0x0b}},
        // {"速率2"  ,{0x01, 0x0c}},
        // {"速率3"  ,{0x01, 0x0d}},
        // {"温度1"  ,{0x01, 0x03}},
        // {"温度2"  ,{0x01, 0x04}},
        // {"温度3"  ,{0x01, 0x05}},
        // {"保温1"  ,{0x01, 0x08}},
        // {"保温2"  ,{0x01, 0x09}},
        // {"保温3"  ,{0x01, 0x0a}}
    };
    #endif
void *threadRecvCommFunc(void *pUser)
{
    if (setCpu(1))
    {
        printf("set cpu error\n");
    }
    // enter in class function
    HgThreadManage *pHGThreadManage = (HgThreadManage *)pUser;
    if (NULL == pHGThreadManage)
    {
        char *res = (char *)("comm recv thread failed");
        return res;
    }

    pHGThreadManage->runRecv();

    // std::ostringstream logtext;
    // logtext << /*CARLAMPAPPNAME << */"quit comm recv thread";
    // HGLog4Cplus::getLogInstance(LOG_PATH)->logout(logtext.str(),LOGINFO);
    // pthread_exit(NULL);
    return NULL;
}
void *threadSendCommFunc(void *pUser)
{
    if (setCpu(1))
    {
        printf("set cpu error\n");
    }
    // enter in class function
    HgThreadManage *pHGThreadManage = (HgThreadManage *)pUser;
    if (NULL == pHGThreadManage)
    {
        char *res = (char *)("com send thread failed");
        return res;
    }

    pHGThreadManage->runSend();
      
    std::ostringstream logtext;
    logtext <</* CARLAMPAPPNAME <<*/ "quit comm send thread";
    HGLog4Cplus::getLogInstance(LOG_PATH)->logout(logtext.str(),LOGINFO);
    // pthread_exit(NULL);
    return NULL;
}
void *threadBreakProcessFunc(void *pUser){
    if (setCpu(1))
    {
        printf("set cpu error\n");
    }
    // enter in class function
    HgThreadManage *pHGThreadManage = (HgThreadManage *)pUser;
    if (NULL == pHGThreadManage)
    {
        char *res = (char *)("com send thread failed");
        return res;
    }

    pHGThreadManage->runBreakProcess();
      
    std::ostringstream logtext;
    logtext <<"quit break process thread";
    HGLog4Cplus::getLogInstance(LOG_PATH)->logout(logtext.str(),LOGINFO);
    // pthread_exit(NULL);
    return NULL;
}
void *listenProtectProcess(void *pUser){
    if (setCpu(1)){
        printf("set cpu error\n");
    }
    HgThreadManage *pHGThreadManage = (HgThreadManage *)pUser;
    if (NULL==pHGThreadManage){
        char *res = (char *)("listen protect process thread failed");
        return res;
    }
    pHGThreadManage->runListenProtectProcess();
    std::ostringstream logtext;
    logtext <<"quit listen protect process thread";
    HGLog4Cplus::getLogInstance(LOG_PATH)->logout(logtext.str(),LOGINFO);
    // pthread_exit(NULL);
    return NULL;
}
HgThreadManage::HgThreadManage()
{
    m_makeSampleFlag=false;
    m_recvComThreadFlag=FLAGEND;
    m_sendComThreadFlag=FLAGEND;
    m_listenProtectProcessFlag=FLAGEND;
    m_threadBreakFlag=FLAGEND;
    std::ostringstream logtext;
    int res=pthread_create(&m_listenProtectProcess,NULL,listenProtectProcess,this);
    if (res!=0){
        char *errstr = strerror(res);
        logtext.str("");
        logtext << "create thread listen protect process failed,ret:" << errstr;
        HGLog4Cplus::getLogInstance(LOG_PATH)->logout(logtext.str(),LOGERROR);
        m_exitTrd = FLAGEND;
        return ;
    }
    // HGOnlineRWDB::deleteDeviceFromLPCManageInfo(DeviceFromLPCManageInfoDBNAME);
    m_comPort=HgOnlinePlatformModule::getComPort();
    m_sendInfo.count=0;
    m_deviceRunStatuss.push_back(DEVICE_INIT);
    initSendInfoCAS6();
    m_styleALT=new StyleALT(LOAD,HgOnlinePlatformModule::getTaskSeqName());

    if (0!=commOpen()) {
        HGLog4Cplus::getLogInstance(LOG_PATH)->logout("打开串口失败",LOGERROR);
        printf("open comm failed\n");
        m_connectWithMCUStatus=LINK_OFFLINE;
        m_cur_mcu_status="打开串口失败";
        return;
    }

    res = pthread_create(&threadRecvComm, NULL, threadRecvCommFunc, this);
    if (res != 0)
    {
        char *errstr = strerror(res);
        logtext.str("");
        logtext << /*CARLAMPAPPNAME << */"create thread recv failed,ret:" << errstr;
        HGLog4Cplus::getLogInstance(LOG_PATH)->logout(logtext.str(),LOGERROR);
        m_exitTrd = FLAGEND;
        return ;
    }
    res = pthread_create(&threadSendComm, NULL, threadSendCommFunc, this);
    if (res != 0)
    {
        char *errstr = strerror(res);
        logtext.str("");
        logtext << /*CARLAMPAPPNAME << */"create thread send failed,ret:" << errstr;
        HGLog4Cplus::getLogInstance(LOG_PATH)->logout(logtext.str(),LOGERROR);
        m_exitTrd = FLAGEND;
        return ;
    }
    res=pthread_create(&m_threadBreak, NULL, threadBreakProcessFunc, this);
    if (res!=0){
        char *errstr = strerror(res);
        logtext.str("");
        logtext << "create thread process break failed,ret:" << errstr;
        HGLog4Cplus::getLogInstance(LOG_PATH)->logout(logtext.str(),LOGERROR);
        m_exitTrd = FLAGEND;
        return ;
    }
    m_connectWithMCUStatus=LINK_ONLINE;
}
HgThreadManage::~HgThreadManage()
{
    try
    {
        m_serial.close();
        if (m_listenProtectProcessFlag == FLAGSTART)
        {
            if (pthread_join(m_listenProtectProcess, nullptr) != 0)
            {
                HGLog4Cplus::getLogInstance(LOG_PATH)->logout("Failed to join thread daemon", LOGERROR);
            }
        }
        if (m_threadBreakFlag == FLAGSTART)
        {
            if (pthread_join(m_threadBreak, nullptr) != 0)
            {
                HGLog4Cplus::getLogInstance(LOG_PATH)->logout("Failed to join thread break", LOGERROR);
            }
        }
        if (m_recvComThreadFlag == FLAGSTART)
        {
            if (pthread_join(threadRecvComm, nullptr) != 0)
            {
                HGLog4Cplus::getLogInstance(LOG_PATH)->logout("Failed to join thread recv", LOGERROR);
            }
        }
        if (m_sendComThreadFlag == FLAGSTART)
        {
            if (pthread_join(threadSendComm, nullptr) != 0)
            {
                HGLog4Cplus::getLogInstance(LOG_PATH)->logout("Failed to join thread send", LOGERROR);
            }
        }
        std::ostringstream logtext;
        logtext << "release thread";
        HGLog4Cplus::getLogInstance(LOG_PATH)->logout(logtext.str(), LOGINFO);
    }
    catch (const std::exception &e)
    {
        // 记录异常信息，确保程序继续执行以释放其他资源
        HGLog4Cplus::getLogInstance(LOG_PATH)->logout("Exception in destructor: " + std::string(e.what()), LOGERROR);
    }
}

void HgThreadManage::startThread(){
    m_exitTrd=FLAGSTART;
}
void HgThreadManage::startRunTaskSequence(){
    m_deviceRunStatus=DEVICE_TASK_RUN;
}
void HgThreadManage::pauseTaskSequence(){
    m_deviceRunStatus=DEVICE_WAIT;
}
void HgThreadManage::resetTaskSequence(){
    m_deviceRunStatus=DEVICE_RESET;
}
void HgThreadManage::stopThread(){
    m_exitTrd=FLAGEND;
}
void HgThreadManage::runRecv(){
    m_recvComThreadFlag=FLAGSTART;
    do {
        readBuffers();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    } while(FLAGSTART==m_exitTrd);
    printf("serial end recv \n");
    HGLog4Cplus::getLogInstance(LOG_PATH)->logout("serial end recv",LOGINFO);
    HGOnlineRWDB::clearCAS6DeviceStatus();
}
void HgThreadManage::updateFlowInfo(){
    m_styleALT->updateFlowInfo();
}
void HgThreadManage::updateStyleInfo(){
    SAFE_DELETE(m_styleALT);
    m_styleALT=new StyleALT(LOAD,HgOnlinePlatformModule::getTaskSeqName());
}
std::vector<uint8_t> doubleToAsciiVector(double value) {
    std::ostringstream oss;
    oss << std::setprecision(15) << std::noshowpoint << std::fixed;

    // 用 std::ostringstream 保留有效小数位
    oss << value;

    std::string str = oss.str();

    // 去除末尾多余的 .0
    // e.g. 1.0000 -> "1"
    if (str.find('.') != std::string::npos) {
        // 去除末尾 0
        while (!str.empty() && str.back() == '0') {
            str.pop_back();
        }
        // 若末尾为 '.'，也去掉
        if (!str.empty() && str.back() == '.') {
            str.pop_back();
        }
    }

    // 转换成 uint8_t 向量
    std::vector<uint8_t> asciiBytes;
    for (char c : str) {
        asciiBytes.push_back(static_cast<uint8_t>(c));
    }

    return asciiBytes;
}
void HgThreadManage::setRunStatus(int status) 
{
    m_deviceRunStatuss.push_back(status);
    // initSendInfoCAS6();
}
void HgThreadManage::initSendInfoCAS6(){
    if (m_deviceRunStatuss.size() <=0) return;
    {
        int type=m_deviceRunStatuss[0];
        switch (type)
        {
        case DEVICE_INIT:
            printf("device init\n");
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout("device init", LOGINFO);
            m_sendbytes.push_back(std::vector<uint8_t>{
                0xEE, 0xB1, 0x11, 0x00, 0x03, 0x00, 0x2C, 0x10, 0x01, 0x01, 0xFF, 0xFC, 0xFF, 0xFF}); // get version
            break;
        case SWIRL_SET_INIT1:
        {
            printf("swirl set init1\n");
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout("swirl set init1", LOGINFO);
            m_sendbytes.push_back(std::vector<uint8_t>{
                0xEE, 0xB1, 0x11, 0x00, 0x03, 0x00, 0x0E, 0x10, 0x01, 0x01, 0xFF, 0xFC, 0xFF, 0xFF});
            m_cas6_swirl_speed = 1;
            break;
        }
        case HEAT_ON:
            printf("heat on\n");
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout("heat on", LOGINFO);
            m_sendbytes.push_back(std::vector<uint8_t>{
                0xEE, 0xB1, 0x11, 0x00, 0x03, 0x00, 0x0C, 0x10, 0x01, 0x01, 0xFF, 0xFC, 0xFF, 0xFF});
            break;
        case HEAT_OFF:
            printf("heat off\n");
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout("heat off", LOGINFO);
            m_sendbytes.push_back(std::vector<uint8_t>{
                0xEE, 0xB1, 0x11, 0x00, 0x03, 0x00, 0x0C, 0x10, 0x01, 0x00, 0xFF, 0xFC, 0xFF, 0xFF});
            break;
        case FAN_ON:
            printf("fan on\n");
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout("fan on", LOGINFO);
            m_sendbytes.push_back(std::vector<uint8_t>{
                0xEE, 0xB1, 0x14, 0x00, 0x06, 0x00, 0x11, 0x1A, 0x00, 0x00, 0xFF, 0xFC, 0xFF, 0xFF});
            break;
        case FAN_OFF:
            printf("fan off\n");
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout("fan off", LOGINFO);
            m_sendbytes.push_back(std::vector<uint8_t>{
                0xEE, 0xB1, 0x14, 0x00, 0x06, 0x00, 0x11, 0x1A, 0x01, 0x00, 0xFF, 0xFC, 0xFF, 0xFF});
            break;
        case PUMP_ON:
        {
            printf("pump on\n");
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout("pump on", LOGINFO);
            m_sendbytes.push_back(std::vector<uint8_t>{
                0xEE, 0xB1, 0x11, 0x00, 0x09, 0x00, 0x09, 0x10, 0x01, 0x01, 0xFF, 0xFC, 0xFF, 0xFF});
            break;
        }
        case PUMP_OFF:
        {
            printf("pump off\n");
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout("pump off", LOGINFO);
            m_sendbytes.push_back(std::vector<uint8_t>{
                0xEE, 0xB1, 0x11, 0x00, 0x09, 0x00, 0x09, 0x10, 0x01, 0x00, 0xFF, 0xFC, 0xFF, 0xFF});
            break;
        }
        case READ_SWIRL_SPEED:
        {
            printf("read swirl speed\n");
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout("read swirl speed", LOGINFO);
            m_sendbytes.push_back(std::vector<uint8_t>{
                0xEE, 0xB1, 0x11, 0x00, 0x03, 0x00, 0x0E, 0x10, 0x01, 0x00, 0xFF, 0xFC, 0xFF, 0xFF});
            break;
        }
        case SWIRL_ASCEND:
        {
            printf("swirl ascend\n");
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout("swirl ascend", LOGINFO);
            m_cas6_swirl_speed++;
            if (m_cas6_swirl_speed > SWIRL_SPEED_MAX)
                m_cas6_swirl_speed = SWIRL_SPEED_MAX;
            m_sendbytes.push_back(std::vector<uint8_t>{
                0xEE, 0xB1, 0x11, 0x00, 0x03, 0x00, 0x0E, 0x10, 0x01, static_cast<uint8_t>(m_cas6_swirl_speed), 0xFF, 0xFC, 0xFF, 0xFF});
            break;
        }
        case SWIRL_DESCEND:
        {
            printf("swirl descend\n");
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout("swirl descend", LOGINFO);
            m_cas6_swirl_speed--;
            if (m_cas6_swirl_speed < SWIRL_SPEED_MIN)
                m_cas6_swirl_speed = SWIRL_SPEED_MIN;
            m_sendbytes.push_back(std::vector<uint8_t>{
                0xEE, 0xB1, 0x11, 0x00, 0x03, 0x00, 0x0E, 0x10, 0x01, static_cast<uint8_t>(m_cas6_swirl_speed), 0xFF, 0xFC, 0xFF, 0xFF});
            break;
        }
        case NEEDLE_RELEASE:
        {
            printf("needle release\n");
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout("needle release", LOGINFO);
            m_sendbytes.push_back(std::vector<uint8_t>{
                // EEB1 11 00 09 00 06 10 01 01 FF FC FF FF
                0xEE, 0xB1, 0x11, 0x00, 0x09, 0x00, 0x06, 0x10, 0x01, 0x01, 0xFF, 0xFC, 0xFF, 0xFF});
            break;
        }
        case CIRCLE_EXECUTE:
        {
            printf("circle execute\n");
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout("circle execute", LOGINFO);
            m_sendbytes.push_back(std::vector<uint8_t>{
                // EE B1 11 00 09 00 0A 10 01 01 FF FC FF FF
                0xEE, 0xB1, 0x11, 0x00, 0x09, 0x00, 0x0A, 0x10, 0x01, 0x01, 0xFF, 0xFC, 0xFF, 0xFF});
            break;
        }
        case SET_CIRCLE_NUMBER:
        {
            printf("set circle number\n");
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout("set circle number", LOGINFO);
            m_sendbytes.push_back(std::vector<uint8_t>{
                // EE B1 1100 09 00 04 11 盘位号 00 FF FC FF FF
                0xEE, 0xB1, 0x11, 0x00, 0x09, 0x00, 0x04, 0x11, static_cast<uint8_t>(m_circle_number + 48), 0x00, 0xFF, 0xFC, 0xFF, 0xFF});
            break;
        }
        case SET_CUR_CIRCLE_NO:
        {
            printf("set cur circle no\n");
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout("set cur circle no", LOGINFO);
            m_sendbytes.push_back(std::vector<uint8_t>{
                // ee b1 11 00 08 00 03 11 盘号 00 ff fc ff ff
                0xEE, 0xB1, 0x11, 0x00, 0x08, 0x00, 0x03, 0x11, static_cast<uint8_t>(m_cur_circle_no + 48), 0x00, 0xFF, 0xFC, 0xFF, 0xFF});
            break;
        }
        case NEEDLE_ARRIVAL_LIMIT_POS:
        {
            printf("needle arrival limit pos\n");
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout("needle arrival limit pos", LOGINFO);
            m_sendbytes.push_back(std::vector<uint8_t>{
                // EE B1 11 00 09 00 07 10 01 00 FF FC FF FF
                0xEE, 0xB1, 0x11, 0x00, 0x09, 0x00, 0x07, 0x10, 0x01, 0x00, 0xFF, 0xFC, 0xFF, 0xFF});
            break;
        }
        case NEEDLE_PUNCTURE_RELEASE:
        {
            printf("needle puncture release\n");
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout("needle puncture release", LOGINFO);
            m_sendbytes.push_back(std::vector<uint8_t>{
                // EE B1 11 00 03 00 23 10 01 00 FF FC FF FF
                0xEE, 0xB1, 0x11, 0x00, 0x03, 0x00, 0x23, 0x10, 0x01, 0x00, 0xFF, 0xFC, 0xFF, 0xFF});
            break;
        }
        case NEEDLE_PUNCTURE_STEP:
        {
            printf("needle puncture step\n");
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout("needle puncture step", LOGINFO);
            m_sendbytes.push_back(std::vector<uint8_t>{
                // EE B1 11 00 03 00 26 10 01 00 FF FC FF FF
                0xEE, 0xB1, 0x11, 0x00, 0x03, 0x00, 0x26, 0x10, 0x01, 0x00, 0xFF, 0xFC, 0xFF, 0xFF});
            break;
        }
        case SET_NEEDLE_PUNCTURE_STEP:
        {
            printf("set needle puncture step\n");
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout("set needle puncture step", LOGINFO);
            //
            auto vec = doubleToAsciiVector(m_needle_puncture_step);
            m_sendbytes.push_back(std::vector<uint8_t>{
                // EE B1 11 00 03 00 25 11 31 2e 36 00 FF FC FF FF
                0xEE, 0xB1, 0x11, 0x00, 0x03, 0x00, 0x25, 0x11 //, 0x00, 0xFF, 0xFC, 0xFF, 0xFF
            });
            for (uint8_t c : vec)
            {
                m_sendbytes[m_sendbytes.size() - 1].push_back(c);
            }
            m_sendbytes[m_sendbytes.size() - 1].push_back(0x00);
            m_sendbytes[m_sendbytes.size() - 1].push_back(0xFF);
            m_sendbytes[m_sendbytes.size() - 1].push_back(0xFC);
            m_sendbytes[m_sendbytes.size() - 1].push_back(0xFF);
            m_sendbytes[m_sendbytes.size() - 1].push_back(0xFF);
            break;
        }
        case SET_PUNCTURE_POS_INDEX:
        {
            printf("set puncture pos index\n");
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout("set puncture pos index", LOGINFO);
            m_sendbytes.push_back(std::vector<uint8_t>{
                // EE B1 11 00 03 00 2f 10 01 01 FF FC FF FF
                0xEE, 0xB1, 0x11, 0x00, 0x03, 0x00, 0x2f, 0x10, 0x01, static_cast<uint8_t>(m_puncture_position_index), 0xFF, 0xFC, 0xFF, 0xFF});
            break;
        }
        case NEEDLE_PUNCTURE_TARGET_POS:
        {
            printf("needle puncture target pos\n");
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout("needle puncture target pos", LOGINFO);
            // EE B1 11 00 09 00 07 10 01 01 FF FC FF FF
            m_sendbytes.push_back(std::vector<uint8_t>{
                // EE B1 11 00 09 00 07 10 01 01 FF FC FF FF
                0xEE, 0xB1, 0x11, 0x00, 0x09, 0x00, 0x07, 0x10, 0x01, 0x01, 0xFF, 0xFC, 0xFF, 0xFF});
            break;
        }
        case GET_NEEDLE_PUNCTURE_CUR_POS:
        {
            printf("get needle puncture cur pos\n");
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout("get needle puncture cur pos", LOGINFO);
            // EE B1 11 00 03 00 2e 10 00 05 FF FC FF FF
            m_sendbytes.push_back(std::vector<uint8_t>{
                // EE B1 11 00 03 00 2e 10 00 05 FF FC FF FF
                0xEE, 0xB1, 0x11, 0x00, 0x03, 0x00, 0x2e, 0x10, 0x00, 0x05, 0xFF, 0xFC, 0xFF, 0xFF});
            break;
        }
        case MAKE_A_SAMPLE:
        {
            printf("make a sample\n");
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout("make a sample", LOGINFO);
            // ee b1 11 00 03 00 04 10 01 01 ff fc ff ff
            m_sendbytes.push_back(std::vector<uint8_t>{
                // ee b1 11 00 03 00 04 10 01 01 ff fc ff ff
                0xEE, 0xB1, 0x11, 0x00, 0x03, 0x00, 0x04, 0x10, 0x01, 0x01, 0xFF, 0xFC, 0xFF, 0xFF});
            break;
        }
        case SEND_METHOD_PARAM:
        {
            printf("send method param\n");
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout("send method param", LOGINFO);
            m_sendbytes.push_back(std::vector<uint8_t>{
                // ee b1 12 00 05 2个字节为编号 2个字节为内容大小 内容 … FF FC FF FF
                0xEE, 0xB1, 0x12, 0x00, 0x05});
            // printf("sendMethodValues size:%d\n",int(m_sendMethodValues.size()));
            // 遍历
            for (const auto &pair : m_sendMethodValues)
            {
                const std::vector<uint8_t> &key = pair.first;
                const std::vector<uint8_t> &value = pair.second;

                for (const uint8_t &byte : key)
                {
                    m_sendbytes[m_sendbytes.size() - 1].push_back(byte);
                }
                // 计算 value 的长度
                uint16_t length = static_cast<uint16_t>(value.size());
                // 分高字节和低字节
                uint8_t highByte = (length >> 8) & 0xFF;
                uint8_t lowByte = length & 0xFF;
                m_sendbytes[m_sendbytes.size() - 1].push_back(highByte);
                m_sendbytes[m_sendbytes.size() - 1].push_back(lowByte);
                for (const uint8_t &byte : value)
                {
                    m_sendbytes[m_sendbytes.size() - 1].push_back(byte);
                }
            }

            m_sendbytes[m_sendbytes.size() - 1].push_back(0xFF);
            m_sendbytes[m_sendbytes.size() - 1].push_back(0xFC);
            m_sendbytes[m_sendbytes.size() - 1].push_back(0xFF);
            m_sendbytes[m_sendbytes.size() - 1].push_back(0xFF);
            break;
        }
        case REQUEST_DATA:
        {
            // printf("request data\n");
            // HGLog4Cplus::getLogInstance(LOG_PATH)->logout("request data",LOGINFO);
            m_sendbytes.push_back(std::vector<uint8_t>{
                // ee b1 11 00 03 00 29 10 01 01 ff fc ff ff
                0xEE, 0xB1, 0x11, 0x00, 0x03, 0x00, 0x29, 0x10, 0x01, 0x01, 0xFF, 0xFC, 0xFF, 0xFF});
            break;
        }
        case DEVICE_WAIT:
            break;
        }
    }
}
void HgThreadManage::initSendInfo(int type)
{
    std::ostringstream logtext;
    UpperPCToLowerPC sendLowerData=initUpperPCToLowerPC();
    switch (type)
    {
    case DEVICE_INIT:{
        sendLowerData=initUpperPCToLowerPC();
        std::string syncslice = getStandardCurTime();
        for (int i=0;i<int(syncslice.length());i++){
            sendLowerData.SyncTime[i]=syncslice[i];
        }
        sendLowerData.SyncTime[syncslice.length()]='\0';
        m_sendInfo=serialize(sendLowerData);
        break;
    }
    case DEVICE_GETTYPE:
        break;
    case DEVICE_TASK_RUN:
        sendLowerData = initUpperPCToLowerPC();
        sendLowerData=m_styleALT->getControlCode();
        m_sendInfo = serialize(sendLowerData);
        m_deviceRunStatus=DEVICE_TASK_WAIT_RESPONSE;
        break;
    case DEVICE_WAIT:
        break;
    case DEVICE_RESET:
        m_styleALT->reset();
        break;
    case DEVICE_GET_DATA:
        break;
    case DEVICE_TASK_WAIT_RESPONSE:
        std::string running_status;
        m_styleALT->getRunIndex(running_status);
        if (running_status=="已完成") 
            m_deviceRunStatus=DEVICE_TASK_RUN;
        break;
    }
}
void HgThreadManage::runBreakProcess(){
    m_threadBreakFlag=FLAGSTART;
}
bool isZombieProcess(int pid) {
    std::string path = "/proc/" + std::to_string(pid) + "/status";
    std::ifstream file(path);
    std::string line;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            if (line.find("State:") != std::string::npos) {
                if (line.find("Z") != std::string::npos) {
                    return true;
                }
                break;
            }
        }
        file.close();
    }
    return false;
}
void handlechild(int) {
    while (waitpid(-1, NULL, WNOHANG) > 0);  // 非阻塞回收所有子进程
}

void HgThreadManage::runListenProtectProcess(){
    m_listenProtectProcessFlag=FLAGSTART;
    std::string processName = "HGDaemon";
    pid_t daemonPid;
    bool createProcessFlag=false;
    bool connect=false;
    int count=0;
    do {
        if (isProcessRunning(processName)) {
            if (isZombieProcess(daemonPid)){
                HGLog4Cplus::getLogInstance(LOG_PATH)->logout("zombie process, restart",LOGERROR);
            }
            // HGLog4Cplus::getLogInstance(LOG_PATH)->logout(processName+" process is running",LOGINFO);
        } else {
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout(processName+" process is not running",LOGERROR);
            std::string curPath=HGOnlineRWDB::readCurDirPath()+"/process/";
            HGMkDir(curPath);
            curPath+=processName;
            
            posix_spawnattr_t attr;
            // 初始化属性
            posix_spawnattr_init(&attr);
            // 创建子进程
            const char* childArgs[]={(char*)(curPath.c_str()),"5",NULL};
            if (posix_spawn(&daemonPid,childArgs[0],nullptr,
                &attr,const_cast<char *const *>(childArgs),environ)==0)
            {
                createProcessFlag=true;
                signal(SIGCHLD, handlechild);  // 处理 SIGCHLD，防止僵尸进程
                HGLog4Cplus::getLogInstance(LOG_PATH)->logout("create process success, PID:"+std::to_string(daemonPid),LOGERROR);
            } else {
                createProcessFlag=false;
                HGLog4Cplus::getLogInstance(LOG_PATH)->logout("create process failed",LOGERROR);
            }
            setsid();
            // 释放属性
            posix_spawnattr_destroy(&attr);
            sleep(1);
            // 记录子进程 PID
            std::ofstream pid_file(CHILD_PID_FILE);
            if (pid_file)
            {
                pid_file << daemonPid;
                pid_file.close();
            }
        }

        if (!connect){
            const char * pPubAddr = PUB_ADDR;
            int ret=m_comwithzmq.createPub(pPubAddr);
            if (ret==0) connect=true;
            else {
                connect=false;
                count++;
            }
        } 
        if (!connect) {
            if (count >= 5){
                // restart protect process
            }
            continue;
        }

        ComDataStruct sendComData;
        sendComData.id = HGOnlineRWDB::readLoginName();
        sendComData.pid=getpid();
        // HGLog4Cplus::getLogInstance(LOG_PATH)->logout(
        //     "send pid:"+std::to_string(sendComData.pid)+",userId:"+sendComData.id,
        //     LOGINFO);
        int rc = m_comwithzmq.sendStruct(sendComData);
        std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    } while (FLAGSTART == m_exitTrd);

    // if (createProcessFlag && daemonPid > 0) {
    //     HGLog4Cplus::getLogInstance(LOG_PATH)->logout(
    //         "Killing process: " + std::to_string(daemonPid),
    //         LOGINFO);
    //     kill(daemonPid, SIGTERM);  // 发送终止信号
    // }
}
void HgThreadManage::runSend(){
    m_sendComThreadFlag=FLAGSTART;
    do {
        std::ostringstream logtext;
        #ifdef ENABLE_CAS6
        // printf("m_sendbytes.size():%d\n",int(m_sendbytes.size()));
        // std::vector<uint8_t> bytes={
        //     0xEE, 0xB1, 0x11, 0x00, 0x03, 0x00, 0x2C, 0x10, 0x01, 0x01, 0xFF, 0xFC, 0xFF, 0xFF
        // }; // request version
        if (m_deviceRunStatuss.size() <=0) continue;
        initSendInfoCAS6();
        if (m_sendbytes.size() <=0) continue;
        std::string data(m_sendbytes[0].begin(), m_sendbytes[0].end());
        std::ostringstream ss;
        ss << "send data: ";
        printf("send data:");
        for (int i=0;i<int(m_sendbytes[0].size());i++){
            printf(" %02X ", m_sendbytes[0][i]);
            ss << std::hex << std::setw(2) << std::setfill('0') << (int)m_sendbytes[0][i] << " ";
        }
        printf("\n");
        HGLog4Cplus::getLogInstance(LOG_PATH)->logout(ss.str(), LOGINFO);

        // printf("send data: %s\n", hexString.c_str());
       // EE B1 11 00 03 00 2C 10 01 01 FF FC FF FF
        writeBuffer(data);
        if (!m_sendbytes.empty()){
            m_sendbytes.erase(m_sendbytes.begin());// erase zero
        }
        if (!m_deviceRunStatuss.empty()){
            m_deviceRunStatuss.erase(m_deviceRunStatuss.begin());
        }
        // if (m_deviceRunStatus == NEEDLE_PUNCTURE_STEP) m_deviceRunStatus = SET_NEEDLE_PUNCTURE_STEP;
        // if (NEEDLE_RELEASE == m_deviceRunStatus || 
        //     NEEDLE_ARRIVAL_LIMIT_POS == m_deviceRunStatus ||
        //     SET_PUNCTURE_POS_INDEX == m_deviceRunStatus ||
        //     NEEDLE_PUNCTURE_TARGET_POS == m_deviceRunStatus ||
        //     NEEDLE_PUNCTURE_STEP == m_deviceRunStatus) 
        // {
        //     m_deviceRunStatus=DEVICE_WAIT;
        // }
        // if (SET_NEEDLE_PUNCTURE_STEP == m_deviceRunStatus){
        //     m_deviceRunStatus=NEEDLE_PUNCTURE_STEP;
        // }
        // m_deviceRunStatus=DEVICE_WAIT;
        #else
        initSendInfo(m_deviceRunStatus);
        if (m_sendInfo.count > 0)
        {
            std::string data(reinterpret_cast<char *>(m_sendInfo.data), m_sendInfo.count);
            printf("send info:count:%d,%s\n", m_sendInfo.count, data.c_str());

            logtext << "send info:count:" << m_sendInfo.count << "," << data.c_str();
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout(logtext.str(), LOGINFO);

            {
                printf("[SEND] serialize:count:%d\n", m_sendInfo.count);
                logtext.str("");
                logtext << "[SEND] serialize: count:" << m_sendInfo.count;

                for (int i = 0; i < m_sendInfo.count; i++)
                {
                    std::stringstream ss;
                    ss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<int>(m_sendInfo.data[i]);

                    std::string hexString = ss.str();
                    printf("%s ", hexString.c_str());
                    logtext << hexString;
                }

                printf("[SEND end]\n");
                logtext << "[SEND end]";

                HGLog4Cplus::getLogInstance(LOG_PATH)->logout(logtext.str(), LOGINFO);
            }
            writeBuffer(data);
        }

        m_sendInfo.count = 0;
        #endif

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    } while (FLAGSTART == m_exitTrd);
}


int HgThreadManage::getInterfaceCount(){
    // return m_sendInfo.interfaceCount;
    return 0;
}
std::string HgThreadManage::curDeviceParamDBName(){
    // return m_sendInfo.curTableName;
    return "";
}
int HgThreadManage::getRunTaskInfo(std::string &status){
    if (m_styleALT==NULL) return -1;
    int index=m_styleALT->getRunIndex(status);
    return index;
}
// get the status of self check
std::vector<std::string> HgThreadManage::getSelfCheckStatus(){
    std::vector<std::string> status;
    if (m_styleALT==NULL){
        return status;
    }
    status=(m_styleALT->getSelfCheckStatus());
    return status;
}
int HgThreadManage::commOpen(){
    std::ostringstream logtext;
    std::string comPort=HgOnlinePlatformModule::getComPort();
    
    int stop=0;
    int bits=8;
    char event='N';
    unsigned int speed=115200;
    m_serial.set(comPort,speed,bits,event,stop);
    if (0==m_serial.open(comPort.c_str())) {
        logtext.str("");
        logtext << /*CARLAMPAPPNAME << */"open com success";
        HGLog4Cplus::getLogInstance(LOG_PATH)->logout(logtext.str(),LOGERROR);
    }
    else {
        logtext.str("");
        logtext << /*CARLAMPAPPNAME << */"open com failed!";
        HGLog4Cplus::getLogInstance(LOG_PATH)->logout(logtext.str(),LOGERROR);
        return -1;
    }
    HGSERIAL_STANDARD_INFO hgserial_info;
    hgserial_info.head.push_back(COM_HEAD);
    hgserial_info.tail.push_back(COM_TAIL1);
    hgserial_info.tail.push_back(COM_TAIL2);
    hgserial_info.tail.push_back(COM_TAIL3);
    hgserial_info.tail.push_back(COM_TAIL4);
    m_serial.setStandardInfo(hgserial_info);

    return 0;
}
std::vector<std::pair<size_t, size_t>> matchStartEndPairs(
    const std::vector<size_t>& starts,
    const std::vector<size_t>& ends,
    size_t maxLength
) {
    std::vector<std::pair<size_t, size_t>> result;
    size_t i = 0, j = 0;

    while (i < starts.size() && j < ends.size()) {
        size_t start = starts[i];
        size_t end = ends[j];

        if (start >= maxLength) {
            ++i;
            continue;
        }

        if (end > start && end <= maxLength) {
            result.emplace_back(start, end);
            ++i;
            ++j;
        } else {
            ++j; // end 不合法，继续找
        }
    }

    return result;
}

std::vector<splitDataInfo> HgThreadManage::splitRawDataCAS6(const std::vector<uint8_t> &contents){
    std::vector<splitDataInfo> rawDatas;
    if (contents.empty()) return rawDatas;
    int start=0;
    std::vector<size_t> posStartList, posEndList;
    posStartList=m_serial.findPattern(contents,m_serial.getStandardInfo().head);
    posEndList=m_serial.findPattern(contents,m_serial.getStandardInfo().tail);

    printf("posStartList：");
    for (int i=0;i<posStartList.size();i++){
        printf("%d ",posStartList[i]);
    }
    printf("\nposEndList：");
    for (int i=0;i<posEndList.size();i++){
        printf("%d ",posEndList[i]);
    }
    printf("\n");
    auto matches = matchStartEndPairs(posStartList, posEndList, 2000);

    printf("匹配结果：");
    for (const auto& [start, end] : matches) {
        splitDataInfo rawData;
        rawData.start=start;
        rawData.end=end;
        rawDatas.push_back(rawData);
        printf("Start: %d,  End: %d\n",start, end);
    }
    return rawDatas;
}
std::vector<ResSerialize> HgThreadManage::splitRawData(const std::vector<uint8_t> &contents){
    std::vector<ResSerialize> rawDatas;
    if (contents.empty()) return rawDatas;
    int start=0;
    std::vector<size_t> posStartList, posEndList;
    posStartList=m_serial.findPattern(contents,m_serial.getStandardInfo().head);
    posEndList=m_serial.findPattern(contents,m_serial.getStandardInfo().tail);
    auto matches = matchStartEndPairs(posStartList, posEndList, 200);

    printf("匹配结果：");
    for (const auto& [start, end] : matches) {
        std::vector<uint8_t> rawContents;
        for (int i = start; i < end + 4; i++)
        {
            std::stringstream ss;
            ss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<int>(contents[i]);

            std::string hexString = ss.str();
            printf("%s ", hexString.c_str());
            rawContents.push_back(contents[i]);
        }
        ResSerialize rawData;
        rawData.count = end+4 - start;
        rawDatas.push_back(rawData);
        printf("Start: %d,  End: %d\n",start, end);
    }
    return rawDatas;
}
std::vector<std::string> splitStringBySemicolon(const std::string& input) {
    std::vector<std::string> result;
    std::stringstream ss(input);
    std::string item;

    while (std::getline(ss, item, ';')) {
        result.push_back(item);
    }

    return result;
}

std::map<std::vector<uint8_t>, std::string> parsePacketCAS6Batch(const std::vector<uint8_t> &data)
{
    size_t i = 0;
    std::map<std::vector<uint8_t>, std::string> rawDatas;

    // 1. 帧头
    if (data[i] == 0xEE && data[i + 1] == 0xB1)
    {
        std::cout << "帧头匹配 ✅" << std::endl;
        i += 2;
    }
    else
    {
        std::cout << "无效帧头 ❌" << std::endl;
        return rawDatas;
    }

    i+=3;
    
    // 4. 解析TLV
    while (i <= data.size())
    { // 至少要能读Tag+Len
        // 检查帧尾
        if (i + 1 < data.size() && data[i] == 0xFF && data[i + 1] == 0xFC)
        {
            std::cout << "帧尾到达 ✅" << std::endl;
            break;
        }
        printf("tag:0x%02X, 0x%02X, len:0x%02X, 0x%02X \n", data[i],data[i+1],data[i+2],data[i+3]);
        uint16_t tag = data[i] | (data[i + 1] << 8);
        uint16_t len = (data[i+2] << 8) | data[i+3];
        i += 4;
        printf("len:%d\n", len);
        // 检查长度是否合理
        if (i + len > data.size())
        {
            std::cout << "错误：数据超出范围 ❌" << std::endl;
            break;
        }

        std::string valueStr;
        for (size_t j = 0; j < len; ++j)
        {
            valueStr += static_cast<char>(data[i + j]);
        }
        i += len;

        std::cout << "Tag: 0x" << std::hex << std::setw(4) << std::setfill('0') << tag
                  << " | Len: " << std::dec << len
                  << " | Value: \"" << valueStr << "\"" << std::endl;

        std::vector<uint8_t> key;
        key.push_back(static_cast<uint8_t>(tag & 0xFF));
        key.push_back(static_cast<uint8_t>((tag >> 8) & 0xFF));
        rawDatas[key] = valueStr;
    }
}
    void HgThreadManage::readBuffers()
    {
        std::ostringstream logtext;
        std::vector<uint8_t> contents;
        int timeout = 100;
        int ret = m_serial.read(contents, timeout);
        // printf("readBuffers:ret=%d\n",ret);
        if (ret < 0)
        {
            // HGLog4Cplus::getLogInstance(LOG_PATH)->logout("Error reading serial port", LOGERROR);
        }

        // logtext.str("");
        // logtext << "[READ] serialize: count:" << int(contents.size());
        // printf("[READ]serialize:count:%d\n", int(contents.size()));
        for (int i = 0; i < int(contents.size()); i++)
        {
            std::stringstream ss;
            ss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<int>(contents[i]);

            std::string hexString = ss.str();
            printf("%s ", hexString.c_str());
            logtext << hexString;
        }

        printf("\n");
        HGLog4Cplus::getLogInstance(LOG_PATH)->logout(logtext.str(), LOGINFO);

        logtext.str("");

#ifdef ENABLE_CAS6
        std::vector<splitDataInfo> rawData = splitRawDataCAS6(contents);
        for (int j = 0; j < int(rawData.size()); j++)
        {
            std::vector<uint8_t> commdata;
            printf("[Split][%d]serialize:count:%d\n", j, rawData[j].end + 4 - rawData[j].start);
            logtext << "[Split][" << j << "]serialize : count:" << rawData[j].end + 4 - rawData[j].start;
            for (int i = rawData[j].start; i < rawData[j].end + 4; i++)
            {
                commdata.push_back(contents[i]);
                std::stringstream ss;
                ss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<int>(contents[i]);

                std::string hexString = ss.str();
                printf("%s ", hexString.c_str());
                logtext << hexString;
            }

            int second = rawData[j].start + 1;
            int third = rawData[j].start + 2;
            int fourth = rawData[j].start + 3;
            int fifth = rawData[j].start + 4;
            int sixth = rawData[j].start + 5;
            int seventh = rawData[j].start + 6;
            int eighth = rawData[j].start + 7;
            if (second >= contents.size() || third >= contents.size())
                continue;
            if (rawData[j].start + 7 >= contents.size())
                continue;
            if (rawData[j].end <= rawData[j].start + 7)
                continue;

            std::string ascii;
            for (int k = rawData[j].start + 7; k < rawData[j].end; k++)
            {
                ascii += static_cast<char>(contents[k]);
            }

            if (contents[second] == 0xB1 && contents[third] == 0x10 &&
                contents[fourth] == 0x00 && contents[fifth] == 0x0A &&
                contents[sixth] == 0x00 && contents[seventh] == 0x15)
            {
                // bin version
                // EE B1 10 00 0A 00 15 56 33 2E 30 32 2D 32 35 2E 34 2E 35 3B 56 33 2E 31 2E 30 2D 32 35 2E 33 2E 32 37 3B 56 33 2E 33 2E 33 2D 32 35 2E 30 34 2E 35 3B FF FC FF FF
                // from seven

                std::vector<std::string> partsVersion = splitStringBySemicolon(ascii);
                printf("bin version: %s\n", ascii.c_str());
                HGLog4Cplus::getLogInstance(LOG_PATH)->logout("bin version: " + ascii, LOGINFO);
                std::vector<std::map<std::string, std::string>> versions;
                int count = 0;
                std::vector<std::string> binsName = {"shuifenyi", "zidongjinyangqi", "jiareban"};
                for (const auto &s : partsVersion)
                {
                    count++;
                    std::map<std::string, std::string> version;
                    version["BinName"] = binsName[count - 1];
                    version["BinVersion"] = s;
                    version["BinIndex"] = std::to_string(count);
                    if (s == "")
                        version["Online"] = "offline";
                    else
                        version["Online"] = "online";

                    printf("bin version part: %s\n", s.c_str());
                    HGLog4Cplus::getLogInstance(LOG_PATH)->logout("bin version part: " + s, LOGINFO);
                    versions.push_back(version);
                }
                HGOnlineRWDB::writeCAS6DeviceInfo(versions);
                m_deviceRunStatuss.push_back(SWIRL_SET_INIT1);
            }
            else if (contents[second] == 0xB1 && contents[third] == 0x10 &&
                     contents[fourth] == 0x00 && contents[fifth] == 0x03 &&
                     contents[sixth] == 0x00 && contents[seventh] == 0x0E) // response of set swirl speed
            {
                // EE B1 10 00 03 00 0E 搅拌速度 FF FC FF FF
                printf("swirl speed: %s\n", ascii.c_str());
                HGLog4Cplus::getLogInstance(LOG_PATH)->logout("swirl speed: " + ascii, LOGINFO);
                m_cas6_swirl_speed = std::atoi(ascii.c_str());
                // m_deviceRunStatus = DEVICE_WAIT;
            }
            else if (contents[second] == 0xB1 && contents[third] == 0x10 &&
                     contents[fourth] == 0x00 && contents[fifth] == 0x03 &&
                     contents[sixth] == 0x00 && contents[seventh] == 0x0C &&
                     contents[eighth] == 0x01) // response of heat on
            {
                // EE B1 10 00 03 00 0C 01 FF FC FF FF
                // m_deviceRunStatus = DEVICE_WAIT;
            }
            else if (contents[second] == 0xB1 && contents[third] == 0x12 &&
                     contents[fourth] == 0x00 && contents[fifth] == 0x03)
            {
                // batch data
                // m_rawValues.clear();
                std::map<std::vector<uint8_t>, std::string> batchData = parsePacketCAS6Batch(commdata);
                m_rawValues.push_back(batchData);
            }
            printf("\n");

            HGLog4Cplus::getLogInstance(LOG_PATH)->logout(logtext.str(), LOGINFO);
        }
#else
        std::vector<ResSerialize> rawData = splitRawData(contents);
        for (int j = 0; j < int(rawData.size()); j++)
        {
            if (rawData[j].count > 0)
            {
                printf("[Split][%d]serialize:count:%d\n", j, rawData[j].count);
                logtext << "[Split][" << j << "]serialize : count:" << rawData[j].count;
                for (int i = 0; i < rawData[j].count; i++)
                {
                    std::stringstream ss;
                    ss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<int>(rawData[j].data[i]);

                    std::string hexString = ss.str();
                    printf("%s ", hexString.c_str());
                    logtext << hexString;
                }

                printf("\n");

                HGLog4Cplus::getLogInstance(LOG_PATH)->logout(logtext.str(), LOGINFO);
            }
            else
            {
                return;
            }

            UpperPCToLowerPC upperToLowerStruct = deSerialize(rawData[j]);
            int curPhase = std::atoi(std::to_string(upperToLowerStruct.CurPhase).c_str());
            logtext.str("");
            logtext << "curPhase:" << curPhase;
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout(logtext.str(), LOGINFO);

            printf("%s\n", logtext.str().c_str());
            logtext.str("");
            if (curPhase == CHECK_DEVICE)
            {
                std::ostringstream ss;
                ss << std::dec << "faceNumber:" << std::to_string(upperToLowerStruct.CheckDevice.FaceNumber) << " Type:" << upperToLowerStruct.CheckDevice.Type << " InstrumentID:" << upperToLowerStruct.CheckDevice.Instrument_Id << " CanId:" << std::to_string(upperToLowerStruct.CheckDevice.CanId) << " \n";
                int faceNumber = std::atoi(std::to_string(upperToLowerStruct.CheckDevice.FaceNumber).c_str());
                for (int i = 0; i < faceNumber; i++)
                    ss << "FaceIndex:" << i << " FaceType:" << upperToLowerStruct.CheckDevice.FaceType[i] << " FaceName:" << std::string(upperToLowerStruct.CheckDevice.Face_Name[i]) << " FaceRun:" << std::to_string(upperToLowerStruct.CheckDevice.FaceRun[i]) << " FaceState:" << std::to_string(upperToLowerStruct.CheckDevice.FaceState[i]) << "\n";

                std::string ssstring = ss.str();
                printf("deSerialize: %s\n", ssstring.c_str());
                logtext << "deSerialize:" << ssstring;

                HGLog4Cplus::getLogInstance(LOG_PATH)->logout(logtext.str(), LOGINFO);

                HGOnlineRWDB::writeLowerPCDeviceInfo(upperToLowerStruct);
                UpperPCToLowerPC sendLowerData = initUpperPCToLowerPC();
                sendLowerData.CheckDevice.Type = upperToLowerStruct.CheckDevice.Type;
                sendLowerData.CheckDevice.FaceReceiveStatus = 1; // initial ok
                strcpy(sendLowerData.CheckDevice.Instrument_Id, upperToLowerStruct.CheckDevice.Instrument_Id);
                sendLowerData.CheckDevice.CanId = upperToLowerStruct.CheckDevice.CanId;
                sendLowerData.CheckDevice.FaceNumber = 0;
                sendLowerData.CurPhase = upperToLowerStruct.CurPhase;
                m_sendInfo = serialize(sendLowerData);
                m_deviceRunStatus = DEVICE_GETTYPE;
                if (upperToLowerStruct.CheckDevice.FaceReceiveStatus == 0x80)
                {
                    printf("80\n");
                    HGLog4Cplus::getLogInstance(LOG_PATH)->logout("80", LOGINFO);
                }
            }
            else if (curPhase == RUN_FLOW)
            {
                printf("receive RUN_FLOW\n");
                HGLog4Cplus::getLogInstance(LOG_PATH)->logout("receive RUN_FLOW", LOGINFO);
            }
            else if (curPhase == RESPONSE_FLOW)
            {
                if (upperToLowerStruct.Instrument_Run == 0xFF)
                {
                    printf("lower pc receive flow already\n");
                    HGLog4Cplus::getLogInstance(LOG_PATH)->logout("lower pc receive flow already", LOGINFO);
                }
                else if (upperToLowerStruct.Instrument_Run == 0x00)
                {
                    printf("lower pc is initializing...\n");
                    HGLog4Cplus::getLogInstance(LOG_PATH)->logout("lower pc is initializing...", LOGINFO);
                }
                else
                {
                    // set current running step index
                    m_styleALT->setCurrentRunningStepIndex(static_cast<int>(upperToLowerStruct.Instrument_Run));
                    UpperPCToLowerPC sendLowerData = initUpperPCToLowerPC();
                    sendLowerData.CurPhase = RESPONSE_FLOW;
                    sendLowerData.Instrument_Run = upperToLowerStruct.Instrument_Run;
                    m_sendInfo = serialize(sendLowerData);
                }
            }
            else if (curPhase == INTERACTE_DATA)
            {
                printf("lower pc send data\n");
                HGLog4Cplus::getLogInstance(LOG_PATH)->logout("lower pc send data", LOGINFO);
                std::string curStatus;
                if (m_styleALT != NULL && (getRunTaskInfo(curStatus) != -1))
                {
                    Task curTaskInfo = m_styleALT->getTaskInfo(getRunTaskInfo(curStatus));
                    std::map<std::string, std::string> dataInfo = HGOnlineRWDB::decodeDeviceDataInfoAndWrite(upperToLowerStruct, curTaskInfo);
                    m_dataInfos.push_back(dataInfo);
                }
            }
            printf("[RECV end]\n");
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout("[RECV end]", LOGINFO);
        }
#endif
    }
    std::string findKeyByValue(const std::map<std::string, std::vector<uint8_t>> &maps, const std::vector<uint8_t> &targetValue)
    {
        for (const auto &pair : maps)
        {
            if (pair.second == targetValue)
            { // vector比较内容是否一致
                return pair.first;
            }
        }
        return ""; // 没找到，返回空字符串
    }
    int safeStoi(const std::string &str){
        try {
            return std::stoi(str);
        } catch (const std::invalid_argument& e) {
            printf("safeStoi error\n");
            return 0;
        }
    }
    std::map<std::string, std::string> HgThreadManage::getCAS6RawValues()
    {
        std::map<std::string, std::string> rawValues;
        #ifdef ENABLE_CAS6
        if (m_rawValues.size() <= 0) return rawValues;
        std::ostringstream ss;
        ss << "rawValues key: ";
        for (const auto &pair : m_rawValues[0])
        {
            for (int i = 0; i < int(pair.first.size()); i++)
            {
                ss << " 0x" << std::hex << std::setw(2) << std::setfill('0') << (int)pair.first[i] << " ";
            }
            

            // printf("rawValues: %s\n", pair.second.c_str());
            std::string key = findKeyByValue(gMapRecvMethod, pair.first);
            // printf("key: %s\n", key.c_str());
            if (key == "")
                continue;
            if (key == "状态结果"){
                int nResult=0,nInfo=0,nSec=0;
                if (pair.second.length()>=2) nResult=safeStoi(pair.second.substr(0,2));
                if (pair.second.length()>=4) nInfo=safeStoi(pair.second.substr(2,2));
                if (pair.second.length()>=5) nSec=safeStoi(pair.second.substr(4));
                std::string stateResult="";
                std::string stateInfo="";
                if (nResult >=0 && nResult < int(gCAS6MCUResult.size())){
                    stateResult=gCAS6MCUResult[nResult];
                }
                if (nInfo >= 0 && nInfo <int(gCAS6MCUResultInfo.size())){
                    if (pair.second.length()>=5){
                        stateInfo=gCAS6MCUResultInfo[nInfo];
                        size_t pos=stateInfo.find("%1");
                        if (pos != std::string::npos){
                            stateInfo.replace(pos,2,std::to_string(nSec));
                        }
                    } else{
                        stateInfo=gCAS6MCUResultInfo[nInfo];
                    }
                }
                m_cur_mcu_status=stateResult+" "+stateInfo;
            }
            rawValues[key] = pair.second;
        }
        printf("%s\n",ss.str().c_str());
        HGLog4Cplus::getLogInstance(LOG_PATH)->logout(ss.str(), LOGINFO);

        ss.str("");
        for (const auto &pair : rawValues){
            ss << "[" << pair.first << "] " << pair.second;
        }

        printf("%s\n", ss.str().c_str());
        HGLog4Cplus::getLogInstance(LOG_PATH)->logout(ss.str(), LOGINFO);
        m_rawValues.erase(m_rawValues.begin());

        #else

        #endif
        return rawValues;
    }
    std::map<std::string, std::string> HgThreadManage::getDataInfo()
    {
        std::map<std::string, std::string> curDataInfo;
        if (int(m_dataInfos.size()) > 0)
        {
            curDataInfo = m_dataInfos[0];
            m_dataInfos.erase(m_dataInfos.begin());
        }
        return curDataInfo;
    }
    void HgThreadManage::writeBuffer(const std::string &data)
    {
        m_serial.write(data);
    }
}