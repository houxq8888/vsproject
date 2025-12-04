#include "DetectDimentionVuxInterface.h"
#include "hxqDetectDimentionVux.h"

using namespace HGMACHINE;

namespace HXQACHINE {
HXQDetectDimentionVux  hxqDetectDimentionVux;

void detDimentionVux(const HGImg2D &img, const HGRect2D &roi)
{
    hxqDetectDimentionVux.detDimentionVux(img,roi);
}
float getDimentionValue()
{
    return hxqDetectDimentionVux.getDimentionValue();
}
HGImg2D getDst()
{
    return hxqDetectDimentionVux.getDst();
}
}
