// #include "libpinyinHelp.h"

// #if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

// #else
// #include <pinyin.h>

// bool LibPinyinHelper::init(const std::string &dbPath){
//     if (!pinyin_init_with_path(m_instance)){  // 加载数据库（默认路径 /usr/local/share/libpinyin）\
//         return false;
//     }
//     m_lookup = create_lookup2(m_instance);
//     set_lookup_phrase_filter(m_lookup,NULL);
//     return true;
// }
// std::vector<std::string> LibPinyinHelper::lookup(const std::string &pinyin){
//     std::vector<std::string> ret;
//     if (!lookup_predict_sentence(m_lookup,pinyin.c_str())){
//         printf("lookup failed\n");
//         return false;
//     }

//     char **results;
//     size_t result_len;
//     get_best_sentence(m_lookup,&results,&result_len);
//     for (size_t i = 0; i < result_len; ++i){
//         ret.push_back(results[i]);
//     }
//     return ret;
// }
// void LibPinyinHelper::cleanup(){
//     destroy_lookup2(lookup);
//     pinyin_fini(m_instance);
// }
// #endif