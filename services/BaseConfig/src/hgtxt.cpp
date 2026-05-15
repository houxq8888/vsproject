#include "hgtxt.h"
#include <sstream>
#include <iostream>
#include <fstream>

namespace HGMACHINE{


    int saveTableToTxt(const std::vector<std::map<std::string,std::string>> & content,
        const std::string& filename)
    {
        // 打开文件以写入
        std::ofstream file(filename);
        if (!file.is_open())
        {
            std::cerr << "无法打开文件: " << filename << std::endl;
            return -1;
        }

        // 遍历 table 数据并写入文件
        for (int i=0;i<int(content.size());i++)
        {
            for (const auto &col : content[i])
            {
                if (i==0) {
                    file << col.first;
                } else {
                    file << col.second; // 写入每个单元格内容
                }
                file << "\t"; // 列之间用制表符分隔
            }
            file << "\n"; // 行之间用换行符分隔
        }

        file.close();
        std::cout << "TXT 文件已保存：" << filename << std::endl;
        return 0;
    }
}