#include "hgsaveimgtolocaldisk.h"
#include "hgcommonutility.h"

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#else
namespace HGMACHINE {

HGSaveImgToLocalDisk::HGSaveImgToLocalDisk()
{

}
HGSaveImgToLocalDisk::~HGSaveImgToLocalDisk()
{

}
HGSaveImgToLocalDisk* HGSaveImgToLocalDisk::getSaveImgInstance()
{
    static HGSaveImgToLocalDisk obj;
    return (&obj);
}

void HGSaveImgToLocalDisk::save(const cv::Mat &mat, const std::string &path)
{
    std::ostringstream saveimgpath;
    saveimgpath<<path<<"/"<<HGGetTimeSlice()<<".bmp";
    cv::imwrite(saveimgpath.str().c_str(),mat);
}
}
#endif