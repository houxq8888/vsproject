#include "HGOnlinePlatformInterface.h"
#include "hgonlineplatformmodule.h"
#include "hgsavedatatodb.h"
#include "hgcommonutility.h"
#include "hgthreadManage.h"
#include <map>
#include <thread>
#include "hgonlinerwDB.h"
#include <iomanip>
#include "styleALT.h"
#include "hgserial.h"
#include "hgsecurity.h"
#include "HGMacroData.h"
#include "hglog4cplus.h"
#include "hgcommonutility.h"


namespace HGMACHINE {
HgThreadManage *hgThreadManage=NULL;
StyleALT *style=NULL;

#define CONFIG_DIR "/config"

template<typename K,typename V>
std::pair<bool, K> findKeyByValue(const std::map<K,V>& m,const V& value){
    for (const auto& [key,val] : m){
        if (val==value){
            return {true,key};
        }
    }
    return {false,K()};
}

void openDB(const std::string&dbName){
    HGOnlineRWDB::openDB(dbName);
}

void closeDB(){
    HGOnlineRWDB::closeDB();
}
/// @brief ///
/// @return ////////////




/// @brief  threading
///


void createThread()
{
    hgThreadManage = new HgThreadManage();
}
void startThread(){
    if (hgThreadManage==NULL) return;
    hgThreadManage->startThread();
}
void startRunTaskSequence(){
    if (hgThreadManage==NULL) return;
    hgThreadManage->startRunTaskSequence();
}
void pauseTaskSequence(){
    if (hgThreadManage==NULL) return;
    hgThreadManage->pauseTaskSequence();
}
// 获取当前正在执行的任务索引值 
int getCurRunTaskIndex(std::string &status){
    if (hgThreadManage==NULL) return 0;
    return (hgThreadManage->getRunTaskInfo(status));
}
// 获取自检状态
std::vector<std::string> getSelfCheckStatus(){
    std::vector<std::string> result;
    if (hgThreadManage==NULL) return result;
    return (hgThreadManage->getSelfCheckStatus());
}
void updateFlowInfo(){
    if (hgThreadManage==NULL) return;
    hgThreadManage->updateFlowInfo();
}
void updateStyleInfo(){
    if (hgThreadManage==NULL) return;
    hgThreadManage->updateStyleInfo();
}
void resetTaskSequence(){
    if (hgThreadManage==NULL) return;
    hgThreadManage->resetTaskSequence();
}

int getInterfaceCount(){
    int count=-1;
    if (hgThreadManage!=NULL) count=hgThreadManage->getInterfaceCount();
    return count;
}
std::string curDeviceParamDBName(){
    std::string dbName="";
    if (hgThreadManage!=NULL) dbName=hgThreadManage->curDeviceParamDBName();
    return dbName;
}
void stopThread(){
    if (hgThreadManage==NULL) return;
    hgThreadManage->stopThread();
    SAFE_DELETE(hgThreadManage);
}
int getLinkStatus(){
    int linkStatus=LINK_UNKNOWN;
    if (curDeviceParamDBName()=="") linkStatus=LINK_OFFLINE;
    else linkStatus=LINK_ONLINE;
    return linkStatus;
}
int getConnectWithMCUStatus(){
    if (hgThreadManage==NULL) return LINK_OFFLINE;
    else {
        return hgThreadManage->getConnectWithMCUStatus();
    }
}
std::map<std::string,std::string> getDataInfo(){
    std::map<std::string,std::string> info;
    info.clear();
    if (hgThreadManage!=NULL) {
        // printf("hgThreadManage is not null\n");
        return hgThreadManage->getDataInfo();
    } else {
        // printf("hgThreadManage is null\n");
        return info;
    }
}
void setHeatOn(){
    if (hgThreadManage!=NULL){
        hgThreadManage->setRunStatus(HEAT_ON);
    }
}
void setHeatOff(){
    if (hgThreadManage!=NULL){
        hgThreadManage->setRunStatus(HEAT_OFF);
    }
}
void setFanOn(){
    if (hgThreadManage!=NULL){
        hgThreadManage->setRunStatus(FAN_ON);
    }
}
void setFanOff(){
    if (hgThreadManage!=NULL){
        hgThreadManage->setRunStatus(FAN_OFF);
    }
}
void setPumpOn(){
    if (hgThreadManage!=NULL){
        hgThreadManage->setRunStatus(PUMP_ON);
    }
}
void setPumpOff(){
    if (hgThreadManage!=NULL){
        hgThreadManage->setRunStatus(PUMP_OFF);
    }
}
void setSwirlAscend(){
    if (hgThreadManage!=NULL){
        hgThreadManage->setRunStatus(SWIRL_ASCEND);
    }
}
void setSwirlDescend(){
    if (hgThreadManage!=NULL){
        hgThreadManage->setRunStatus(SWIRL_DESCEND);
    }
}
void setNeedleRelease(){
    if (hgThreadManage!=NULL){
        hgThreadManage->setRunStatus(NEEDLE_RELEASE);
    }
}
void setCircleExecute(){
    if (hgThreadManage!=NULL){
        hgThreadManage->setRunStatus(CIRCLE_EXECUTE);
    }
}
void setCircleNumber(int number){
    if (hgThreadManage!=NULL){
        hgThreadManage->setCircleNumber(number);
        hgThreadManage->setRunStatus(SET_CIRCLE_NUMBER);
    }
}
void setNeedleArrivalLimitPos(){
    if (hgThreadManage!=NULL){
        hgThreadManage->setRunStatus(NEEDLE_ARRIVAL_LIMIT_POS);
    }
}
void setNeedlePunctureRelease(){
    if (hgThreadManage!=NULL){
        hgThreadManage->setRunStatus(NEEDLE_PUNCTURE_RELEASE);
    }
}
void setNeedlePunctureStep(double step){
    if (hgThreadManage!=NULL){
        hgThreadManage->setNeedlePunctureStep(step);
        hgThreadManage->setRunStatus(NEEDLE_PUNCTURE_STEP);
        hgThreadManage->setRunStatus(SET_NEEDLE_PUNCTURE_STEP);
    }
}
void setPuncturePosIndex(int index){
    if (hgThreadManage!=NULL){
        hgThreadManage->setPuncturePosIndex(index);
        hgThreadManage->setRunStatus(SET_PUNCTURE_POS_INDEX);
    }
}
void setNeedleArrivalTargetPos(){
    if (hgThreadManage!=NULL){
        hgThreadManage->setRunStatus(NEEDLE_PUNCTURE_TARGET_POS);
    }
}
void setMethodInfoToMCU(const std::map<std::vector<uint8_t>,std::vector<uint8_t>> &sendMethodValues){
    if (hgThreadManage!=NULL){
        hgThreadManage->sendMethodParam(sendMethodValues);
        hgThreadManage->setRunStatus(SEND_METHOD_PARAM);
    }
}
void setMakeASample(){
    if (hgThreadManage!=NULL){
        hgThreadManage->setRunStatus(MAKE_A_SAMPLE);
    }
}
void setCurCircleNo(int circleNo){
    if (hgThreadManage!=NULL){
        hgThreadManage->setCurCircleNo(circleNo);
        hgThreadManage->setRunStatus(SET_CUR_CIRCLE_NO);
    }
}
std::string getMCUState(){
    if (hgThreadManage!=NULL){
        return hgThreadManage->getCurMcuStatus();
    } else {
        return "请求数据离线";
    }
}
double getNeedlePunctureCurPos(){
    if (hgThreadManage!=NULL){
        hgThreadManage->setRunStatus(GET_NEEDLE_PUNCTURE_CUR_POS);
        return hgThreadManage->getNeedlePunctureCurPos();
    }else{
        return 0.0;
    }
}
std::map<std::string,std::string> getCAS6RawValues() { 
    if (hgThreadManage!=NULL){
        return hgThreadManage->getCAS6RawValues();
    }else{
        return std::map<std::string,std::string>();
    }
}
bool makeNextSample(){
    if (hgThreadManage!=NULL){
        bool flag=hgThreadManage->isMakeNextSample();
        if (!flag) return false;

        std::string mcuState=getMCUState();
        if (mcuState=="已就绪 样品分析完成!") {
            int curIndex=hgThreadManage->getCurRunningTaskIndex();
            int taskCount=getTaskCount();
            if ((curIndex+1)==taskCount) return false;
            curIndex++;
            hgThreadManage->setCurRunningTaskIndex(curIndex);
            makeCurSample();
        }
        return true;
    } else return false;
}
std::map<std::string,std::string> getCurRunningTaskInfo(){
    if (hgThreadManage!=NULL){
        return hgThreadManage->getCurRunningTaskInfo();
    }else{
        return std::map<std::string,std::string>();
    }
}
void makeCurSample()
{
    #ifdef ENABLE_CAS6
    std::map<std::string, std::string> taskInfo = getCurRunningTaskInfo();
    std::map<std::vector<uint8_t>, std::vector<uint8_t>> sendMethodInfo;
    printf("cirlce no:%s\n",taskInfo["circleNo"].c_str());
    HGLog4Cplus::getLogInstance(LOG_PATH)->logout("circle no:"+taskInfo["circleNo"],LOGINFO);
    std::string paramstr = taskInfo["param"];
    int circleNo = std::atoi(taskInfo["circleNo"].c_str());
    std::map<std::string, std::string> wparam = getParamMap(paramstr);
    for (const auto &pair : gMapSendMethod)
    {
        if (wparam.find(pair.first) == wparam.end())
            continue;
        std::string paramStr = wparam[pair.first];
        std::vector<uint8_t> input = std::vector<uint8_t>(paramStr.begin(), paramStr.end());
        sendMethodInfo[pair.second] = input;
    }
    setMethodInfoToMCU(sendMethodInfo);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    // for (int i=0;i<10;i++)
    {
        setCurCircleNo(circleNo);
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
    setMakeASample();
    #endif
}
void setRequestData(){
    if (hgThreadManage!=NULL){
        hgThreadManage->setRunStatus(REQUEST_DATA);
    }
}
void setStartRunningTaskSequence(bool flag){
    if (hgThreadManage!=NULL){
        if (flag) {
            hgThreadManage->setEnableMakeSample(flag);
            hgThreadManage->setCurRunningTaskIndex(0);
            makeCurSample();
        }
        else hgThreadManage->setCurRunningTaskIndex(-1);
    }
}
void setCurRunningTaskIndex(int index){
    if (hgThreadManage==NULL) return;
    else hgThreadManage->setCurRunningTaskIndex(index);
}
int getCurRunningTaskIndex(){
    if (hgThreadManage==NULL) return -1;
    else return hgThreadManage->getCurRunningTaskIndex();
}
// style
int getTaskCount(){
    if (hgThreadManage!=NULL){
        return hgThreadManage->getCurTaskCount();
    } else return 0;
}
Task getTaskInfo(const int &taskIndex){
    if (hgThreadManage!=NULL){
        return hgThreadManage->getCurTaskInfo(taskIndex);
    } else return Task();
}
// config
bool loadConfig(std::string curPath){
    std::string configPath=curPath+CONFIG_DIR;
    HGMkDir(configPath);
    std::string path=configPath+"/config.xml";
    bool valid=isFileExist(path);
    if (!valid){
        FileConfig::createConfigFile(path,"ALT");
    } else {
        FileConfig::loadConfigFile(path);
    }
    FileConfig::setDirPath(curPath);
    return true;
}
void saveConfig(){
    FileConfig::saveConfigFile("ALT");
}


void changeUserLoginAndQuitTime(const std::string &userNo, const std::string &key)
{
    std::vector<std::map<std::string, std::string>> userInfos = HGOnlineRWDB::readUserInfo();
    int index=-1;
    for (int i = 0; i < int(userInfos.size()); i++)
    {
        if (userInfos[i]["UserAccount"] == userNo)
        {
            index=i;
            break;
        }
    }
    if (index==-1) return;
    userInfos[index][key]+=(getStandardCurTime()+";");
    HGOnlineRWDB::writeUserInfo(userInfos[index]);
}


}
