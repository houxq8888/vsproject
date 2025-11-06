/*
    @ 
    @file   flowALT.cpp
    @brief  Implementation file
    @author xiaoqin.hou
*/
#include "flowALT.h"
#include "hgonlinerwDB.h"
#include "hgcommonutility.h"
#include <sstream>
#include "hglog4cplus.h"


namespace HGMACHINE
{
    FlowALT::FlowALT(std::string flowname,std::string methodname)
        : FlowBase()
    {
        m_name=flowname;
        m_flowOfTask=HGOnlineRWDB::readFlowOfTask(flowname);
        m_methodALT=new MethodALT(methodname);
    }

    FlowALT::~FlowALT(void)
    {
        m_flowOfTask.Clear();
    }
    void FlowALT::updateFlow(){
        m_flowOfTask=HGOnlineRWDB::readFlowOfTask(m_name);
    }
    int FlowALT::getStepsCount(){
        return int(m_flowOfTask.steps.size());
    }
    bool FlowALT::decodeCanIdAndIndex(const std::string &param, uint8_t &canId, uint8_t &index)
    {
        int pos = param.find_last_of('_');
        bool flag=false;
        if (pos != -1 && pos >= 0 && pos < param.length())
        {
            std::ostringstream logtext;
            logtext<<"pos:"<<pos;
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout(logtext.str(),LOGINFO);

            std::string canIdSTR = param.substr(0, pos);
            std::string indexStr = param.substr(pos + 1, param.length() - pos - 1);

            logtext.str("");
            logtext<<"canId:"<<canIdSTR<<",index:"<<indexStr;
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout(logtext.str(),LOGINFO);

            int canIDInt=std::atoi(canIdSTR.c_str());
            if (canIDInt<0||canIDInt>255) canId = 0;
            else canId=canIDInt;

            int indexInt=std::atoi(indexStr.c_str());
            if (indexInt<0||indexInt>255) index = 0;
            else index=indexInt;

            flag=true;
        }
        return flag;
    }
    std::string FlowALT::selfCheckDevice(){
        std::ostringstream status;
        status.str("");
        std::vector<StepOfFlow> steps=m_flowOfTask.steps;
        for (int index = 0; index < int(steps.size()); index++)
        {
            std::map<std::string, std::string> wparam = getParamMap(steps[index].param);
            std::string actionPort = wparam["执行端口"];
            std::string stepName=steps[index].name;
            uint8_t canId=0;
            uint8_t curFaceTypeIndex=0;
            bool flag = decodeCanIdAndIndex(actionPort, canId, curFaceTypeIndex);
            if (!flag)
            {
                status<<"\n["<<stepName<<"]没有配置执行端口";
            } else {
                bool canIdFlag=HGOnlineRWDB::isCanIdOnline(canId);
                if (!canIdFlag){
                    status<<"\n["<<stepName<<"][CanId:"<<std::to_string(canId)<<"][离线]";
                }
                else
                {
                    bool faceIndexFlag = HGOnlineRWDB::isFaceIndexOnline(canId, curFaceTypeIndex);
                    if (!faceIndexFlag)
                    {
                        status << "\n[" << stepName << "][FaceIndex:" << std::to_string(curFaceTypeIndex) << "][离线]";
                    }
                }
            }
        }
        if (status.str()=="") {
            if (int(steps.size()>0)) status<<"[在线]";
            else status<<"[请检查流程]";
        }
              
        return status.str();
    }
    std::string FlowALT::getCurRunningStepName(int index){
        if (index>=int(m_flowOfTask.steps.size())) return "已完成";
        if (index<0) return "初始化";
        return m_flowOfTask.steps[index].name;
    }
    template <typename K, typename V>
    std::pair<bool, K> findKeyByValue(const std::map<K, V> &m, const V &value)
    {
        for (const auto &[key, val] : m)
        {
            if (val == value)
            {
                return {true, key};
            }
        }
        return {false, K()};
    }

