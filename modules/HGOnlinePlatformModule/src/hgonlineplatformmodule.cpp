#include "hglog4cplus.h"
#include "hgcommonutility.h"
#include "hgserial.h"
#include "hgonlineplatformmodule.h"
#include "hgxml.h"

using namespace HGMACHINE;


namespace HGMACHINE{
    std::map<std::string,std::string> taskWash ={
        {"排液模式",""},
        {"注入试剂",""},
        {"注入体积",""},
        {"循环次数",""}
    };
    std::map<std::string,std::string> taskInjectReagent ={
        {"注入试剂",""},
        {"注入模式",""},
        {"注入速率",""},
        {"注入体积",""}
    };
    std::map<std::string,std::string> taskBlank ={
        {"滴定剂",""},
        {"固定时长",""},
        {"输出",""}
    };
    std::map<std::string,std::string> taskGetSample ={
        {"取样模式",""},
        {"取样速率",""},
        {"取样时间",""}
    };
    std::map<std::string,std::string> taskDetect ={
        {"最小滴定体积(ul)",""},
        {"最大滴定体积(ul)",""},
        {"每滴暂停(ms)",""},
        {"预滴定(ml)",""},
        {"预滴后停(s)",""},
        {"搅拌速度",""}
    };
    std::map<std::string,std::string> taskHoldSample ={
        {"计量方式",""},
        {"留液速度(rps)",""},
        {"留液时间(s)",""}
    };
    std::map<int,std::string> getSampleMode={
        {0,"计时模式"},
        {1,"体积模式"}
    };
    std::map<int,std::string> washPaiYeMode={
        {0,"完全排空"},
        {1,"部分排空"}
    };
    std::map<int,std::string> injectReagentMode={
        {0,"计时注入"},
        {1,"体积注入"}
    };
    std::map<int,std::string> kulunMethodShift={
        {0,"1"},
        {1,"2"},
        {2,"3"},
        {3,"4"},
        {4,"5"},
        {5,"自动档"}
    };
    std::map<int,std::string> targetType={
        {0,"延时终止"},
        {1,"漂移终止"}
    };

HgOnlinePlatformModule::HgOnlinePlatformModule()
{

}
HgOnlinePlatformModule::~HgOnlinePlatformModule()
{

}


}