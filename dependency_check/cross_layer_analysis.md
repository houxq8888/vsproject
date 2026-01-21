# 跨多层依赖分析报告

总跨多层依赖数: 25


## 按跨越层数统计

- 跨越4层: 4个依赖
- 跨越3层: 16个依赖
- 跨越2层: 5个依赖

## 按层级对统计

- 第4层 -> 第1层: 16个依赖
- 第3层 -> 第1层: 5个依赖
- 第4层 -> 第0层: 4个依赖

## 问题最严重的模块


### 第4层模块

- UIAnalysisRecord: 4个跨多层依赖
- UIBaseApp: 4个跨多层依赖
- HGUIAnalysisRecord: 4个跨多层依赖
- HGUIBaseApp: 4个跨多层依赖
- UIChannel: 2个跨多层依赖
- HGUIChannel: 2个跨多层依赖

### 第3层模块

- CoreLog: 5个跨多层依赖

## 具体的跨多层依赖示例

- 第4层 UIChannel -> 第0层 HGCommon (跨越4层)
- 第4层 UIChannel -> 第0层 HGConfig (跨越4层)
- 第4层 HGUIChannel -> 第0层 HGCommon (跨越4层)
- 第4层 HGUIChannel -> 第0层 HGConfig (跨越4层)
- 第4层 UIAnalysisRecord -> 第1层 HGSvcFrame (跨越3层)
- 第4层 UIAnalysisRecord -> 第1层 HGBaseConfig (跨越3层)
- 第4层 UIAnalysisRecord -> 第1层 HGBaseCommon (跨越3层)
- 第4层 UIAnalysisRecord -> 第1层 HGSvcSave (跨越3层)
- 第4层 UIBaseApp -> 第1层 HGSvcFrame (跨越3层)
- 第4层 UIBaseApp -> 第1层 HGBaseConfig (跨越3层)
- 第4层 UIBaseApp -> 第1层 HGBaseCommon (跨越3层)
- 第4层 UIBaseApp -> 第1层 HGSvcSave (跨越3层)
- 第4层 HGUIAnalysisRecord -> 第1层 HGSvcFrame (跨越3层)
- 第4层 HGUIAnalysisRecord -> 第1层 HGBaseConfig (跨越3层)
- 第4层 HGUIAnalysisRecord -> 第1层 HGBaseCommon (跨越3层)
- 第4层 HGUIAnalysisRecord -> 第1层 HGSvcSave (跨越3层)
- 第4层 HGUIBaseApp -> 第1层 HGSvcFrame (跨越3层)
- 第4层 HGUIBaseApp -> 第1层 HGBaseConfig (跨越3层)
- 第4层 HGUIBaseApp -> 第1层 HGBaseCommon (跨越3层)
- 第4层 HGUIBaseApp -> 第1层 HGSvcSave (跨越3层)