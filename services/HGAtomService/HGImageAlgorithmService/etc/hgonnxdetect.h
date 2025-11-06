#ifndef HGONNXDETECT_H
#define HGONNXDETECT_H

#include "hgabsensedetect.h"
#include <opencv2/opencv.hpp>

namespace HGMACHINE {

class HGOnnxDetect : public HGAbsenseDetect
{
public:
    HGOnnxDetect();
    ~HGOnnxDetect();

    bool detect(cv::Mat& frame,const cv::Rect& roi);

private:
    
};

}

#endif // HGONNXDETECT_H
