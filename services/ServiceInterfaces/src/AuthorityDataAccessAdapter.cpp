#include "AuthorityDataAccessAdapter.h"
#include "authorityinfocache.h"

class AuthorityDataAccessAdapter::Impl {
public:
    AuthorityInfoCache cache;
};

AuthorityDataAccessAdapter::AuthorityDataAccessAdapter() : m_impl(new Impl()) {
}

AuthorityDataAccessAdapter::~AuthorityDataAccessAdapter() {
    if (m_impl) {
        delete m_impl;
        m_impl = nullptr;
    }
}

void AuthorityDataAccessAdapter::Load() {
    m_impl->cache.load();
}

void AuthorityDataAccessAdapter::Save() {
    m_impl->cache.save();
}

std::vector<std::string> AuthorityDataAccessAdapter::GetWholeAuthority() {
    return m_impl->cache.getWholeAuthority();
}

void AuthorityDataAccessAdapter::SetWholeAuthority(const std::vector<std::string>& wholeAuthority) {
    m_impl->cache.setWholeAuthority(wholeAuthority);
}

std::vector<std::string> AuthorityDataAccessAdapter::GetAuthorityDetail() {
    return m_impl->cache.getAuthorityDetail();
}

std::vector<std::string> AuthorityDataAccessAdapter::GetAuthorityDetail(const std::string& authority) {
    return m_impl->cache.getAuthorityDetail(authority);
}

std::vector<std::map<std::string, std::string>> AuthorityDataAccessAdapter::GetAuthorityInfo() {
    return m_impl->cache.getAuthorityInfo();
}

std::string AuthorityDataAccessAdapter::GetAuthorityField(int index, const std::string& fieldName) const {
    return m_impl->cache.getAuthorityField(index, fieldName);
}

void AuthorityDataAccessAdapter::SetAuthorityField(int index, const std::string& fieldName, const std::string& value) {
    m_impl->cache.setAuthorityField(index, fieldName, value);
}

void AuthorityDataAccessAdapter::AddAuthorityField(int index, const std::string& fieldName, const std::string& value) {
    m_impl->cache.addAuthorityField(index, fieldName, value);
}

void AuthorityDataAccessAdapter::AddAuthorityRecord(const std::map<std::string, std::string>& info) {
    m_impl->cache.addAuthorityRecord(info);
}

void AuthorityDataAccessAdapter::DelAuthorityRecord(const std::string& value) {
    m_impl->cache.delAuthorityRecord(value);
}

bool AuthorityDataAccessAdapter::BeginEdit() {
    return m_impl->cache.beginAuthorityEdit();
}

bool AuthorityDataAccessAdapter::CommitEdit() {
    return m_impl->cache.commitAuthorityEdit();
}

bool AuthorityDataAccessAdapter::RollbackEdit() {
    return m_impl->cache.rollbackAuthorityEdit();
}

bool AuthorityDataAccessAdapter::IsEditing() {
    return m_impl->cache.isAuthorityEditing();
}

void AuthorityDataAccessAdapter::AddFieldToEdit(const std::string& authorityName, const std::string& fieldValue) {
    m_impl->cache.addAuthorityFieldToEdit(authorityName, fieldValue);
}

void AuthorityDataAccessAdapter::RemoveFieldFromEdit(const std::string& authorityName, const std::string& fieldValue) {
    m_impl->cache.removeAuthorityFieldFromEdit(authorityName, fieldValue);
}
