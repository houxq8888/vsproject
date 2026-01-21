#ifndef HGSERVICECONTAINER_H
#define HGSERVICECONTAINER_H

#include <memory>
#include <map>
#include <string>
#include <functional>
#include <vector>
#include <typeinfo>
#include <mutex>
#include <stdexcept>

namespace HGMACHINE {

// 服务生命周期枚举
enum class ServiceLifetime {
    Singleton,    // 单例：整个应用程序生命周期内只有一个实例
    Transient,    // 瞬态：每次请求都创建新实例
    Scoped        // 作用域：在特定作用域内保持单例
};

// 服务注册信息
struct ServiceDescriptor {
    std::string name;
    std::function<std::shared_ptr<void>()> factory;
    ServiceLifetime lifetime;
    std::string interfaceName;
    
    ServiceDescriptor(const std::string& n, 
                     std::function<std::shared_ptr<void>()> f, 
                     ServiceLifetime l,
                     const std::string& iface)
        : name(n), factory(f), lifetime(l), interfaceName(iface) {}
};

// 作用域管理器
class ServiceScope {
public:
    ServiceScope() = default;
    ~ServiceScope() = default;
    
    // 获取作用域内的服务实例
    template<typename T>
    std::shared_ptr<T> getService(const std::string& name) {
        auto it = m_services.find(name);
        if (it != m_services.end()) {
            return std::static_pointer_cast<T>(it->second);
        }
        return nullptr;
    }
    
    // 设置作用域内的服务实例
    void setService(const std::string& name, std::shared_ptr<void> service) {
        m_services[name] = service;
    }
    
    // 清除作用域内的所有服务
    void clear() {
        m_services.clear();
    }
    
private:
    std::map<std::string, std::shared_ptr<void>> m_services;
};

// 服务容器
class ServiceContainer {
public:
    ServiceContainer() = default;
    ~ServiceContainer() = default;
    
    // 注册单例服务
    template<typename Interface, typename Implementation>
    void registerSingleton() {
        registerService<Interface, Implementation>(ServiceLifetime::Singleton);
    }
    
    // 注册单例服务（带工厂函数）
    template<typename Interface>
    void registerSingleton(std::function<std::shared_ptr<Interface>()> factory) {
        std::string interfaceName = getTypename<Interface>();
        std::string name = interfaceName;
        
        auto voidFactory = [factory]() -> std::shared_ptr<void> {
            return std::static_pointer_cast<void>(factory());
        };
        
        std::lock_guard<std::mutex> lock(m_mutex);
        m_services[interfaceName] = ServiceDescriptor(name, voidFactory, ServiceLifetime::Singleton, interfaceName);
    }
    
    // 注册瞬态服务
    template<typename Interface, typename Implementation>
    void registerTransient() {
        registerService<Interface, Implementation>(ServiceLifetime::Transient);
    }
    
    // 注册瞬态服务（带工厂函数）
    template<typename Interface>
    void registerTransient(std::function<std::shared_ptr<Interface>()> factory) {
        std::string interfaceName = getTypename<Interface>();
        std::string name = interfaceName;
        
        auto voidFactory = [factory]() -> std::shared_ptr<void> {
            return std::static_pointer_cast<void>(factory());
        };
        
        std::lock_guard<std::mutex> lock(m_mutex);
        m_services[interfaceName] = ServiceDescriptor(name, voidFactory, ServiceLifetime::Transient, interfaceName);
    }
    
    // 注册作用域服务
    template<typename Interface, typename Implementation>
    void registerScoped() {
        registerService<Interface, Implementation>(ServiceLifetime::Scoped);
    }
    
    // 注册作用域服务（带工厂函数）
    template<typename Interface>
    void registerScoped(std::function<std::shared_ptr<Interface>()> factory) {
        std::string interfaceName = getTypename<Interface>();
        std::string name = interfaceName;
        
        auto voidFactory = [factory]() -> std::shared_ptr<void> {
            return std::static_pointer_cast<void>(factory());
        };
        
        std::lock_guard<std::mutex> lock(m_mutex);
        m_services[interfaceName] = ServiceDescriptor(name, voidFactory, ServiceLifetime::Scoped, interfaceName);
    }
    
    // 注册命名服务
    template<typename Interface, typename Implementation>
    void registerNamed(const std::string& name) {
        std::string interfaceName = getTypename<Interface>();
        
        auto factory = []() -> std::shared_ptr<void> {
            return std::static_pointer_cast<void>(std::make_shared<Implementation>());
        };
        
        std::lock_guard<std::mutex> lock(m_mutex);
        m_services[interfaceName + ":" + name] = ServiceDescriptor(name, factory, ServiceLifetime::Singleton, interfaceName);
    }
    
    // 获取服务
    template<typename T>
    std::shared_ptr<T> getService() {
        return getService<T>(getTypename<T>());
    }
    
    // 获取命名服务
    template<typename T>
    std::shared_ptr<T> getNamedService(const std::string& name) {
        std::string interfaceName = getTypename<T>();
        return getService<T>(interfaceName + ":" + name);
    }
    
