#include "decodecodeutility.h"
#include <iostream>

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#else
#include "opencv2/opencv.hpp"
BarcodeDecoder::BarcodeDecoder()
    : options()
{
    options.setTextMode(ZXing::TextMode::Escaped);
    options.setMaxNumberOfSymbols(1);// 默认只解码一个条形码
}
// 读取并解码图像
    std::vector<ZXing::Barcode> BarcodeDecoder::DecodeImage(const std::string& filePath) {
        cv::Mat img = cv::imread(filePath,cv::IMREAD_COLOR);
        if (img.empty()){
            std::cerr << "Failed to read image: " << filePath << "\n";
            return {};
        }
        // 转换为 RGB 格式，因为 ZXing 需要 RGB 图像
        cv::Mat imgRGB;
        cv::cvtColor(img,imgRGB,cv::COLOR_BGR2RGB);

        ZXing::ImageView image{imgRGB.data, imgRGB.cols, imgRGB.rows, ZXing::ImageFormat::RGB};
        auto barcodes = ZXing::ReadBarcodes(image.rotated(rotate), options);
        return barcodes;
    }

    // 获取解码结果文本
    std::string BarcodeDecoder::GetDecodedText(const ZXing::Barcode& barcode) {
        return barcode.isValid() ? barcode.text(ZXing::TextMode::Escaped) : "No valid barcode found";
    }

    // 获取解码的字节数据
    std::vector<uint8_t> BarcodeDecoder::GetDecodedBytes(const ZXing::Barcode& barcode) {
        return barcode.bytes();
    }
#endif