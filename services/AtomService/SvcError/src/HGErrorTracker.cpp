#include "HGErrorTracker.h"
#include <algorithm>
#include <deque>

namespace HGMACHINE {

// StandardErrorTracker的内部实现类
class StandardErrorTracker::Impl {
public:
    Impl(int maxTrackedErrors) 
        : m_maxTrackedErrors(maxTrackedErrors) {
    }
    
    ~Impl() = default;
    
    // 记录异常
    void trackException(const HGException& ex) {
        std::lock_guard<std::mutex> lock(m_mutex);
        
        // 更新统计信息
        m_statistics.update(ex);
        
        // 添加到错误列表
        m_trackedErrors.push_back(ex);
        
        // 如果超过最大数量，移除最旧的错误
        while (m_trackedErrors.size() > m_maxTrackedErrors) {
            m_trackedErrors.pop_front();
        }
        
        // 调用回调函数
        if (m_trackingCallback) {
            try {
                m_trackingCallback(ex);
            } catch (...) {
                // 忽略回调函数中的异常
            }
        }
    }
    
    // 记录错误信息
    void trackErrorInfo(const ErrorInfo& info) {
        if (!info.hasError()) {
            return; // 没有错误则不记录
        }
        
        std::lock_guard<std::mutex> lock(m_mutex);
        
        // 将ErrorInfo转换为HGException进行记录
        HGException ex(info.code(), info.severity(), info.message());
        trackException(ex);
    }
    
    // 获取错误统计
    ErrorStatistics getStatistics() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_statistics;
    }
    
    // 获取最近的错误
    std::vector<HGException> getRecentErrors(int count) const {
        std::lock_guard<std::mutex> lock(m_mutex);
        
        std::vector<HGException> result;
        
        // 从最新的错误开始取
        int startIdx = std::max(0, static_cast<int>(m_trackedErrors.size()) - count);
        
        for (auto it = m_trackedErrors.begin() + startIdx; it != m_trackedErrors.end(); ++it) {
            result.push_back(*it);
        }
        
        return result;
    }
    
    // 清除追踪记录
    void clear() {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_trackedErrors.clear();
        m_statistics.reset();
    }
    
    // 设置错误回调
    void setTrackingCallback(std::function<void(const HGException&)> callback) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_trackingCallback = callback;
    }
    
    // 设置最大追踪错误数量
    void setMaxTrackedErrors(int maxErrors) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_maxTrackedErrors = maxErrors;
        
        // 如果当前错误数量超过新的限制，移除最旧的错误
        while (m_trackedErrors.size() > m_maxTrackedErrors) {
            m_trackedErrors.pop_front();
        }
    }
    
private:
    mutable std::mutex m_mutex;
    std::deque<HGException> m_trackedErrors;
    ErrorStatistics m_statistics;
    int m_maxTrackedErrors;
    std::function<void(const HGException&)> m_trackingCallback;
};

// StandardErrorTracker实现
StandardErrorTracker::StandardErrorTracker(int maxTrackedErrors) 
    : pImpl(std::make_unique<Impl>(maxTrackedErrors)) {
}

StandardErrorTracker::~StandardErrorTracker() = default;

void StandardErrorTracker::trackException(const HGException& ex) {
    pImpl->trackException(ex);
}

void StandardErrorTracker::trackErrorInfo(const ErrorInfo& info) {
    pImpl->trackErrorInfo(info);
}

ErrorStatistics StandardErrorTracker::getStatistics() const {
    return pImpl->getStatistics();
}

std::vector<HGException> StandardErrorTracker::getRecentErrors(int count) const {
    return pImpl->getRecentErrors(count);
}

void StandardErrorTracker::clear() {
    pImpl->clear();
}

void StandardErrorTracker::setTrackingCallback(std::function<void(const HGException&)> callback) {
    pImpl->setTrackingCallback(callback);
}

StandardErrorTracker& StandardErrorTracker::getInstance() {
    static StandardErrorTracker instance;
    return instance;
}

// ErrorTrackerManager实现
ErrorTrackerManager::ErrorTrackerManager() 
    : m_defaultTracker(&StandardErrorTracker::getInstance()), 
      m_currentTracker(m_defaultTracker),
      m_autoTrackingEnabled(true) {
}

ErrorTrackerManager& ErrorTrackerManager::getInstance() {
    static ErrorTrackerManager instance;
    return instance;
}

IErrorTracker& ErrorTrackerManager::getDefaultTracker() {
    std::lock_guard<std::mutex> lock(m_mutex);
    return *m_currentTracker;
}

void ErrorTrackerManager::setCustomTracker(std::shared_ptr<IErrorTracker> tracker) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (tracker) {
        m_currentTracker = tracker;
    } else {
        m_currentTracker = m_defaultTracker;
    }
}

void ErrorTrackerManager::setAutoTracking(bool enabled) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_autoTrackingEnabled = enabled;
}

bool ErrorTrackerManager::isAutoTrackingEnabled() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_autoTrackingEnabled;
}

// 全局便捷函数实现
void trackException(const HGException& ex) {
    auto& manager = ErrorTrackerManager::getInstance();
    if (manager.isAutoTrackingEnabled()) {
        manager.getDefaultTracker().trackException(ex);
    }
}

void trackErrorInfo(const ErrorInfo& info) {
    auto& manager = ErrorTrackerManager::getInstance();
    if (manager.isAutoTrackingEnabled()) {
        manager.getDefaultTracker().trackErrorInfo(info);
    }
}

ErrorStatistics getErrorStatistics() {
    return ErrorTrackerManager::getInstance().getDefaultTracker().getStatistics();
}

std::vector<HGException> getRecentErrors(int count) {
    return ErrorTrackerManager::getInstance().getDefaultTracker().getRecentErrors(count);
}

void clearErrorTracking() {
    ErrorTrackerManager::getInstance().getDefaultTracker().clear();
}

void setErrorTrackingCallback(std::function<void(const HGException&)> callback) {
    ErrorTrackerManager::getInstance().getDefaultTracker().setTrackingCallback(callback);
}

void setMaxTrackedErrors(int maxErrors) {
    auto& manager = ErrorTrackerManager::getInstance();
    auto* tracker = dynamic_cast<StandardErrorTracker*>(&manager.getDefaultTracker());
    if (tracker) {
        tracker->setMaxTrackedErrors(maxErrors);
    }
}

void enableAutoTracking(bool enabled) {
    ErrorTrackerManager::getInstance().setAutoTracking(enabled);
}

void disableAutoTracking() {
    ErrorTrackerManager::getInstance().setAutoTracking(false);
}

} // namespace HGMACHINE