#ifndef SVC_FRAME_ADAPTER_H
#define SVC_FRAME_ADAPTER_H

#include "ISvcFrame.h"
#include "ServiceInterfaces_global.h"
#include <string>
#include <memory>

class SERVICEINTERFACES_EXPORT SvcFrameAdapter : public ISvcFrame {
public:
    SvcFrameAdapter();
    virtual ~SvcFrameAdapter();
    
    std::string ReadCurDirPath() override;
    bool loadConfig(const std::string &curPath) override;
    void saveConfig() override;

    std::string getAppName() override;
    std::string GetDirPath() const override;
    uint16_t getScannerVendorID() override;
    uint16_t getScannerProductID() override;
};

#endif // SVC_FRAME_ADAPTER_H
