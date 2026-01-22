#include "ISvcManager.h"
#include <map>
#include <mutex>

class SvcManagerAdapter : public ISvcManager {
private:
    std::map<std::string, std::function<void*()>> m_factories;
    std::map<std::string, std::shared_ptr<void>> m_instances;
    mutable std::mutex m_mutex;
    bool m_initialized;
    
public:
    SvcManagerAdapter() : m_initialized(false) {}
    
    virtual ~SvcManagerAdapter() {
        Shutdown();
    }
    
    bool Initialize() override {
        if (m_initialized) return true;
        m_initialized = true;
        return true;
    }
    
    bool Shutdown() override {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_instances.clear();
        m_factories.clear();
        m_initialized = false;
        return true;
    }
    
    void RegisterService(const std::string& serviceName, 
                            std::function<void*()> factory) override {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_factories[serviceName] = factory;
    }
    
    void* GetService(const std::string& serviceName) override {
        std::lock_guard<std::mutex> lock(m_mutex);
        
        auto instanceIt = m_instances.find(serviceName);
        if (instanceIt != m_instances.end()) {
            return instanceIt->second.get();
        }
        
        auto factoryIt = m_factories.find(serviceName);
        if (factoryIt == m_factories.end()) {
            return nullptr;
        }
        
        void* rawPtr = factoryIt->second();
        m_instances[serviceName] = std::shared_ptr<void>(rawPtr, [](void* ptr) {
            delete static_cast<char*>(ptr);
        });
        
        return m_instances[serviceName].get();
    }
    
    bool UnregisterService(const std::string& serviceName) override {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_factories.erase(serviceName);
        m_instances.erase(serviceName);
        return true;
    }
    
    std::vector<std::string> GetRegisteredServices() const override {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::vector<std::string> services;
        for (const auto& pair : m_factories) {
            services.push_back(pair.first);
        }
        return services;
    }
    
    bool IsServiceRegistered(const std::string& serviceName) const override {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_factories.find(serviceName) != m_factories.end();
    }
};
