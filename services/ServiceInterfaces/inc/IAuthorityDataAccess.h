#ifndef I_AUTHORITY_DATA_ACCESS_H
#define I_AUTHORITY_DATA_ACCESS_H

#include <string>
#include <vector>
#include <map>
#include "ServiceInterfaces_global.h"

class SERVICEINTERFACES_EXPORT IAuthorityDataAccess {
public:
    virtual ~IAuthorityDataAccess() = default;

    virtual void Load() = 0;
    virtual void Save() = 0;
    
    virtual std::vector<std::string> GetWholeAuthority() = 0;
    virtual void SetWholeAuthority(const std::vector<std::string>& wholeAuthority) = 0;
    virtual std::vector<std::string> GetAuthorityDetail() = 0;
    virtual std::vector<std::string> GetAuthorityDetail(const std::string& authority) = 0;
    virtual std::vector<std::map<std::string, std::string>> GetAuthorityInfo() = 0;
    virtual std::string GetAuthorityField(int index, const std::string& fieldName) const = 0;
    virtual void SetAuthorityField(int index, const std::string& fieldName, const std::string& value) = 0;
    virtual void AddAuthorityField(int index, const std::string& fieldName, const std::string& value) = 0;
    virtual void AddAuthorityRecord(const std::map<std::string, std::string>& info) = 0;
    virtual void DelAuthorityRecord(const std::string& value) = 0;
    
    virtual bool BeginEdit() = 0;
    virtual bool CommitEdit() = 0;
    virtual bool RollbackEdit() = 0;
    virtual bool IsEditing() = 0;
    virtual void AddFieldToEdit(const std::string& authorityName, const std::string& fieldValue) = 0;
    virtual void RemoveFieldFromEdit(const std::string& authorityName, const std::string& fieldValue) = 0;
};

#endif // I_AUTHORITY_DATA_ACCESS_H
