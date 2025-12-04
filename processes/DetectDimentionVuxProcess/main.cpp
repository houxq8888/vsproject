#include <opencv2/opencv.hpp>
#include "DetectDimentionVuxInterface.h"
#include <stdlib.h>
#include <string>
#include <algorithm>

using namespace HXQACHINE;

std::vector<cv::Point> findSmoothPerimeter(int gray_gaussian_filter, std::vector<cv::Point> contours)
{
    // contour smoothing parameters for gaussian filter
    int filterRadius = gray_gaussian_filter;
    int filterSize = 2 * filterRadius + 1;
    double sigma = 10;

    size_t len = contours.size() + 2 * filterRadius;
    size_t idx = (contours.size() - filterRadius);
    std::vector<float> x, y;
    for (size_t i = 0; i < len; i++)
    {
        if (contours.size() != 0)
        {
            x.push_back(float(contours[(idx + i) % contours.size()].x));
            y.push_back(float(contours[(idx + i) % contours.size()].y));
        }
    }

    std::vector<cv::Point> smoothContours;
    smoothContours.clear();

    if (x.size() <= 0 || y.size() <= 0)
        return smoothContours;
    // filter 1-D signals
    std::vector<float> xFilt, yFilt;
    cv::GaussianBlur(x, xFilt, cv::Size(filterSize, filterSize), sigma, sigma);
    cv::GaussianBlur(y, yFilt, cv::Size(filterSize, filterSize), sigma, sigma);

    // build smoothed contour
    for (size_t i = filterRadius; i < contours.size() + filterRadius; i++)
    {
        smoothContours.push_back(cv::Point(int(xFilt[i]), int(yFilt[i])));
    }

    return smoothContours;
}
        typedef struct detectLine {
            int linePos;
            cv::Scalar color;
            std::vector<cv::Point> rawPoints;    // 结果曲线点
            std::vector<cv::Point> smoothedPoints; // 平滑后曲线点
            std::vector<int> maxminPos; // 极值位置
            std::vector<cv::Point> maxminPoints;
            std::vector<cv::Point> validMaxMinPoints;
            std::vector<bool> isValid;

            detectLine()
            {
                Clear();
            }
            detectLine(int _linePos, cv::Scalar _color)
            {
                linePos = _linePos;
                color = _color;
            }
            void Clear()
            {
                linePos = 0;
                color = cv::Scalar(0, 0, 0);
            }
            detectLine operator+(const detectLine& obj)
            {
                detectLine sum;
                sum.linePos = linePos + obj.linePos;
                sum.color = color + obj.color;
                return (sum);
            }
            detectLine operator-(const detectLine& obj)
            {
                detectLine sum;
                sum.linePos = linePos - obj.linePos;
                sum.color = color - obj.color;
                return (sum);
            }
            detectLine& operator*(double scale)
            {
                linePos = int(double(linePos) * scale);
                color[0] = color[0] * scale;
                color[1] = color[1] * scale;
                color[2] = color[2] * scale;
                return (*this);
            }
            detectLine& operator+=(const detectLine& obj)
            {
                linePos += obj.linePos;
                color[0] += obj.color[0];
                color[1] += obj.color[1];
                color[2] += obj.color[2];
                return (*this);
            }
            detectLine& operator-=(const detectLine& obj)
            {
                linePos -= obj.linePos;
                color[0] -= obj.color[0];
                color[1] -= obj.color[1];
                color[2] -= obj.color[2];
                return (*this);
            }
            detectLine& operator=(const detectLine& obj)
            {
                linePos = obj.linePos;
                color[0] = obj.color[0];
                color[1] = obj.color[1];
                color[2] = obj.color[2];
                return (*this);
            }
            bool operator==(const detectLine& obj) const
            {
                bool ret = false;
                if (linePos == obj.linePos && color[0] == obj.color[0] && color[1] == obj.color[1] && color[2] == obj.color[2])
                    ret = true;
                return (ret);
            }
            bool operator!=(const detectLine& obj) const
            {
                return (!(*this == obj));
            }
        }DETECTLINE, * PDETECTLINE;
        typedef struct pointInfo {
            int x;
            int y;
            int diff;
            pointInfo()
            {
                Clear();
            }
            pointInfo(int valuex, int valuey, int valuediff)
            {
                x = valuex;
                y = valuey;
                diff = valuediff;
            }
            void Clear()
            {
                x = 0;
                y = 0;
                diff = 0;
            }
            pointInfo operator+(const pointInfo& obj)
            {
                pointInfo sum;
                sum.x = x + obj.x;
                sum.y = y + obj.y;
                sum.diff = y + obj.diff;
                return (sum);
            }
            pointInfo operator-(const pointInfo& obj)
            {
                pointInfo sum;
                sum.x = x - obj.x;
                sum.y = y - obj.y;
                sum.diff = diff - obj.diff;
                return (sum);
            }
            pointInfo& operator*(double scale)
            {
                x = int((double)x * scale);
                y = int((double)y * scale);
                diff = int((double)diff * scale);
                return (*this);
            }
            pointInfo& operator+=(const pointInfo& obj)
            {
                x += obj.x;
                y += obj.y;
                diff += obj.diff;
                return (*this);
            }
            pointInfo& operator-=(const pointInfo& obj)
            {
                x -= obj.x;
                y -= obj.y;
                diff -= obj.diff;
                return (*this);
            }
            pointInfo& operator=(const pointInfo& obj)
            {
                x = obj.x;
                y = obj.y;
                diff = obj.diff;
                return (*this);
            }
            bool operator==(const pointInfo& obj) const
            {
                bool ret = false;
                if (x == obj.x && y == obj.y && diff == obj.diff)
                    ret = true;
                return (ret);
            }
            bool operator!=(const pointInfo& obj) const
            {
                return (!(*this == obj));
            }
        }POINTINFO, * PPOINTINFO;
