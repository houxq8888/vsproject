# 模块匹配分析报告 v2

总模块数: 170
匹配设计的模块数: 116
未匹配设计的模块数: 54
匹配率: 68.24%

## 按层统计匹配的模块

### 第0层 (4个模块)
- BaseCommon
- BaseConfig
- HGBaseCommon
- HGBaseConfig

### 第1层 (22个模块)
- HGSvcAuthority
- HGSvcCommunicate
- HGSvcDataPostProcess
- HGSvcError
- HGSvcFrame
- HGSvcImageAlgorithm
- HGSvcInteractive
- HGSvcLog
- HGSvcOTA
- HGSvcSave
- HGSvcSecurity
- SvcAuthority
- SvcCommunicate
- SvcDataPostProcess
- SvcError
- SvcFrame
- SvcImageAlgorithm
- SvcInteractive
- SvcLog
- SvcOTA
- SvcSave
- SvcSecurity

### 第2层 (6个模块)
- HGHwLowerPC
- HGHwRobot
- HGHwSensor
- HwLowerPC
- HwRobot
- HwSensor

### 第3层 (10个模块)
- CoreCupDet
- CoreDashboard
- CoreLog
- CoreOnlinePlatform
- CoreV6
- HGCoreCupDet
- HGCoreDashboard
- HGCoreLog
- HGCoreOnlinePlatform
- HGCoreV6

### 第4层 (14个模块)
- HGIfCameraControl
- HGIfCameraRecognize
- HGIfCupDet
- HGIfLog
- HGIfOnlinePlatform
- HGIfPackFunc
- HGIfV6
- IfCameraControl
- IfCameraRecognize
- IfCupDet
- IfLog
- IfOnlinePlatform
- IfPackFunc
- IfV6

### 第5层 (38个模块)
- HGUIAnalysisRecord
- HGUIBaseApp
- HGUICameraRecognize
- HGUIChannel
- HGUICurve
- HGUIEBalance
- HGUIExceptionHandle
- HGUIFlow
- HGUILog
- HGUILogin
- HGUIMethod
- HGUIPrint
- HGUIReagent
- HGUIScanner
- HGUISearch
- HGUISharedFile
- HGUITask
- HGUIUart
- HGUIUserAudit
- UIAnalysisRecord
- UIBaseApp
- UICameraRecognize
- UIChannel
- UICurve
- UIEBalance
- UIExceptionHandle
- UIFlow
- UILog
- UILogin
- UIMethod
- UIPrint
- UIReagent
- UIScanner
- UISearch
- UISharedFile
- UITask
- UIUart
- UIUserAudit

### 第6层 (12个模块)
- AppCloud
- AppComDebug
- AppCupDet
- AppDashboard
- AppOnlinePlatform
- AppSpectrometer
- HGAppCloud
- HGAppComDebug
- HGAppCupDet
- HGAppDashboard
- HGAppOnlinePlatform
- HGAppSpectrometer

### 第7层 (10个模块)
- CommonCommunicationDemo
- CommonCommunicationDemoC
- HGTestPlugin
- HGYModemTest
- HXQCommonCommunicationDemo
- HXQCommonCommunicationDemoC
- HXQtestCameraInterface
- TestPlugin
- YModemTest
- testCameraInterface

## 未匹配的模块

