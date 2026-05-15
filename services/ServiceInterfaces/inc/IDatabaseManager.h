#ifndef I_DATABASE_MANAGER_H
#define I_DATABASE_MANAGER_H

#include <string>
#include <vector>
#include <map>
#include "ServiceInterfaces_global.h"

class SERVICEINTERFACES_EXPORT IDatabaseManager {
public:
    virtual ~IDatabaseManager() = default;

    virtual bool Initialize() = 0;
    virtual bool Connect(const std::string& dbPath) = 0;
    virtual bool Disconnect() = 0;
    virtual bool IsConnected() const = 0;
    
    virtual bool Execute(const std::string& sql) = 0;
    virtual bool Query(const std::string& sql, std::vector<std::map<std::string, std::string>>& results) = 0;
    virtual bool Insert(const std::string& table, const std::map<std::string, std::string>& data) = 0;
    virtual bool Update(const std::string& table, const std::map<std::string, std::string>& data, const std::string& condition) = 0;
    virtual bool Delete(const std::string& table, const std::string& condition) = 0;
    
    virtual bool BeginTransaction() = 0;
    virtual bool Commit() = 0;
    virtual bool Rollback() = 0;
};

#endif // I_DATABASE_MANAGER_H
