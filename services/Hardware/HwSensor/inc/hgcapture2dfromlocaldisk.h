#ifndef HGCAPTURE2DFROMLOCALDISK_H
#define HGCAPTURE2DFROMLOCALDISK_H

#include "HGSensor_global.h"
#include "hgcapture2d.h"
#ifdef __cplusplus
extern "C" {
#endif
namespace HGMACHINE {
class HGCapture2DFromLocalDisk : public HGCapture2D
{
public:
    HGCapture2DFromLocalDisk();
    ~HGCapture2DFromLocalDisk();

    virtual bool open();
    virtual void close();

private:


};
}
#ifdef __cplusplus
}
#endif
#endif // HGCAPTURE2DFROMLOCALDISK_H
