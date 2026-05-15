# HGAppModule 数据架构设计方案

## 一、架构概述

本设计方案采用**分层架构 + 接口封装**的设计模式，实现UI层与业务逻辑层的完全解耦，支持多种业务逻辑实现的无缝切换。

### 1.1 架构层次

```
┌─────────────────────────────────────────┐
│   projects/                            │  带UI界面的可执行程序
│   - 可调用多个HGAppModules与interfaces  │
├─────────────────────────────────────────┤
│   processes/                            │  不带UI界面的可执行程序
├─────────────────────────────────────────┤
│   HGAppModule/                         │  UI交互层（带UI界面的模块动态库）
│   - HGLogModule/                       │  日志UI模块
│   - HGUserModule/                      │  用户UI模块
│   - HGTaskModule/                      │  任务UI模块
│   - ...                                │
├─────────────────────────────────────────┤
│   interfaces/                          │  modules的封装实现（UI和业务逻辑的桥梁）
│   - LogInterface/                      │  日志接口封装
│   - UserInterface/                     │  用户接口封装
│   - TaskInterface/                     │  任务接口封装
│   - MethodInterface/                   │  方法接口封装
│   - ReagentInterface/                  │  试剂接口封装
│   - FlowInterface/                     │  流程接口封装
├─────────────────────────────────────────┤
│   modules/                             │  业务逻辑层
│   - LogModule/                         │  日志业务逻辑
│   - UserModule/                        │  用户业务逻辑
│   - TaskModule/                        │  任务业务逻辑
│   - MethodModule/                      │  方法业务逻辑
│   - ReagentModule/                     │  试剂业务逻辑
│   - FlowModule/                        │  流程业务逻辑
├─────────────────────────────────────────┤
│   services/                             │  底层服务层
│   ├── HGCommon/                        │  常用函数封装（时间、编解码、路径等）
│   ├── HGConfig/                        │  文件读写函数封装（txt、csv、pdf、json等）
│   └── HGAtomService/                   │  原子服务层
│       ├── HGSaveService/                │  数据库读写封装
│       ├── HGCommunicateService/          │  通讯封装
│       ├── HGImageAlgorithmService/      │  算子封装
│       ├── HGSecurityService/            │  安全类
│       ├── HGInteractiveService/         │  交互类
│       └── ...                          │
└─────────────────────────────────────────┘
```

## 二、核心设计原则

### 2.1 各层职责

#### UI层（HGAppModule）
- **职责单一**：只负责UI展示和用户交互
- **零业务依赖**：不直接访问数据库，不直接调用业务逻辑
- **依赖接口**：只依赖interfaces层
- **数据隔离**：使用标准数据结构（map、vector等）

#### interfaces层
- **接口封装**：封装modules的业务逻辑
- **数据转换**：将modules的数据转换为UI层可用的格式
- **生命周期管理**：管理modules的初始化和销毁
- **统一接口**：为UI层提供统一的调用接口

#### modules层（业务逻辑层）
- **业务封装**：封装具体的业务逻辑
- **数据访问**：通过services层访问数据库和文件
- **业务规则**：实现业务规则和验证
- **可替换性**：不同的业务实现可以相互替换

#### services层（底层服务层）
- **HGCommon**：常用函数封装（时间、编解码、路径等）
- **HGConfig**：文件读写函数封装（txt、csv、pdf、json等）
- **HGAtomService**：原子服务层（通讯、数据库、算子、安全、交互等）

## 三、数据结构设计

### 3.1 数据传输方式

采用**标准数据结构**（map、vector等）作为UI层和业务逻辑层之间的数据传输方式：

```cpp
// 使用标准数据结构
std::map<std::string, std::string> logData;
logData["Time"] = "2024-01-01 10:00:00";
logData["LogContent"] = "系统启动";
logData["Operator"] = "admin";

std::vector<std::map<std::string, std::string>> logList;
logList.push_back(logData);
```

**优势**：
- UI层和业务逻辑层都使用标准数据结构
- 不需要额外的DTO定义
- 数据结构变化时，只需修改interfaces层
- UI层和业务逻辑层完全解耦

### 3.2 interfaces层设计

```cpp
class LogInterface
{
public:
    LogInterface();
    ~LogInterface();
    
    bool initialize();
    void shutdown();
    
    std::vector<std::map<std::string, std::string>> getAuditTrailLogs(
        const std::string& tableName);
    
    std::vector<std::map<std::string, std::string>> searchLogs(
        const std::string& keyword,
        const std::string& timeFrom,
        const std::string& timeTo,
        int page,
        int pageSize,
        int* totalCount);
    
    // ... 其他接口
};
```

