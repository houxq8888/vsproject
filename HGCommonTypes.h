#ifndef HGCOMMONTYPES_H
#define HGCOMMONTYPES_H

#include <string>
#include <opencv2/opencv.hpp>

#define SAFE_DELETE(ptr) if (ptr) {delete ptr; ptr = NULL;}

namespace HGMACHINE {

struct HGRect2D {
    int x1;
    int y1;
    int x2;
    int y2;
    
    HGRect2D() : x1(0), y1(0), x2(0), y2(0) {}
    HGRect2D(int x1, int y1, int x2, int y2) : x1(x1), y1(y1), x2(x2), y2(y2) {}
};

struct HGImg2D {
    const unsigned char* data;
    int width;
    int height;
    int type;
    int channels;
    int steps;
    
    HGImg2D() : data(nullptr), width(0), height(0), type(0), channels(0), steps(0) {}
};

struct HGErrorDetail {
    int code;
    std::string category;
    std::string message;
    std::string timestamp;
    
    HGErrorDetail() : code(0) {}
    
    bool hasError() const {
        return code != 0;
    }
    
    std::string toString() const {
        return "[" + category + ":" + std::to_string(code) + "] " + message;
    }
};

struct MatchResult2D {
    HGRect2D rect;
    std::string name; 
    float score;
    HGImg2D dst;
    cv::Mat dstMat;
    bool flag;
    MatchResult2D() : flag(false), score(0.0f) {}
};

typedef struct tagCameraRecognizeInfo
{
    std::string cameraType;
    std::string cameraName;
    std::string templatePath;
    float score;
    HGRect2D roi;
}CameraRecognizeInfo;
}

#endif 
