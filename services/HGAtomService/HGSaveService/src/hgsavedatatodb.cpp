// The functions contained in this file are pretty dummy
// and are included only as a placeholder. Nevertheless,
// they *will* get included in the static library if you
// don't remove them :)
//
// Obviously, you 'll have to write yourself the super-duper
// functions to include in the resulting library...
// Also, it's not necessary to write every function in this file.
// Feel free to add more files in this project. They will be
// included in the resulting library.

// A function adding two integers and returning the result

/*
    @file   hgsavedatatodb.cpp
    @brief  Implementation file
    @author XiaoQin.Hou
*/

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include "hgsavedatatodb.h"

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#else
#include "hglog4cplus.h"
#include <boost/lexical_cast.hpp>
#endif


namespace HGMACHINE {

// bool HGSaveDataToDB::getInt(std::string key,int *value)
// {
//     std::string szVal = "";
//     if ( getStr("",key,szVal) ) {
//         try {
//              if ( szVal.size() ) {
//                 *value = boost::lexical_cast<int>(szVal);
//                 return true;
//              }
//         } catch(boost::bad_lexical_cast& e) {
//             // bad lexical cast: source type value could not be interpreted as target
//             std::cout<< "database get int failure:" << e.what() << std::endl;
//         }
//     }
//     return false;
// }
HGSaveDataToDB::HGSaveDataToDB(){}
void HGSaveDataToDB::openDB(const std::string &dbName)
{
    m_dbName = dbName;
    try {
        m_db.open(dbName.c_str());
    } catch (const std::exception& e){
        std::cerr<<"Failed to open db:"<<dbName<<std::endl;
        throw;
    }
}
HGSaveDataToDB::~HGSaveDataToDB(){
    closeDB();
}
void HGSaveDataToDB::closeDB()
{
    // if (m_db.is_open())
    {
        m_db.close();
    }
}
bool HGSaveDataToDB::getStr(std::string tableName,std::string key,std::string &value)
{
    try
    {
        if( m_db.tableExists(tableName.c_str()) == true ) /* 仍没有建立该表 */
        {
           std::ostringstream sql;
           sql<<"SELECT * FROM["<<tableName<<"] WHERE keyname='"<<key<<"'";
            CSQLiteQuery query = m_db.execQuery(sql.str().c_str());
            if(!query.eof())
            {
                value = query.getStringField("keyvalue");
                query.finalize();
                return true;
            }
            query.finalize();
        }
    } catch (CSQLiteException &e)
    {
       // return ERR_FALSE;
    }
    return false;
}

// bool HGSaveDataToDB::setInt(std::string key,int value)
// {
//     try {
//         std::string szVal = boost::lexical_cast<std::string>(value);
//          if ( szVal.size() ) {
//             return setStr("",key,szVal);
//          }
//     } catch(boost::bad_lexical_cast& e) {
//         // bad lexical cast: source type value could not be interpreted as target
//         std::cout<< "database set int failure:" << e.what() << std::endl;
//     }
//     return false;
// }
bool HGSaveDataToDB::isTableExist(std::string tableName){
    try {
        if( m_db.tableExists(tableName.c_str()) == false ) /* 仍没有建立该表 */
        {
            return false;
        }
    } catch (CSQLiteException &e) {
        return false;
    }
    return true;
}
void HGSaveDataToDB::clearDB(std::string tableName){
    try {
        std::ostringstream sql;
        sql<<"DELETE FROM "<<tableName;
        m_db.execDML(sql.str().c_str());
    } catch(CSQLiteException &e){
        return;
    }
}
std::vector<std::string> HGSaveDataToDB::getAllTables(){
    std::vector<std::string> names;
    try{
        std::ostringstream str;
        str<<"SELECT name FROM "<<"sqlite_master"<<" WHERE type='table';";
        names=m_db.getAllTables(str.str().c_str());
        #ifndef _WIN32
        sync();
        #endif
    }catch (CSQLiteException &e) {
        return names;
    }
    return names;
}
int HGSaveDataToDB::countOfTable(std::string tableName)
{
    int count=0;
    try {
        std::ostringstream str;
        str<<"SELECT * FROM "<<tableName<<";";
        CSQLiteTable table=m_db.getTable(str.str().c_str());
        count=table.numRows();
        #ifndef _WIN32
        sync();
        #endif
    } catch (CSQLiteException &e) {
        return 0;
    }
    return count;
}

int HGSaveDataToDB::countOfTable(std::string tableName, const std::string &whereClause)
{
    int count=0;
    try {
        std::ostringstream str;
        str<<"SELECT COUNT(*) FROM "<<tableName;
        if (!whereClause.empty()) {
            str<<" WHERE "<<whereClause;
        }
        str<<";";
        CSQLiteQuery query=m_db.execQuery(str.str().c_str());
        if (!query.eof()) {
            count=query.getIntField(0);
        }
        query.finalize();
        #ifndef _WIN32
        sync();
        #endif
    } catch (CSQLiteException &e) {
        return 0;
    }
    return count;
}
bool HGSaveDataToDB::writeData(std::string sql){
    // printf("writeData sql:%s\n",sql.c_str());
    try {
        int ret=m_db.execDML(sql.c_str());
        // printf("exec DML ret:%d\n",ret);
        #ifndef _WIN32
        sync();
        #endif
    } catch (CSQLiteException &e) {
        printf("writeData failed, %s\n",e.errorMessage());
        return false;
    }
    return true;
}
void HGSaveDataToDB::sortTable(const std::string& tableName, const std::string& sortKey) {
    return;
    try {
        // 1. 获取原表的完整结构（包括索引、约束等）
        std::string getTableSchemaSql = 
            "SELECT sql FROM sqlite_master WHERE type='table' AND name='" + tableName + "';";
        CSQLiteQuery schemaQuery = m_db.execQuery(getTableSchemaSql.c_str());
        std::string tableSchema = schemaQuery.eof() ? "" : schemaQuery.getStringField(0);
        schemaQuery.finalize();

        if (tableSchema.empty()) {
            printf("Error: Table %s not found!\n", tableName.c_str());
            return;
        }

        // 2. 创建临时表（与原表结构一致）
        std::string createTempTableSql = "CREATE TABLE temp_sorted AS SELECT * FROM " + tableName + " LIMIT 0;";
        m_db.execDML(createTempTableSql.c_str());

        // 3. 插入数据到临时表（按 sortKey 排序）
        std::string insertDataSql = 
            "INSERT INTO temp_sorted SELECT * FROM " + tableName + " ORDER BY " + sortKey + ";";

        // 4. 删除原表
        std::string dropOriginalTableSql = "DROP TABLE " + tableName + ";";

        // 5. 重命名临时表
        std::string renameTableSql = "ALTER TABLE temp_sorted RENAME TO " + tableName + ";";

        // 6. 执行事务（确保原子性）
        m_db.execDML("BEGIN;");
        m_db.execDML(createTempTableSql.c_str());
        m_db.execDML(insertDataSql.c_str());
        m_db.execDML(dropOriginalTableSql.c_str());
        m_db.execDML(renameTableSql.c_str());
        m_db.execDML("COMMIT;");

        // printf("sortTable success: %s physically reordered by %s.\n", tableName.c_str(), sortKey.c_str());

    } catch (CSQLiteException &e) {
        m_db.execDML("ROLLBACK;");
        printf("sortTable failed: %s\n", e.errorMessage());
    }
}
bool HGSaveDataToDB::readData(std::string sql,std::vector<std::map<std::string,std::string>> &infos){
    try {
        CSQLiteQuery query=m_db.execQuery(sql.c_str());
        int numField=query.numFields();
        while (!query.eof()){
            std::map<std::string,std::string> info;
            for (int i=0;i<numField;i++){
                if (query.fieldIsNull(i)) {
                    info[query.fieldName(i)]="";
                    continue;
                }
                info[query.fieldName(i)]=(query.fieldValue(i));
            }
            infos.push_back(info);
            query.nextRow();
        }
            
        query.finalize();
        #ifndef _WIN32
        sync();
        #endif
    } catch (CSQLiteException &e) {
        return false;
    }

    return true;
}
bool HGSaveDataToDB::copyTable(const std::string& sourceDName,
    const std::string& sourceTableName){
    CSQLiteDB sourceDB;
    sqlite3_stmt* stmt = nullptr;
    int rc;

    // 打开源数据库
    try{
        sourceDB.open(sourceDName.c_str());
    } catch (const std::exception& e){
        std::cerr<<"Failed to open db:"<<sourceDName<<std::endl;
        return false;
    }

    // 获取源表的结构
    std::string getTableStructure = "SELECT sql FROM sqlite_master WHERE type='table' AND name='" + sourceTableName + "';";
    rc = sqlite3_prepare_v2(sourceDB.getDB(), getTableStructure.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to fetch table structure: " << sqlite3_errmsg(sourceDB.getDB()) << std::endl;
        sourceDB.close();
        return false;
    }

    std::string createTableSQL;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        createTableSQL = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    }
    sqlite3_finalize(stmt);

