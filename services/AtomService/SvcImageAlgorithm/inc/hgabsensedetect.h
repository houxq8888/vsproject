#ifndef HGABSENSEDETECT_H
#define HGABSENSEDETECT_H

#include "HGImageAlgorithmService_global.h"
#ifdef __cplusplus
extern "C" {
#endif
namespace HGMACHINE {

class HGAbsenseDetect
{
public:
    HGAbsenseDetect();
    virtual ~HGAbsenseDetect();


protected:
    cv::Mat _gaussian_filter(const cv::Mat& mat);
    void _sobel_gradient(const cv::Mat& mat, cv::Mat& dx, cv::Mat& dy, cv::Mat& magnitudes, cv::Mat& angles,
        int apertureSize, bool L2gradient);
    void _calculate_hysteresis_threshold_value(const cv::Mat& dx, const cv::Mat& dy, const cv::Mat& magnitudes,
        const cv::Mat& angles, cv::Mat& NMSImage, int& low, int& high);
    void _non_maximum_suppression(const cv::Mat& NMSImage, cv::Mat& map, std::deque<int>& mapIndicesX,
        std::deque<int>& mapIndicesY, int low, int high);
    void _hysteresis_thresholding(std::deque<int>& mapIndicesX, std::deque<int>& mapIndicesY, cv::Mat& map);
    cv::Mat _get_canny_result(const cv::Mat& map);
};
}
#ifdef __cplusplus
}
#endif
#endif // HGABSENSEDETECT_H
