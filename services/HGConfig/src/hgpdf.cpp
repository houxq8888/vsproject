#include "hgpdf.h"
#include <sstream>
#include <codecvt>
#include <locale>

namespace HGMACHINE{

void errorHandler(HPDF_STATUS errorNo, HPDF_STATUS detailNo, void *userData) {
    std::ostringstream ss;
    ss << "ERROR: " << errorNo << ", Detail: " << detailNo;
    printf("%s\n",ss.str().c_str());
}

int generatePDF(const std::string& filename, const std::string& content) {
    // 创建一个 PDF 文档
    HPDF_Doc pdf = HPDF_New(errorHandler, NULL);

    if (!pdf) {
        printf("Error: cannot create PDF document\n");
        return -1;
    }

    // 创建一个页面
    HPDF_Page page = HPDF_AddPage(pdf);
    
    // 获取页面尺寸
    float page_width = HPDF_Page_GetWidth(page);
    float page_height = HPDF_Page_GetHeight(page);
    
    // 设置字体
    HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", NULL);
    
    // 开始绘制文本
    HPDF_Page_BeginText(page);
    HPDF_Page_SetFontAndSize(page, font, 24);
    HPDF_Page_MoveTextPos(page, 50, page_height - 100);  // 设置文本位置
    HPDF_Page_ShowText(page, content.c_str());
    HPDF_Page_EndText(page);

    // 保存 PDF 到文件
    HPDF_SaveToFile(pdf, filename.c_str());

    // 清理资源
    HPDF_Free(pdf);

    return 0;
}
std::string utf8ToUtf16(const std::string &utf8) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring wideStr = converter.from_bytes(utf8);

    std::string utf16Str(reinterpret_cast<const char*>(wideStr.c_str()), wideStr.size() * 2);
    return utf16Str;
}
int saveTableToPdf(const std::vector<std::map<std::string, std::string>>& content,
                   const std::string& savename, const std::string& charset) {
    // 创建一个 PDF 文档
    HPDF_Doc pdf = HPDF_New(errorHandler, NULL);
    if (!pdf) {
        printf("Error: cannot create PDF document\n");
        return -1;
    }
    // **启用简体中文支持**
    HPDF_UseUTFEncodings(pdf);
    // HPDF_UseCNSEncodings(pdf);
    // HPDF_UseCNSFonts(pdf);
    // 设置页面参数
    const float PAGE_MARGIN = 50;
    const float FONT_SIZE = 12;
    const float LINE_HEIGHT = 14;

    // 创建一个页面
    HPDF_Page page = HPDF_AddPage(pdf);
    if (!page) {
        HPDF_Free(pdf);
        return -1;
    }
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

    // 获取页面尺寸
    float page_width = HPDF_Page_GetWidth(page);
    float page_height = HPDF_Page_GetHeight(page);

    // 设置字体
    // **加载 TTF 字体**
    HPDF_Font font = HPDF_GetFont(pdf, HPDF_LoadTTFontFromFile(pdf, charset.c_str(), HPDF_TRUE), 
        "StandardEncoding");
    if (!font)
    {
        // std::cerr << "加载 TTF 字体失败" << std::endl;
        HPDF_Free(pdf);
        return -1;
    }
    // HPDF_Font font = HPDF_GetFont(pdf, "SimSun", "GB-EUC-H");
    // **加载 TTF 字体**
    // HPDF_Font font = HPDF_GetTTFontDefFromFile(pdf, "./resources/SimHei.ttf", false);
    // HPDF_LoadTTFontFromFile(pdf, "./resources/SimSun.ttf", false);
    if (!font) {
        HPDF_Free(pdf);
        return -1;
    }
    HPDF_Page_SetFontAndSize(page, font, FONT_SIZE);

    float x = PAGE_MARGIN, y = page_height - PAGE_MARGIN - FONT_SIZE;

    // 开始绘制文本
    HPDF_Page_BeginText(page);

    // 遍历 table 数据并写入 PDF 文件
    for (size_t i = 0; i < content.size(); ++i) {
        if (y - LINE_HEIGHT < PAGE_MARGIN) {
            HPDF_Page_EndText(page);
            page = HPDF_AddPage(pdf);
            if (!page) {
                HPDF_Free(pdf);
                return -1;
            }
            HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
            HPDF_Page_SetFontAndSize(page, font, FONT_SIZE);
            HPDF_Page_BeginText(page);
            y = page_height - PAGE_MARGIN - FONT_SIZE;
        }

        std::ostringstream line;
        for (const auto& col : content[i]) {
            if (i == 0) {
                line << col.first << "      ";
            } else {
                line << col.second << "      ";
            }
        }
        // **转换字符串为 UTF-16**
        std::string utf16Str = /*utf8ToUtf16*/(line.str().c_str());
        HPDF_Page_TextOut(page, x, y, utf16Str.c_str());
        y -= LINE_HEIGHT;
    }

    HPDF_Page_EndText(page);

    // 保存 PDF 到文件
    if (HPDF_SaveToFile(pdf, savename.c_str()) != HPDF_OK) {
        HPDF_Free(pdf);
        return -1;
    }

    // 清理资源
    HPDF_Free(pdf);

    return 0;
}
}