    if (createTableSQL.empty()) {
        std::cerr << "Table " << sourceTableName << " not found in source database." << std::endl;
        sourceDB.close();
        return false;
    }

    // 在目标数据库中创建表
    rc = sqlite3_exec(m_db.getDB(), createTableSQL.c_str(), nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to create table in target database: " << sqlite3_errmsg(m_db.getDB()) << std::endl;
        sourceDB.close();
        return false;
    }

    // 复制数据
    std::string copyDataSQL = "SELECT * FROM " + sourceTableName;
    rc = sqlite3_prepare_v2(sourceDB.getDB(), copyDataSQL.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to fetch data from source table: " << sqlite3_errmsg(sourceDB.getDB()) << std::endl;
        sourceDB.close();
        return false;
    }

    // 构建插入语句
    std::string insertSQL = "INSERT INTO " + sourceTableName + " VALUES (";
    int colCount = sqlite3_column_count(stmt);
    for (int i = 0; i < colCount; ++i) {
        insertSQL += (i == 0 ? "?" : ", ?");
    }
    insertSQL += ");";

    sqlite3_stmt* insertStmt = nullptr;
    rc = sqlite3_prepare_v2(m_db.getDB(), insertSQL.c_str(), -1, &insertStmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare insert statement: " << sqlite3_errmsg(m_db.getDB()) << std::endl;
        sqlite3_finalize(stmt);
        sourceDB.close();
        return false;
    }

