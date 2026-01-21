#ifndef HGCOMMONWITHSOMEIP_H
#define HGCOMMONWITHSOMEIP_H

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#else
#include "hgbasecom.h"
#include <vsomeip/vsomeip.hpp>

namespace HGMACHINE {
class HGCOMMUNICATE_SERVICE_EXPORT HGCommonWithSomeIP : public HGBaseCommon
{
public:
    HGCommonWithSomeIP();
    ~HGCommonWithSomeIP();

    virtual int basesend();
    virtual int baserecv();

    void init();
    bool start();
    void stop();

    void deSerializeStruct(const std::vector<uint8_t> &contents);
    std::vector<vsomeip::byte_t> serializeStruct();

private:
    std::shared_ptr<vsomeip::runtime> rtm_;
    std::shared_ptr<vsomeip::application> app_;
};
}

#endif
#endif // HGCOMMONWITHSOMEIP_H
