#ifndef AUTHORITY_DATA_ACCESS_ADAPTER_H
#define AUTHORITY_DATA_ACCESS_ADAPTER_H

#include "IAuthorityDataAccess.h"
#include "ServiceInterfaces_global.h"
#include <memory>

class SERVICEINTERFACES_EXPORT AuthorityDataAccessAdapter : public IAuthorityDataAccess {
public:
    AuthorityDataAccessAdapter();
    virtual ~AuthorityDataAccessAdapter() = default;

    void Load() override;
    void Save() override;
    
    std::vector<std::string> GetWholeAuthority() override;
    void SetWholeAuthority(const std::vector<std::string>& wholeAuthority) override;
    std::vector<std::string> GetAuthorityDetail() override;
    std::vector<std::string> GetAuthorityDetail(const std::string& authority) override;
    std::vector<std::map<std::string, std::string>> GetAuthorityInfo() override;
    std::string GetAuthorityField(int index, const std::string& fieldName) const override;
    void SetAuthorityField(int index, const std::string& fieldName, const std::string& value) override;
    void AddAuthorityField(int index, const std::string& fieldName, const std::string& value) override;
    void AddAuthorityRecord(const std::map<std::string, std::string>& info) override;
    void DelAuthorityRecord(const std::string& value) override;
    
    bool BeginEdit() override;
    bool CommitEdit() override;
    bool RollbackEdit() override;
    bool IsEditing() override;
    void AddFieldToEdit(const std::string& authorityName, const std::string& fieldValue) override;
    void RemoveFieldFromEdit(const std::string& authorityName, const std::string& fieldValue) override;

private:
    class Impl;
    Impl* m_impl;
};

#endif // AUTHORITY_DATA_ACCESS_ADAPTER_H
