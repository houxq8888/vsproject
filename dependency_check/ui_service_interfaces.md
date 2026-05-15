# UI服务接口层设计

## 1. 概述

UI服务接口层位于第3.5层，介于模块层（第3层）和UI应用模块层（第4层）之间，为UI模块提供对底层服务的抽象接口，消除跨多层依赖。

## 2. 接口设计原则

1. **单一职责原则**：每个接口只负责一类服务
2. **接口隔离原则**：客户端不应该依赖它不需要的接口
3. **依赖倒置原则**：高层模块不应该依赖低层模块，两者都应该依赖抽象
4. **开闭原则**：对扩展开放，对修改关闭

## 3. 核心接口设计

### 3.1 IUIConfigService - 配置服务接口

```cpp
class IUIConfigService {
public:
    virtual ~IUIConfigService() = default;
    
    // 基本配置操作
    virtual bool GetString(const std::string& section, const std::string& key, std::string& value) = 0;
    virtual bool GetInt(const std::string& section, const std::string& key, int& value) = 0;
    virtual bool GetBool(const std::string& section, const std::string& key, bool& value) = 0;
    virtual bool GetDouble(const std::string& section, const std::string& key, double& value) = 0;
    
    // 设置配置
    virtual bool SetString(const std::string& section, const std::string& key, const std::string& value) = 0;
    virtual bool SetInt(const std::string& section, const std::string& key, int value) = 0;
    virtual bool SetBool(const std::string& section, const std::string& key, bool value) = 0;
    virtual bool SetDouble(const std::string& section, const std::string& key, double value) = 0;
    
    // 配置文件操作
    virtual bool SaveConfig() = 0;
    virtual bool ReloadConfig() = 0;
    virtual bool HasSection(const std::string& section) = 0;
    virtual bool HasKey(const std::string& section, const std::string& key) = 0;
    
    // 配置变更通知
    virtual void RegisterConfigChangeCallback(const std::string& section, 
                                            std::function<void(const std::string&, const std::string&)> callback) = 0;
    virtual void UnregisterConfigChangeCallback(const std::string& section) = 0;
};
```

### 3.2 IUISaveService - 存储服务接口

```cpp
class IUISaveService {
public:
    virtual ~IUISaveService() = default;
    
    // 文件操作
    virtual bool SaveFile(const std::string& filePath, const std::vector<uint8_t>& data) = 0;
    virtual bool LoadFile(const std::string& filePath, std::vector<uint8_t>& data) = 0;
    virtual bool FileExists(const std::string& filePath) = 0;
    virtual bool DeleteFile(const std::string& filePath) = 0;
    
    // 目录操作
    virtual bool CreateDirectory(const std::string& dirPath) = 0;
    virtual bool DirectoryExists(const std::string& dirPath) = 0;
    virtual bool DeleteDirectory(const std::string& dirPath, bool recursive = false) = 0;
    virtual std::vector<std::string> ListFiles(const std::string& dirPath, const std::string& pattern = "*") = 0;
    
    // 异步操作
    virtual void SaveFileAsync(const std::string& filePath, const std::vector<uint8_t>& data,
                              std::function<void(bool)> callback) = 0;
    virtual void LoadFileAsync(const std::string& filePath, 
                              std::function<void(bool, const std::vector<uint8_t>&)> callback) = 0;
    
    // 备份和恢复
    virtual bool BackupFile(const std::string& filePath, const std::string& backupPath) = 0;
    virtual bool RestoreFile(const std::string& backupPath, const std::string& filePath) = 0;
};
```

### 3.3 IUIServiceManager - 服务管理接口

