#ifndef I_SVC_MANAGER_H
#define I_SVC_MANAGER_H

#include <string>
#include <functional>
#include <memory>

class ISvcManager {
public:
    virtual ~ISvcManager() = default;

    virtual bool Initialize() = 0;
    virtual bool Shutdown() = 0;
    
    virtual void RegisterService(const std::string& serviceName, 
                            std::function<void*()> factory) = 0;
    virtual void* GetService(const std::string& serviceName) = 0;
    
    template<typename ServiceType>
    ServiceType* GetServiceT(const std::string& serviceName) {
        return static_cast<ServiceType*>(GetService(serviceName));
    }
    
    virtual bool UnregisterService(const std::string& serviceName) = 0;
    
    virtual std::vector<std::string> GetRegisteredServices() const = 0;
    virtual bool IsServiceRegistered(const std::string& serviceName) const = 0;
};

#endif // I_SVC_MANAGER_H