    // 插入数据
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        for (int i = 0; i < colCount; ++i) {
            sqlite3_bind_value(insertStmt, i + 1, sqlite3_column_value(stmt, i));
        }
        rc = sqlite3_step(insertStmt);
        if (rc != SQLITE_DONE) {
            std::cerr << "Failed to insert row into target table: " << sqlite3_errmsg(m_db.getDB()) << std::endl;
        }
        sqlite3_reset(insertStmt);
    }

    // 清理
    sqlite3_finalize(stmt);
    sqlite3_finalize(insertStmt);
    sourceDB.close();
    return true;
}

bool HGSaveDataToDB::setStr(std::string tableName,std::string key,const std::string value)
{
    try {
        std::ostringstream sql;
        if( m_db.tableExists(tableName.c_str()) == false ) /* 仍没有建立该表 */
        {
            sql<<"CREATE TABLE["<<tableName<<"] ([keyname] varchar(128) unique,[keyvalue] varchar(128))";
            m_db.execDML(sql.str().c_str());
        }
        sql.str("");
        sql<<"INSERT OR REPLACE INTO ["<<tableName<<"] ([keyname],[keyvalue]) VALUES('"<<key<<"','"<<value<<"')";
        m_db.execDML(sql.str().c_str());
        #ifndef _WIN32
        sync();
        #endif
    } catch (CSQLiteException &e) {
        return false;
    }
    return true;
}

