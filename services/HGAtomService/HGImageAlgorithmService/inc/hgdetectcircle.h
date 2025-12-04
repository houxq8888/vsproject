#ifndef HGDETECTCIRCLE_H
#define HGDETECTCIRCLE_H

#include "hgabsensedetect.h"
#ifdef __cplusplus
extern "C" {
#endif
namespace HGMACHINE {

class HGCircleDetect  : public HGAbsenseDetect
{
public:
    HGCircleDetect();
    ~HGCircleDetect();

    bool detect(cv::Mat& frame,const cv::Rect& roi,int &targetPosX);
    bool detectBottleCap(const cv::Mat &gray,const cv::Rect& calRoi,cv::Mat &dst);
private:
    void detectDistance(cv::Mat &frame,cv::Mat& dst, cv::Mat &gray,int &targetPosX);
    bool detectBottleCap2(const cv::Mat &gray);

};

}
#ifdef __cplusplus
}
#endif
#endif // HGDETECTCIRCLE_H
