#ifndef HGSAVEDATATODB_H
#define HGSAVEDATATODB_H

/*!
\mainpage HGSaveDataToDB 文档

## 📌 项目简介
HGSaveDataToDB 是一个用于将数据保存到数据库的模块，支持 SQLite，MySQL 等数据库驱动，具备线程安全、事务支持、批处理等特性。

---

## 📚 文档版本说明

| 版本号   | 日期       | 作者      | 说明                                                         |
|--------|------------|----------|--------------------------------------------------------------|
| v1.0.0 | 2025-06-30 | Xiaoqin Hou | 🎉 初始版本。生成了基础的 API 文档，涵盖全部 .h/.cpp 接口说明 |


---
*/
#include "HGSaveService_global.h"
#include <map>
#include <vector>
#include "SQLite.h"

#ifdef __cplusplus
extern "C" {
#endif

namespace HGMACHINE {

/**
 * @brief 提供 SQLite 数据库的封装操作类，包括打开、关闭、读写表格等功能。
 */
class HGSaveDataToDB
{
public:
    /**
     * @brief 构造函数，初始化数据库操作类。
     */
    HGSaveDataToDB();
    /**
     * @brief 析构函数，关闭数据库连接并释放资源。
     */
    ~HGSaveDataToDB();
public:
    /**
     * @brief 打开指定数据库文件。
     * @param dbName 数据库文件名。
     */
    void openDB(const std::string &dbName);
    /**
     * @brief 关闭当前数据库连接。
     */
    void closeDB();
    /**
     * @brief 获取表中指定 key 对应的 value。
     * @param tableName 表名。
     * @param key 要查询的键。
     * @param value 返回的值。
     * @return 是否查询成功。
     */  
    bool getStr(std::string tableName,std::string key,std::string &value);
    /**
     * @brief 设置表中 key 对应的 value（存在则更新，不存在则插入）。
     * @param tableName 表名。
     * @param key 要设置的键。
     * @param value 要设置的值。
     * @return 是否设置成功。
     */
    bool setStr(std::string tableName,std::string key,const std::string value);
    /**
     * @brief 执行任意 SQL 写入语句。
     * @param sql SQL 语句（如 INSERT、UPDATE、DELETE）。
     * @return 是否执行成功。
     */
    bool writeData(std::string sql);
    /**
     * @brief 执行 SELECT 查询并获取结果。
     * @param sql SELECT 查询语句。
     * @param infos 查询结果，每条记录为一个 map。
     * @return 是否查询成功。
     */
    bool readData(std::string sql,std::vector<std::map<std::string,std::string>> &infos);
    /**
     * @brief 清空指定表中的所有数据。
     * @param tableName 表名。
     */
    void clearDB(std::string tableName);

    /**
     * @brief 判断表是否存在。
     * @param tableName 表名。
     * @return 表存在返回 true，否则 false。
     */
    bool isTableExist(std::string tableName);

    /**
     * @brief 获取指定表中的记录数。
     * @param tableName 表名。
     * @return 表中的记录数。
     */
    int countOfTable(std::string tableName);

    /**
     * @brief 获取指定表中满足条件的记录数。
     * @param tableName 表名。
     * @param whereClause WHERE 子句条件。
     * @return 满足条件的记录数。
     */
    int countOfTable(std::string tableName, const std::string &whereClause);

    /**
     * @brief 获取当前数据库中的所有表名。
     * @return 表名列表。
     */
    std::vector<std::string> getAllTables();
    /**
     * @brief 创建新表。
     * @param tableName 表名。
     * @param keyName 主键字段名。
     * @param infoS 字段名及字段类型的映射（如 {"name", "TEXT"}）。
     */
    void createDB(std::string tableName,
                  std::string keyName,
                  const std::map<std::string,std::string> &infoS);

    /**
     * @brief 删除指定表。
     * @param tableName 表名。
     */
    void deleteDB(const std::string& tableName);

    /**
     * @brief 插入一条记录到表中。
     * @param tableName 表名。
     * @param infoS 字段名及值的映射。
     */
    void recordSingleInfo(std::string tableName, const std::map<std::string,std::string> &infoS);

    /**
     * @brief 从表中读取一条记录。
     * @param tableName 表名。
     * @param infoS 返回的数据映射。
     */
    void readSingleInfo(std::string tableName, std::map<std::string,std::string> &infoS);

    /**
     * @brief 向多主键表写入数据（存在则更新，不存在则插入）。
     * @param tableName 表名。
     * @param keyNames 主键字段名列表。
     * @param infoS 字段名及值的映射。
     */
    void writeDataWithMultiKey(std::string tableName,
                                std::vector<std::string> keyNames,
                                const std::map<std::string,std::string> &infoS);

    /**
     * @brief 向表中写入记录。
     * @param tableName 表名。
     * @param keyName 主键字段名。
     * @param infoS 字段名及值的映射。
     */
    void writeRecord(std::string tableName,
                     std::string keyName,
                     const std::map<std::string,std::string> &infoS);

    /**
     * @brief 读取满足条件的多条记录。
     * @param tableName 表名。
     * @param infoS 查询条件（字段名及值）。
     * @return 满足条件的记录列表。
     */
    std::vector<std::map<std::string,std::string>> readRecord(std::string tableName,
                                                              std::map<std::string,std::string> &infoS);

    /**
     * @brief 读取满足 WHERE 子句条件的多条记录。
     * @param tableName 表名。
     * @param infoS 查询字段映射。
     * @param whereClause WHERE 子句条件。
     * @return 满足条件的记录列表。
     */
    std::vector<std::map<std::string,std::string>> readRecord(std::string tableName,
                                                              std::map<std::string,std::string> &infoS,
                                                              const std::string &whereClause);

    /**
     * @brief 读取满足 WHERE 子句条件的多条记录（带 LIMIT 限制）。
     * @param tableName 表名。
     * @param infoS 查询字段映射。
     * @param whereClause WHERE 子句条件。
     * @param limit 返回记录的最大数量。
     * @return 满足条件的记录列表。
     */
    std::vector<std::map<std::string,std::string>> readRecordWithLimit(std::string tableName,
                                                              std::map<std::string,std::string> &infoS,
                                                              const std::string &whereClause,
                                                              int limit);

    /**
     * @brief 删除表中满足条件的记录。
     * @param tableName 表名。
     * @param deleteKey 条件字段名。
     * @param deleteValue 条件字段值。
     */
    void deleteRecord(std::string tableName, std::string deleteKey, std::string deleteValue);

    /**
     * @brief 将指定数据库中的表复制到当前数据库。
     * @param sourceDName 源数据库文件名。
     * @param sourceTableName 源表名。
     * @return 是否复制成功。
     */
    bool copyTable(const std::string& sourceDName, const std::string& sourceTableName);

    /**
     * @brief 向已有表中添加一个新字段。
     * @param tableName 表名。
     * @param columnName 新字段名。
     * @return 是否添加成功。
     */
    bool addColumnToTable(const std::string& tableName, const std::string& columnName);

    /**
     * @brief 从表中删除字段（重建表并迁移数据）。
     * @param tableName 表名。
     * @param columnToRemove 要移除的字段名。
     * @return 是否删除成功。
     */
    bool removeColumnFromTable(const std::string& tableName, const std::string& columnToRemove);

    /** 
     * @brief 获取表中的字段信息。
     * @param tableName 表名。
     * @return 字段信息列表。 name-type-pk
     */
    std::vector<std::map<std::string,std::string>> getTableInfo(const std::string &tableName);

    void sortTable(const std::string& tableName,const std::string& sortKey);
private:
    CSQLiteDB m_db;
    std::string m_dbName;
};
}
#ifdef __cplusplus
}
#endif
#endif // HGSAVEDATATODB_H
