/*
**
**	版权:	
**
**	名称:	libpinyinHelp.h
**
**	作者:	侯 晓 琴
**
**	日期:	2025/06/10
**
**	描述:
**
**	功能:	libpinyin 帮助类
**
**	备注:
**
*/

#ifndef LIBPINYINHELP_H
#define LIBPINYINHELP_H

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#else
// #include <string>
// #include <vector>
// #include "pinyin_lookup2.h"

// class LibPinyinHelper {
// public:
//     bool init(const std::string &dbPath);
//     std::vector<std::string> lookup(const std::string &pinyin);
//     void cleanup();


// private:
//     // pinyin_instance_t *m_instance;
//     // lookup_candidate_t *m_lookup;

// };

#endif
#endif // LIBPINYINHELP_H