```cpp
class IUIServiceManager {
public:
    virtual ~IUIServiceManager() = default;
    
    // 服务注册和获取
    template<typename Interface>
    void RegisterService(const std::string& name, std::shared_ptr<Interface> service) {
        RegisterServiceInternal(name, std::static_pointer_cast<void>(service), 
                               [](void* ptr) { delete static_cast<Interface*>(ptr); });
    }
    
    template<typename Interface>
    std::shared_ptr<Interface> GetService(const std::string& name) {
        void* service = GetServiceInternal(name);
        if (service) {
            return std::shared_ptr<Interface>(static_cast<Interface*>(service),
                                             [](Interface* ptr) { /* 不删除，由容器管理 */ });
        }
        return nullptr;
    }
    
    // 服务生命周期管理
    virtual bool StartService(const std::string& name) = 0;
    virtual bool StopService(const std::string& name) = 0;
    virtual bool RestartService(const std::string& name) = 0;
    virtual bool IsServiceRunning(const std::string& name) = 0;
    
    // 服务查询
    virtual std::vector<std::string> GetServiceNames() = 0;
    virtual bool HasService(const std::string& name) = 0;
    
    // 服务依赖管理
    virtual bool AddServiceDependency(const std::string& service, const std::string& dependency) = 0;
    virtual std::vector<std::string> GetServiceDependencies(const std::string& service) = 0;
    
protected:
    virtual void RegisterServiceInternal(const std::string& name, void* service, 
                                       std::function<void(void*)> deleter) = 0;
    virtual void* GetServiceInternal(const std::string& name) = 0;
};
```

### 3.4 IUICommonService - 通用服务接口

```cpp
class IUICommonService {
public:
    virtual ~IUICommonService() = default;
    
    // 字符串处理
    virtual std::string FormatString(const std::string& format, ...) = 0;
    virtual std::vector<std::string> SplitString(const std::string& str, const std::string& delimiter) = 0;
    virtual std::string JoinStrings(const std::vector<std::string>& strings, const std::string& delimiter) = 0;
    virtual std::string TrimString(const std::string& str) = 0;
    
    // 时间和日期
    virtual std::string GetCurrentTime(const std::string& format = "%Y-%m-%d %H:%M:%S") = 0;
    virtual uint64_t GetCurrentTimestamp() = 0;
    virtual std::string FormatTimestamp(uint64_t timestamp, const std::string& format = "%Y-%m-%d %H:%M:%S") = 0;
    
    // 系统信息
    virtual std::string GetOperatingSystem() = 0;
    virtual std::string GetComputerName() = 0;
    virtual std::string GetApplicationVersion() = 0;
    
    // 加密和哈希
    virtual std::string CalculateMD5(const std::string& data) = 0;
    virtual std::string CalculateSHA256(const std::string& data) = 0;
    virtual std::string EncryptString(const std::string& data, const std::string& key) = 0;
    virtual std::string DecryptString(const std::string& encryptedData, const std::string& key) = 0;
    
    // UUID和GUID
    virtual std::string GenerateUUID() = 0;
    virtual std::string GenerateGUID() = 0;
};
```

## 4. 适配器实现

### 4.1 UIConfigServiceAdapter

