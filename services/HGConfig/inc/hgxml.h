#ifndef HGXML_H
#define HGXML_H

#include "HGConfig_global.h"
#include "tinyxml2.h"


namespace HGMACHINE {

#define XMLElePtr tinyxml2::XMLElement*
void XML_GETSTRING(string &value,string defaultValue,
                                                string nodeName, XMLElePtr node);

void XML_GETINT(int &value,int defaultValue,string nodeName,XMLElePtr node);
void XML_GETINT(unsigned int &value, unsigned int defaultValue,string nodeName,XMLElePtr node);
void XML_GETDOUBLE(double &value,double defaultValue,string nodeName,
                                                XMLElePtr node);

void XML_SAVESTRING(const char* value,const char* key,XMLElePtr node);

void XML_SAVEINT(int value,const char* key,XMLElePtr node);
void XML_SAVEDOUBLE(double value,const char* key, XMLElePtr node);
void XML_SAVEFLOAT(float value,const char* key,XMLElePtr node);
void XML_SAVEDOUBLE4(double value1,double value2,double value3,double value4,const char* key,XMLElePtr node);
void XML_SAVEFLOAT4(float value1,float value2,float value3,float value4,
                       const char* key,XMLElePtr node);
void XML_SAVEDOUBLE2(double value1,double value2,const char* key,XMLElePtr node);
void XML_SAVEFLOAT2(float value1,float value2,const char* key,XMLElePtr node);
void XML_SAVEINT2(int value1,int value2,const char* key,XMLElePtr node);
void XML_SAVEFLOAT7INT1(int value1,int value2,int value3,int value4,int value5,int value6,
                         float value7,int value8,const char* key,XMLElePtr node);
void XML_SAVEDOUBLE7INT1(int value1,int value2,int value3,int value4,int value5,int value6,
                        double value7,int value8,const char* key,XMLElePtr node);
std::string getAppDir();
XMLElePtr LoadXMLNode(XMLElePtr node, const char* key);
XMLElePtr SaveXMLNode(XMLElePtr node, const char* key);

std::string loadTranslation(const string& language, const string& key) ;
std::string findTranslationKey(const string& language, const string& key) ;
}

#endif // HGXML_H
