#ifndef HXQDIMENTIONVUX_H
#define HXQDIMENTIONVUX_H


#include "hgabsensedetect.h"

#ifdef __cplusplus
extern "C" {
#endif
namespace HXQACHINE {

class HxqDimentionVux : public HGMACHINE::HGAbsenseDetect
{
public:
    HxqDimentionVux();
    ~HxqDimentionVux();

    float detect(cv::Mat& frame,const cv::Rect& roi);

private:

};

}
#ifdef __cplusplus
}
#endif
#endif // HXQDIMENTIONVUX_H