```cpp
class UIConfigServiceAdapter : public IUIConfigService {
private:
    HGBaseConfig* m_config;
    std::map<std::string, std::function<void(const std::string&, const std::string&)>> m_callbacks;
    
public:
    UIConfigServiceAdapter(HGBaseConfig* config) : m_config(config) {}
    
    bool GetString(const std::string& section, const std::string& key, std::string& value) override {
        return m_config->GetString(section, key, value);
    }
    
    bool GetInt(const std::string& section, const std::string& key, int& value) override {
        return m_config->GetInt(section, key, value);
    }
    
    bool GetBool(const std::string& section, const std::string& key, bool& value) override {
        return m_config->GetBool(section, key, value);
    }
    
    bool GetDouble(const std::string& section, const std::string& key, double& value) override {
        return m_config->GetDouble(section, key, value);
    }
    
    bool SetString(const std::string& section, const std::string& key, const std::string& value) override {
        bool result = m_config->SetString(section, key, value);
        if (result) {
            NotifyConfigChange(section, key);
        }
        return result;
    }
    
    bool SetInt(const std::string& section, const std::string& key, int value) override {
        bool result = m_config->SetInt(section, key, value);
        if (result) {
            NotifyConfigChange(section, key);
        }
        return result;
    }
    
    bool SetBool(const std::string& section, const std::string& key, bool value) override {
        bool result = m_config->SetBool(section, key, value);
        if (result) {
            NotifyConfigChange(section, key);
        }
        return result;
    }
    
    bool SetDouble(const std::string& section, const std::string& key, double value) override {
        bool result = m_config->SetDouble(section, key, value);
        if (result) {
            NotifyConfigChange(section, key);
        }
        return result;
    }
    
    bool SaveConfig() override {
        return m_config->SaveConfig();
    }
    
    bool ReloadConfig() override {
        return m_config->ReloadConfig();
    }
    
    bool HasSection(const std::string& section) override {
        return m_config->HasSection(section);
    }
    
    bool HasKey(const std::string& section, const std::string& key) override {
        return m_config->HasKey(section, key);
    }
    
    void RegisterConfigChangeCallback(const std::string& section, 
                                     std::function<void(const std::string&, const std::string&)> callback) override {
        m_callbacks[section] = callback;
    }
    
    void UnregisterConfigChangeCallback(const std::string& section) override {
        m_callbacks.erase(section);
    }
    
private:
    void NotifyConfigChange(const std::string& section, const std::string& key) {
        auto it = m_callbacks.find(section);
        if (it != m_callbacks.end()) {
            it->second(section, key);
        }
    }
};
```

### 4.2 UISaveServiceAdapter

```cpp
class UISaveServiceAdapter : public IUISaveService {
private:
    HGSvcSave* m_saveService;
    
public:
    UISaveServiceAdapter(HGSvcSave* saveService) : m_saveService(saveService) {}
    
    bool SaveFile(const std::string& filePath, const std::vector<uint8_t>& data) override {
        return m_saveService->SaveFile(filePath, data.data(), data.size());
    }
    
    bool LoadFile(const std::string& filePath, std::vector<uint8_t>& data) override {
        size_t size = 0;
        void* buffer = m_saveService->LoadFile(filePath, &size);
        if (buffer && size > 0) {
            data.resize(size);
            memcpy(data.data(), buffer, size);
            free(buffer);
            return true;
        }
        return false;
    }
    
    bool FileExists(const std::string& filePath) override {
        return m_saveService->FileExists(filePath);
    }
    
    bool DeleteFile(const std::string& filePath) override {
        return m_saveService->DeleteFile(filePath);
    }
    
    bool CreateDirectory(const std::string& dirPath) override {
        return m_saveService->CreateDirectory(dirPath);
    }
    
    bool DirectoryExists(const std::string& dirPath) override {
        return m_saveService->DirectoryExists(dirPath);
    }
    
    bool DeleteDirectory(const std::string& dirPath, bool recursive) override {
        return m_saveService->DeleteDirectory(dirPath, recursive);
    }
    
    std::vector<std::string> ListFiles(const std::string& dirPath, const std::string& pattern) override {
        // 实现文件列表获取
        // 这里需要根据HGSvcSave的实际API实现
        return std::vector<std::string>();
    }
    
    void SaveFileAsync(const std::string& filePath, const std::vector<uint8_t>& data,
                      std::function<void(bool)> callback) override {
        // 实现异步保存
        std::thread([this, filePath, data, callback]() {
            bool result = SaveFile(filePath, data);
            callback(result);
        }).detach();
    }
    
    void LoadFileAsync(const std::string& filePath, 
                      std::function<void(bool, const std::vector<uint8_t>&)> callback) override {
        // 实现异步加载
        std::thread([this, filePath, callback]() {
            std::vector<uint8_t> data;
            bool result = LoadFile(filePath, data);
            callback(result, data);
        }).detach();
    }
    
    bool BackupFile(const std::string& filePath, const std::string& backupPath) override {
        std::vector<uint8_t> data;
        if (LoadFile(filePath, data)) {
            return SaveFile(backupPath, data);
        }
        return false;
    }
    
    bool RestoreFile(const std::string& backupPath, const std::string& filePath) override {
        std::vector<uint8_t> data;
        if (LoadFile(backupPath, data)) {
            return SaveFile(filePath, data);
        }
        return false;
    }
};
```

