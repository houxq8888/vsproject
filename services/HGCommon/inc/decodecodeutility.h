#ifndef DECODECODEUTILITY_H
#define DECODECODEUTILITY_H

#include "ZXing/ReadBarcode.h"
#include "ZXing/BarcodeFormat.h"
#include "ZXing/ReaderOptions.h"



class BarcodeDecoder{
public:
    BarcodeDecoder();

    // 设置解码选项（如是否旋转图像，支持的条形码格式等）
    void SetTryHarder(bool value) { options.setTryHarder(value); }
    void SetRotate(int degrees) { rotate = degrees; }
    void SetFormats(const std::vector<ZXing::BarcodeFormat>& formats);

    // 读取并解码图像
    std::vector<ZXing::Barcode> DecodeImage(const std::string& filePath);

    // 获取解码结果文本
    std::string GetDecodedText(const ZXing::Barcode& barcode);

    // 获取解码的字节数据
    std::vector<uint8_t> GetDecodedBytes(const ZXing::Barcode& barcode);

private:
    ZXing::ReaderOptions options;
    int rotate = 0;  // 图像旋转角度
};
#endif // DECODECODEUTILITY_H