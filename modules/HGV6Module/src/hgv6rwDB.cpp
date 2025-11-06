#include "hgv6rwDB.h"
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <iomanip>
#include "HGExactTime.h"
#include "hgcommonutility.h"
#include "config.h"

namespace HGMACHINE{
    std::vector<std::string> methodInfoName={
        "methodIndex",
        "name",
        "param",
        "publicParam",
        "choose",
        "status",
        "esignHistoryTableName",
        "paramHash"
    };
    std::vector<std::string> systemInfoName={
        "reagentRefillThreashold",       // 试剂补液阈值  
        "measuringCubeVolume",           // 计量管体积ml
        "polarizationCurrent",           // 极化电流uA
        "autoHoldEndpoint",              // 自动保持终点
        "autoPrint",                     // 自动打印
        "instrumentId",                  // 仪器编号
        "instrumentType",                // 仪器型号
        "Version"                       // 版本
    };
    std::vector<std::string> reagentInfoName={
        "TitrationCellVolumeMonitoring", // 滴定池容量监控  true-开,false-关
        "TitrationCellCurVolume",        // 滴定池当前容量
        "TitrationCellInitVolume",       // 滴定池初始容量
        "TitrationCellWarnVolume",       // 滴定池报警容量
        "ReagentBottleVolumeMonitoring", // 试剂瓶容量监控  true-开,false-关
        "ReagentBottoleCurVolume",       // 试剂瓶当前容量
        "ReagentBottleInitVolume",       // 试剂瓶初始容量
        "ReagentBottleWarnVolume",        // 试剂瓶报警容量
        "WasteBottleVolumeMonitoring",   //  废液瓶容量监控  true-开,false-关
        "WasteBottleCurVolume",          //  废液瓶当前容量
        "WasteBottoleInitVolume",        //  废液瓶初始容量
        "WastBottleWarnVolume",          //   废液瓶报警容量
        "SolventBottleVolumeMonitoring", //  溶剂瓶容量监控  true-开,false-关
        "SolveBottleCurVolume",          //  溶剂瓶当前容量
        "SolveBottleInitVolume",         //   溶剂瓶初始容量
        "SolveBottleWarnVolume",        //  溶剂瓶报警容量
        "SolventSuctionFlowRate",       //   吸溶剂流量
        "SolventSuctionTime",           //  吸溶剂时间
        "WasteDischargeFlowRate",       //   排废液流量
        "WasteDischargeTime"            //   排废液时间
    };
    std::vector<std::string> sampleInfoName={
        "EstimatedWaterVolume",       // 预计含水量  
        "Serial",                    // 样品编号
        "Weight",                    // 样品重量
        "InjectionMode",             // 进样方式  重量/体积%/体积gL
        "Density"                   // 样品密度
    };
    std::vector<std::string> detInfoName={
        "CurTemp",
        "TitrantDensity",
        "SetInjectionVolume",
        "NumberofVerificationCycles"
    };
    std::vector<std::string> oneStartInfoName ={
        "Wastedrainagetimeenable",           // 排废液时间使能			 			
        "Wastedrainagetime",                 // 排废液时间
        "Solventaspirationtimeenable",       // 吸溶剂时间使能
        "Solventaspirationtime",             // 吸溶剂时间
        "Measuringtuberinsecountenable",     // 计量管清洗次数使能
        "Measuringtuberinsecount",           // 计量管清洗次数
        "Runblank"                          // 打空白}
    };
    std::vector<std::string> sampleDetectInfoName={
        "LoginName",                     // 当前用户  
        "MethodName",                    // 当前方法
        "TitrationValue",                // 滴定度
        "MeasuringCubeVolume",           // 计量管体积
        "InjectionVolume",               // 进样量
        "DriftValue",                    // 漂移值
        "TitrationSpeed",                // 滴定速率
        "ReagentConsumption",            // 消耗试剂
        "Data",                          // 滴定结果
        "SwirlSpeed",                    // 搅拌速度
        "AnalysisDuration",              // 分析时长
        "Voltage",                       // 电压
        "Date"                           // 时间
    };
    std::vector<std::string> tirationName={
        "RecordNo",          // 记录号
        "Time",              // 日期时间
        "Serial",            // 样品编号
        "SampleName",        // 样品名称
        "Data",              // 滴定结果
        "TitrationValue",    // 滴定值
        "AnalysisDuration",  // 分析时长
        "InStatistics",      // 参与统计
        "methodIndex",       // 方法索引
        "SampleWeight",      // 样品重量
        "SampleCube" ,       // 样品体积
        "Density",           // 样品密度
        "ReagentConsumption",// 消耗试剂"
        "WaterWeight",       // 水质量
        "DriftValue",        // 漂移值
        "LoginName",         // 当前用户
        "status",             // 状态
        "esignHistoryTableName" // 签名历史表名
    };
    std::vector<std::string> calibrationName={
        "RecordNo",                          // 记录号
        "Time",                              // 日期时间
        "CalibrationResult",                 // 标定结果
        "Average",                           // 平均值
       // "AnalysisDuration",                  // 分析时长
        "InStatistics",                      // 参与统计
        "Manipulator",                       // 操作员
        "CalibrationWithPureWater",          // 标定纯水
        "CalibrationDuration",               // 标定时长
        "DriftCorrection",                   // 漂移扣除
        "DriftValue",                        // 漂移值
        "ReagentConsumption",                // 消耗试剂
    };
    std::vector<std::string> calibrationnStatisticsName={
        "RecordCount",                       // 统计记录总数
        "RecordNo",                          // 记录编号
        "Operator",                          // 操作员
        "Time",                              // 日期时间
        "TitrationConcentration",            // 滴定剂浓度
        "Average",                           // 平均值
        "StandardDeviation",                 // 绝对标准偏差
        "RelativeStandardDeviation",         // 相对标准偏差
        "DownloadMCU"                        // 统计结果下发给MCU标志位
    };
    std::vector<std::string> inspectionName={
        "RecordNo",          // 记录号
        "Time",              // 日期时间
        "SetValue",          // 设定体积(mL)
        "MeasuredValue",     // 实际体积(mL)
        "MeasuredWeight",    // 实际重量
        "Average",           // 平均值
        "Count",             // 校正次数
        "ReagentDensity",    // 试剂密度
        "Deviation",         // 偏差
        "LoginName",         // 当前用户
        "Coefficient",       // 系数
        "Calibrated"         // 校正
    };
    std::vector<std::string> inspectionStatisticsName={
        "RecordNoRange",
        "Average",
        "StandardDeviation",
        "RelativeStandardDeviation"
    };
    std::vector<std::string> esignHistoryName={
        "Time",
        "UserNo",
        "Flow",
        "Reason"
    };
    HGV6RWDB::HGV6RWDB(){

    }
    HGV6RWDB::~HGV6RWDB(){

    }
    /**
     * @brief 读取方法记录
     *  表名tableName: METHODRECORDDBNAME
     * @return std::vector<std::map<std::string,std::string>> 数据 {
     *     "methodIndex": "",
     *     "name": "",
     *     "param": "参数名1:内容1;参数名2:内容2;......",
     *     "choose": ""}
     */
    std::vector<std::map<std::string, std::string>> HGV6RWDB::readMethodRecord(){
        std::vector<std::map<std::string, std::string>> fillContent;
        std::map<std::string, std::string> infoS;
        for (int i = 0; i < int(methodInfoName.size()); i++)
        {
            infoS[methodInfoName[i]] = "";
        }
        fillContent = readRecord(METHODRECORDDBNAME, infoS);
        return fillContent;
    }
    std::vector<std::map<std::string,std::string>> HGV6RWDB::readMethodEsignHistory(const std::string& tableName){
        std::vector<std::map<std::string,std::string>> fillContent;
        std::map<std::string,std::string> infoS;
        for (int i = 0; i < int(esignHistoryName.size()); i++){
            infoS[esignHistoryName[i]] = "";
        }
        fillContent=readRecord(tableName,infoS);
        return fillContent;
    }
    std::map<std::string,std::string> HGV6RWDB::readOneStartInfo(){
        std::map<std::string, std::string> infoS;
        for (int i = 0; i < int(oneStartInfoName.size()); i++)
        {
            infoS[oneStartInfoName[i]] = "";
        }
        readSingleInfo(ONESTARTDBNAME, infoS);
        return infoS;
    }
    /**
     * @brief 读取试剂记录
     *  表名tableName: REAGENDBNAME
     * @param std::map<std::string,std::string> output 数据 {
     *     "TitrationCellVolumeMonitoring": "", // 滴定池容量监控  true-开,false-关
     *     "TitrationCellCurVolume": "",        // 滴定池当前容量
     *     "TitrationCellInitVolume": "",       // 滴定池初始容量
     *     "TitrationCellWarnVolume": "",       // 滴定池报警容量
     *     "ReagentBottleVolumeMonitoring": "", // 试剂瓶容量监控  true-开,false-关
     *     "ReagentBottoleCurVolume": "",       // 试剂瓶当前容量
     *     "ReagentBottleInitVolume": "",       // 试剂瓶初始容量
     *     "ReagentBottleWarnVolume": "",        // 试剂瓶报警容量
     *       "WasteBottleVolumeMonitoring":"",   //  废液瓶容量监控  true-开,false-关
     *       "WasteBottleCurVolume":"",          //  废液瓶当前容量
     *       "WasteBottoleInitVolume":"",        //  废液瓶初始容量
     *       "WastBottleWarnVolume":"",          //   废液瓶报警容量
     *       "SolventBottleVolumeMonitoring":"", //  溶剂瓶容量监控  true-开,false-关
     *       "SolveBottleCurVolume":"",          //  溶剂瓶当前容量
     *       "SolveBottleInitVolume":"",         //   溶剂瓶初始容量
     *       "SolveBottleWarnVolume": "",        //  溶剂瓶报警容量
     *       "SolventSuctionFlowRate": "",       //   吸溶剂流量
     *       "SolventSuctionTime": "",           //  吸溶剂时间
     *       "WasteDischargeFlowRate": "",       //   排废液流量
     *       "WasteDischargeTime": ""            //   排废液时间
     * }
     */
    std::map<std::string, std::string> HGV6RWDB::readReagentInfo()
    {
        std::map<std::string, std::string> infoS;
        for (int i = 0; i < int(reagentInfoName.size()); i++)
        {
            infoS[reagentInfoName[i]] = "";
        }
        if (dbOpera.isTableExist(REAGENDBNAME)){
            readSingleInfo(REAGENDBNAME, infoS);
        }
        return infoS;
    }
    /**
     * @brief 读取系统参数
     *  表名tableName: SYSTEMINFODBNAME
     * @param std::map<std::string,std::string> output 数据 {
     *     "reagentRefillThreashold": "",       // 试剂补液阈值
     *     "measuringCubeVolume": "",           // 计量管体积ml
     *     "polarizationCurrent": "",           // 极化电流uA
     *     "autoHoldEndpoint": "",              // 自动保持终点
     *     "autoPrint": "",                     // 自动打印
     *     "instrumentId": "",                  // 仪器编号
     *     "instrumentType": "",                // 仪器型号
     *     "Version": "",                        // 版本
     * }
     */
    std::map<std::string, std::string> HGV6RWDB::readSystemInfo(){
        std::map<std::string, std::string> infoS;
        for (int i = 0; i < int(systemInfoName.size()); i++)
        {
            infoS[systemInfoName[i]] = "";
        }
        if (dbOpera.isTableExist(SYSTEMINFODBNAME)){
            readSingleInfo(SYSTEMINFODBNAME, infoS);
        }
        
        return infoS;
    }
    std::map<std::string,std::string> HGV6RWDB::readDetInfo(){
        std::map<std::string,std::string> infoS;
        for (int i = 0; i < int(detInfoName.size()); i++){
            infoS[detInfoName[i]] = "";
        }
        if (dbOpera.isTableExist(DETDBNAME)){
            readSingleInfo(DETDBNAME, infoS);
        }
        return infoS;
    }
    /**
     * @brief 读取样品参数
     *  表名tableName: SAMPLEDBNAME
     * @param std::map<std::string,std::string> output 数据 {
     *     "EstimatedWaterVolume": "",       // 预计含水量
     *     "Serial": "",                    // 样品编号
     *     "Weight": "",                    // 样品重量
     *     "InjectionMode": "",             // 进样方式  重量/体积%/体积gL
     *     "Density": "",                   // 样品密度
     * }
     */
    std::map<std::string, std::string> HGV6RWDB::readSampleInfo(){
        std::map<std::string, std::string> infoS;
        for (int i = 0; i < int(sampleInfoName.size()); i++)
        {
            infoS[sampleInfoName[i]] = "";
        }
        if (dbOpera.isTableExist(SAMPLEDBNAME)){
            readSingleInfo(SAMPLEDBNAME, infoS);
        }
        return infoS;
    }
    void HGV6RWDB::writeMethodRecord(const std::vector<std::map<std::string,std::string>> &infoSS,bool coverFlag){
        if (coverFlag){
            dbOpera.clearDB(METHODRECORDDBNAME);
        }
        std::string keyName = "methodIndex";
        for (int i = 0; i < int(infoSS.size()); i++)
        {
            dbOpera.writeRecord(METHODRECORDDBNAME, keyName, infoSS[i]);
        }
    }
    void HGV6RWDB::writeMethodEsignHistory(const std::vector<std::map<std::string,std::string>> &infoSS,const std::string& tableName){
        std::string keyName="Time";
        for (int i = 0; i < int(infoSS.size()); i++){
            dbOpera.writeRecord(tableName, keyName, infoSS[i]);
        }
    }
    /**
     * @brief 写入试剂记录
     *  表名tableName: REAGENDBNAME
     * @param infoSS input 数据 {
     *     "TitrationCellVolumeMonitoring": "", // 滴定池容量监控  true-开,false-关
     *     "TitrationCellCurVolume": "",        // 滴定池当前容量
     *     "TitrationCellInitVolume": "",       // 滴定池初始容量
     *     "TitrationCellWarnVolume": "",       // 滴定池报警容量
     *     "ReagentBottleVolumeMonitoring": "", // 试剂瓶容量监控  true-开,false-关
     *     "ReagentBottoleCurVolume": "",       // 试剂瓶当前容量
     *     "ReagentBottleInitVolume": "",       // 试剂瓶初始容量
    *     "ReagentBottleWarnVolume": "",        // 试剂瓶报警容量
    *       "WasteBottleVolumeMonitoring":"",   //  废液瓶容量监控  true-开,false-关
    *       "WasteBottleCurVolume":"",          //  废液瓶当前容量
    *       "WasteBottoleInitVolume":"",        //  废液瓶初始容量
    *       "WastBottleWarnVolume":"",          //   废液瓶报警容量
    *       "SolventBottleVolumeMonitoring":"", //  溶剂瓶容量监控  true-开,false-关
    *       "SolveBottleCurVolume":"",          //  溶剂瓶当前容量
    *       "SolveBottleInitVolume":"",         //   溶剂瓶初始容量
    *       "SolveBottleWarnVolume": "",        //  溶剂瓶报警容量
    *       "SolventSuctionFlowRate": "",       //   吸溶剂流量
    *       "SolventSuctionTime": "",           //  吸溶剂时间
    *       "WasteDischargeFlowRate": "",       //   排废液流量
    *       "WasteDischargeTime": ""            //   排废液时间
    * }
    */
    void HGV6RWDB::writeReagentInfo(const std::map<std::string,std::string> &infoS){
        dbOpera.recordSingleInfo(REAGENDBNAME, infoS);
    }
    void HGV6RWDB::writeOneStartInfo(const std::map<std::string,std::string> &infoS){
        dbOpera.recordSingleInfo(ONESTARTDBNAME, infoS);
    }
    void HGV6RWDB::writeDetInfo(const std::map<std::string,std::string> &infoS){
        dbOpera.recordSingleInfo(DETDBNAME,infoS);
    }