- PluginInterfaceModule -> PluginInterfaceModule
- IfDetectDimentionVux -> IfDetectDimentionVux
- DetectDimentionVuxModule -> DetectDimentionVuxModule
- CommonProtocolAPI -> CommonProtocolAPI
- DetectDimentionVuxProcess -> DetectDimentionVuxProcess
- DFPProcess -> DFPProcess
- TCPPushClient -> TCPPushClient
- CommonProtocolServiceModule -> CommonProtocolServiceModule
- CommonProtocolService -> CommonProtocolService
- HGUIAnalysisRecordRun -> UIAnalysisRecordRun
- HGUICameraRecognizeRun -> UICameraRecognizeRun
- HGUIChannelRun -> UIChannelRun
- HGUICurveRun -> UICurveRun
- HGUIEBalanceRun -> UIEBalanceRun
- HGUIExceptionHandleRun -> UIExceptionHandleRun
- HGUIFlowRun -> UIFlowRun
- HGUILogRun -> UILogRun
- HGUILoginRun -> UILoginRun
- HGUIMethodRun -> UIMethodRun
- HGUIPrintRun -> UIPrintRun
- HGUIReagentRun -> UIReagentRun
- HGUIScannerRun -> UIScannerRun
- HGUISearchRun -> UISearchRun
- HGUISharedFileRun -> UISharedFileRun
- HGUITaskRun -> UITaskRun
- HGUIUartRun -> UIUartRun
- HGUIUserAuditRun -> UIUserAuditRun
- HXQCommonProtocolAPI -> CommonProtocolAPI
- HXQDetectDimentionVuxProcess -> DetectDimentionVuxProcess
- HXQDFPProcess -> DFPProcess
- HXQTCPPushClient -> TCPPushClient
- PluginInterfaceStatic -> PluginInterfaceStatic
- HGUIAnalysisRecordStatic -> UIAnalysisRecordStatic
- HGUIBaseAppStatic -> UIBaseAppStatic
- HGUICameraRecognizeStatic -> UICameraRecognizeStatic
- HGUIChannelStatic -> UIChannelStatic
- HGUICurveStatic -> UICurveStatic
- HGUIEBalanceStatic -> UIEBalanceStatic
- HGUIExceptionHandleStatic -> UIExceptionHandleStatic
- HGUIFlowStatic -> UIFlowStatic
- HGUILogStatic -> UILogStatic
- HGUILoginStatic -> UILoginStatic
- HGUIMethodStatic -> UIMethodStatic
- HGUIPrintStatic -> UIPrintStatic
- HGUIReagentStatic -> UIReagentStatic
- HGUIScannerStatic -> UIScannerStatic
- HGUISearchStatic -> UISearchStatic
- HGUISharedFileStatic -> UISharedFileStatic
- HGUITaskStatic -> UITaskStatic
- HGUIUartStatic -> UIUartStatic
- HGUIUserAuditStatic -> UIUserAuditStatic
- PluginInterface -> PluginInterface
- HGIfDetectDimentionVux -> IfDetectDimentionVux
- HXQDetectDimentionVuxModule -> DetectDimentionVuxModule

## 未匹配模块特征分析

### Static/Run后缀模块 (38个)
- HGUIAnalysisRecordRun -> UIAnalysisRecordRun (基础模块: UIAnalysisRecord)
- HGUICameraRecognizeRun -> UICameraRecognizeRun (基础模块: UICameraRecognize)
- HGUIChannelRun -> UIChannelRun (基础模块: UIChannel)
- HGUICurveRun -> UICurveRun (基础模块: UICurve)
- HGUIEBalanceRun -> UIEBalanceRun (基础模块: UIEBalance)
- HGUIExceptionHandleRun -> UIExceptionHandleRun (基础模块: UIExceptionHandle)
- HGUIFlowRun -> UIFlowRun (基础模块: UIFlow)
- HGUILogRun -> UILogRun (基础模块: UILog)
- HGUILoginRun -> UILoginRun (基础模块: UILogin)
- HGUIMethodRun -> UIMethodRun (基础模块: UIMethod)
- HGUIPrintRun -> UIPrintRun (基础模块: UIPrint)
- HGUIReagentRun -> UIReagentRun (基础模块: UIReagent)
- HGUIScannerRun -> UIScannerRun (基础模块: UIScanner)
- HGUISearchRun -> UISearchRun (基础模块: UISearch)
- HGUISharedFileRun -> UISharedFileRun (基础模块: UISharedFile)
- HGUITaskRun -> UITaskRun (基础模块: UITask)
- HGUIUartRun -> UIUartRun (基础模块: UIUart)
- HGUIUserAuditRun -> UIUserAuditRun (基础模块: UIUserAudit)
- PluginInterfaceStatic -> PluginInterfaceStatic (基础模块: PluginInterface)
- HGUIAnalysisRecordStatic -> UIAnalysisRecordStatic (基础模块: UIAnalysisRecord)
- HGUIBaseAppStatic -> UIBaseAppStatic (基础模块: UIBaseApp)
- HGUICameraRecognizeStatic -> UICameraRecognizeStatic (基础模块: UICameraRecognize)
- HGUIChannelStatic -> UIChannelStatic (基础模块: UIChannel)
- HGUICurveStatic -> UICurveStatic (基础模块: UICurve)
- HGUIEBalanceStatic -> UIEBalanceStatic (基础模块: UIEBalance)
- HGUIExceptionHandleStatic -> UIExceptionHandleStatic (基础模块: UIExceptionHandle)
- HGUIFlowStatic -> UIFlowStatic (基础模块: UIFlow)
- HGUILogStatic -> UILogStatic (基础模块: UILog)
- HGUILoginStatic -> UILoginStatic (基础模块: UILogin)
- HGUIMethodStatic -> UIMethodStatic (基础模块: UIMethod)
- HGUIPrintStatic -> UIPrintStatic (基础模块: UIPrint)
- HGUIReagentStatic -> UIReagentStatic (基础模块: UIReagent)
- HGUIScannerStatic -> UIScannerStatic (基础模块: UIScanner)
- HGUISearchStatic -> UISearchStatic (基础模块: UISearch)
- HGUISharedFileStatic -> UISharedFileStatic (基础模块: UISharedFile)
- HGUITaskStatic -> UITaskStatic (基础模块: UITask)
- HGUIUartStatic -> UIUartStatic (基础模块: UIUart)
- HGUIUserAuditStatic -> UIUserAuditStatic (基础模块: UIUserAudit)

