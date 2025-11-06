/*
 * Copyright(C) 2016-2020
 * Filename: HGExactTime.h
 * Author: L.Y.
 * Contact: qwetuqq@outlook.com
 * Brief: 精确时间类，精确到微妙；用于取代Windows库中的SYSTMTIME
 * Dependencies: C++11
 * Update log:
 *     1) 20200410-20200413 V1.0.0
 *         1, 初次版本，精确时间类，精确到微妙；用于取代Windows库中的SYSTMTIME。
 *     2) 20200629 V2.0.0
 *         1, 去除opencv库的依赖。
 *         2, 更改成员函数，使接口更加清晰。
 *     3) 20200629 V2.0.1
 *         1, bug1：std::localtime函数多线程不安全，导致currentTime函数多线程不安全。
 *            解决方法：使用静态的互斥锁。
 *     4) 20200724 V2.0.2
 *         1, bug2: 使用静态的互斥锁，不能完全解决bug1。解决方法：删除静态的互斥锁，
 *			  std::localtime改为std::localtime_s(WIndows)和std::localtime_r(Linux)。
 *         2, 功能优化：编译器为MS VC++时，使用sprintf_s取代sprintf。
 *     5) 20200824 V2.0.3
 *         1, 代码优化：使用snprintf函数取代sprintf函数。
 *     6) 20200929 V2.1.0
 *         1, 功能增加：增加公有成员函数toStringForFilename。
 *     7) 20201112 V2.2.0
 *         1, 代码优化：去除“数据成员设置函数”。
 *         2, 接口增加：新增构造函数ExactTime(const std::tm&, int, int)。
 * Performance:
 *     1) 获取当前时间耗时(@i7-6700k)：C++ API：10e-7ms； Windows API：5.5e-7ms。
 * TODO:
 *     1)) 20200413 currentTime函数可以设定精度：分、秒、毫秒、微妙。
 *     2)) 20200707 自己实现std::localtime函数，去除条件预编译。
 *     3) 20200724 自己实现format函数，用于取代sprintf函数。（V2.0.3）
 */

#ifndef HGEXACTTIME_H
#define HGEXACTTIME_H

#include "HGCommon_global.h"
#include <ctime>
#include <string>
//
// \brief: 精确时间类
//


class HGExactTime : public std::tm
{
public:
    int tm_millisec; // 毫秒
    int tm_microsec; // 微秒

    //! 默认构造函数
    HGExactTime();

    //! 构造函数（重载）
    HGExactTime(const std::tm& _tm, int _tm_millisec, int _tm_microsec);

    //! 转换为字符串（此字符串含有':'，不能用于文件名）
    std::string toString() const;
    std::string toStandardString() const;
    std::string toStringFromYearToSec() const;
    std::string toStringFromYearToSecAndZone() const;
    std::string toStringFromYearToDay() const ;
    //! 转换为字符串，可用于文件名
    std::string toStringForFilename() const;

    //! 获取当前时间
    static HGExactTime currentTime();
    static int fasterThanThirtyMimutes(const std::string &timeStr);
    static void setSystemTime(int year, int month, int day, int hour, int minute, int second);

    bool operator<=(const HGExactTime &obj) const
    {
        return (!(*this > obj));
    }
    bool operator>=(const HGExactTime &obj) const
    {
        return (!(*this < obj));
    }
    HGExactTime& operator+=(const int &value);
    bool operator<(const HGExactTime &obj) const;
    bool operator>(const HGExactTime &obj) const;
    int toDays() const; // 将日期转换为“天数”
    int operator-(const HGExactTime& other) const;

protected:
private:
};


#endif // HGEXACTTIME_H