void HGSaveDataToDB::createDB(std::string tableName,
    std::string keyName,
    const std::map<std::string,std::string> &infoS)
{
    std::ostringstream sql;
    if (!isTableExist(tableName)){
        //create
        sql<<"CREATE TABLE IF NOT EXISTS "<<tableName<<" (";
        for (auto info:infoS){
            if (info.first==keyName) sql<<info.first<<" TEXT PRIMARY KEY,";
            else sql<<info.first<<" TEXT,";
        }
        std::string sqlstr= sql.str().substr(0,sql.str().find_last_of(","));
        sql.str("");
        sql<<sqlstr<<");";

        writeData(sql.str());
    } 
}
void HGSaveDataToDB::deleteDB(const std::string &tableName){
    try {
        std::ostringstream sql;
        if( m_db.tableExists(tableName.c_str()) == true ) /* 仍没有建立该表 */
        {
            sql<<"DROP TABLE IF EXISTS "<<tableName<<";";
            m_db.execDML(sql.str().c_str());
        }
        #ifndef _WIN32
        sync();
        #endif
    } catch (CSQLiteException &e) {
        return ;
    }
}
void HGSaveDataToDB::writeDataWithMultiKey(std::string tableName,
    std::vector<std::string> keyNames,const std::map<std::string,std::string> &infoS)
{
    std::ostringstream sql;
    if (!isTableExist(tableName)){
        //create
        sql<<"CREATE TABLE IF NOT EXISTS "<<tableName<<" (";
        for (auto info:infoS)
            sql << info.first << " TEXT,";
        sql << "PRIMARY KEY (";
        for (auto key: keyNames)
            sql << key << ",";
        std::string sqlstr= sql.str().substr(0,sql.str().find_last_of(","));
        sql.str("");
        sql<<sqlstr<<"));";
        {
            // printf("sql:%s\n",sql.str().c_str());
        }
        writeData(sql.str());
    } 

    sql.str("");
    std::ostringstream valueStr;
    sql<<"INSERT OR REPLACE INTO "<<tableName<<" (";

    valueStr<<") VALUES (";
    for (auto info:infoS){
        sql<<info.first<<",";
        valueStr<<"'"<<info.second<<"',";
    }
    std::string sqlstr= sql.str().substr(0,sql.str().find_last_of(","));
    std::string valuestr=valueStr.str().substr(0,valueStr.str().find_last_of(","));
    sql.str("");
    sql<<sqlstr<<valuestr<<");";
    
    {
        // printf("sql:%s\n",sql.str().c_str());
    }
    writeData(sql.str());
}
void HGSaveDataToDB::writeRecord(std::string tableName,
                std::string keyName,
                const std::map<std::string,std::string> &infoS){
    std::ostringstream sql;
    if (!isTableExist(tableName)){
        //create
        sql<<"CREATE TABLE IF NOT EXISTS "<<tableName<<" (";
        for (auto info:infoS){
            if (info.first==keyName) sql<<info.first<<" TEXT PRIMARY KEY,";
            else sql<<info.first<<" TEXT,";
        }
        std::string sqlstr= sql.str().substr(0,sql.str().find_last_of(","));
        sql.str("");
        sql<<sqlstr<<");";
        {
            // printf("sql:%s\n",sql.str().c_str());
        }
        bool flag=writeData(sql.str());
        if (!flag) printf("create %s fail\n",tableName.c_str());
        // else printf("create %s success\n",tableName.c_str());
    } 
    sql.str("");
    std::ostringstream valueStr;
    sql<<"INSERT OR REPLACE INTO "<<tableName<<" (";

    valueStr<<") VALUES (";
    for (auto info:infoS){
        sql<<info.first<<",";
        valueStr<<"'"<<info.second<<"',";
    }
    std::string sqlstr= sql.str().substr(0,sql.str().find_last_of(","));
    std::string valuestr=valueStr.str().substr(0,valueStr.str().find_last_of(","));
    sql.str("");
    sql<<sqlstr<<valuestr<<");";
    
    {
        // printf("sql:%s\n",sql.str().c_str());
    }
    writeData(sql.str());
}
std::vector<std::map<std::string,std::string>> HGSaveDataToDB::readRecord(std::string tableName, std::map<std::string,std::string> &infoS){
    std::vector<std::map<std::string,std::string>> pkCols = getTableInfo(tableName);
    std::string keyname="";
    for (int i=0;i<int(pkCols.size());i++) {
        // printf("%s %s %s\n",pkCols[i]["name"].c_str(),pkCols[i]["pk"].c_str(),pkCols[i]["type"].c_str());
        if (pkCols[i]["pk"] == "1"){
            keyname=pkCols[i]["name"];
        }
    }

    std::vector<std::map<std::string,std::string>> infos;
    std::ostringstream sql;
    sql<<"SELECT ";
    for (auto info:infoS){
        sql<<info.first<<",";
    }
    std::string sqlstr= sql.str().substr(0,sql.str().find_last_of(","));
    sql.str("");
    sql<<sqlstr<<" FROM "<<tableName<<" ORDER BY CAST("+keyname+" AS INTEGER);";
    // printf("sql:%s\n",sql.str().c_str());
    if (!readData(sql.str(),infos))
    {
        sql.str("");
        sql<<HGSAVESERVICENAME<<"get str failed";
        printf("%s\n",sql.str().c_str());
    } 
    return infos;
}

std::vector<std::map<std::string,std::string>> HGSaveDataToDB::readRecord(std::string tableName,
                                                                         std::map<std::string,std::string> &infoS,
                                                                         const std::string &whereClause){
    std::vector<std::map<std::string,std::string>> infos;
    std::ostringstream sql;
    sql<<"SELECT ";
    for (auto info:infoS){
        sql<<info.first<<",";
    }
    std::string sqlstr= sql.str().substr(0,sql.str().find_last_of(","));
    sql.str("");
    sql<<sqlstr<<" FROM "<<tableName;
    if (!whereClause.empty()) {
        sql<<" WHERE "<<whereClause;
    }
    sql<<" ORDER BY Time DESC;";
    if (!readData(sql.str(),infos))
    {
        sql.str("");
        sql<<HGSAVESERVICENAME<<"get str failed";
        printf("%s\n",sql.str().c_str());
    } 
    return infos;
}