**优势**：
- 封装modules的业务逻辑
- 为UI层提供统一的调用接口
- 管理modules的生命周期
- 支持数据转换和格式化

### 3.3 modules层设计

```cpp
class LogModule
{
public:
    LogModule();
    ~LogModule();
    
    bool initialize(const std::string& dbPath);
    void shutdown();
    
    std::vector<std::map<std::string, std::string>> getAuditTrailLogs(
        const std::string& tableName);
    
    std::vector<std::map<std::string, std::string>> searchLogs(
        const std::string& keyword,
        const std::string& timeFrom,
        const std::string& timeTo,
        int page,
        int pageSize,
        int* totalCount);
    
    // ... 其他业务逻辑
};
```

**优势**：
- 封装具体的业务逻辑
- 通过services层访问数据库和文件
- 实现业务规则和验证
- 支持多种业务实现

## 四、使用示例

### 4.1 UI层使用interfaces

```cpp
#include "../../interfaces/LogInterface/inc/loginterface.h"

class HGLogWidgetNew : public QWidget {
private:
    std::unique_ptr<HGMACHINE::LogInterface> m_logInterface;
    
public:
    HGLogWidgetNew(std::string lang, QWidget *parent) {
        // 创建interfaces实例
        m_logInterface = std::make_unique<HGMACHINE::LogInterface>();
        m_logInterface->initialize();
        
        // 使用interfaces
        auto logs = m_logInterface->getAuditTrailLogs(tableName);
        displayLogs(logs);
    }
    
    ~HGLogWidgetNew() {
        if (m_logInterface) {
            m_logInterface->shutdown();
        }
    }
};
```

### 4.2 interfaces层调用modules

```cpp
#include "../modules/LogModule/inc/logmodule.h"

class LogInterface {
private:
    std::unique_ptr<LogModule> m_logModule;
    
public:
    LogInterface() : m_logModule(std::make_unique<LogModule>()) {}
    
    bool initialize() {
        return m_logModule->initialize("/app/data/hgdata.db");
    }
    
    std::vector<std::map<std::string, std::string>> getAuditTrailLogs(
        const std::string& tableName) {
        return m_logModule->getAuditTrailLogs(tableName);
    }
};
```

### 4.3 modules层调用services

```cpp
#include "../../services/HGAtomService/HGSaveService/inc/hgsavedatatodb.h"
#include "../../services/HGCommon/inc/hgcommonutility.h"
#include "../../services/HGConfig/inc/hgcsv.h"

class LogModule {
public:
    std::vector<std::map<std::string, std::string>> getAuditTrailLogs(
        const std::string& tableName) {
        // 调用services层
        return RWDb::readAuditTrailLog(tableName);
    }
    
    bool saveLogsToFile(const std::vector<std::map<std::string, std::string>>& logs,
                       const std::string& filePath,
                       const std::string& format) {
        // 调用services层
        if (format == "txt") {
            return saveTableToTxt(logs, filePath);
        } else if (format == "csv") {
            return saveTableToCsv(logs, filePath);
        }
        return false;
    }
};
```

## 五、数据结构变化处理

### 5.1 数据库结构变化

**场景**：数据库表结构发生变化（如字段名改变、新增字段）

**处理方式**：
1. 修改modules层的数据处理逻辑
2. interfaces层无需修改（如果数据格式不变）
3. UI层无需修改

```cpp
// modules层修改
std::vector<std::map<std::string, std::string>> LogModule::getAuditTrailLogs(
    const std::string& tableName) {
    
    auto dbData = RWDb::readAuditTrailLog(tableName);
    
    // 数据库字段名从 "Time" 改为 "LogTime"
    // 在modules层进行转换
    std::vector<std::map<std::string, std::string>> result;
    for (const auto& data : dbData) {
        std::map<std::string, std::string> logData;
        
        auto it = data.find("LogTime");
        if (it != data.end()) {
            logData["Time"] = it->second;  // 转换为标准字段名
        }
        
        // 新增字段 "LogLevel"
        it = data.find("LogLevel");
        if (it != data.end()) {
            logData["LogLevel"] = it->second;
        }
        
        result.push_back(logData);
    }
    
    return result;
}
```

