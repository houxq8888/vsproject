#ifndef SVC_MANAGER_ADAPTER_H
#define SVC_MANAGER_ADAPTER_H

#include "ISvcManager.h"
#include <map>
#include <mutex>
#include <functional>
#include <memory>

class SvcManagerAdapter : public ISvcManager {
private:
    std::map<std::string, std::function<void*()>> m_factories;
    std::map<std::string, std::shared_ptr<void>> m_instances;
    mutable std::mutex m_mutex;
    bool m_initialized;
    
public:
    SvcManagerAdapter();
    virtual ~SvcManagerAdapter();
    
    bool Initialize() override;
    bool Shutdown() override;
    
    void RegisterService(const std::string& serviceName, 
                            std::function<void*()> factory) override;
    void* GetService(const std::string& serviceName) override;
    
    bool UnregisterService(const std::string& serviceName) override;
    
    std::vector<std::string> GetRegisteredServices() const override;
    bool IsServiceRegistered(const std::string& serviceName) const override;
};

#endif // SVC_MANAGER_ADAPTER_H