## 5. 依赖注入容器实现

```cpp
class UIDependencyContainer : public IUIServiceManager {
private:
    struct ServiceInfo {
        void* instance;
        std::function<void(void*)> deleter;
        std::vector<std::string> dependencies;
        bool isRunning;
    };
    
    std::map<std::string, ServiceInfo> m_services;
    std::mutex m_mutex;
    
public:
    UIDependencyContainer() = default;
    virtual ~UIDependencyContainer() {
        std::lock_guard<std::mutex> lock(m_mutex);
        for (auto& pair : m_services) {
            if (pair.second.deleter && pair.second.instance) {
                pair.second.deleter(pair.second.instance);
            }
        }
    }
    
protected:
    void RegisterServiceInternal(const std::string& name, void* service, 
                               std::function<void(void*)> deleter) override {
        std::lock_guard<std::mutex> lock(m_mutex);
        ServiceInfo info;
        info.instance = service;
        info.deleter = deleter;
        info.isRunning = false;
        m_services[name] = info;
    }
    
    void* GetServiceInternal(const std::string& name) override {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_services.find(name);
        if (it != m_services.end()) {
            return it->second.instance;
        }
        return nullptr;
    }
    
public:
    bool StartService(const std::string& name) override {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_services.find(name);
        if (it != m_services.end() && !it->second.isRunning) {
            // 首先启动依赖服务
            for (const auto& dep : it->second.dependencies) {
                if (!StartService(dep)) {
                    return false;
                }
            }
            
            // 这里可以添加服务启动逻辑
            it->second.isRunning = true;
            return true;
        }
        return false;
    }
    
    bool StopService(const std::string& name) override {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_services.find(name);
        if (it != m_services.end() && it->second.isRunning) {
            // 首先停止依赖于此服务的其他服务
            for (auto& pair : m_services) {
                for (const auto& dep : pair.second.dependencies) {
                    if (dep == name && pair.second.isRunning) {
                        StopService(pair.first);
                    }
                }
            }
            
            // 这里可以添加服务停止逻辑
            it->second.isRunning = false;
            return true;
        }
        return false;
    }
    
    bool RestartService(const std::string& name) override {
        return StopService(name) && StartService(name);
    }
    
    bool IsServiceRunning(const std::string& name) override {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_services.find(name);
        if (it != m_services.end()) {
            return it->second.isRunning;
        }
        return false;
    }
    
    std::vector<std::string> GetServiceNames() override {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::vector<std::string> names;
        for (const auto& pair : m_services) {
            names.push_back(pair.first);
        }
        return names;
    }
    
    bool HasService(const std::string& name) override {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_services.find(name) != m_services.end();
    }
    
    bool AddServiceDependency(const std::string& service, const std::string& dependency) override {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_services.find(service);
        if (it != m_services.end()) {
            it->second.dependencies.push_back(dependency);
            return true;
        }
        return false;
    }
    
    std::vector<std::string> GetServiceDependencies(const std::string& service) override {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_services.find(service);
        if (it != m_services.end()) {
            return it->second.dependencies;
        }
        return std::vector<std::string>();
    }
};
```

## 6. UI模块改造示例

### 6.1 UIAnalysisRecord模块改造

