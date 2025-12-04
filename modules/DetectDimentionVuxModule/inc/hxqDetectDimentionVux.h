#ifndef HXQDETECTDIMENTIONVUX_H
#define HXQDETECTDIMENTIONVUX_H


#include "HGMacroData.h"
#include "hgcommonutility.h"
#include "dimentionVux.h"

using namespace HGMACHINE;

namespace HXQACHINE {

class HXQDetectDimentionVux
{
public:
    HXQDetectDimentionVux();
    ~HXQDetectDimentionVux();

    void detDimentionVux(const HGImg2D &img, const HGRect2D &roi);
    float getDimentionValue() {return m_dimentionValue;};
    HGImg2D getDst() {return m_dst;};

private:
    HxqDimentionVux m_dimentionVux;
    float m_dimentionValue;
    HGImg2D m_dst;
};
}

#endif // HXQDETECTDIMENTIONVUX_H
