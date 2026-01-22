#ifndef DEPENDENCY_CONTAINER_H
#define DEPENDENCY_CONTAINER_H

#include <map>
#include <string>
#include <memory>
#include <functional>
#include <mutex>
#include "IBaseConfig.h"
#include "ISvcSave.h"
#include "ISvcManager.h"
#include "IBaseCommon.h"

class DependencyContainer {
private:
    std::map<std::string, std::function<void*()>> m_factories;
    std::map<std::string, std::shared_ptr<void>> m_instances;
    mutable std::mutex m_mutex;
    bool m_initialized;
    
public:
    DependencyContainer() : m_initialized(false) {}
    virtual ~DependencyContainer() {
        Shutdown();
    }
    
    template<typename Interface, typename Implementation, typename... Args>
    void RegisterService(Args&&... args) {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::string key = typeid(Interface).name();
        m_factories[key] = [args...]() -> void* {
            return static_cast<void*>(new Implementation(std::forward<Args>(args)...));
        };
    }
    
    template<typename Interface>
    std::shared_ptr<Interface> GetService() {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::string key = typeid(Interface).name();
        
        auto instanceIt = m_instances.find(key);
        if (instanceIt != m_instances.end()) {
            return std::static_pointer_cast<Interface>(instanceIt->second);
        }
        
        auto factoryIt = m_factories.find(key);
        if (factoryIt == m_factories.end()) {
            return nullptr;
        }
        
        void* rawPtr = factoryIt->second();
        std::shared_ptr<void> instance(rawPtr, [](void* ptr) {
            delete static_cast<char*>(ptr);
        });
        
        m_instances[key] = instance;
        return std::static_pointer_cast<Interface>(instance);
    }
    
    template<typename Interface>
    Interface* GetServiceRaw() {
        auto sharedPtr = GetService<Interface>();
        return sharedPtr ? sharedPtr.get() : nullptr;
    }
    
    bool Initialize() {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_initialized) return true;
        
        for (auto& pair : m_instances) {
            auto serviceManager = std::dynamic_pointer_cast<ISvcManager>(pair.second);
            if (serviceManager) {
                serviceManager->Initialize();
            }
        }
        
        m_initialized = true;
        return true;
    }
    
    bool Shutdown() {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (!m_initialized) return true;
        
        for (auto& pair : m_instances) {
            auto serviceManager = std::dynamic_pointer_cast<ISvcManager>(pair.second);
            if (serviceManager) {
                serviceManager->Shutdown();
            }
        }
        
        m_instances.clear();
        m_factories.clear();
        m_initialized = false;
        return true;
    }
    
    bool IsInitialized() const {
        return m_initialized;
    }
    
    std::vector<std::string> GetRegisteredServices() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::vector<std::string> services;
        for (const auto& pair : m_factories) {
            services.push_back(pair.first);
        }
        return services;
    }
    
    template<typename Interface>
    bool IsServiceRegistered() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::string key = typeid(Interface).name();
        return m_factories.find(key) != m_factories.end();
    }
};

#endif // DEPENDENCY_CONTAINER_H