```cpp
// 改造前
class UIAnalysisRecord {
private:
    HGBaseConfig* m_config;
    HGSvcSave* m_saveService;
    HGSvcFrame* m_frameService;
    HGBaseCommon* m_commonService;
    
public:
    UIAnalysisRecord() {
        // 直接依赖底层服务
        m_config = GetServiceManager()->GetService<HGBaseConfig>("HGBaseConfig");
        m_saveService = GetServiceManager()->GetService<HGSvcSave>("HGSvcSave");
        m_frameService = GetServiceManager()->GetService<HGSvcFrame>("HGSvcFrame");
        m_commonService = GetServiceManager()->GetService<HGBaseCommon>("HGBaseCommon");
    }
    
    void SaveAnalysisResult(const AnalysisResult& result) {
        // 直接使用底层服务
        std::string data;
        m_commonService->Serialize(result, data);
        m_saveService->SaveFile("analysis_result.dat", data);
        
        // 更新配置
        m_config->SetInt("Analysis", "LastResultId", result.id);
        m_config->SaveConfig();
        
        // 通知框架
        m_frameService->NotifyEvent("AnalysisResultSaved");
    }
};

// 改造后
class UIAnalysisRecord {
private:
    std::shared_ptr<IUIConfigService> m_configService;
    std::shared_ptr<IUISaveService> m_saveService;
    std::shared_ptr<IUIServiceManager> m_serviceManager;
    std::shared_ptr<IUICommonService> m_commonService;
    
public:
    UIAnalysisRecord(std::shared_ptr<IUIServiceManager> serviceManager) 
        : m_serviceManager(serviceManager) {
        // 通过依赖注入获取服务
        m_configService = m_serviceManager->GetService<IUIConfigService>("UIConfigService");
        m_saveService = m_serviceManager->GetService<IUISaveService>("UISaveService");
        m_commonService = m_serviceManager->GetService<IUICommonService>("UICommonService");
    }
    
    void SaveAnalysisResult(const AnalysisResult& result) {
        // 使用抽象接口
        std::string data = m_commonService->Serialize(result);
        std::vector<uint8_t> binaryData(data.begin(), data.end());
        m_saveService->SaveFile("analysis_result.dat", binaryData);
        
        // 更新配置
        m_configService->SetInt("Analysis", "LastResultId", result.id);
        m_configService->SaveConfig();
        
        // 通过服务管理器通知
        m_serviceManager->NotifyEvent("AnalysisResultSaved");
    }
};
```

## 7. 服务初始化代码

```cpp
// 在应用启动时初始化服务
void InitializeUIServices() {
    // 创建服务容器
    auto container = std::make_shared<UIDependencyContainer>();
    
    // 获取底层服务实例
    auto baseConfig = GetServiceManager()->GetService<HGBaseConfig>("HGBaseConfig");
    auto svcSave = GetServiceManager()->GetService<HGSvcSave>("HGSvcSave");
    auto svcFrame = GetServiceManager()->GetService<HGSvcFrame>("HGSvcFrame");
    auto baseCommon = GetServiceManager()->GetService<HGBaseCommon>("HGBaseCommon");
    
    // 创建并注册适配器
    auto configAdapter = std::make_shared<UIConfigServiceAdapter>(baseConfig);
    auto saveAdapter = std::make_shared<UISaveServiceAdapter>(svcSave);
    auto commonAdapter = std::make_shared<UICommonServiceAdapter>(baseCommon);
    
    container->RegisterService<IUIConfigService>("UIConfigService", configAdapter);
    container->RegisterService<IUISaveService>("UISaveService", saveAdapter);
    container->RegisterService<IUICommonService>("UICommonService", commonAdapter);
    container->RegisterService<IUIServiceManager>("UIServiceManager", container);
    
    // 设置服务依赖关系
    container->AddServiceDependency("UIConfigService", "UIServiceManager");
    container->AddServiceDependency("UISaveService", "UIServiceManager");
    container->AddServiceDependency("UICommonService", "UIServiceManager");
    
    // 启动所有服务
    container->StartService("UIConfigService");
    container->StartService("UISaveService");
    container->StartService("UICommonService");
    container->StartService("UIServiceManager");
    
    // 将容器注册到全局服务管理器
    GetGlobalServiceManager()->RegisterService<UIDependencyContainer>("UIContainer", container);
}
```

通过这个UI服务接口层设计，我们可以有效地消除UI模块对底层服务的直接依赖，实现清晰的分层架构，提高系统的可维护性和可测试性。