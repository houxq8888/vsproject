#ifndef HGTHREADMANAGE_H
#define HGTHREADMANAGE_H

#include "pthread.h"
#include <vector>
#include "hgonlinerwDB.h"
#include "hgserial.h"
#include "structUpperPcToLowerPC.h"
#include "styleALT.h"
#include "hgcomwithzmq.h"

#define PUB_ADDR                "ipc:///app/ipc/ipc-HG-server" 

namespace HGMACHINE{
    #define SWIRL_SPEED_MAX 15
    #define SWIRL_SPEED_MIN 1
    typedef struct typsplitDataInfo{
        size_t start;
        size_t end;
    } splitDataInfo;
enum{
    LINK_UNKNOWN=-1,
    LINK_ONLINE=1,
    LINK_OFFLINE=2
};

enum {
    KUIYEUNIT,
    FUZHUBEN,
    JIAOBANQI,
    WAIJIESHEBEI,
    KUOZHANJIEKOU,
    PHJIANCEQI
};


enum {
    FLAGSTART=1,
    FLAGINTERRUPT=2,
    FLAGEND=3,
    FLAGRUNTASK=4,
    FLAGPAUSETASK=5
};

#ifdef ENABLE_CAS6
extern std::map<std::string,std::vector<uint8_t>> gMapSendMethod;
extern std::map<std::string,std::vector<uint8_t>> gMapRecvMethod;
#endif


enum{
    DEVICE_INIT,
    DEVICE_GETTYPE,
    DEVICE_TASK_RUN,
    DEVICE_WAIT,
    DEVICE_RESET,
    DEVICE_TASK_WAIT_RESPONSE,
    DEVICE_GET_DATA,
    HEAT_ON,
    HEAT_OFF,
    FAN_ON,
    FAN_OFF,
    PUMP_ON,
    PUMP_OFF,
    READ_SWIRL_SPEED,
    SWIRL_SET_INIT1,
    SWIRL_ASCEND,
    SWIRL_DESCEND,
    NEEDLE_ARRIVAL_LIMIT_POS,
    NEEDLE_PUNCTURE_RELEASE,
    NEEDLE_PUNCTURE_STEP,
    SET_NEEDLE_PUNCTURE_STEP,
    NEEDLE_RELEASE,
    SET_CIRCLE_NUMBER,
    CIRCLE_EXECUTE,
    SET_PUNCTURE_POS_INDEX,
    NEEDLE_PUNCTURE_TARGET_POS,
    GET_NEEDLE_PUNCTURE_CUR_POS,
    SEND_METHOD_PARAM,
    SET_CUR_CIRCLE_NO,
    MAKE_A_SAMPLE,
    GET_METHOD_PARAM,
    REQUEST_DATA
};


    class HgThreadManage
    {
    public:
        HgThreadManage();
        ~HgThreadManage();
        void startThread();
        void stopThread();
        void startRunTaskSequence();
        void pauseTaskSequence();
        void resetTaskSequence();
        int getRunTaskInfo(std::string &status);
        std::map<std::string,std::string> getCAS6RawValues();
        int getConnectWithMCUStatus() {return m_connectWithMCUStatus;};
        void sendMethodParam(const std::map<std::vector<uint8_t>,std::vector<uint8_t>> &sendMethodValues) {
            m_sendMethodValues = sendMethodValues;
        };
        std::map<std::string,std::string> getCurRunningTaskInfo() {return m_styleALT->getCurRunningTaskInfo();};
        void setCurRunningTaskIndex(int index) {m_styleALT->setCurRunningTaskIndex(index);};
        bool isMakeNextSample() {return m_makeSampleFlag;};
        void setEnableMakeSample(bool flag) {m_makeSampleFlag = flag;};
        int getCurRunningTaskIndex() {return m_styleALT->getCurRunningTaskIndex();};
        std::string getCurMcuStatus() {return m_cur_mcu_status;};
        double getNeedlePunctureCurPos() {return m_needlePunctureCurPos;};
        void setRunStatus(int status);
        void setCircleNumber(int number) {m_circle_number=number;};
        void setCurCircleNo(int circleNo) {m_cur_circle_no=circleNo;};
        void setNeedlePunctureStep(double step) {m_needle_puncture_step=step;};
        void setPuncturePosIndex(int index) {m_puncture_position_index=index;};

        int getCurTaskCount() {return m_styleALT->getTaskCount();};
        Task getCurTaskInfo(const int &taskIndex) {return m_styleALT->getTaskInfo(taskIndex);};
        void runRecv();
        void runSend();
        void runBreakProcess();
        void runListenProtectProcess();
        int getInterfaceCount();
        std::string curDeviceParamDBName();
        std::vector<std::string> getSelfCheckStatus();
        std::map<std::string,std::string> getDataInfo();
        void updateFlowInfo();
        void updateStyleInfo();


    private:
        void initSendInfo(int type);
        void initSendInfoCAS6();
        void readBuffers();
        int commOpen();
        void writeBuffer(const std::string& data);
        std::vector<ResSerialize> splitRawData(const std::vector<uint8_t> &contents);
        std::vector<splitDataInfo> splitRawDataCAS6(const std::vector<uint8_t> &contents);

    private:
        pthread_t threadRecvComm, threadSendComm;
        pthread_t m_listenProtectProcess;
        pthread_t m_threadBreak;
        int m_exitTrd;
     
        int m_recvComThreadFlag,m_sendComThreadFlag;
        int m_listenProtectProcessFlag, m_threadBreakFlag;
        
        ResSerialize m_sendInfo;
        HGSerial m_serial;
        HGComWithZmq m_comwithzmq;

        std::vector<int> m_deviceRunStatuss;
        int m_deviceRunStatus;

        StyleALT* m_styleALT;
        std::vector<std::map<std::string,std::string>> m_dataInfos;
        std::string m_comPort;

        // CAS6
        bool m_makeSampleFlag;
        int m_connectWithMCUStatus;
        std::string m_cur_mcu_status;
        std::vector<std::vector<uint8_t>> m_sendbytes;
        int m_cas6_swirl_speed;
        int m_circle_number;
        int m_cur_circle_no;
        double m_needle_puncture_step;
        int m_puncture_position_index;
        double m_needlePunctureCurPos;
        std::vector<std::map<std::vector<uint8_t>,std::string>> m_rawValues;
        std::map<std::vector<uint8_t>,std::vector<uint8_t>> m_sendMethodValues;
        
    };
}
#endif