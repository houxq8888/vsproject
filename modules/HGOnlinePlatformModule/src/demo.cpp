#include <iomanip>
#include <sstream>
#include "structUpperPcToLowerPC.h"


int testSerialize()
{
    UpperPCToLowerPC structS=initUpperPCToLowerPC();
    structS.CurPhase=CHECK_DEVICE;
    structS.CheckDevice.FaceNumber=5;
    structS.CheckDevice.Type=3;
    // for (int i=0;i<2;i++)
    strcpy(structS.CheckDevice.Instrument_Id,"ab");
    structS.CheckDevice.Instrument_Id[2]='\0';
    structS.CheckDevice.CanId=7;
    for (int i=0;i<structS.CheckDevice.FaceNumber;i++){
        structS.CheckDevice.FaceType[i]=0+i;
        // for (int j=0;j<4;j++){
        //     structS.CheckDevice.Face_Name[i][j]=10*(i+1)+j;
        // }
        // structS.CheckDevice.Face_Name[i][4]='\0';
        structS.CheckDevice.FaceRun[i]=0;
        structS.CheckDevice.FaceState[i]=0;
    }
    strcpy(structS.CheckDevice.Face_Name[0],"SENSOR1#");
    structS.CheckDevice.Face_Name[0][8]='\0';
    strcpy(structS.CheckDevice.Face_Name[1],"CAN-1#didingzujian");
    structS.CheckDevice.Face_Name[1][18]='\0';
    strcpy(structS.CheckDevice.Face_Name[2],"PW-1#jiekou");
    structS.CheckDevice.Face_Name[2][11]='\0';
    strcpy(structS.CheckDevice.Face_Name[3],"jiaobanjiekou1#");
    structS.CheckDevice.Face_Name[3][15]='\0';
    strcpy(structS.CheckDevice.Face_Name[4],"COM1#");
    structS.CheckDevice.Face_Name[4][5]='\0';
    structS.Swirl[0].swirlSpeed=12;
    structS.Swirl[0].index=0;
    structS.Swirl[0].curFaceTypeIndex=0;
    structS.Swirl[0].Type=0;
    structS.Swirl[0].continueTime=0;
    structS.Swirl[0].CanId=7;

    structS.BalanceSample[0].index=1;
    structS.BalanceSample[0].continueTime=0;
    structS.BalanceSample[0].curFaceTypeIndex=0;
    structS.BalanceSample[0].Type=0;
    structS.BalanceSample[0].CanId=8;

    structS.GetSample[0].index=2;
    structS.GetSample[0].mode=0;
    structS.GetSample[0].speed=23;
    structS.GetSample[0].time=45;

    structS.InjectSample[0].index=3;
    structS.InjectSample[0].mode=0;
    structS.InjectSample[0].speed=0;
    structS.InjectSample[0].time=0;

    structS.DetectSample[0].index=4;
    structS.DetectSample[0].targetType=0;

    // structS.Swirl[1].index=5;
    // structS.BalanceSample[1].index=6;
    // structS.GetSample[1].index=7;
    // structS.InjectSample[1].index=8;
    // structS.DetectSample[1].index=9;

    structS.Instrument_Run=0x0;
    structS.AnalysisData.TimeSpan=67;
    structS.AnalysisData.Drift=2.22;
    structS.AnalysisData.WaterWeigth=8.934;
    structS.AnalysisData.Voltage=123;
    structS.AnalysisData.Balance_Speed=5.67;

    ResSerialize res = serialize(structS);
    printf("serialize:count:%d\n",res.count);
    for (int i=0;i<res.count;i++){
        std::stringstream ss;
        ss<<std::hex<<std::uppercase<<std::setfill('0')<<std::setw(2)<<static_cast<int>(res.data[i]);

        std::string hexString=ss.str();
        printf("%s ",hexString.c_str());
    }

    printf("\n");

    UpperPCToLowerPC upperToLowerStruct= deSerialize(res);
    std::ostringstream ss;
    int faceNumber=std::atoi(std::to_string(upperToLowerStruct.CheckDevice.FaceNumber).c_str());
    ss<<std::dec<<"faceNumber:"<<std::to_string(upperToLowerStruct.CheckDevice.FaceNumber)<<" Type:"<<upperToLowerStruct.CheckDevice.Type<<" InstrumentID:"<< \
                std::string(upperToLowerStruct.CheckDevice.Instrument_Id)<<" CanId:"<<std::to_string(upperToLowerStruct.CheckDevice.CanId)<<" \n";
    for (int i=0;i<faceNumber;i++)
        ss<<"FaceIndex:"<<i<<" FaceType:"<<upperToLowerStruct.CheckDevice.FaceType[i]<<" FaceName:"<<std::string(upperToLowerStruct.CheckDevice.Face_Name[i])<<" FaceRun:"<<std::to_string(upperToLowerStruct.CheckDevice.FaceRun[i]) \
            <<" FaceState:"<<std::to_string(upperToLowerStruct.CheckDevice.FaceState[i])<<"\n";
    ss<<"ReceiveStatus:"<<std::to_string(upperToLowerStruct.CheckDevice.FaceReceiveStatus);
    ss<<"CurPhase:"<<std::to_string(upperToLowerStruct.CurPhase)<<"\n";
    std::string ssstring=ss.str();
    printf("deSerialize: %s\n",ssstring.c_str());

    return 0;
}


/*
EE 
00 00 00 84 
01 
00 00 00 7B 
05 
00 03 
00 00 00 07 
02 02 02 02 02 02 02 Instrucment_Id
07  canId
00 00 00 0A 
00 00 
00 01 
00 02 
00 03 
00 04 
00 00 00 47 
00 00 00 02 
53 45 SENSOR1#
00 00 00 12 
43 41 4E 2D 31 23 64 69 64 69 6E 67 7A 75 6A 69 61 6E  CAN-1#didingzujian
00 00 00 0B 
50 57 2D 31 23 6A 69 65 6B 6F 75 PW-1#jiekou
00 00 00 0F 
6A 69 61 6F 62 61 6E 6A 69 65 6B 6F 75 31 23  jiaobanjiekou1# 
00 00 00 05 
43 4F 4D 31 23 COM1#
00 03 E0 05 
00 
00 
00 
00 
00 
00 03 E0 05 
00 
00 
00 
00 
00 
00 
10 8C 
FF FC FF FF 
*/