    /**
     * @brief 写入系统参数
     *  表名tableName: SYSTEMINFODBNAME
     * @param infoSS input 数据 {
     *     "reagentRefillThreashold": "",       // 试剂补液阈值  
     *     "measuringCubeVolume": "",           // 计量管体积ml
     *     "polarizationCurrent": "",           // 极化电流uA
     *     "autoHoldEndpoint": "",              // 自动保持终点
     *     "autoPrint": "",                     // 自动打印
     *     "instrumentId": "",                  // 仪器编号
     *     "instrumentType": "",                // 仪器型号
    *     "Version": "",                        // 版本
    * }
    */
    void HGV6RWDB::writeSystemInfo(const std::map<std::string,std::string> &infoS){
        dbOpera.recordSingleInfo(SYSTEMINFODBNAME, infoS);
    }
    /**
     * @brief 写入样品参数
     *  表名tableName: SAMPLEDBNAME
     * @param input 数据 {
     *     "EstimatedWaterVolume": "",       // 预计含水量  
     *     "Serial": "",                    // 样品编号
     *     "Weight": "",                    // 样品重量
     *     "InjectionMode": "",             // 进样方式  重量/体积%/体积gL
     *     "Density": "",                   // 样品密度
    * }
    */
    void HGV6RWDB::writeSampleInfo(const std::map<std::string,std::string> &infoS){
        dbOpera.recordSingleInfo(SAMPLEDBNAME, infoS);
    }
    void HGV6RWDB::writeSampleDetectInfo(const std::map<std::string, std::string> &infoS)
    {
        dbOpera.writeRecord(SAMPLEDETECTDBNAME, "Date", infoS);
        dbOpera.sortTable(SAMPLEDETECTDBNAME, "Date");
    }
    std::vector<std::map<std::string,std::string>> HGV6RWDB::readSampleDetectInfo(){
        std::vector<std::map<std::string, std::string>> fillContent;
        std::map<std::string, std::string> infoS;
        for (int i = 0; i < int(sampleDetectInfoName.size()); i++)
        {
            infoS[sampleDetectInfoName[i]] = "";
        }
        fillContent = readRecord(SAMPLEDETECTDBNAME, infoS);
        return fillContent;
    }
    std::vector<std::map<std::string,std::string>> HGV6RWDB::readTitrationRecord(){
        std::vector<std::map<std::string, std::string>> fillContent;
        std::map<std::string, std::string> infoS;
        for (int i = 0; i < int(tirationName.size()); i++)
        {
            infoS[tirationName[i]] = "";
        }
        fillContent = readRecord(TIRATIONRECORDDBNAME, infoS);
        return fillContent;
    }
    std::vector<std::map<std::string,std::string>> HGV6RWDB::readCalibrationRecord(){
        std::vector<std::map<std::string, std::string>> fillContent;
        std::map<std::string, std::string> infoS;
        for (int i = 0; i < int(calibrationName.size()); i++)
        {
            infoS[calibrationName[i]] = "";
        }
        fillContent = readRecord(CALIBRATIONDBNAME, infoS);
        return fillContent;
    }
    std::vector<std::map<std::string,std::string>> HGV6RWDB::readCalibrationStatisticsRecord(){
        std::vector<std::map<std::string, std::string>> fillContent;
        std::map<std::string, std::string> infoS;
        for (int i=0;i<int(calibrationnStatisticsName.size());i++){
            infoS[calibrationnStatisticsName[i]]="";
        }
        fillContent = readRecord(CALIBRATIONSTATISTICSDBNAME, infoS);
        return fillContent;
    }
    std::vector<std::map<std::string,std::string>> HGV6RWDB::readInspectionRecord(){
        std::vector<std::map<std::string, std::string>> fillContent;
        std::map<std::string, std::string> infoS;
        for (int i = 0; i < int(inspectionName.size()); i++)
        {
            infoS[inspectionName[i]] = "";
        }
        fillContent = readRecord(INSPECTIONDBNAME, infoS);
        return fillContent;
    }
    std::vector<std::map<std::string,std::string>> HGV6RWDB::readInspectionStatistics(){
        std::vector<std::map<std::string, std::string>> fillContent;
        std::map<std::string, std::string> infoS;
        for (int i = 0; i < int(inspectionStatisticsName.size()); i++)
        {
            infoS[inspectionStatisticsName[i]] = "";
        }
        fillContent = readRecord(INSPECTIONSTATICSDBNAME, infoS);
        return fillContent;
    }
    void HGV6RWDB::writeTitrationRecord(const std::vector<std::map<std::string,std::string>> &infoS){
        std::string keyName = "RecordNo";
        for (int i = 0; i < int(infoS.size()); i++)
        {
            dbOpera.writeRecord(TIRATIONRECORDDBNAME, keyName, infoS[i]);
        }
        dbOpera.sortTable(TIRATIONRECORDDBNAME, keyName);
    }
    void HGV6RWDB::writeCalibrationRecord(const std::vector<std::map<std::string,std::string>> &infoS){
        std::string keyName = "RecordNo";
        for (int i = 0; i < int(infoS.size()); i++)
        {
            dbOpera.writeRecord(CALIBRATIONDBNAME, keyName, infoS[i]);
        }
        dbOpera.sortTable(CALIBRATIONDBNAME, keyName);
    }
    void HGV6RWDB::writeCalibrationStatisticsRecord(const std::vector<std::map<std::string,std::string>> &infoS){
        std::string keyName = "Time";
        for (int i = 0; i < int(infoS.size()); i++)
        {
            dbOpera.writeRecord(CALIBRATIONSTATISTICSDBNAME, keyName, infoS[i]);
        }
    }
    void HGV6RWDB::writeInspectionRecord(const std::vector<std::map<std::string,std::string>> &infoS, 
                    const std::vector<std::map<std::string,std::string>> &statistics)
    {
        std::string keyName = "RecordNo";
        for (int i = 0; i < int(infoS.size()); i++)
        {
            dbOpera.writeRecord(INSPECTIONDBNAME, keyName, infoS[i]);
        }
        keyName = "RecordNoRange";
        for (int i=0;i<int(statistics.size());i++)
        {
            dbOpera.writeRecord(INSPECTIONSTATICSDBNAME, keyName, statistics[i]);
        }
        dbOpera.sortTable(INSPECTIONDBNAME, keyName);
        dbOpera.sortTable(INSPECTIONSTATICSDBNAME, keyName);
    }
}