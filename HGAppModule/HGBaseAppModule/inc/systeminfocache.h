#ifndef SYSTEMINFOCACHE_H
#define SYSTEMINFOCACHE_H

#include <map>
#include <string>
#include <mutex>

class SystemInfoCache
{
public:
    void load();
    void save();
    void setValue(const std::string &key, const std::string &value);
    void addValue(const std::string &key, const std::string &value);
    void delValue(const std::string &key, const std::string &value);
    const std::string &getValue(const std::string &key) const;

private:
    std::map<std::string,std::string> m_data;
    bool m_dirty = false;
    mutable std::mutex m_mutex;
    bool m_loaded = false;
};

#endif