### 5.2 UI层变化

**场景**：UI层需要显示新的字段或改变显示方式

**处理方式**：
1. UI层修改显示逻辑
2. interfaces层和modules层无需修改

```cpp
// UI层修改
void HGLogWidgetNew::displayLogs(
    const std::vector<std::map<std::string, std::string>>& logs) {
    
    // 添加新的列
    m_tableW->setColumnCount(4);
    m_tableW->setHorizontalHeaderLabels({"时间", "日志内容", "操作员", "日志级别"});
    
    // 显示数据
    for (const auto& log : logs) {
        int row = m_tableW->rowCount();
        m_tableW->insertRow(row);
        
        // 显示原有字段
        auto it = log.find("Time");
        if (it != log.end()) {
            m_tableW->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(it->second)));
        }
        
        // 显示新字段
        it = log.find("LogLevel");
        if (it != log.end()) {
            m_tableW->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(it->second)));
        }
    }
}
```

### 5.3 业务逻辑变化

**场景**：需要切换到不同的业务逻辑实现（如从本地数据库切换到远程服务）

**处理方式**：
1. 创建新的modules实现
2. interfaces层无需修改（如果接口不变）
3. UI层无需修改

```cpp
// 新的modules实现
class RemoteLogModule : public LogModule {
public:
    std::vector<std::map<std::string, std::string>> getAuditTrailLogs(
        const std::string& tableName) {
        
        // 从远程服务获取数据
        auto remoteData = m_remoteClient->fetchLogs(tableName);
        
        // 转换为标准格式
        std::vector<std::map<std::string, std::string>> result;
        for (const auto& data : remoteData) {
            std::map<std::string, std::string> logData;
            logData["Time"] = data.timestamp;
            logData["LogContent"] = data.message;
            logData["Operator"] = data.user;
            result.push_back(logData);
        }
        
        return result;
    }
    
private:
    std::shared_ptr<RemoteLogClient> m_remoteClient;
};

// interfaces层无需修改，只需在编译时链接不同的modules实现
```

## 六、优势总结

### 6.1 解耦优势
- UI层与业务逻辑层完全解耦
- UI层与数据库结构完全解耦
- 业务逻辑层与UI层完全解耦
- 各层可以独立开发和测试

### 6.2 可维护性优势
- 数据结构变化只需修改modules层
- UI层变化不影响业务逻辑层
- 业务逻辑层变化不影响UI层
- 接口层提供稳定的契约

### 6.3 可扩展性优势
- 支持多种业务逻辑实现
- 支持多种数据源
- 支持动态切换实现
- 易于添加新的业务模块

### 6.4 可测试性优势
- UI层可以独立测试（使用Mock interfaces）
- 业务逻辑层可以独立测试
- interfaces层可以独立测试
- 各层可以并行开发

## 七、文件结构

```
d:\virtualMachine\github\vsproject\
├── services/                              # 底层服务层
│   ├── HGCommon/                          # 常用函数封装
│   ├── HGConfig/                          # 文件读写函数封装
│   └── HGAtomService/                     # 原子服务层
│       ├── HGSaveService/                 # 数据库读写封装
│       ├── HGCommunicateService/           # 通讯封装
│       ├── HGImageAlgorithmService/         # 算子封装
│       ├── HGSecurityService/              # 安全类
│       └── HGInteractiveService/           # 交互类
├── modules/                              # 业务逻辑层
│   ├── LogModule/                        # 日志业务逻辑
│   │   ├── inc/
│   │   │   ├── logmodule.h
│   │   │   └── LogModule_global.h
│   │   ├── src/
│   │   │   └── logmodule.cpp
│   │   └── CMakeLists.txt
│   ├── UserModule/                       # 用户业务逻辑
│   ├── TaskModule/                       # 任务业务逻辑
│   ├── MethodModule/                     # 方法业务逻辑
│   ├── ReagentModule/                    # 试剂业务逻辑
│   └── FlowModule/                       # 流程业务逻辑
├── interfaces/                           # modules的封装实现
│   ├── LogInterface/                     # 日志接口封装
│   │   ├── inc/
│   │   │   ├── loginterface.h
│   │   │   └── LogInterface_global.h
│   │   ├── src/
│   │   │   └── loginterface.cpp
│   │   └── CMakeLists.txt
│   ├── UserInterface/                    # 用户接口封装
│   ├── TaskInterface/                    # 任务接口封装
│   ├── MethodInterface/                  # 方法接口封装
│   ├── ReagentInterface/                 # 试剂接口封装
│   └── FlowInterface/                    # 流程接口封装
├── HGAppModule/                          # UI交互层
│   ├── HGLogModule/                      # 日志UI模块
│   │   ├── inc/
│   │   │   ├── hglogwidget.h            # 原始UI实现
│   │   │   └── hglogwidgetnew.h        # 新的UI实现（使用interfaces）
│   │   ├── src/
│   │   │   ├── hglogwidget.cpp
│   │   │   └── hglogwidgetnew.cpp
│   │   └── CMakeLists.txt
│   ├── HGUserModule/                     # 用户UI模块
│   ├── HGTaskModule/                     # 任务UI模块
│   └── ...
├── processes/                            # 不带UI界面的可执行程序
└── projects/                             # 带UI界面的可执行程序
```

