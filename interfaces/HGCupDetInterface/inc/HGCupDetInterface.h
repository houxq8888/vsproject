#ifndef HGCUPDETINTERFACE_H
#define HGCUPDETINTERFACE_H

#include "HGMacroData.h"
#include "HGCupDetInterface_global.h"
#include <string>

namespace HGMACHINE {

#ifdef __cplusplus
extern "C" {
#endif

    void openUSBCamera(const int &index);
    HGImg2D getImgFromUSBOneShot();
// det plastic cup is absense or not,input img struct and roi
    void detCupExistence(const HGImg2D &img, const HGRect2D &roi);
    void detCircle(const HGImg2D &img, const HGRect2D &roi);
// get absense flag,true:absense,flag:non absense
    bool getAbsenseFlag();
// get detect dst img
    HGImg2D getDst();
    int getTargetPosX();
    void closeUSBCamera();
}

#ifdef __cplusplus
}
#endif
#endif // HGCUPDETINTERFACE_H
