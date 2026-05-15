#ifndef DATABASE_MANAGER_ADAPTER_H
#define DATABASE_MANAGER_ADAPTER_H

#include "IDatabaseManager.h"
#include "ServiceInterfaces_global.h"
#include <memory>

class SERVICEINTERFACES_EXPORT DatabaseManagerAdapter : public IDatabaseManager {
public:
    static DatabaseManagerAdapter& instance();
    
    DatabaseManagerAdapter();
    virtual ~DatabaseManagerAdapter();

    bool Initialize() override;
    bool Connect(const std::string& dbPath) override;
    bool Disconnect() override;
    bool IsConnected() const override;
    
    bool Execute(const std::string& sql) override;
    bool Query(const std::string& sql, std::vector<std::map<std::string, std::string>>& results) override;
    bool Insert(const std::string& table, const std::map<std::string, std::string>& data) override;
    bool Update(const std::string& table, const std::map<std::string, std::string>& data, const std::string& condition) override;
    bool Delete(const std::string& table, const std::string& condition) override;
    
    bool BeginTransaction() override;
    bool Commit() override;
    bool Rollback() override;

private:
    DatabaseManagerAdapter(const DatabaseManagerAdapter&) = delete;
    DatabaseManagerAdapter& operator=(const DatabaseManagerAdapter&) = delete;
    
    class Impl;
    Impl* m_impl;
};

#endif // DATABASE_MANAGER_ADAPTER_H