    UpperPCToLowerPC FlowALT::getStepControlCode(){
        UpperPCToLowerPC upperToLower=initUpperPCToLowerPC();
        upperToLower.CheckDevice.FaceNumber = 0;
        upperToLower.CurPhase=RUN_FLOW;
        std::vector<StepOfFlow> steps=m_flowOfTask.steps;
        std::map<std::string,int> stepCount;
        for (int i=0;i<int(steps.size());i++){
            stepCount[steps[i].name]=0;
        }
        for (int i = 0; i < int(steps.size()); i++)
        {
            StepOfFlow step = steps[i];
            std::map<std::string, std::string> wparam = getParamMap(step.param);
            HGLog4Cplus::getLogInstance(LOG_PATH)->logout(step.name, LOGINFO);
            std::string actionPort = wparam["执行端口"];
            uint8_t canId,curFaceTypeIndex;
            uint16_t type;
            bool flag = decodeCanIdAndIndex(actionPort, canId, curFaceTypeIndex);
            if (step.name == "搅拌")
            {
                if (!flag)
                {
                    upperToLower.Swirl[stepCount[step.name]].CanId = 5;
                    upperToLower.Swirl[stepCount[step.name]].curFaceTypeIndex = 2;
                    upperToLower.Swirl[stepCount[step.name]].Type = 3;
                } else {
                    upperToLower.Swirl[stepCount[step.name]].CanId = canId;
                    upperToLower.Swirl[stepCount[step.name]].curFaceTypeIndex = curFaceTypeIndex;
                    upperToLower.Swirl[stepCount[step.name]].Type = HGOnlineRWDB::getDeviceTypeFromCanId(upperToLower.Swirl[stepCount[step.name]].CanId);
                }

                int speed = std::atoi(wparam["搅拌速度"].c_str());
                if (speed < 0 || speed > 255)
                    upperToLower.Swirl[stepCount[step.name]].swirlSpeed = 0;
                else
                    upperToLower.Swirl[stepCount[step.name]].swirlSpeed = static_cast<uint8_t>(speed);
                upperToLower.Swirl[stepCount[step.name]].index=static_cast<uint8_t>(i);
                int time = std::atoi(wparam["时长"].c_str());
                if (time < 0 || time > 65535)
                    upperToLower.Swirl[stepCount[step.name]].continueTime = 0;
                else 
                    upperToLower.Swirl[stepCount[step.name]].continueTime = static_cast<uint16_t>(time);
            }
            else if (step.name == "平衡")
            {
                if (!flag)
                {
                    upperToLower.BalanceSample[stepCount[step.name]].CanId = 2;
                    upperToLower.BalanceSample[stepCount[step.name]].Type = 6;
                } else {
                    upperToLower.BalanceSample[stepCount[step.name]].CanId = canId;
                    upperToLower.BalanceSample[stepCount[step.name]].curFaceTypeIndex = curFaceTypeIndex;
                    upperToLower.BalanceSample[stepCount[step.name]].Type = HGOnlineRWDB::getDeviceTypeFromCanId(upperToLower.BalanceSample[stepCount[step.name]].CanId);
                }
                
                upperToLower.BalanceSample[stepCount[step.name]].index=static_cast<uint8_t>(i);
                int time = std::atoi(wparam["时长"].c_str());
                if (time < 0 || time > 65535)
                    upperToLower.BalanceSample[stepCount[step.name]].continueTime = 0;
                else 
                    upperToLower.BalanceSample[stepCount[step.name]].continueTime = static_cast<uint16_t>(time);                
            }
            else if (step.name == "取样")
            {
                if (!flag)
                {
                    upperToLower.GetSample[stepCount[step.name]].CanId = 7;
                    upperToLower.GetSample[stepCount[step.name]].curFaceTypeIndex = 1;
                    upperToLower.GetSample[stepCount[step.name]].Type == 3;
                } else {
                    upperToLower.GetSample[stepCount[step.name]].CanId = canId;
                    upperToLower.GetSample[stepCount[step.name]].curFaceTypeIndex = curFaceTypeIndex;
                    upperToLower.GetSample[stepCount[step.name]].Type = HGOnlineRWDB::getDeviceTypeFromCanId(upperToLower.GetSample[stepCount[step.name]].CanId);
                }
                auto modeResult=findKeyByValue(getSampleMode,wparam["取样模式"]);
                int mode=0;
                if (modeResult.first) mode=modeResult.second;
                if (mode < 0 || mode > 255)
                    upperToLower.GetSample[stepCount[step.name]].mode = 0;
                else
                    upperToLower.GetSample[stepCount[step.name]].mode = static_cast<uint8_t>(mode);

                int speed = std::atoi(wparam["取样速率"].c_str());
                if (speed < 0 || speed > 65535)
                    upperToLower.GetSample[stepCount[step.name]].speed = 0;
                else
                    upperToLower.GetSample[stepCount[step.name]].speed = static_cast<uint16_t>(speed);

                int time = std::atoi(wparam["取样时间"].c_str());
                if (time < 0 || time > 65535)
                    upperToLower.GetSample[stepCount[step.name]].time = 0;
                else
                    upperToLower.GetSample[stepCount[step.name]].time = static_cast<uint16_t>(time);
                printf("mode:%d,speed:%d,time:%d\n", upperToLower.GetSample[stepCount[step.name]].mode, 
                    upperToLower.GetSample[stepCount[step.name]].speed, 
                    upperToLower.GetSample[stepCount[step.name]].time);

                std::ostringstream logtext;
                logtext << "mode:" << upperToLower.GetSample[stepCount[step.name]].mode << \
                    ",speed:" << upperToLower.GetSample[stepCount[step.name]].speed << \
                    ",time:" << upperToLower.GetSample[stepCount[step.name]].time;
                HGLog4Cplus::getLogInstance(LOG_PATH)->logout("quyang", LOGINFO);
                upperToLower.GetSample[stepCount[step.name]].index = static_cast<uint8_t>(i);
                time = std::atoi(wparam["时长"].c_str());
                if (time < 0 || time > 65535)
                    upperToLower.GetSample[stepCount[step.name]].continueTime = 0;
                else 
                    upperToLower.GetSample[stepCount[step.name]].continueTime = static_cast<uint16_t>(time);
            }
            else if (step.name == "测定")
            {
                if (!flag)
                {
                    upperToLower.DetectSample[stepCount[step.name]].CanId = 2;
                    upperToLower.DetectSample[stepCount[step.name]].Type = 6;
                } else {
                    upperToLower.DetectSample[stepCount[step.name]].CanId = canId;
                    upperToLower.DetectSample[stepCount[step.name]].curFaceTypeIndex = curFaceTypeIndex;
                    upperToLower.DetectSample[stepCount[step.name]].Type = HGOnlineRWDB::getDeviceTypeFromCanId(upperToLower.DetectSample[stepCount[step.name]].CanId);
                }

                upperToLower.DetectSample[stepCount[step.name]].index = static_cast<uint8_t>(i);
                int time = std::atoi(wparam["时长"].c_str());
                if (time < 0 || time > 65535)
                    upperToLower.DetectSample[stepCount[step.name]].continueTime = 0;
                else 
                    upperToLower.DetectSample[stepCount[step.name]].continueTime = static_cast<uint16_t>(time);    
                auto modeResult=findKeyByValue(kulunMethodShift,wparam["电解电极"]);
                int mode=0;
                if (modeResult.first) mode=modeResult.second;
                if (mode < 0 || mode > 255)
                    upperToLower.DetectSample[stepCount[step.name]].dddj = 0;
                else
                    upperToLower.DetectSample[stepCount[step.name]].dddj=static_cast<uint8_t>(mode);
                upperToLower.DetectSample[stepCount[step.name]].ddTypeMin=std::atoi(wparam["最短时长"].c_str());
                upperToLower.DetectSample[stepCount[step.name]].ddTypeMax=std::atoi(wparam["最长时长"].c_str());
                upperToLower.DetectSample[stepCount[step.name]].ddCuiQuTime=std::atoi(wparam["萃取时间"].c_str());
                upperToLower.DetectSample[stepCount[step.name]].targetDW=std::atoi(wparam["终点电位"].c_str());
                modeResult=findKeyByValue(kulunMethodShift,wparam["终点方式"]);
                if (modeResult.first) mode=modeResult.second;
                if (mode < 0 || mode > 255) upperToLower.DetectSample[stepCount[step.name]].targetType=0;
                else upperToLower.DetectSample[stepCount[step.name]].targetType=static_cast<uint8_t>(mode);
                upperToLower.DetectSample[stepCount[step.name]].targetElapsed=std::atoi(wparam["终点延时"].c_str());
                upperToLower.DetectSample[stepCount[step.name]].targetRange=std::atoi(wparam["动态范围"].c_str());
            }
            else if (step.name == "进样")
            {
                if (!flag)
                {
                    upperToLower.InjectSample[stepCount[step.name]].CanId = 7;
                    upperToLower.InjectSample[stepCount[step.name]].curFaceTypeIndex = 4;
                    upperToLower.InjectSample[stepCount[step.name]].Type = 3;
                } else {
                    upperToLower.InjectSample[stepCount[step.name]].CanId = canId;
                    upperToLower.InjectSample[stepCount[step.name]].curFaceTypeIndex = curFaceTypeIndex;
                    upperToLower.InjectSample[stepCount[step.name]].Type = HGOnlineRWDB::getDeviceTypeFromCanId(upperToLower.InjectSample[stepCount[step.name]].CanId);
                }

                auto modeResult=findKeyByValue(getSampleMode,wparam["取样模式"]);
                int mode=0;
                if (modeResult.first) mode=modeResult.second;
                if (mode < 0 || mode > 255)
                    upperToLower.InjectSample[stepCount[step.name]].mode = 0;
                else
                    upperToLower.InjectSample[stepCount[step.name]].mode = static_cast<uint8_t>(mode);

                int speed = std::atoi(wparam["取样速率"].c_str());
                if (speed < 0 || speed > 65535)
                    upperToLower.InjectSample[stepCount[step.name]].speed = 0;
                else
                    upperToLower.InjectSample[stepCount[step.name]].speed = static_cast<uint16_t>(speed);

                int time = std::atoi(wparam["取样时间"].c_str());
                if (time < 0 || time > 65535)
                    upperToLower.InjectSample[stepCount[step.name]].time = 0;
                else
                    upperToLower.InjectSample[stepCount[step.name]].time = static_cast<uint16_t>(time);
                upperToLower.InjectSample[stepCount[step.name]].index = static_cast<uint8_t>(i);
                time = std::atoi(wparam["时长"].c_str());
                if (time < 0 || time > 65535)
                    upperToLower.InjectSample[stepCount[step.name]].continueTime = 0;
                else 
                    upperToLower.InjectSample[stepCount[step.name]].continueTime = static_cast<uint16_t>(time);
            }

            stepCount[step.name]++;
        }
        return upperToLower;
    }
}
