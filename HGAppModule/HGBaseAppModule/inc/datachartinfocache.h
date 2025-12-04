#ifndef DATACHARTINFOCACHE_H
#define DATACHARTINFOCACHE_H 

#include <map>
#include <string>
#include <vector>
#include <mutex>


extern std::vector<std::string> recordNames;
extern std::vector<std::string> displayNames;
extern std::vector<std::string> recordInTimeNames;

class DataChartInfoCache
{
public:
    void load();
    void save();
    void setValue(const std::string &key, const std::string &value);
    void addValue(const std::string &key, const std::string &value);
    const std::string &getValue(const std::string &key) const;

private:
    std::map<std::string,std::string> m_data;
    bool m_dirty = false;
    mutable std::mutex m_mutex;
    bool m_loaded = false;
};

#endif