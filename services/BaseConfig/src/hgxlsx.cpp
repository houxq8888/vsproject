
#include <iostream>
#include "hgxlsx.hpp"

namespace HGMACHINE{
int readExcel(const std::string& filePath,
    std::vector<std::vector<std::string>>& data) {
    // xlsxioreader handle = xlsxioread_open(filePath.c_str());
    // if (!handle) {
    //     std::cerr << "无法打开 Excel 文件！" << std::endl;
    //     return -1;
    // }

    // xlsxioreadersheet sheet = xlsxioread_sheet_open(handle, NULL, XLSXIOREAD_SKIP_EMPTY_ROWS);
    // if (!sheet) {
    //     std::cerr << "无法打开工作表！" << std::endl;
    //     xlsxioread_close(handle);
    //     return -1;
    // }

    // while (xlsxioread_sheet_next_row(sheet))
    // {
    //     std::vector<std::string> rowData;
    //     const char *value;
    //     while ((value = xlsxioread_sheet_next_cell(sheet)) != NULL)
    //     {
    //         rowData.push_back(value);
    //         free((void *)value);
    //     }
    //     data.push_back(rowData);
    // }

    // xlsxioread_sheet_close(sheet);
    // xlsxioread_close(handle);

    XLSXIOReader *xlsxfile = new XLSXIOReader(filePath.c_str());
    XLSXIOReaderSheet *xlsxsheet = xlsxfile->OpenSheet(NULL, XLSXIOREAD_SKIP_EMPTY_ROWS);
    if (xlsxsheet)
    {
        std::string value;
        while (xlsxsheet->GetNextRow())
        {
            data.push_back(std::vector<std::string>());
            while (xlsxsheet->GetNextCellString(value)) {
                data[data.size() - 1].push_back(value);
                // printf("%s\t", value.c_str());
            }
        }
        delete xlsxsheet;
    }
    delete xlsxfile;
    return 0;
}

int writeExcel(const std::string &filePath,const std::vector<std::vector<std::string>>& data) {
    XLSXIOWriter* xlsxfile = new XLSXIOWriter(filePath.c_str());
    xlsxfile->SetRowHeight(1);
    for (int i=0;i<int(data.size());i++){
        if (i==0){
            for (int j=0;j<int(data[i].size());j++){
                xlsxfile->AddColumn(data[i][j].c_str());
            }
            xlsxfile->NextRow();
        } else {
            for (int j=0;j<int(data[i].size());j++){
                *xlsxfile << (data[i][j].c_str());
            }
            xlsxfile->NextRow();
        }
    }
    delete xlsxfile;
    std::cout << "Excel file written successfully!\n";
    return 0;
}

}