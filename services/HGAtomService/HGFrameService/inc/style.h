/*
**
**	版权:	
**
**	名称:	style.h
**
**	作者:	侯 晓 琴
**
**	日期:	2024/11/11
**
**	描述:
**
**	功能:	式样类
**
**	备注:
**
*/

#ifndef STYLE_H
#define STYLE_H

#include <string>

namespace HGMACHINE
{
    class Style
    {
    public:
        Style(int type, std::string stylePath);
        virtual ~Style();
        virtual bool LoadDB() = 0;

    protected:
        int m_type_style;
        std::string m_stylePath;
    };
}

#endif // STYLE_H
