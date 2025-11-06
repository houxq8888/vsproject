#ifndef HGV6RWDB_H
#define HGV6RWDB_H

#include <map>
#include <vector>
#include <list>
#include <stdint.h>
#include <stdlib.h>
#include "hgsavedatatodb.h"
#include "rwDb.h"


#ifdef __cplusplus
extern "C" {
#endif
namespace HGMACHINE{
    #define METHODRECORDDBNAME "MethodRecord"
    #define REAGENDBNAME "ReagentInfo"
    #define SAMPLEDBNAME "SampleInfo"
    #define DETDBNAME "DetectionInfo"
    #define ONESTARTDBNAME "OneStartInfo"
    #define SAMPLEDETECTDBNAME "SampleDetectRecord"
    #define TIRATIONRECORDDBNAME "TitrationRecord"
    #define CALIBRATIONDBNAME "CalibrationRecord"
    #define CALIBRATIONSTATISTICSDBNAME "CalibrationStatisticsRecord"
    #define INSPECTIONDBNAME "InspectionRecord"
    #define INSPECTIONSTATICSDBNAME "InspectionStatistics"

    class HGV6RWDB : public RWDb
    {
        public:
            HGV6RWDB();
            ~HGV6RWDB();
            
            /**
             * @brief 读取方法记录
             *  表名tableName: METHODRECORDDBNAME
             * @return std::vector<std::map<std::string,std::string>> 数据 {
             *     "methodIndex": "",
             *     "name": "",
             *     "param": "参数名1:内容1;参数名2:内容2;......",
             *     "publicParam" : "参数名1:内容1;参数名2:内容2;......",
             *     "choose": "",
             *     "status": "",
             *     "esignHistoryTableName": ""
             *     "paramHash": "" param hash
             * }
             */
            static std::vector<std::map<std::string,std::string>> readMethodRecord();
            /**
             * @brief 读取方法电子签名历史
             *  表名tableName
             * @return std::vector<std::map<std::string,std::string>> 数据 {
             *     "Time": "",
             *     "UerNo": "",
             *     "Flow": "参数名1:内容1;参数名2:内容2;......",
             *     "Reason": ""}
             */
            static std::vector<std::map<std::string,std::string>> readMethodEsignHistory(const std::string& tableName);
            /**
             * @brief 读取滴定记录
             *  表名tableName: TIRATIONRECORDDBNAME
             * @param std::map<std::string,std::string> output 数据 {
             *     "RecordNo": "",          // 记录号
             *     "Time": "",              // 日期时间
             *     "Serial": "",            // 样品编号
             *     "SampleName": "",        // 样品名称
             *     "Data": "",              // 滴定结果
             *     "TitrationValue",        // 滴定值
             *     "AnalysisDuration": "",  // 分析时长
             *     "InStatistics": "",      // 参与统计
             *     "methodIndex"            // 方法号
             *     "SampleWeight""          // 样品重量
             *     "SampleCube"             // 样品体积
             *     "Density"""              // 样品密度
             *     "ReagentConsumption": "",// 消耗试剂"
             *     "WaterWeight"            // 水质量
             *     "DriftValue"             // 漂移值
             *     "LoginName",             // 当前用户
             *     "status"                  // 状态
             *     "esignHistoryTableName"  // 电子签名表名
            * }
            */
            static std::vector<std::map<std::string,std::string>> readTitrationRecord();
            /**
             * @brief 读取标定记录
             *  表名tableName: CALIBRATIONDBNAME
             * @param std::map<std::string,std::string> output 数据 {
             *     "RecordNo": "",                          // 记录号
             *     "Time": "",                              // 日期时间
             *     "CalibrationResult": "",                 // 标定结果
             *     "Average": "",                           // 平均值
             *     "AnalysisDuration": "",                  // 分析时长 (delete)
             *     "InStatistics": "",                      // 参与统计
             *     "Manipulator": "",                       // 操作员
             *     "CalibrationWithPureWater": "",          // 标定纯水
             *     "CalibrationDuration": "",               // 标定时长
             *     "DriftCorrection": "",                   // 漂移扣除
             *     "DriftValue": "",                        // 漂移值
             *     "ReagentConsumption": "",                // 消耗试剂
            * }
            */
            static std::vector<std::map<std::string,std::string>> readCalibrationRecord();
            /**
             * @brief 读取标定统计记录
             *  表名tableName: CALIBRATIONSTATISTICSDBNAME
             * @param std::map<std::string,std::string> output 数据 {
             * "RecordCount",                       // 统计记录总数
             * "RecordNo",                          // 记录编号
             * "Operator",                          // 操作员
             * "Time",                              // 日期时间
             * "TitrationConcentration",            // 滴定剂浓度
             * "Average",                           // 平均值
             * "StandardDeviation",                 // 绝对标准偏差
             * "RelativeStandardDeviation",         // 相对标准偏差
             * "DownloadMCU"                        // 统计结果下发给MCU标志位
            * }
            */
            static std::vector<std::map<std::string,std::string>> readCalibrationStatisticsRecord();
            
            /**
             * @brief 读取检定记录
             *  表名tableName: INSPECTIONDBNAME
             * @param std::map<std::string,std::string> output 数据 {
             *     "RecordNo": "",          // 记录号
             *     "Time": "",              // 日期时间
             *     "SetValue": "",          // 设定体积(mL)
             *     "MeasuredValue": "",     // 实际体积(mL)
             *     "MeasuredWeight": "",    // 实际重量
             *     "Average": "",           // 平均值
             *     "Count"                  // 校正次数
             *     "ReagentDensity": "",    // 试剂密度
             *     "Deviation": "",         // 偏差
             *     "LoginName"              // 当前用户
             *     "Coefficient"            // 系数
             *     "Calibrated"             // 校正
            * }
            */
            static std::vector<std::map<std::string,std::string>> readInspectionRecord();
            /**
             * @brief 读取统计信息
             *  表名tableName: INSPECTIONSTATICSDBNAME  检定统计信息
             *    std::map<std::string,std::string> input statistics{
             *     "RecordNoRange": "",                 // 记录号范围
             *     "Average" :  "",                     // 平均值
             *     "StandardDeviation": "",             // 标准偏差"
             *     "RelativeStandardDeviation": "",     // 相对标准偏差
             * }
            */
            static std::vector<std::map<std::string,std::string>> readInspectionStatistics();
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
            static std::map<std::string,std::string> readReagentInfo();
            /**
             * @brief 读取一键准备
             *  表名tableName: ONESTARTDBNAME
             * @param infoSS output 数据 {
             *     "Wastedrainagetimeenable": "",           // 排废液时间使能			 			
             *     "Wastedrainagetime": "",                 // 排废液时间
             * 	   "Solventaspirationtimeenable": "",       // 吸溶剂时间使能
             *     "Solventaspirationtime": "",             // 吸溶剂时间
             *     "Measuringtuberinsecountenable": "",     // 计量管清洗次数使能
             * 	   "Measuringtuberinsecount": "",           // 计量管清洗次数
             * 	   "Runblank": "",                          // 打空白}
             * @return void
             */
            static std::map<std::string,std::string> readOneStartInfo();
            /**
             * @brief 读取检定
             *  表名tableName: DETDBNAME
             * @param std::map<std::string,std::string> output 数据 {
             *     "CurTemp": "",                           // 当前温度 
             *     "TitrantDensity": "",                    // 滴定剂密度
             *     "SetInjectionVolume": "",                // 设定注液量
             *     "NumberofVerificationCycles": "",        // 检定次数
            * }
            */
            static std::map<std::string,std::string> readDetInfo();
            /**
             * @brief 读取样品检测Data
             *  表名tableName: SAMPLEDETECTDBNAME
             * @param output 数据 {
             *     "LoginName": "",                     // 当前用户  
             *     "MethodName": "",                    // 当前方法
             *     "TitrationValue": "",                // 滴定度
             *     "MeasuringCubeVolume": "",           // 计量管体积
             *     "InjectionVolume": "",               // 进样量
             *     "DriftValue": "",                    // 漂移值
             *     "TitrationSpeed": "",                // 滴定速率
             *     "ReagentConsumption": "",            // 消耗试剂
             *     "Data": "",                          // 滴定结果
             *     "SwirlSpeed": "",                    // 搅拌速度
             *     "AnalysisDuration": "",              // 分析时长
             *     "Voltage": "",                       // 电压
             *     "Date": "",                          // 时间
            * }
            */
            static std::vector<std::map<std::string,std::string>> readSampleDetectInfo();
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
            *      "Version": "",                       // 版本
            * }
            */
            static std::map<std::string,std::string> readSystemInfo();
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
           static std::map<std::string,std::string> readSampleInfo();
            /**
             * @brief 写入方法记录
             *  表名tableName: METHODRECORDDBNAME
             * @param infoSS input 数据 {
             *     "methodIndex": "",
             *     "name": "",
             *     "param": "",
             *     "choose": ""
             *     "status": "",
             *     "esignHistoryTableName": ""}
             * @param coverFlag input 是否覆盖
             * @return void
             */
            static void writeMethodRecord(const std::vector<std::map<std::string,std::string>> &infoSS,bool coverFlag=false);
            /**
             * @brief 写入方法电子签名历史
             *  表名tableName
             * @return std::vector<std::map<std::string,std::string>> 数据 {
             *     "Time": "",
             *     "UerNo": "",
             *     "Flow": "参数名1:内容1;参数名2:内容2;......",
             *     "Reason": ""}
             */
            static void writeMethodEsignHistory(const std::vector<std::map<std::string,std::string>> &infoSS,const std::string& tableName);
            /**
             * @brief 写入一键准备
             *  表名tableName: ONESTARTDBNAME
             * @param infoSS input 数据 {
             *     "Wastedrainagetimeenable": "",           // 排废液时间使能			 			
             *     "Wastedrainagetime": "",                 // 排废液时间
             * 	   "Solventaspirationtimeenable": "",       // 吸溶剂时间使能
             *     "Solventaspirationtime": "",             // 吸溶剂时间
             *     "Measuringtuberinsecountenable": "",     // 计量管清洗次数使能
             * 	   "Measuringtuberinsecount": "",           // 计量管清洗次数
             * 	   "Runblank": "",                          // 打空白}
             * @return void
             */
            static void writeOneStartInfo(const std::map<std::string,std::string> &infoS);
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
            static void writeReagentInfo(const std::map<std::string,std::string> &infoS);
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
            static void writeSystemInfo(const std::map<std::string,std::string> &infoS);
            /**
             * @brief 写入检定
             *  表名tableName: DETDBNAME
             * @param std::map<std::string,std::string> input 数据 {
             *     "CurTemp": "",                           // 当前温度 
             *     "TitrantDensity": "",                    // 滴定剂密度
             *     "SetInjectionVolume": "",                // 设定注液量
             *     "NumberofVerificationCycles": "",        // 检定次数
            * }
            */
            static void writeDetInfo(const std::map<std::string,std::string>& info);
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
            static void writeSampleInfo(const std::map<std::string,std::string> &infoS);
            
            /**
             * @brief 写入样品检测Data
             *  表名tableName: SAMPLEDETECTDBNAME
             * @param input 数据 {
             *     "LoginName": "",                     // 当前用户  
             *     "MethodName": "",                    // 当前方法
             *     "TitrationValue": "",                // 滴定度
             *     "MeasuringCubeVolume": "",           // 计量管体积
             *     "InjectionVolume": "",               // 进样量
             *     "DriftValue": "",                    // 漂移值
             *     "TitrationSpeed": "",                // 滴定速率
             *     "ReagentConsumption": "",            // 消耗试剂
             *     "Data": "",                          // 滴定结果
             *     "SwirlSpeed": "",                    // 搅拌速度
             *     "AnalysisDuration": "",              // 分析时长
             *     "Voltage": "",                       // 电压
             *     "Date": "",                          // 时间 系统时间，自动生成Primary key
            * }
            */
            static void writeSampleDetectInfo(const std::map<std::string,std::string> &infoS);
            /**
             * @brief 写入滴定记录
             *  表名tableName: TIRATIONRECORDDBNAME
             * @param std::map<std::string,std::string> input 数据 {
             *     "RecordNo": "",          // 记录号
             *     "Time": "",              // 日期时间
             *     "Serial": "",            // 样品编号
             *     "SampleName": "",        // 样品名称
             *     "Data": "",              // 滴定结果
             *     "TitrationValue",        // 滴定值
             *     "AnalysisDuration": "",  // 分析时长
             *     "InStatistics": "",      // 参与统计
             *     "methodIndex"            // 方法号
             *     "SampleWeight""          // 样品重量
             *     "SampleCube"             // 样品体积
             *     "Density"""              // 样品密度
             *     "ReagentConsumption": "",// 消耗试剂"
             *     "WaterWeight"            // 水质量
             *     "DriftValue"             // 漂移值
             *     "status"                  // 状态
             *     "esignHistoryTableName"  // 电子签名表名
            * }
            */
            static void writeTitrationRecord(const std::vector<std::map<std::string,std::string>> &infoS);
            /**
             * @brief 写入标定记录
             *  表名tableName: CALIBRATIONDBNAME
             * @param std::map<std::string,std::string> input 数据 {
             *     "RecordNo": "",                          // 记录号
             *     "Time": "",                              // 日期时间
             *     "CalibrationResult": "",                 // 标定结果
             *     "Average": "",                           // 平均值
             *     "InStatistics": "",                      // 参与统计
             *     "Manipulator": "",                       // 操作员
             *     "CalibrationWithPureWater": "",          // 标定纯水
             *     "CalibrationDuration": "",               // 标定时长
             *     "DriftCorrection": "",                   // 漂移扣除
             *     "DriftValue": "",                        // 漂移值
             *     "ReagentConsumption": "",                // 消耗试剂
            * }
            */
            static void writeCalibrationRecord(const std::vector<std::map<std::string,std::string>> &infoS);
            static void writeCalibrationStatisticsRecord(const std::vector<std::map<std::string,std::string>> &infoS);
            /**
             * @brief 写入检定记录
             *  表名tableName: INSPECTIONDBNAME  检定记录 
             * INSPECTIONSTATICSDBNAME  检定统计信息
             * @param std::map<std::string,std::string> input infoS {
             *     "RecordNo": "",          // 记录号
             *     "Time": "",              // 日期时间
             *     "SetValue": "",          // 设定体积(mL)
             *     "MeasuredValue": "",     // 实际体积(mL)
             *     "MeasuredWeight": "",    // 实际重量
             *     "Average": "",           // 平均值
             *     "Count"                  // 校正次数
             *     "ReagentDensity": "",    // 试剂密度
             *     "Deviation": "",         // 偏差
             *     "LoginName"              // 当前用户
             *     "Coefficient"            // 系数
             *     "Calibrated"             // 校正
             * * }
             *    std::map<std::string,std::string> input statistics{
             *     "RecordNoRange": "",                 // 记录号范围
             *     "Average" :  "",                     // 平均值
             *     "StandardDeviation": "",             // 标准偏差"
             *     "RelativeStandardDeviation": "",     // 相对标准偏差
             * }
            */
            static void writeInspectionRecord(const std::vector<std::map<std::string,std::string>> &infoS, 
                    const std::vector<std::map<std::string,std::string>> &statistics);

        private:
    };
}
#ifdef __cplusplus
}
#endif
#endif // HGV6RWDB_H