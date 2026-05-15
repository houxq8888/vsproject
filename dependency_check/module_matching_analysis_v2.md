# 模块匹配分析报告 v2

总模块数: 168
匹配设计的模块数: 128
未匹配设计的模块数: 40
匹配率: 76.19%

## 按层统计匹配的模块

### 第0层 (4个模块)
- BaseCommon
- BaseConfig
- HGBaseCommon
- HGBaseConfig

### 第1层 (24个模块)
- HGSvcAuthority
- HGSvcCommonProtocol
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
- SvcCommonProtocol
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

### 第3层 (12个模块)
- CoreCupDet
- CoreDashboard
- CoreDetectDimentionVux
- CoreLog
- CoreOnlinePlatform
- CoreV6
- HGCoreCupDet
- HGCoreDashboard
- HGCoreDetectDimentionVux
- HGCoreLog
- HGCoreOnlinePlatform
- HGCoreV6

### 第4层 (16个模块)
- HGIfCameraControl
- HGIfCameraRecognize
- HGIfCupDet
- HGIfDetectDimentionVux
- HGIfLog
- HGIfOnlinePlatform
- HGIfPackFunc
- HGIfV6
- IfCameraControl
- IfCameraRecognize
- IfCupDet
- IfDetectDimentionVux
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

### 第6层 (18个模块)
- AppCloud
- AppComDebug
- AppCupDet
- AppDashboard
- AppOnlinePlatform
- AppSpectrometer
- CmdCommonProtocol
- CmdDFP
- CmdDetectDimentionVux
- HGAppCloud
- HGAppComDebug
- HGAppCupDet
- HGAppDashboard
- HGAppOnlinePlatform
- HGAppSpectrometer
- HGCmdCommonProtocol
- HGCmdDFP
- HGCmdDetectDimentionVux

### 第7层 (10个模块)
- HGTestCommonCommunicationDemo
- HGTestCommonCommunicationDemoC
- HGTestPlugin
- HGTestTCPPushClient
- HGTestYModem
- TestCommonCommunicationDemo
- TestCommonCommunicationDemoC
- TestPlugin
- TestTCPPushClient
- TestYModem

## 未匹配的模块

- PluginInterfaceModule -> PluginInterfaceModule
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

### Other类模块 (37个)
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