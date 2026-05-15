#include "hgabsensedetect.h"

namespace HGMACHINE {


HGAbsenseDetect::HGAbsenseDetect()
{

}

HGAbsenseDetect::~HGAbsenseDetect()
{

}
cv::Mat HGAbsenseDetect::_gaussian_filter(const cv::Mat& mat)
{
    cv::Mat matDouble;
    mat.convertTo(matDouble,CV_64FC1);
    cv::Mat kernel=(cv::Mat_<double>(5,5) <<
                    2,4,5,4,2,
                    4,9,12,9,4,
                    5,12,15,12,5,
                    4,9,12,9,4,
                    2,4,5,4,2);
    kernel=kernel/159;
    cv::Mat resDouble;
    cv::filter2D(matDouble,resDouble,-1,kernel,cv::Point(-1,-1),0.0,cv::BORDER_REFLECT101);
    cv::Mat res;
    resDouble.convertTo(res,CV_8UC1);
    return res;
}
void HGAbsenseDetect::_sobel_gradient(const cv::Mat& mat, cv::Mat& dx, cv::Mat& dy, cv::Mat& magnitudes, cv::Mat& angles,
    int apertureSize, bool L2gradient)
{
    CV_Assert(apertureSize == 3 || apertureSize == 5);

    double scale = 1.0;
    cv::Sobel(mat, dx, CV_16S, 1, 0, apertureSize, scale, cv::BORDER_REPLICATE);
    cv::Sobel(mat, dy, CV_16S, 0, 1, apertureSize, scale, cv::BORDER_REPLICATE);

    const int TAN225 = 13573;			//tan22.5 * 2^15(2 << 15)

    angles = cv::Mat(mat.size(), CV_8UC1);  // 0-> horizontal, 1 -> vertical, 2 -> diagonal
    magnitudes = cv::Mat::zeros(mat.rows + 2, mat.cols + 2, CV_32SC1);
    cv::Mat magROI = cv::Mat(magnitudes, cv::Rect(1, 1, mat.cols, mat.rows));

    for (int i = 0; i < mat.rows; i++)
    {
        for (int j = 0; j < mat.cols; j++)
        {
            short xs = dx.ptr<short>(i)[j];
            short ys = dy.ptr<short>(i)[j];
            int x = (int)std::abs(xs);
            int y = (int)std::abs(ys) << 15;

            if (L2gradient) {
                //magROI.ptr<int>(i)[j] = int(xs) * xs + int(ys) * ys;
                magROI.ptr<int>(i)[j] = (int)std::sqrt(xs * xs + ys * ys);
            }
            else {
                magROI.ptr<int>(i)[j] = std::abs(int(xs)) + std::abs(int(ys));
            }

            int tan225x = x * TAN225;
            if (y < tan225x) {  // horizontal
                angles.ptr<uchar>(i)[j] = 0;
            }
            else
            {
                int tan675x = tan225x + (x << 16);
                if (y > tan675x) {  // vertical
                    angles.ptr<uchar>(i)[j] = 1;
                }
                else {  // diagonal
                    angles.ptr<uchar>(i)[j] = 2;
                }
            }
        }
    }
}
void HGAbsenseDetect::_calculate_hysteresis_threshold_value(const cv::Mat& dx, const cv::Mat& dy, const cv::Mat& magnitudes,
    const cv::Mat& angles, cv::Mat& NMSImage, int& low, int& high)
{
    NMSImage = cv::Mat::zeros(magnitudes.size(), magnitudes.type());		//CV_32SC1

    for (int i = 0; i < dx.rows; ++i)
    {
        int r = i + 1;
        for (int j = 0; j < dx.cols; ++j)
        {
            int c = j + 1;
            int m = magnitudes.ptr<int>(r)[c];
            uchar angle = angles.ptr<uchar>(i)[j];

            if (angle == 0)			//horizontal
            {
                if (m > magnitudes.ptr<int>(r)[c - 1] && m >= magnitudes.ptr<int>(r)[c + 1])
                    NMSImage.ptr<int>(r)[c] = m;
            }
            else if (angle == 1)	//vertical
            {
                if (m > magnitudes.ptr<int>(r - 1)[c] && m >= magnitudes.ptr<int>(r + 1)[c])
                    NMSImage.ptr<int>(r)[c] = m;
            }
            else if (angle == 2)	//diagonal
            {
                short xs = dx.ptr<short>(i)[j];
                short ys = dy.ptr<short>(i)[j];
                if ((xs > 0 && ys > 0) || (xs < 0 && ys < 0))
                {	//45 degree
                    if (m > magnitudes.ptr<int>(r - 1)[c - 1] && m > magnitudes.ptr<int>(r + 1)[c + 1])
                        NMSImage.ptr<int>(r)[c] = m;
                }
                else
                {	//135 degree
                    if (m > magnitudes.ptr<int>(r - 1)[c + 1] && m > magnitudes.ptr<int>(r + 1)[c - 1])
                        NMSImage.ptr<int>(r)[c] = m;
                }
            }
        }
    }

    //利用Otsu对非极大值抑制图像进行处理，将计算得到的阈值作为高阈值high, 低阈值取高阈值的0.5倍
    cv::normalize(NMSImage, NMSImage, 0, 255, cv::NORM_MINMAX);
    NMSImage.convertTo(NMSImage, CV_8UC1);

    cv::Mat temp;
    high = (int)cv::threshold(NMSImage, temp, 0, 255, cv::THRESH_OTSU);
    low = (int)(0.5 * high);
}



//对非极大值抑制后的图根据高低阈值进行标记，当当前像素小于low，则标记为1，当当前像素大于low且大于high，则标记为2
//当大于low小于high时标记为0，并将标记为2的像素坐标压入队列
void HGAbsenseDetect::_non_maximum_suppression(const cv::Mat& NMSImage, cv::Mat& map, std::deque<int>& mapIndicesX,
    std::deque<int>& mapIndicesY, int low, int high)
{
    // 0 -> the pixel may be edge
    // 1 -> the pixel is not edge
    // 2 -> the pixel is edge
    map = cv::Mat::ones(NMSImage.size(), CV_8UC1);

    for (int i = 0; i < NMSImage.rows; ++i)
    {
        for (int j = 0; j < NMSImage.cols; ++j)
        {
            int m = NMSImage.ptr<uchar>(i)[j];				//nms -> CV_8UC1
            if (m > low)
            {
                if (m > high)
                {
                    map.ptr<uchar>(i)[j] = 2;
                    mapIndicesX.push_back(j);
                    mapIndicesY.push_back(i);
                }
                else
                    map.ptr<uchar>(i)[j] = 0;
            }
        }
    }
}
//双阈值滞后处理：根据队列中的像素坐标，进行8领域边缘点寻找，即在map中与2相连的0均认作为边缘点
void HGAbsenseDetect::_hysteresis_thresholding(std::deque<int>& mapIndicesX, std::deque<int>& mapIndicesY, cv::Mat& map)
{
    while (!mapIndicesX.empty())
    {
        int r = mapIndicesY.back();
        int c = mapIndicesX.back();
        //获取到边缘点之后要将其弹出
        mapIndicesX.pop_back();
        mapIndicesY.pop_back();

        // top left
        if (map.ptr<uchar>(r - 1)[c - 1] == 0)
        {
            mapIndicesX.push_back(c - 1);
            mapIndicesY.push_back(r - 1);
            map.ptr<uchar>(r - 1)[c - 1] = 2;
        }
        // top
        if (map.ptr<uchar>(r - 1)[c] == 0)
        {
            mapIndicesX.push_back(c);
            mapIndicesY.push_back(r - 1);
            map.ptr<uchar>(r - 1)[c] = 2;
        }
        // top right
        if (map.ptr<uchar>(r - 1)[c + 1] == 0)
        {
            mapIndicesX.push_back(c + 1);
            mapIndicesY.push_back(r - 1);
            map.ptr<uchar>(r - 1)[c + 1] = 2;
        }
        // left
        if (map.ptr<uchar>(r)[c - 1] == 0)
        {
            mapIndicesX.push_back(c - 1);
            mapIndicesY.push_back(r);
            map.ptr<uchar>(r)[c - 1] = 2;
        }
        // right
        if (map.ptr<uchar>(r)[c + 1] == 0)
        {
            mapIndicesX.push_back(c + 1);
            mapIndicesY.push_back(r);
            map.ptr<uchar>(r)[c + 1] = 2;
        }
        // bottom left
        if (map.ptr<uchar>(r + 1)[c - 1] == 0)
        {
            mapIndicesX.push_back(c - 1);
            mapIndicesY.push_back(r + 1);
            map.ptr<uchar>(r + 1)[c - 1] = 2;
        }
        // bottom
        if (map.ptr<uchar>(r + 1)[c] == 0)
        {
            mapIndicesX.push_back(c);
            mapIndicesY.push_back(r + 1);
            map.ptr<uchar>(r + 1)[c] = 2;
        }
        // bottom right
        if (map.ptr<uchar>(r + 1)[c + 1] == 0)
        {
            mapIndicesX.push_back(c + 1);
            mapIndicesY.push_back(r + 1);
            map.ptr<uchar>(r + 1)[c + 1] = 2;
        }
    }
}


cv::Mat HGAbsenseDetect::_get_canny_result(const cv::Mat& map)
{
    cv::Mat dst(map.rows - 2, map.cols - 2, CV_8UC1);
    for (int i = 0; i < dst.rows; i++) {
        for (int j = 0; j < dst.cols; j++) {
            dst.ptr<uchar>(i)[j] = (map.ptr<uchar>(i + 1)[j + 1] == 2 ? 255 : 0);
        }
    }
    return dst;
}

}
