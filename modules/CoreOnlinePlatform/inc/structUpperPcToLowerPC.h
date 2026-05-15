/*
    @ 
    @file   structUpperPcToLowerPC.h
    @brief  Declaration file
    @author xiaoqin.hou
    @date   2025/04/16
    @version 1.0.0
*/
#ifndef _STRUCT_UPPERPC_TO_LOWERPC_H_
#define _STRUCT_UPPERPC_TO_LOWERPC_H_

#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define COM_HEAD 0xEE
#define COM_TAIL1 0xFF
#define COM_TAIL2 0xFC
#define COM_TAIL3 0xFF
#define COM_TAIL4 0xFF

#define SMALLER_SAVE 0
#define MAX_NUM 10



#pragma pack(4)
enum Enum_GETSAMPLE_MODE {
    TIME_MODE,
    CUBE_MODE
};
typedef struct tagswirlStruct{
    uint8_t swirlSpeed;
    uint8_t index;
    uint16_t continueTime;
    uint8_t CanId;//CAN地址
    uint8_t curFaceTypeIndex;
    uint16_t Type;//设备类型
} swirlStruct;

typedef struct taggetSampleStruct{
    uint8_t mode;// Enum_GETSAMPLE_MODE
    uint16_t speed;
    uint16_t time;// lowerPC quyangjindu 
    uint8_t index;
    uint16_t continueTime;
    uint8_t CanId;//CAN地址
    uint8_t curFaceTypeIndex;
    uint16_t Type;//设备类型
} getSampleStruct;

typedef struct taginjectSampleStruct{
    uint8_t mode; 
    uint16_t speed;
    uint16_t time;
    uint8_t index;
    uint16_t continueTime;// unit (S)
    uint8_t CanId;//CAN地址
    uint8_t curFaceTypeIndex;
    uint16_t Type;//设备类型
} injectSampleStruct;

enum Enum_SHIFT {
    ONE_SHIFT,
    TWO_SHIFT,
    THREE_SHIFT,
    FOUR_SHIFT,
    FIVE_SHIFT,
    AUTO_SHIFT
};
typedef struct tagbalanceStruct{
    uint8_t index;
    uint16_t continueTime;// unit (S)
    uint8_t CanId;//CAN地址
    uint8_t curFaceTypeIndex;
    uint16_t Type;//设备类型
} balanceStruct;

enum Enum_TARGET_MODE{
    DELAY_TERMINATION,
    DRIFT_TERMINATION
};
typedef struct tagdetectStruct{
    uint8_t dddj;
    uint16_t ddTypeMin;
    uint16_t ddTypeMax;
    uint16_t ddCuiQuTime;
    float targetDW;
    uint8_t targetType;
    uint16_t targetElapsed;
    uint16_t targetRange;
    uint8_t index;
    uint16_t continueTime;// unit (S)
    uint8_t CanId;//CAN地址
    uint8_t curFaceTypeIndex;
    uint16_t Type;//设备类型
} detectStruct;

typedef struct taganalysisData{
    uint16_t TimeSpan;//分析时长 sec
    float Drift;//漂移值  ug/min
    float WaterWeigth;//水质量 ug
    uint16_t Voltage;//测量电压 mv
    float Balance_Speed;//电解速率 ug/min
} analysisData;

enum Enum_PHASE{
    SERIALIZE_INIT,
    CHECK_DEVICE,
    RUN_FLOW,
    RESPONSE_FLOW,
    INTERACTE_DATA
};


typedef struct tagCheckDevice{
    uint8_t FaceNumber; // 端口
    uint16_t Type;//设备类型
    char Instrument_Id[20];//出厂编号
    uint8_t CanId;//CAN地址
    uint16_t FaceType[MAX_NUM];//端口类型
    char Face_Name[MAX_NUM][20];//接口名称
    // uint8_t curFaceTypeIndex; // 当前执行端口的索引值
    uint8_t FaceRun[MAX_NUM];//接口运行停止命令
    uint8_t FaceState[MAX_NUM];//接口状态 0空闲 1忙 2结束 
    uint8_t FaceReceiveStatus; // status 0 -- no ready ,1---上位机发device初始化OK,2---init,3----上位机request data
                            //    80---下位机发自检OK,81---下位机发每条指令运行完成,82---下位机发data
} checkDevice;

// Init uint16_t Type;//设备类型==0 && FaceReceiveStatus==2
typedef struct tagUpperPCToLowerPC{
    uint8_t CurPhase; // PHASE
    checkDevice CheckDevice;

    // 取数据
    analysisData AnalysisData;

    // flow
    swirlStruct Swirl[MAX_NUM]; // 搅拌
    balanceStruct BalanceSample[MAX_NUM];
    getSampleStruct GetSample[MAX_NUM];//取样
    injectSampleStruct InjectSample[MAX_NUM]; // 进样
    detectStruct DetectSample[MAX_NUM];


    // uint8_t Instrument_State;//仪器状态 0x01~0x07 1未就绪 2错误状态 3进样等待状态 4~ 5终点保持 6 过碘状态 7样品分析中 8电解平衡中
    uint8_t Instrument_Run;// 0xFF get flow already 0x00 initial value, 
                            // 0x01 first step, 0x02 second step, and etc 
    char SyncTime[128]; // sync time with lower pc 
    // uint8_t FaceReceiveStatus; // status 0 -- no ready ,1---上位机发device初始化OK,2---init,3----上位机request data
                            //    80---下位机发自检OK,81---下位机发每条指令运行完成,82---下位机发data
} UpperPCToLowerPC;
#pragma pack()

typedef struct tagResSerialize{
    uint8_t data[1024];
    int count;
} ResSerialize;


ResSerialize serialize(UpperPCToLowerPC stuctS);
UpperPCToLowerPC deSerialize(ResSerialize content);
UpperPCToLowerPC initUpperPCToLowerPC(void);

checkDevice initCheckDevice(void);
swirlStruct initSwirlStruct(void);
getSampleStruct initGetSampleStruct(void);
injectSampleStruct initInjectSampleStruct(void);
detectStruct initDetectStruct(void);
balanceStruct initBalanceStruct(void);
analysisData initAnalysisData(void);

ResSerialize serializeCheckDevice(checkDevice checkDeviceS);
checkDevice deSerializeCheckDevice(ResSerialize contents);

ResSerialize serializeSwirl(swirlStruct swirlS);
swirlStruct deSerializeSwirl(ResSerialize contents);

ResSerialize serializeGetSample(getSampleStruct getSampleS);
getSampleStruct deSerializeGetSample(ResSerialize contents);

ResSerialize serializeInjectSample(injectSampleStruct injectSampleS);
injectSampleStruct deSerializeInjectSample(ResSerialize contents);

ResSerialize serializeDetectSample(detectStruct detectS);
detectStruct deSerializeDetectSample(ResSerialize contents);

ResSerialize serializeBalance(balanceStruct balanceS);
balanceStruct deSerializeBalance(ResSerialize contents);

ResSerialize serializeAnalysisData(analysisData analysisS);
analysisData deSerializeAnalysisData(ResSerialize contents);


#endif