#ifndef CONFIG_H
#define CONFIG_H


#include "hgxml.h"

#ifdef __cplusplus
extern "C" {
#endif 

namespace HGMACHINE{
    #define TaskSequenceDBName "TaskSequence"
    typedef struct tagConfig{
        std::string appVer;
        std::string path;
        std::string appName;
        std::string taskSequenceName;
        std::string reagentName;
        std::string dirPath;
        std::string comPort;
        tagConfig(){
            Clear();
        }
        tagConfig(const tagConfig& obj){
            Copy(obj);
        }
        void Copy(const tagConfig& obj){
            appVer=obj.appVer;
            path=obj.path;
            taskSequenceName=obj.taskSequenceName;
            reagentName=obj.reagentName;
            dirPath=obj.dirPath;
            comPort=obj.comPort;
            appName=obj.appName;
        }
        void Clear(){
            appVer="";
            path="";
            taskSequenceName="";
            reagentName="";
            dirPath="";
            comPort="";
            appName="";
        }
        void setVersion(const std::string& version){
            if (appVer!=version) appVer=version;
        }
        void setPath(const std::string& _path){
            path=_path;
        }
        void setTaskSeqName(const std::string& taskSeqName){
            taskSequenceName=taskSeqName;
        }
        void setReagentName(const std::string& _reagentName){
            reagentName=_reagentName;
        }
        void setDirPath(const std::string& _dirPath){
            dirPath=_dirPath;
        }
        void setComPort(const std::string& _comPort){
            comPort=_comPort;
        }
        tagConfig& operator=(const tagConfig& obj){
            Copy(obj);
            return *this;
        }
        bool operator==(const tagConfig& obj) const{
            bool ret=false;
            if (appVer==obj.appVer &&
                path==obj.path &&
                reagentName==obj.reagentName &&
                taskSequenceName==obj.taskSequenceName &&
                dirPath==obj.dirPath && 
                comPort==obj.comPort && 
                appName==obj.appName)
            {
                ret=true;
            }
            return ret;
        }
        bool operator!=(const tagConfig& obj)const
        {
            return (!(*this==obj));
        }
        tagConfig* Clone() const
        {
            return (new tagConfig(*this));
        }
        bool LoadXML(tinyxml2::XMLElement* root)
        {
            if (!root) return false;
            XML_GETSTRING(appVer,"","AppVersion",root);
            XML_GETSTRING(appName,"","AppName",root);
            XML_GETSTRING(taskSequenceName,"","TaskSeqName",root);
            XML_GETSTRING(reagentName,"","ReagentName",root);
            XML_GETSTRING(dirPath,"","DirPath",root);
            XML_GETSTRING(comPort,"","ComPort",root);
            return true;
        }
        void SaveXML(tinyxml2::XMLElement* root){
            XML_SAVESTRING(appVer.c_str(),"AppVersion",root);
            XML_SAVESTRING(appName.c_str(),"AppName",root);
            XML_SAVESTRING(taskSequenceName.c_str(),"TaskSeqName",root);
            XML_SAVESTRING(reagentName.c_str(),"ReagentName",root);
            XML_SAVESTRING(dirPath.c_str(),"DirPath",root);
            XML_SAVESTRING(comPort.c_str(),"ComPort",root);
        }

    } Config,*PConfig;

    class FileConfig
    {
    public:
        FileConfig();
        ~FileConfig();

        static bool createConfigFile(std::string path,const std::string &name);
        static bool loadConfigFile(std::string path);
        static Config getConfig() {return m_config;};
        static void setDirPath(std::string path) {m_config.setDirPath(path);};
        static std::string getDirPath() {return m_config.dirPath;};
        static std::string getComPort() {return m_config.comPort;};
        static std::string getAppName() {return m_config.appName;};
        static std::string getVersion() {return m_config.appVer;};
        static void saveConfigFile(const std::string &name);
        static void setVersion(const std::string& version);

        static void setTaskSeqName(std::string name) {m_config.setTaskSeqName(name);};
        static void setReagentName(std::string name) {m_config.setReagentName(name);};
        static std::string getTaskSeqName() {return m_config.taskSequenceName;};
        static std::string getReagentName() {return m_config.reagentName;};

   
    private:
        static Config m_config;
    };
}
#ifdef __cplusplus
}
#endif
#endif