void findPeak(int gray_peak_range, const std::vector<POINTINFO>& v, std::vector<int>& peakPositions)
        {
            if (v.size()<=1) return;
            std::vector<POINTINFO> diff_v(v.size() - 1, POINTINFO(0, 0, 0));
            // 计算V的一阶差分和符号函数trend
            for (std::vector<int>::size_type i = 0; i != diff_v.size(); i++)
            {
                if (v[i + 1].y - v[i].y > 0) {
                    diff_v[i].x = v[i].x;
                    diff_v[i].y = v[i].y;
                    diff_v[i].diff = 1;
                }
                else if (v[i + 1].y - v[i].y < 0) {
                    diff_v[i].x = v[i].x;
                    diff_v[i].y = v[i].y;
                    diff_v[i].diff = -1;
                }
                else {
                    diff_v[i].x = v[i].x;
                    diff_v[i].y = v[i].y;
                    diff_v[i].diff = 0;
                }
            }
            // 对Trend作了一个遍历
            for (int i = diff_v.size() - 1; i >= 0; i--)
            {
                if (diff_v[i].diff == 0 && i == int(diff_v.size() - 1))
                {
                    diff_v[i].diff = 1;
                }
                else if (diff_v[i].diff == 0)
                {
                    diff_v[i].diff = diff_v[i + 1].diff;
                }
            }
            int gap = gray_peak_range;
            for (std::vector<int>::size_type i = gap; i != diff_v.size() - gap; i++)
            {
                bool flag = false;
                std::vector<POINTINFO> frontPoint, backPoint;
                for (int j = 1; j <= gap; j++) {
                    int index = i - j;
                    int diffy = 0;
                    if (diff_v[i].diff == 1)
                        diffy = diff_v[i].y - diff_v[index].y;
                    else if (diff_v[i].diff == -1)
                        diffy = diff_v[index].y - diff_v[i].y;
                    if (diffy <= gap)
                        frontPoint.push_back(diff_v[index]);

                    index = i + j;
                    if (diff_v[i].diff == 1)
                        diffy = diff_v[i].y - diff_v[index].y;
                    else if (diff_v[i].diff == -1)
                        diffy = diff_v[index].y - diff_v[i].y;
                    if (diffy <= gap)
                        backPoint.push_back(diff_v[index]);
                }

                if (frontPoint.size() <= 0) {
                    int index = i - 1;
                    if (index >= 0)
                        frontPoint.push_back(diff_v[index]);
                }
                if (backPoint.size() <= 0) {
                    int index = i + 1;
                    if (index < int(diff_v.size()))
                        backPoint.push_back(diff_v[index]);
                }

                for (int j = 0; j < int(frontPoint.size()); j++) {
                    if (diff_v[i].diff - frontPoint[j].diff == 0)
                        flag = true;
                    else {
                        flag = false;
                        break;
                    }
                }
                if (flag) {
                    for (int j = 0; j < int(backPoint.size()); j++) {
                        if (backPoint[j].diff - diff_v[i].diff == -2)
                            flag = true;
                        else {
                            flag = false;
                            break;
                        }
                    }
                    if (!flag) {
                        for (int j = 0; j < int(backPoint.size()); j++) {
                            if (backPoint[j].diff - diff_v[i].diff == 2)
                                flag = true;
                            else {
                                flag = false;
                                break;
                            }
                        }
                    }
                }
                if (!flag) {
                    for (int j = 0; j < int(frontPoint.size()); j++) {
                        if (diff_v[i].diff - frontPoint[j].diff == -2)
                            flag = true;
                        else {
                            flag = false;
                            break;
                        }
                    }
                    if (!flag) {
                        for (int j = 0; j < int(frontPoint.size()); j++) {
                            if (diff_v[i].diff - frontPoint[j].diff == 2)
                                flag = true;
                            else {
                                flag = false;
                                break;
                            }
                        }
                    }
                    if (flag) {
                        for (int j = 0; j < int(backPoint.size()); j++) {
                            if (backPoint[j].diff - diff_v[i].diff == 0)
                                flag = true;
                            else {
                                flag = false;
                                break;
                            }
                        }
                    }
                }
                if (flag)
                    peakPositions.push_back(i + 2);
            }
            if (peakPositions.size() > 1)
            {
                for (int i = 1; i < int(peakPositions.size()); )
                {
                    if (abs(peakPositions[i] - peakPositions[i - 1]) == 1)
                    {
                        //peakPositions.erase(peakPositions.begin() + i);
                        std::swap(*(std::begin(peakPositions)+i),*(std::end(peakPositions)-1));
                        peakPositions.pop_back();
                    }else
                        i++;
                }
            }
        }

        void calculateLineEquation(const std::vector<cv::Point>& points,double& m,double & b){
            int n=points.size();
            if (n==0) return;
            double sum_x=0,sum_y=0,sum_xy=0,sum_x2=0;
            for (const auto& point:points){
                double x=point.x;
                double y=point.y;
                sum_x+=x;
                sum_y+=y;
                sum_xy+=x*y;
                sum_x2+=x*x;
            }
            double mean_x=sum_x/n;
            double mean_y=sum_y/n;
            m=(sum_xy-n*mean_x*mean_y)/(sum_x2-n*mean_x*mean_x);
            b=mean_y-m*mean_x;
        }
        bool compareMethod(cv::Point p1,cv::Point p2){
            if (p1.x > p2.x) {
                return false;
            } else return true;
        }
