#ifndef HGERRORTRACKER_H
#define HGERRORTRACKER_H

#include "HGError.h"
#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <chrono>
#include <functional>
#include <map>

namespace HGMACHINE {

// 错误统计信息
struct ErrorStatistics {
    int totalErrors = 0;
    int criticalErrors = 0;
    int errorErrors = 0;
    int warningErrors = 0;
    int infoErrors = 0;
    
    // 按类别统计的错误数量
    std::map<ErrorCategory, int> errorsByCategory;
    
    // 错误发生的时间范围
    std::chrono::system_clock::time_point firstErrorTime;
    std::chrono::system_clock::time_point lastErrorTime;
    
    void reset() {
        totalErrors = 0;
        criticalErrors = 0;
        errorErrors = 0;
        warningErrors = 0;
        infoErrors = 0;
        errorsByCategory.clear();
        firstErrorTime = std::chrono::system_clock::time_point{};
        lastErrorTime = std::chrono::system_clock::time_point{};
    }
    
    void update(const HGException& ex) {
        totalErrors++;
        
        switch (ex.severity()) {
            case ErrorSeverity::CRITICAL:
                criticalErrors++;
                break;
            case ErrorSeverity::ERROR:
                errorErrors++;
                break;
            case ErrorSeverity::WARNING:
                warningErrors++;
                break;
            case ErrorSeverity::INFO:
                infoErrors++;
                break;
        }
        
        errorsByCategory[ex.code()]++;
        
        auto now = std::chrono::system_clock::now();
        if (firstErrorTime == std::chrono::system_clock::time_point{}) {
            firstErrorTime = now;
        }
        lastErrorTime = now;
    }
};

// 错误追踪器接口
class IErrorTracker {
public:
    virtual ~IErrorTracker() = default;
    
    // 记录异常
    virtual void trackException(const HGException& ex) = 0;
    
    // 记录错误信息
    virtual void trackErrorInfo(const ErrorInfo& info) = 0;
    
    // 获取错误统计
    virtual ErrorStatistics getStatistics() const = 0;
    
    // 获取最近的错误
    virtual std::vector<HGException> getRecentErrors(int count = 10) const = 0;
    
    // 清除追踪记录
    virtual void clear() = 0;
    
    // 设置错误回调
    virtual void setTrackingCallback(std::function<void(const HGException&)> callback) = 0;
};

// 标准错误追踪器实现
class StandardErrorTracker : public IErrorTracker {
public:
    StandardErrorTracker(int maxTrackedErrors = 1000);
    ~StandardErrorTracker() override;
    
    // 记录异常
    void trackException(const HGException& ex) override;
    
    // 记录错误信息
    void trackErrorInfo(const ErrorInfo& info) override;
    
    // 获取错误统计
    ErrorStatistics getStatistics() const override;
    
    // 获取最近的错误
    std::vector<HGException> getRecentErrors(int count = 10) const override;
    
    // 清除追踪记录
    void clear() override;
    
    // 设置错误回调
    void setTrackingCallback(std::function<void(const HGException&)> callback) override;
    
    // 获取单例实例
    static StandardErrorTracker& getInstance();
    
    // 设置最大追踪错误数量
    void setMaxTrackedErrors(int maxErrors);
    
private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

// 错误追踪器管理器
class ErrorTrackerManager {
public:
    static ErrorTrackerManager& getInstance();
    
    // 获取默认错误追踪器
    IErrorTracker& getDefaultTracker();
    
    // 设置自定义错误追踪器
    void setCustomTracker(std::shared_ptr<IErrorTracker> tracker);
    
    // 启用/禁用自动追踪
    void setAutoTracking(bool enabled);
    
    // 是否启用自动追踪
    bool isAutoTrackingEnabled() const;
    
private:
    ErrorTrackerManager();
    ~ErrorTrackerManager() = default;
    
    std::shared_ptr<IErrorTracker> m_defaultTracker;
    std::shared_ptr<IErrorTracker> m_currentTracker;
    bool m_autoTrackingEnabled;
    mutable std::mutex m_mutex;
};

// 便捷函数，用于全局错误追踪
void trackException(const HGException& ex);
void trackErrorInfo(const ErrorInfo& info);
ErrorStatistics getErrorStatistics();
std::vector<HGException> getRecentErrors(int count = 10);
void clearErrorTracking();
void setErrorTrackingCallback(std::function<void(const HGException&)> callback);
void setMaxTrackedErrors(int maxErrors);
void enableAutoTracking(bool enabled = true);
void disableAutoTracking();

} // namespace HGMACHINE

#endif // HGERRORTRACKER_H