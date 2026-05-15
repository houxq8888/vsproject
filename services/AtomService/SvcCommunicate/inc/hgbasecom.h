#ifndef HGBASECOMMON_H
#define HGBASECOMMON_H

#include "HGComunicate_global.h"

namespace HGMACHINE {

class HGBaseCommon
{
public:
    HGBaseCommon();
    virtual ~HGBaseCommon();

    virtual int basesend()=0;
    virtual int baserecv()=0;
};
}

#endif // HGBASECOMMON_H
