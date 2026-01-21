#include "hgcsv.h"
#include <sstream>
#include <iostream>
#include <fstream>

namespace HGMACHINE{
    int saveTableToCsv(const std::vector<std::map<std::string,std::string>> & table,
        const std::string& filename)
    {
        // 打开文件以写入
        std::ofstream file(filename,std::ios::binary);
        if (!file.is_open())
        {
            std::cerr << "无法打开文件: " << filename << std::endl;
            return -1;
        }

        // output to UTF-8 BOM
        const unsigned char bom[]={0xEF,0xBB,0xBF};
        file.write(reinterpret_cast<const char*>(bom),sizeof(bom));
        
        // 遍历 table 数据并写入 CSV 文件
        for (int i=0;i<int(table.size());i++)
        {
            for (const auto &col : table[i])
            {
                if (i==0) {
                    file << col.first;
                } else {
                    file << "\"" << col.second << "\""; // 写入每个单元格内容
                }
                file << ","; // 列之间用逗号分隔
            }
            file << "\n"; // 行之间用换行符分隔
        }

        file.close();
        std::cout << "CSV 文件已保存：" << filename << std::endl;
        return 0;
    }
}