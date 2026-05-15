#ifndef SVCERRORADAPTER_H
#define SVCERRORADAPTER_H

#include "ServiceInterfaces_global.h"
#include "ISvcError.h"

namespace HGMACHINE {

class SERVICEINTERFACES_EXPORT SvcErrorAdapter : public ISvcError {
public:
    SvcErrorAdapter();
    SvcErrorAdapter(HGErrorCode code, HGErrorSeverity severity, const std::string& message);
    
    HGErrorCode code() const override;
    HGErrorSeverity severity() const override;
    const std::string& message() const override;
    bool hasError() const override;
    bool isCritical() const override;
    
    void clear() override;
    void set(HGErrorCode code, HGErrorSeverity severity, const std::string& message) override;
    
    std::string toString() const override;

private:
    HGErrorCode m_code;
    HGErrorSeverity m_severity;
    std::string m_message;
};

}

#endif // SVCERRORADAPTER_H
