#ifndef HGCAPTURE2D_H
#define HGCAPTURE2D_H

#include "HGSensor_global.h"

#ifdef __cplusplus
extern "C" {
#endif
namespace HGMACHINE {
class HGCapture2D
{
public:
    HGCapture2D();
    virtual ~HGCapture2D();

    virtual bool open()=0;
    virtual void close()=0;
};
}
#ifdef __cplusplus
}
#endif

#endif // HGCAPTURE2D_H
