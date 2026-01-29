#ifndef RWMETHODDB_H
#define RWMETHODDB_H

#include <string>
#include <map>
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif
namespace HGMACHINE{
    
    typedef struct tagMethod{
        std::string indexStr;
        std::string name;
        std::string type;
        std::string createTime;
        std::string dbName;
        std::string param;
        tagMethod(){
            Clear();
        }
        void Clear(){
            indexStr="";
            name="";
            type="";
            createTime="";
            dbName="";
            param="";
        }
        bool operator==(const tagMethod& obj) const{
            bool ret=false;
            if (indexStr==obj.indexStr &&
                name==obj.name &&
                type==obj.type &&
                createTime==obj.createTime &&
                dbName==obj.dbName && 
                param==obj.param)
            {
                ret=true;
            }
            return ret;
        }
        tagMethod(const tagMethod& obj){
            Copy(obj);
        }
        void Copy(const tagMethod& obj){
            indexStr=obj.indexStr;
            name=obj.name;
            type=obj.type;
            createTime=obj.createTime;
            dbName=obj.dbName;
            param=obj.param;
        }
        tagMethod& operator=(const tagMethod& obj){
            Copy(obj);
            return *this;
        }
        bool operator!=(const tagMethod& obj)const
        {
            return (!(*this==obj));
        }
        tagMethod* Clone() const
        {
            return (new tagMethod(*this));
        }
    } Method,*PMethod;

    #define METHODMANAGENAME "MethodInfo"
    #define MethodDBName "Method"

    class RWMethodDb {
        public:
            RWMethodDb();
            ~RWMethodDb();

            //-------------------------method ------------------------------//
            static Method readMethodParam(std::string dbName);
            static std::map<std::string,std::string> getMethodMap(int index,Method method);
            static std::vector<std::map<std::string,std::string>> getMethodParamMap(Method method);
            static std::vector<std::map<std::string,std::string>> readMethodInfo();
            static std::vector<std::string> getMethodNames();
            static void clearMethodManageRecord();
            static std::string decodeMethodDBName(const std::string &methodName);
            static std::string getMethodName(const std::string &flowName);
            static void writeMethodManageRecord(const std::map<std::string,std::string> &infoS);
            static void writeMethodRecord(std::string dbName,bool coverFlag,const std::vector<std::map<std::string,std::string>> &infoSS);


        protected:
    };
}
#ifdef __cplusplus
}
#endif
#endif // RWMETHODDB_H