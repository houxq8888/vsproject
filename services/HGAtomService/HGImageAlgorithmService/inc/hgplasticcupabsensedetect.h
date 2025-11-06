#ifndef HGPLASTICCUPABSENSEDETECT_H
#define HGPLASTICCUPABSENSEDETECT_H

#include "hgabsensedetect.h"

#ifdef __cplusplus
extern "C" {
#endif
namespace HGMACHINE {

class HGPlasticCupAbsenseDetect : public HGAbsenseDetect
{
public:
    HGPlasticCupAbsenseDetect();
    ~HGPlasticCupAbsenseDetect();

    bool detect(cv::Mat& frame,const cv::Rect& roi);

private:
    
};

}
#ifdef __cplusplus
}
#endif
#endif // HGPLASTICCUPABSENSEDETECT_H
