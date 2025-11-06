#include "hgcomwithsomeip.h"

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#else
namespace HGMACHINE {
HGCommonWithSomeIP::HGCommonWithSomeIP()
{
    rtm_ = vsomeip::runtime::get();
    app_ = rtm_->create_application();
}
HGCommonWithSomeIP::~HGCommonWithSomeIP()
{
    stop();
}
int HGCommonWithSomeIP::basesend()
{
    return 0;
}
int HGCommonWithSomeIP::baserecv()
{
    return 0;
}
void HGCommonWithSomeIP::init()
{

}
bool HGCommonWithSomeIP::start()
{
    return true;
}
void HGCommonWithSomeIP::stop()
{
    app_->stop();
}
void HGCommonWithSomeIP::deSerializeStruct(const std::vector<uint8_t> &contents)
{

}
std::vector<vsomeip::byte_t> HGCommonWithSomeIP::serializeStruct()
{

}
}

#endif