## 八、迁移步骤

### 8.1 逐步迁移策略

1. **第一阶段**：创建modules和interfaces层
   - 创建modules层（业务逻辑）
   - 创建interfaces层（接口封装）
   - 保持原有UI模块不变

2. **第二阶段**：创建新的UI实现
   - 创建新的UI组件（如HGLogWidgetNew）
   - 使用interfaces层的服务
   - 保持原有UI组件不变

3. **第三阶段**：测试验证
   - 测试新UI组件功能
   - 对比新旧UI组件行为
   - 确保功能一致性

4. **第四阶段**：切换使用
   - 逐步切换到新的UI组件
   - 移除旧的UI组件
   - 清理旧代码

### 8.2 兼容性保证

- 新旧UI组件可以并存
- 可以逐步迁移各个模块
- 不影响现有功能

## 九、最佳实践

### 9.1 数据结构设计
- 使用标准数据结构（map、vector等）
- 提供清晰的字段命名
- 保持数据格式的一致性

### 9.2 interfaces设计
- 保持接口简洁明了
- 提供清晰的错误处理
- 管理modules的生命周期
- 支持数据转换和格式化

### 9.3 modules设计
- 封装具体的业务逻辑
- 通过services层访问数据库
- 实现业务规则和验证
- 保持业务逻辑的独立性

### 9.4 错误处理
- 提供清晰的错误信息
- 使用异常或错误码
- 记录错误日志
- 保证系统的稳定性

## 十、总结

本设计方案通过引入interfaces层和modules层，实现了UI层与业务逻辑层的完全解耦，具有以下特点：

1. **高内聚低耦合**：各层职责清晰，相互独立
2. **易于维护**：数据结构变化影响范围小
3. **易于扩展**：支持多种业务逻辑实现
4. **易于测试**：各层可以独立测试
5. **平滑迁移**：新旧实现可以并存

通过这种设计，可以有效地解决UI层与业务逻辑层之间的耦合问题，提高系统的可维护性和可扩展性。

## 十一、与其他架构的对比

### 11.1 与传统三层架构的对比

**传统三层架构**：
- UI层 → 业务逻辑层 → 数据访问层
- UI层直接依赖业务逻辑层
- 业务逻辑层变化会影响UI层

**本设计方案**：
- UI层 → interfaces层 → modules层 → services层
- UI层只依赖interfaces层
- modules层变化不影响UI层
- interfaces层提供稳定的契约

### 11.2 与MVC/MVP架构的对比

**MVC/MVP架构**：
- 适用于单个应用内部
- UI层和业务逻辑层耦合较紧
- 难以支持多种业务实现

**本设计方案**：
- 适用于模块化系统
- UI层和业务逻辑层完全解耦
- 支持多种业务实现的无缝切换

### 11.3 与微服务架构的对比

**微服务架构**：
- 服务之间通过网络通信
- 适合分布式系统
- 复杂度较高

**本设计方案**：
- 模块之间通过接口通信
- 适合单机或小规模分布式系统
- 复杂度较低，易于维护

## 十二、未来扩展方向

### 12.1 支持远程服务
- interfaces层可以调用远程服务
- modules层可以部署为独立服务
- 支持分布式部署

### 12.2 支持插件化
- modules层可以设计为插件
- interfaces层支持动态加载
- 支持热插拔

### 12.3 支持多租户
- interfaces层支持多租户切换
- modules层支持多租户数据隔离
- 支持SaaS模式

### 12.4 支持版本管理
- interfaces层支持版本兼容
- modules层支持多版本共存
- 支持平滑升级