    // 创建作用域
    std::shared_ptr<ServiceScope> createScope() {
        return std::make_shared<ServiceScope>();
    }
    
    // 在作用域内获取服务
    template<typename T>
    std::shared_ptr<T> getScopedService(std::shared_ptr<ServiceScope> scope) {
        std::string interfaceName = getTypename<T>();
        return getScopedService<T>(scope, interfaceName);
    }
    
    // 在作用域内获取命名服务
    template<typename T>
    std::shared_ptr<T> getNamedScopedService(std::shared_ptr<ServiceScope> scope, const std::string& name) {
        std::string interfaceName = getTypename<T>();
        return getScopedService<T>(scope, interfaceName + ":" + name);
    }
    
    // 检查服务是否已注册
    template<typename T>
    bool isRegistered() {
        std::string interfaceName = getTypename<T>();
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_services.find(interfaceName) != m_services.end();
    }
    
    // 检查命名服务是否已注册
    template<typename T>
    bool isNamedRegistered(const std::string& name) {
        std::string interfaceName = getTypename<T>();
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_services.find(interfaceName + ":" + name) != m_services.end();
    }
    
    // 初始化所有单例服务
    void initializeSingletons() {
        std::lock_guard<std::mutex> lock(m_mutex);
        
        for (auto& pair : m_services) {
            const auto& descriptor = pair.second;
            if (descriptor.lifetime == ServiceLifetime::Singleton) {
                // 预先创建所有单例实例
                descriptor.factory();
            }
        }
    }
    
    // 清除所有服务
    void clear() {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_services.clear();
        m_singletonInstances.clear();
    }
    
    // 获取容器实例（单例）
    static ServiceContainer& getInstance() {
        static ServiceContainer instance;
        return instance;
    }
    
private:
    // 注册服务的通用方法
    template<typename Interface, typename Implementation>
    void registerService(ServiceLifetime lifetime) {
        std::string interfaceName = getTypename<Interface>();
        std::string name = interfaceName;
        
        auto factory = []() -> std::shared_ptr<void> {
            return std::static_pointer_cast<void>(std::make_shared<Implementation>());
        };
        
        std::lock_guard<std::mutex> lock(m_mutex);
        m_services[interfaceName] = ServiceDescriptor(name, factory, lifetime, interfaceName);
    }
    
    // 获取服务的通用方法
    template<typename T>
    std::shared_ptr<T> getService(const std::string& key) {
        std::lock_guard<std::mutex> lock(m_mutex);
        
        auto it = m_services.find(key);
        if (it == m_services.end()) {
            throw std::runtime_error("Service not registered: " + key);
        }
        
        const auto& descriptor = it->second;
        
        switch (descriptor.lifetime) {
            case ServiceLifetime::Singleton: {
                auto singletonIt = m_singletonInstances.find(key);
                if (singletonIt != m_singletonInstances.end()) {
                    return std::static_pointer_cast<T>(singletonIt->second);
                }
                
                auto instance = std::static_pointer_cast<T>(descriptor.factory());
                m_singletonInstances[key] = instance;
                return instance;
            }
            
            case ServiceLifetime::Transient:
                return std::static_pointer_cast<T>(descriptor.factory());
                
            case ServiceLifetime::Scoped:
                throw std::runtime_error("Scoped service requires a scope. Use getScopedService() instead.");
        }
        
        return nullptr;
    }
    
    // 在作用域内获取服务的通用方法
    template<typename T>
    std::shared_ptr<T> getScopedService(std::shared_ptr<ServiceScope> scope, const std::string& key) {
        std::lock_guard<std::mutex> lock(m_mutex);
        
        auto it = m_services.find(key);
        if (it == m_services.end()) {
            throw std::runtime_error("Service not registered: " + key);
        }
        
        const auto& descriptor = it->second;
        
        if (descriptor.lifetime != ServiceLifetime::Scoped) {
            // 对于非作用域服务，直接返回
            return getService<T>(key);
        }
        
        // 检查作用域内是否已有实例
        auto instance = scope->getService<T>(key);
        if (instance) {
            return instance;
        }
        
        // 在作用域内创建新实例
        instance = std::static_pointer_cast<T>(descriptor.factory());
        scope->setService(key, std::static_pointer_cast<void>(instance));
        return instance;
    }
    
    // 获取类型名称
    template<typename T>
    static std::string getTypename() {
        return typeid(T).name();
    }
    
    std::mutex m_mutex;
    std::map<std::string, ServiceDescriptor> m_services;
    std::map<std::string, std::shared_ptr<void>> m_singletonInstances;
};

// 便捷宏定义
#define REGISTER_SINGLETON(container, interface, implementation) \
    container->template registerSingleton<interface, implementation>()

#define REGISTER_TRANSIENT(container, interface, implementation) \
    container->template registerTransient<interface, implementation>()

#define REGISTER_SCOPED(container, interface, implementation) \
    container->template registerScoped<interface, implementation>()

#define GET_SERVICE(container, type) \
    container->template getService<type>()

#define GET_NAMED_SERVICE(container, type, name) \
    container->template getNamedService<type>(name)

} // namespace HGMACHINE

#endif // HGSERVICECONTAINER_H