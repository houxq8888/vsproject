#ifndef HGCAPTURE2DFROMUSB_H
#define HGCAPTURE2DFROMUSB_H

#include "hgcapture2d.h"
#include "opencv2/opencv.hpp"
#include <mutex>

#ifdef __cplusplus
extern "C" {
#endif
namespace HGMACHINE {
class HGCapture2DFromUSB : public HGCapture2D
{
public:
    HGCapture2DFromUSB();
    ~HGCapture2DFromUSB();
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
#endif // HGCAPTURE2DFROMUSB_H
