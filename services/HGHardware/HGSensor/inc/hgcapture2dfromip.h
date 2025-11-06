#ifndef HGCAPTURE2DFROMIP_H
#define HGCAPTURE2DFROMIP_H

#include "hgcapture2d.h"
#include "opencv2/opencv.hpp"
#include <mutex>

#ifdef __cplusplus
extern "C" {
#endif
namespace HGMACHINE {
class HGCapture2DFromIP : public HGCapture2D
{
public:
    HGCapture2DFromIP();
    ~HGCapture2DFromIP();
    virtual bool open();
    virtual bool open(const int &index);
    virtual void close();
    int getFrameOne(cv::Mat &frame);

private:
    cv::VideoCapture m_cap;
    int m_index;
};
}
#ifdef __cplusplus
}
#endif
#endif // HGCAPTURE2DFROMIP_H
