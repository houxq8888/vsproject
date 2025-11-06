/*
    @file   style.cpp
    @brief  Implementation file
    @author XiaoQin.Hou
*/
#include "style.h"

namespace HGMACHINE
{
    Style::Style(int type, std::string stylePath)
    {
        m_stylePath = stylePath;
        m_type_style = type;
    }
    Style::~Style()
    {

    }
    bool Style::LoadDB()
    {
        return true;
    }

}
