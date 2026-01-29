#ifndef ISVC_FRAME_H
#define ISVC_FRAME_H

#include <string>
namespace ServiceInterfaces {
    typedef struct tagUserInfoS{
    std::string userno;
    std::string username;
    std::string userpwd;
    std::string authority;
    std::string department;
    tagUserInfoS(){
        username="";
        userpwd="";
        authority="";
        department="";
        userno="";
    }
    tagUserInfoS(std::string _name,
        std::string pwd,
        std::string _authority,
        std::string _depart,
        std::string _userno)
    {
        username=_name;
        userpwd=pwd;
        authority=_authority;
        department=_depart;
        userno=_userno;
    }
    bool operator==(const tagUserInfoS& obj) const{
        bool ret=false;
        if (username==obj.username &&
            userpwd==obj.userpwd &&
            authority==obj.authority &&
            department==obj.department &&
            userno==obj.userno)
        {
            ret=true;
        }
        return ret;
    }
    tagUserInfoS(const tagUserInfoS& obj){
        Copy(obj);
    }
    void Copy(const tagUserInfoS& obj){
        username=obj.username;
        userpwd=obj.userpwd;
        authority=obj.authority;
        department=obj.department;
        userno=obj.userno;
    }
    tagUserInfoS& operator=(const tagUserInfoS& obj){
        Copy(obj);
        return *this;
    }
    bool operator!=(const tagUserInfoS& obj)const
    {
        return (!(*this==obj));
    }
    tagUserInfoS* Clone() const
    {
        return (new tagUserInfoS(*this));
    }
} UserInfoS, *PUserInfoS;
}
class ISvcFrame {
public:
    virtual ~ISvcFrame() = default;

    virtual std::string ReadCurDirPath() = 0;
    virtual bool loadConfig(const std::string &curPath) = 0;
    virtual void saveConfig() = 0;
    virtual std::string getAppName() = 0;
    virtual std::string GetDirPath() const = 0;
    virtual uint16_t getScannerVendorID() = 0;
    virtual uint16_t getScannerProductID() = 0;
    
};

#endif // ISVC_FRAME_H
