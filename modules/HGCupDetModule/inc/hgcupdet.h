#ifndef HGCUPDET_H
#define HGCUPDET_H

#include "hgplasticcupabsensedetect.h"
#include "HGCupDet_global.h"
#include "HGMacroData.h"

#ifdef __cplusplus
extern "C" {
#endif
namespace HGMACHINE {

class HGCupDet
{
public:
    HGCupDet();
    ~HGCupDet();

    void detCupExistence(const HGImg2D &img, const HGRect2D &roi);

    void matchTemplate(const HGImg2D &img, const HGRect2D &roi,const std::string &templateName);

    void detCircle(const HGImg2D &img, const HGRect2D &roi);
    bool getAbsenseFlag() {return m_absenseFlag;};
    float getMatchScore() {return m_matchScore;};
    bool getMatchFlag() {return m_matchFlag;};
    HGImg2D getDst() {return m_dst;};
    int getTargetPosX();

private:
    HGPlasticCupAbsenseDetect m_plasticAbsenseDet;
    bool m_absenseFlag;
    bool m_matchFlag;
    float m_matchScore;
    HGImg2D m_dst;
    int m_targetPosX;
};
}

#ifdef __cplusplus
}
#endif
#endif // HGCUPDET_H
