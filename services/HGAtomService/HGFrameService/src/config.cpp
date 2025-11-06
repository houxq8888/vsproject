#include "config.h"


namespace HGMACHINE{
    Config FileConfig::m_config;
    FileConfig::FileConfig(){

    }
    FileConfig::~FileConfig(){

    }
    bool FileConfig::createConfigFile(std::string path,const std::string &name)
    {
        tinyxml2::XMLDocument doc;
        tinyxml2::XMLDeclaration *declaration = doc.NewDeclaration();
        doc.InsertFirstChild(declaration);
        tinyxml2::XMLElement *root = doc.NewElement(name.c_str());
        doc.InsertEndChild(root);
        root->SetAttribute("Version", "1.0");
        m_config.setVersion("1.0.0");
        m_config.setPath(path);
        m_config.setTaskSeqName(TaskSequenceDBName);
        // 子节点
        m_config.SaveXML(root);
        doc.SaveFile(path.c_str());
        return true;
    }
    bool FileConfig::loadConfigFile(std::string path)
    {
        // 构建一个xml文档类
        tinyxml2::XMLDocument doc;
        // 从磁盘加载文件，成功返回XML_SUCCESS(0),
        tinyxml2::XMLError error = doc.LoadFile(path.c_str());
        if (error != tinyxml2::XMLError::XML_SUCCESS)
            return false;
        // 解析根节点,等效于FirstChildElement
        tinyxml2::XMLElement *root = doc.RootElement();
        m_config.setPath(path);
        if (root)
        {
            m_config.LoadXML(root);
        }
        return true;
    }
    void FileConfig::saveConfigFile(const std::string &name)
    {
        tinyxml2::XMLDocument doc;
        tinyxml2::XMLDeclaration *declaration = doc.NewDeclaration();
        doc.InsertFirstChild(declaration);
        tinyxml2::XMLElement *root = doc.NewElement(name.c_str());
        doc.InsertEndChild(root);
        root->SetAttribute("Version", "1.0");
        // 子节点
        m_config.SaveXML(root);
        doc.SaveFile(m_config.path.c_str());
    }
    void FileConfig::setVersion(const std::string& version){
        m_config.setVersion(version);
    }
}