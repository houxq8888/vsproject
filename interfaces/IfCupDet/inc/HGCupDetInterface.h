#ifndef HGCUPDETINTERFACE_H
#define HGCUPDETINTERFACE_H

#include "HGMacroData.h"
#include "HGCupDetInterface_global.h"
#include <string>
#include "HGError.h"

namespace HGMACHINE {

class HGCupDetInterface
{
public:
    HGCupDetInterface();
    ~HGCupDetInterface();

    void detCupExistence(const HGImg2D &img, const HGRect2D &roi);
    void detCircle(const HGImg2D &img, const HGRect2D &roi);
    bool getAbsenseFlag();
    HGImg2D getDst();
    int getTargetPosX();

    ErrorInfo getLastError() const;
    void clearError();

private:
    class Impl;
    Impl* m_impl;
};

}

#endif // HGCUPDETINTERFACE_H
