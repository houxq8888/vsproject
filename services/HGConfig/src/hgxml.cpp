#include "hgxml.h"
#include <sstream>
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <windows.h>
#endif

namespace HGMACHINE
{

void XML_GETSTRING(string &value,string defaultValue,
                    string nodeName, XMLElePtr node)
{
    if(NULL!=node)
    {
        XMLElePtr pXmlChildNode;
        pXmlChildNode=node->FirstChildElement(nodeName.c_str());
        if(pXmlChildNode)
        {
            try {
                const char* text = pXmlChildNode->GetText();
                if (text) {
                    value=text;
                }
            } catch (...) {
                value = "";
            }
        }
        else
            value=defaultValue;
    }
    else
        value=defaultValue;
}

void XML_GETINT(int &value,int defaultValue,string nodeName,XMLElePtr node)
{
    if(NULL!=node)
    {
        XMLElePtr pXmlChildNode;
        pXmlChildNode=node->FirstChildElement(nodeName.c_str());
        if(pXmlChildNode) {
            const char* text = pXmlChildNode->GetText();
            if (text) {
                value=atoi(text);
            }
        }
        else
            value=defaultValue;
    }
    else
        value=defaultValue;
}
void XML_GETINT(unsigned int &value,unsigned int defaultValue,string nodeName,XMLElePtr node)
{
    if(NULL!=node)
    {
        XMLElePtr pXmlChildNode;
        pXmlChildNode=node->FirstChildElement(nodeName.c_str());
        if(pXmlChildNode){
            const char* text = pXmlChildNode->GetText();
            if (text) {
                value=atoi(text);
            }
        }
        else
            value=defaultValue;
    }
    else
        value=defaultValue;
}
void XML_GETDOUBLE(double &value,double defaultValue,string nodeName,XMLElePtr node)
{
    if(NULL!=node)
    {
        XMLElePtr pXmlChildNode;
        pXmlChildNode=node->FirstChildElement(nodeName.c_str());
        if(pXmlChildNode){
            const char* text = pXmlChildNode->GetText();
            if (text) {
                value=atof(text);
            }
        }
        else
            value=defaultValue;
    }
    else
        value=defaultValue;
}
void XML_SAVESTRING(const char* value,const char* key,XMLElePtr node)
{
    XMLElePtr Enode = node->InsertNewChildElement(key);
    Enode->SetText(value);
}
void XML_SAVEINT(int value, const char* key,XMLElePtr node)
{
    XMLElePtr Enode = node->InsertNewChildElement(key);
    Enode->SetText(std::to_string(value).c_str());
}
void XML_SAVEFLOAT(float value,const char* key,XMLElePtr node)
{
    XMLElePtr Enode = node->InsertNewChildElement(key);
    Enode->SetText(std::to_string(value).c_str());
}
void XML_SAVEDOUBLE(double value,const char* key, XMLElePtr node)
{
    XMLElePtr Enode=node->InsertNewChildElement(key);
    Enode->SetText(std::to_string(value).c_str());
}
void XML_SAVEDOUBLE4(double value1,double value2,double value3,double value4,const char* key,XMLElePtr node)
{
    std::ostringstream buff;
    buff<<value1<<","<<value2<<","<<value3<<","<<value4;
    XMLElePtr Enode = node->InsertNewChildElement(key);
    Enode->SetText(buff.str().c_str());
}
void XML_SAVEFLOAT4(float value1,float value2,float value3,float value4,const char* key,XMLElePtr node)
{
    std::ostringstream buff;
    buff<<value1<<","<<value2<<","<<value3<<","<<value4;
    XMLElePtr Enode = node->InsertNewChildElement(key);
    Enode->SetText(buff.str().c_str());
}
void XML_SAVEDOUBLE2(double value1,double value2,const char* key,XMLElePtr node)
{
    std::ostringstream buff;
    buff<<value1<<","<<value2;
    XMLElePtr Enode = node->InsertNewChildElement(key);
    Enode->SetText(buff.str().c_str());
}
void XML_SAVEFLOAT2(float value1,float value2,const char* key,XMLElePtr node)
{
    std::ostringstream buff;
    buff<<value1<<","<<value2;
    XMLElePtr Enode = node->InsertNewChildElement(key);
    Enode->SetText(buff.str().c_str());
}
void XML_SAVEINT2(int value1,int value2,const char* key,XMLElePtr node)
{
    std::ostringstream buff;
    buff<<value1<<","<<value2;
    XMLElePtr Enode = node->InsertNewChildElement(key);
    Enode->SetText(buff.str().c_str());
}
void XML_SAVEDOUBLE7INT1(int value1,int value2,int value3,int value4,int value5,int value6,
                        double value7,int value8,const char* key,XMLElePtr node)
{
    std::ostringstream buff;
    buff<<value1<<","<<value2<<","<<value3<<","<<value4<<","<<value5<<","<<value6<<","<<value7<<","<<value8;
    XMLElePtr Enode = node->InsertNewChildElement(key);
    Enode->SetText(buff.str().c_str());
}
void XML_SAVEFLOAT7INT1(int value1,int value2,int value3,int value4,int value5,int value6,
                        float value7,int value8,const char* key,XMLElePtr node)
{
    std::ostringstream buff;
    buff<<value1<<","<<value2<<","<<value3<<","<<value4<<","<<value5<<","<<value6<<","<<value7<<","<<value8;
    XMLElePtr Enode = node->InsertNewChildElement(key);
    Enode->SetText(buff.str().c_str());
}

XMLElePtr LoadXMLNode(XMLElePtr node, const char* key)
{
    XMLElePtr node2=node->FirstChildElement(key);
    return node2;
}

XMLElePtr SaveXMLNode(XMLElePtr node, const char* key)
{
    XMLElePtr node2 = node->InsertNewChildElement(key);
    return node2;
}
std::string getAppDir(){
    #define MAXCHAR 128
    // 获得应用目录
    char current_absolute_path[MAXCHAR];
    memset(current_absolute_path, 0, MAXCHAR);
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    //   GetCurrentDirectoryA(MAXCHAR, current_absolute_path);
    GetModuleFileNameA(NULL, current_absolute_path, MAXCHAR);
    PathRemoveFileSpecA(current_absolute_path);
#else
    int ret=readlink("/proc/self/exe",current_absolute_path,sizeof(current_absolute_path)-1);
    if (ret<0||(ret>=int(sizeof(current_absolute_path)-1))) ret=0;
    current_absolute_path[ret]='\0';
    for (int i=ret;i>=0;i--) {
        if (current_absolute_path[i]=='/') {
            current_absolute_path[i]='\0';
            break;
        }
    }
#endif
    std::string key = current_absolute_path;

    return key;
}
// 用于从 XML 文件中读取翻译文本的函数
std::string loadTranslation(const string& language, const string& key) {
    #define MAXCHAR 128
    // 获得应用目录
    char current_absolute_path[MAXCHAR];
    memset(current_absolute_path, 0, MAXCHAR);
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    //   GetCurrentDirectoryA(MAXCHAR, current_absolute_path);
    GetModuleFileNameA(NULL, current_absolute_path, MAXCHAR);
    PathRemoveFileSpecA(current_absolute_path);
#else
    int ret=readlink("/proc/self/exe",current_absolute_path,sizeof(current_absolute_path)-1);
    if (ret<0||(ret>=int(sizeof(current_absolute_path)-1))) ret=0;
    current_absolute_path[ret]='\0';
    for (int i=ret;i>=0;i--) {
        if (current_absolute_path[i]=='/') {
            current_absolute_path[i]='\0';
            break;
        }
    }
#endif
    strcat(current_absolute_path, "/config/translations.xml");

    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError eResult = doc.LoadFile(current_absolute_path);  // 载入 XML 文件
    if (eResult != tinyxml2::XML_SUCCESS) {
        printf("Error loading XML file!");
        return "";
    }

    // 获取所有 <language> 元素
    tinyxml2::XMLElement* root = doc.FirstChildElement("translations");
    if (!root) {
        printf("Error: root element <translations> not found!");
        return "";
    }

    // 查找指定语言的翻译
    tinyxml2::XMLElement* languageElement = root->FirstChildElement("language");
    while (languageElement) {
        const char* langCode = languageElement->Attribute("code");
        if (langCode && language == langCode) {
            // 查找指定 key 的翻译
            tinyxml2::XMLElement* messageElement = languageElement->FirstChildElement("message");
            while (messageElement) {
                const char* msgKey = messageElement->Attribute("key");
                if (msgKey && key == msgKey) {
                    return messageElement->GetText();  // 返回翻译文本
                }
                messageElement = messageElement->NextSiblingElement("message");
            }
        }
        languageElement = languageElement->NextSiblingElement("language");
    }

    return key;  // 如果找不到翻译，返回原始 key
}

std::string findTranslationKey(const string& language, const string& key) {
    #define MAXCHAR 128
    // 获得应用目录
    char current_absolute_path[MAXCHAR];
    memset(current_absolute_path, 0, MAXCHAR);
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    //   GetCurrentDirectoryA(MAXCHAR, current_absolute_path);
    GetModuleFileNameA(NULL, current_absolute_path, MAXCHAR);
    PathRemoveFileSpecA(current_absolute_path);
#else
    int ret=readlink("/proc/self/exe",current_absolute_path,sizeof(current_absolute_path)-1);
    if (ret<0||(ret>=int(sizeof(current_absolute_path)-1))) ret=0;
    current_absolute_path[ret]='\0';
    for (int i=ret;i>=0;i--) {
        if (current_absolute_path[i]=='/') {
            current_absolute_path[i]='\0';
            break;
        }
    }
#endif
    strcat(current_absolute_path, "/config/translations.xml");

    printf("findTranslationKey path:%s",current_absolute_path);
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError eResult = doc.LoadFile(current_absolute_path);  // 载入 XML 文件
    if (eResult != tinyxml2::XML_SUCCESS) {
        printf("Error loading XML file!");
        return "";
    }

    // 获取所有 <language> 元素
    tinyxml2::XMLElement* root = doc.FirstChildElement("translations");
    if (!root) {
        printf("Error: root element <translations> not found!");
        return "";
    }

    // 查找指定语言的翻译
    tinyxml2::XMLElement* languageElement = root->FirstChildElement("language");
    while (languageElement) {
        const char* langCode = languageElement->Attribute("code");
        if (langCode && language == langCode) {
            // 查找指定 key 的翻译
            tinyxml2::XMLElement* messageElement = languageElement->FirstChildElement("message");
            while (messageElement) {
                const char* msgMessage = messageElement->GetText();
                // const char* msgKey = messageElement->Attribute("key");
                if (msgMessage && key == msgMessage) {
                    return messageElement->Attribute("key");  // 返回key
                }
                messageElement = messageElement->NextSiblingElement("message");
            }
        }
        languageElement = languageElement->NextSiblingElement("language");
    }

    return key;  // 如果找不到翻译，返回原始 key
}
} // namespace HGMACHINE