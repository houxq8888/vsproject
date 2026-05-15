#ifndef ERRORADAPTER_H
#define ERRORADAPTER_H

#include "IfError_global.h"
#include "IError.h"

namespace HGMACHINE {

class IFERROR_EXPORT ErrorAdapter : public IError {
public:
    ErrorAdapter();
    ErrorAdapter(HGErrorCode code, HGErrorSeverity severity, const std::string& message);
    
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

#endif // ERRORADAPTER_H