std::vector<std::map<std::string,std::string>> HGSaveDataToDB::readRecordWithLimit(std::string tableName,
                                                                         std::map<std::string,std::string> &infoS,
                                                                         const std::string &whereClause,
                                                                         int limit){
    std::vector<std::map<std::string,std::string>> infos;
    std::ostringstream sql;
    sql<<"SELECT ";
    for (auto info:infoS){
        sql<<info.first<<",";
    }
    std::string sqlstr= sql.str().substr(0,sql.str().find_last_of(","));
    sql.str("");
    sql<<sqlstr<<" FROM "<<tableName;
    if (!whereClause.empty()) {
        sql<<" WHERE "<<whereClause;
    }
    sql<<" ORDER BY Time DESC LIMIT "<<limit<<";";
    if (!readData(sql.str(),infos))
    {
        sql.str("");
        sql<<HGSAVESERVICENAME<<"get str failed";
        printf("%s\n",sql.str().c_str());
    } 
    return infos;
}

void HGSaveDataToDB::deleteRecord(std::string tableName,std::string deleteKey,std::string deleteValue){
    std::ostringstream sql;
    sql<<"DELETE FROM "<<tableName<<" WHERE "<<deleteKey<<" == '"<<deleteValue<<"';";
    writeData(sql.str());
}
void HGSaveDataToDB::recordSingleInfo(std::string tableName,const std::map<std::string,std::string> &infoS)
{
    for (auto info:infoS){
        setStr(tableName,info.first,info.second);
    }
}
void HGSaveDataToDB::readSingleInfo(std::string tableName,std::map<std::string,std::string> &infoS){ 
    for (auto info:infoS){
        getStr(tableName,info.first,info.second);
        infoS[info.first]=info.second;
    }
}
bool HGSaveDataToDB::addColumnToTable(const std::string& tableName, const std::string& columnName) {
    std::string sql = "ALTER TABLE " + tableName + " ADD COLUMN " + columnName + " TEXT" + ";";
    return writeData(sql);
}
std::vector<std::map<std::string,std::string>> HGSaveDataToDB::getTableInfo(const std::string &tableName)
{
    std::vector<std::map<std::string,std::string>> recordInfos;
    // 第一步：获取所有字段
    std::string pragmaSql = "PRAGMA table_info(" + tableName + ");";
    std::vector<std::map<std::string, std::string>> columns;
    if (!readData(pragmaSql, columns))
    {
        std::cerr << "Failed to get table info for: " << tableName << std::endl;
        return recordInfos;
    }

    std::vector<std::string> columnNames;
    for (auto &col : columns)
    {
        std::map<std::string, std::string> colInfo;
        colInfo["name"] = col["name"];
        colInfo["type"] = col["type"];
        colInfo["pk"] = col["pk"];
        recordInfos.push_back(colInfo);
    }

    return recordInfos;
}
bool HGSaveDataToDB::removeColumnFromTable(const std::string& tableName, const std::string& columnToRemove) {
    try {
        // 第一步：获取所有字段
        std::string pragmaSql = "PRAGMA table_info(" + tableName + ");";
        std::vector<std::map<std::string, std::string>> columns;
        if (!readData(pragmaSql, columns)) {
            std::cerr << "Failed to get table info for: " << tableName << std::endl;
            return false;
        }

        std::vector<std::string> columnNames;
        for (auto &col : columns) {
            std::string name = col["name"];
            if (name != columnToRemove) {
                columnNames.push_back(name);
            }
        }

        if (columnNames.empty()) {
            std::cerr << "No columns left after removing " << columnToRemove << std::endl;
            return false;
        }

        // 第二步：创建临时表
        std::ostringstream createSql;
        createSql << "CREATE TABLE temp_" << tableName << " AS SELECT ";
        for (size_t i = 0; i < columnNames.size(); ++i) {
            createSql << columnNames[i];
            if (i != columnNames.size() - 1)
                createSql << ", ";
        }
        createSql << " FROM " << tableName << ";";

        if (!writeData(createSql.str())) {
            std::cerr << "Failed to create temporary table" << std::endl;
            return false;
        }

        // 第三步：删除原表
        std::ostringstream dropSql;
        dropSql << "DROP TABLE " << tableName << ";";
        if (!writeData(dropSql.str())) {
            std::cerr << "Failed to drop original table" << std::endl;
            return false;
        }

        // 第四步：重命名临时表
        std::ostringstream renameSql;
        renameSql << "ALTER TABLE temp_" << tableName << " RENAME TO " << tableName << ";";
        if (!writeData(renameSql.str())) {
            std::cerr << "Failed to rename temporary table" << std::endl;
            return false;
        }

        return true;

    } catch (...) {
        std::cerr << "Exception occurred in removeColumnFromTable" << std::endl;
        return false;
    }
}

}
