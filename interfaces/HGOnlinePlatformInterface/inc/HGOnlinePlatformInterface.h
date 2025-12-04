#ifndef HGONLINEPLATFORMINTERFACE_H
#define HGONLINEPLATFORMINTERFACE_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include "rwDb.h"
#include "hgonlinerwDB.h"
#include "hgthreadManage.h"
#include "hgonlineplatformmodule.h"
#include "HGExactTime.h"
#include "HGMacroData.h"
#include "hgcommonutility.h"
#include "task.h"
#include "flow.h"
#include "method.h"
#include "hgxml.h"
#include "hgtxt.h"
#include "hgpdf.h"
#include "hgcsv.h"
#include "hgserial.h"
#include "hgprinter.h"
#include "hglog4cplus.h"
#include "HGAuthorityService.h"


namespace HGMACHINE {



extern std::vector<std::string> userInfoName;



#ifdef __cplusplus
extern "C" {
#endif


// database
void openDB(const std::string&dbName);
void closeDB();

// communicate
void createThread();
void startThread();
void startRunTaskSequence();
void pauseTaskSequence();
void resetTaskSequence();
int getCurRunTaskIndex(std::string &status);
std::vector<std::string> getSelfCheckStatus();
void updateFlowInfo();
void updateStyleInfo();
std::map<std::string,std::string> getDataInfo();
int getInterfaceCount();
std::string curDeviceParamDBName();
void stopThread();
int getLinkStatus();
void changeUserLoginAndQuitTime(const std::string &userNo, const std::string &key);
int getConnectWithMCUStatus();

void setHeatOn();
void setHeatOff();
void setFanOn();
void setFanOff();
void setPumpOn();
void setPumpOff();
void setSwirlAscend();
void setSwirlDescend();
void setNeedleRelease();
void setCircleExecute();
void setNeedleArrivalLimitPos();
void setNeedlePunctureRelease();
void setNeedlePunctureStep(double step);
void setCircleNumber(int number);
void setPuncturePosIndex(int index);
void setNeedleArrivalTargetPos();
void setMethodInfoToMCU(const std::map<std::vector<uint8_t>,std::vector<uint8_t>> &sendMethodValues);
void setCurCircleNo(int circleNo);
void setMakeASample();
double getNeedlePunctureCurPos();
std::map<std::string,std::string> getCAS6RawValues();
std::string getMCUState();
bool makeNextSample();
void setCurRunningTaskIndex(int index);
int getCurRunningTaskIndex();
void setStartRunningTaskSequence(bool flag);
std::map<std::string,std::string> getCurRunningTaskInfo();
void makeCurSample();
void setRequestData();

// style
int getTaskCount();
Task getTaskInfo(const int &taskIndex);

// config
bool loadConfig(std::string curPath);
void saveConfig();



}

#ifdef __cplusplus
}
#endif
#endif // HGONLINEPLATFORMINTERFACE_H
