#include "putZhText.h"

#ifdef __WIN32

#else
#include <opencv2/freetype.hpp>

namespace HGMACHINE{

void putZhText(const ZhFreeType &zhfreeType,cv::Mat &mat)
{
    cv::Ptr<cv::freetype::FreeType2> ft2 = cv::freetype::createFreeType2();
    ft2->loadFontData(zhfreeType.fontPath, 0);  // 
    // 设置文本内容
    int fontHeight = zhfreeType.fontHeight;  // 字体高度
    int baseline = zhfreeType.baseline;     // 存储基线
    int thickness = zhfreeType.thickness;
    
    // 获取文本尺寸
    cv::Size textSize = ft2->getTextSize(zhfreeType.text, fontHeight, 
            thickness, &baseline);

    int x = (mat.cols - textSize.width) * zhfreeType.pos[0];
    int y = (mat.rows + textSize.height) * zhfreeType.pos[1] - baseline;
    int linestyle = zhfreeType.linestyle;
    ft2->putText(mat,zhfreeType.text,cv::Point(x,y),
        zhfreeType.fontHeight,cv::Scalar(zhfreeType.rgb[0],zhfreeType.rgb[1],
            zhfreeType.rgb[2]),zhfreeType.thickness,zhfreeType.linestyle,true);
}
}

#endif