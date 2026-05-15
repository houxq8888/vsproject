#ifndef PUTZHTEXT_H
#define PUTZHTEXT_H

#ifdef __WIN32

#else
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

namespace HGMACHINE{

    typedef struct tagZhFreeType{
        std::string fontPath;
        std::string text;
        int fontHeight;
        int baseline;
        int thickness;
        int linestyle;
        int rgb[3];
        float pos[2];
    } ZhFreeType;

void putZhText(const ZhFreeType &zhfreeType,cv::Mat &mat);
}

#endif
#endif // PUTZHTEXT_H