### Plugin类模块 (3个)
- PluginInterfaceModule -> PluginInterfaceModule
- PluginInterfaceStatic -> PluginInterfaceStatic
- PluginInterface -> PluginInterface

### Module类模块 (3个)
- DetectDimentionVuxModule -> DetectDimentionVuxModule
- CommonProtocolServiceModule -> CommonProtocolServiceModule
- HXQDetectDimentionVuxModule -> DetectDimentionVuxModule

### Process类模块 (4个)
- DetectDimentionVuxProcess -> DetectDimentionVuxProcess
- DFPProcess -> DFPProcess
- HXQDetectDimentionVuxProcess -> DetectDimentionVuxProcess
- HXQDFPProcess -> DFPProcess

### Other类模块 (44个)
- IfDetectDimentionVux -> IfDetectDimentionVux
- CommonProtocolAPI -> CommonProtocolAPI
- TCPPushClient -> TCPPushClient
- CommonProtocolService -> CommonProtocolService
- HGUIAnalysisRecordRun -> UIAnalysisRecordRun
- HGUICameraRecognizeRun -> UICameraRecognizeRun
- HGUIChannelRun -> UIChannelRun
- HGUICurveRun -> UICurveRun
- HGUIEBalanceRun -> UIEBalanceRun
- HGUIExceptionHandleRun -> UIExceptionHandleRun
- HGUIFlowRun -> UIFlowRun
- HGUILogRun -> UILogRun
- HGUILoginRun -> UILoginRun
- HGUIMethodRun -> UIMethodRun
- HGUIPrintRun -> UIPrintRun
- HGUIReagentRun -> UIReagentRun
- HGUIScannerRun -> UIScannerRun
- HGUISearchRun -> UISearchRun
- HGUISharedFileRun -> UISharedFileRun
- HGUITaskRun -> UITaskRun
- HGUIUartRun -> UIUartRun
- HGUIUserAuditRun -> UIUserAuditRun
- HXQCommonProtocolAPI -> CommonProtocolAPI
- HXQTCPPushClient -> TCPPushClient
- HGUIAnalysisRecordStatic -> UIAnalysisRecordStatic
- HGUIBaseAppStatic -> UIBaseAppStatic
- HGUICameraRecognizeStatic -> UICameraRecognizeStatic
- HGUIChannelStatic -> UIChannelStatic
- HGUICurveStatic -> UICurveStatic
- HGUIEBalanceStatic -> UIEBalanceStatic
- HGUIExceptionHandleStatic -> UIExceptionHandleStatic
- HGUIFlowStatic -> UIFlowStatic
- HGUILogStatic -> UILogStatic
- HGUILoginStatic -> UILoginStatic
- HGUIMethodStatic -> UIMethodStatic
- HGUIPrintStatic -> UIPrintStatic
- HGUIReagentStatic -> UIReagentStatic
- HGUIScannerStatic -> UIScannerStatic
- HGUISearchStatic -> UISearchStatic
- HGUISharedFileStatic -> UISharedFileStatic
- HGUITaskStatic -> UITaskStatic
- HGUIUartStatic -> UIUartStatic
- HGUIUserAuditStatic -> UIUserAuditStatic
- HGIfDetectDimentionVux -> IfDetectDimentionVux