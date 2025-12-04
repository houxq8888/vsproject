#ifndef HGSAVEIMGTOLOCALDISK_H
#define HGSAVEIMGTOLOCALDISK_H

#include "HGSaveService_global.h"

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#else
#include <opencv2/opencv.hpp>
#ifdef __cplusplus
extern "C" {
#endif
namespace HGMACHINE {

class HGSaveImgToLocalDisk
{
public:
    HGSaveImgToLocalDisk();
    ~HGSaveImgToLocalDisk();

    static HGSaveImgToLocalDisk* getSaveImgInstance();
    void save(const cv::Mat& mat, const std::string &path);

private:

};
}
#ifdef __cplusplus
}
#endif
#endif
#endif // HGSAVEIMGTOLOCALDISK_H