int main()
{
    cv::Mat mat=cv::imread("/mnt/extra/projects/camera/test/ok/Image_20240903150904328.bmp",cv::IMREAD_UNCHANGED);
    // HGRect2D roi;
    // roi.x1=0;roi.y1=0;roi.x2=mat.cols-1;roi.y2=mat.rows-1;

    // HGImg2D img;
    // img.data=mat.data;
    // img.height=mat.rows;
    // img.width=mat.cols;
    // img.type=mat.channels();
    // detDimentionVux(img,roi);

    // float value=getDimentionValue();
    // printf("value:%.2f\n",value);

    // HGImg2D dst = getDst();
    // cv::Mat dstmat(dst.height,dst.width,CV_8UC3,(uchar*)img.data);


    cv::Mat dst;
    cv::Mat gray;
    if (mat.channels() != 1) {
        mat.copyTo(dst);
        cv::cvtColor(mat, gray,cv::COLOR_BGR2GRAY);
    } else {
        cv::cvtColor(mat,dst,cv::COLOR_GRAY2BGR);
        mat.copyTo(gray);
    }

   cv::Mat hist;
   cv::equalizeHist(gray, hist);
   cv::imwrite("hist.jpg", hist);


    //利用计算出来的low和high传入opencv Canny进行对比
    cv::Mat opencvCanny;
    cv::Canny(hist, opencvCanny, 30, 50);

    imwrite("canny.jpg",opencvCanny);

    std::vector<DETECTLINE> detectLines(2);
    detectLines[0].color=cv::Scalar(0,0,255);
    detectLines[1].color=cv::Scalar(0,255,0);
    cv::Mat edge=cv::Mat::zeros(cv::Size(mat.cols,mat.rows),CV_8UC1);
    for (int i =0;i<opencvCanny.cols;i++){
        for (int j=0;j<opencvCanny.rows;j++){
            if (opencvCanny.ptr<uchar>(j)[i]==255){
                edge.ptr<uchar>(j)[i]=255;
                detectLines[0].rawPoints.push_back(cv::Point(i,j));
                break;
            }
        }
        for (int j=opencvCanny.rows-1;j>=0;j--){
            if (opencvCanny.ptr<uchar>(j)[i]==255){
                edge.ptr<uchar>(j)[i]=255;
                detectLines[1].rawPoints.push_back(cv::Point(i,j));
                break;
            }
        }
    }

    for (int k = 0; k < int(detectLines.size()); k++)
    {
        std::vector<cv::Point> poisSmooth = findSmoothPerimeter(3, detectLines[k].rawPoints);
        detectLines[k].smoothedPoints = poisSmooth;
        std::vector<POINTINFO> in;
        std::vector<int> out;
        for (int j = 0; j < int(poisSmooth.size()); j++)
        {
            if (poisSmooth[j].x >= 0 && poisSmooth[j].x <= (mat.cols-1))
            {
                POINTINFO value;
                value.x = poisSmooth[j].x;
                value.y = poisSmooth[j].y;
                value.diff = 0;
                in.push_back(value);
            }
        }
        findPeak(4, in, out);
        detectLines[k].maxminPos = out;
    }

    for (int k=0;k<int(detectLines.size());k++){
        for (int i=0;i<int(detectLines[k].maxminPos.size());i++){
            int pos=detectLines[k].maxminPos[i];
            // printf("(%d,%d)",detectLines[k].smoothedPoints[pos].x,
                // detectLines[k].smoothedPoints[pos].y);
            detectLines[k].maxminPoints.push_back(detectLines[k].smoothedPoints[pos]);
            detectLines[k].isValid.push_back(true);
            // cv::circle(dst,detectLines[k].smoothedPoints[pos],3,detectLines[k].color);
        }
        printf("\n");
        
    }
    // sort
    for (int k=0;k<int(detectLines.size());k++){
        std::sort(detectLines[k].maxminPoints.begin(),
            detectLines[k].maxminPoints.end(),
            compareMethod);
    }
    for (int k=0;k<int(detectLines.size());k++){
        for (int i=0;i<int(detectLines[k].maxminPoints.size());i++){
            printf("(%d,%d)",detectLines[k].maxminPoints[i].x,
                detectLines[k].maxminPoints[i].y);
            cv::circle(dst,detectLines[k].maxminPoints[i],3,detectLines[k].color);
        }
        printf("\n"); 
    }
    // delete points
    printf("delete points\n");
    for (int i =0;i<int(detectLines.size());i++){
        for (int j=1;j<int(detectLines[i].maxminPoints.size());j++){
            // printf("(%d,%d),(%d,%d)\n",detectLines[i].maxminPoints[j].x,
            //     detectLines[i].maxminPoints[j].y,detectLines[i].maxminPoints[j-1].x,
            //     detectLines[i].maxminPoints[j-1].y);
            if ((abs(detectLines[i].maxminPoints[j].y-detectLines[i].maxminPoints[j-1].y)) < 100){
                detectLines[i].isValid[j]=false;
            }
        }
    }
    printf("new points\n");
    for (int k=0;k<int(detectLines.size());k++){
        for (int i=0;i<int(detectLines[k].maxminPoints.size());i++){
            if (!detectLines[k].isValid[i]) continue;
            printf("(%d,%d)",detectLines[k].maxminPoints[i].x,
                detectLines[k].maxminPoints[i].y);
            detectLines[k].validMaxMinPoints.push_back(detectLines[k].maxminPoints[i]);
            cv::circle(dst,detectLines[k].maxminPoints[i],3,cv::Scalar(0,255,255));
        }
        printf("\n");
        
    }
    // cal lines A,B
    int radius = 125;
    std::vector<std::vector<double>> As(2), Bs(2);
    std::vector<std::vector<cv::Point>> circleCenter(2);
    for (int k=0;k<int(detectLines.size());k++){ 
        int start=int(detectLines[k].validMaxMinPoints.size())/2;
        int end=int(detectLines[k].validMaxMinPoints.size())*5/6;
        printf("start,end(%d,%d)\n",start,end);
        for (int i=start;i<end;i++)
        {
            cv::Point startPoint=detectLines[k].validMaxMinPoints[i];
            cv::Point endPoint=detectLines[k].validMaxMinPoints[i+1];
            std::vector<cv::Point> points, newPoints;;
            for (int j=0;j<int(detectLines[k].smoothedPoints.size());j++){
                if (detectLines[k].smoothedPoints[j].x >=startPoint.x && 
                    detectLines[k].smoothedPoints[j].x <=endPoint.x)
                {
                    points.push_back(detectLines[k].smoothedPoints[j]);
                    cv::circle(dst,detectLines[k].smoothedPoints[j],3,cv::Scalar(255,0,0));
                }
            }
            for (int j=int(points.size())/3;j<int(points.size())*2/3;j++){
                newPoints.push_back(points[j]);
            }
            double m,b; // y=mx+b, mx-y+b=0
            calculateLineEquation(newPoints, m,b);
            for (int n=startPoint.x;n<=endPoint.x;n++){
                cv::line(dst,cv::Point(startPoint.x,m*startPoint.x+b),
                    cv::Point(endPoint.x,m*endPoint.x+b),cv::Scalar(0,255,255));

            }
            As[k].push_back(m);
            Bs[k].push_back(b);
            circleCenter[k].push_back(cv::Point(startPoint.x,m*startPoint.x+b));
        }
        
    }
    // cal distance circule(a,b) (Ax+By+C=0) distance=(Aa+Bb+C)/sqrt(A*A+B*B)
    // d==r ,OK,d<r,smaller radius;d>r,bigger radius
    std::vector<cv::Point> upperCenters, lowerCenters;
    for (int j=0;j<int(As.size());j++){
        printf("line num:%d\n",As[j].size());
        for (int i=0;i<int(As[j].size());i++)
        {
            cv::Point centerPoint = circleCenter[j][i];
            double A=As[j][i];
            double C=Bs[j][i];
            int cnt=0;
            do
            {
                double distance = (A * centerPoint.x + (-1) * centerPoint.y + C) / sqrt(A * A + 1);
            
                printf("cnt:%d, distance:%.2f\n", cnt,fabs(distance));
                if (j == 0)
                {
                    if (distance < radius) {
                        centerPoint.y -= 10;
                    } else {
                        if (i % 2 == 0) {
                            cv::circle(dst, centerPoint, radius, cv::Scalar(255, 0, 0));
                            upperCenters.push_back(centerPoint);
                        }
                        break;
                    }
                }
                else if (j == 1)
                {
                    if (fabs(distance) < radius) {
                        centerPoint.y += 10;
                        
                    } else {   
                        if (i % 2 == 1){
                            cv::circle(dst, centerPoint, radius, cv::Scalar(255, 0, 0));
                            lowerCenters.push_back(centerPoint);
                        }
                        break;
                    }

                    if (cnt>=20){
                        centerPoint.x-=1;
                    }
                }
                cnt++;
            } while (cnt < 40);
        }
    }

    int topMin=mat.rows-1,bottomMax=-1;
    printf("upper:");
    for (int i=0;i<int(upperCenters.size());i++){
        printf("(%d,%d),",upperCenters[i].x,upperCenters[i].y);
        if (upperCenters[i].y < topMin) topMin=upperCenters[i].y;
    }
    printf("\n");

    printf("lower:");
    for (int i=0;i<int(lowerCenters.size());i++){
        printf("(%d,%d),",lowerCenters[i].x,lowerCenters[i].y);
        if (lowerCenters[i].y > bottomMax) bottomMax = lowerCenters[i].y;
    }
    printf("\n");

    cv::line(dst,cv::Point(0,topMin-radius),cv::Point(dst.cols-1,topMin-radius),cv::Scalar(0,0,255));
    cv::line(dst,cv::Point(0,bottomMax+radius),cv::Point(dst.cols-1,bottomMax+radius),cv::Scalar(0,0,255));
    int distancePix=bottomMax+radius-(topMin-radius);
    cv::putText(dst,"distance of pixel:"+std::to_string(distancePix),cv::Point(dst.cols/2,100),cv::FONT_HERSHEY_COMPLEX,1,cv::Scalar(0,0,255));
    cv::imwrite("edge.jpg",edge);
    // std::ostringstream content;
    // bool flag=false;
    // content<<"min:"<<minVal<<",max:"<<maxVal;
    // if ((maxVal-128.00001) > 0.00001) {
    //     content << "Y";
    //     flag=true;
    //     cv::rectangle(dst, cv::Point(calRoi.x+calRoi.width,calRoi.y),cv::Point(calRoi.x+calRoi.width+10,calRoi.y+10),cv::Scalar(0,255,0),-1);
    // } else {
    //     content << "N";
    //     flag=false;
    //     cv::rectangle(dst, cv::Point(calRoi.x+calRoi.width,calRoi.y),cv::Point(calRoi.x+calRoi.width+10,calRoi.y+10),cv::Scalar(0,0,255),-1);
    // }
    // set a radius =125 circle to fit vux
    cv::circle(dst, cv::Point(2640,366),125,cv::Scalar(0,0,255));
    // cv::putText(dst,content.str().c_str(),cv::Point(calRoi.x,calRoi.y),cv::FONT_HERSHEY_COMPLEX,1.0,cv::Scalar(0,255,0));

    cv::imwrite("dst.jpg",dst);
    return 0;
}