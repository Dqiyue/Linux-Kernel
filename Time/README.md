内核时钟介绍
Linux系统时钟以读取的硬件时钟为起始点，根据系统启动后的滴答数来计算时间，系统内的所有计时均基于它。系统用一个全局变量jiffies表示，该变量每个时钟周期更新一次，即表示系统自启动以来的时钟滴答数目。在这里我们需要借助一些内核函数来完成系统内核时间获取。

在内核中获取系统时间接口
1.  获取时间
void do_gettimeofday (struct timeval *tv)
此函数获取从1970-1-1 0:0:0到现在的时间值，存在timeval的结构体里边。
变量 tv 以秒和微妙表示当前系统时间，tv 指定获取当前系统时间的结构体变量地址。
struct timeval 结构体： 
struct timeval { 
     time_t         tv_sec; 
     suseconds_t   tv_usec;      
};
struct timeval 为设定时间或获取时间时使用的结构体，tv_sec 变量把当前时间换算为秒，tv_usec 值指定或获取 tv_usec 无法表示的 us 单位经过的时间。
头文件：#include <linux/timer.h> 
 
2. 时间格式转换
void rtc_time_to_tm(unsigned long time, struct rtc_time *tm)
此内核函数将系统实时时钟时间转换为格林尼治标准时间（GMT）。如果要得到北京时间需。需要将此时间处理（年份加上1900，月份加上1，小时加上8）。具体参看内核中rtc.h中rtc_time_to_tm代码实现。
头文件：#include <linux/rtc.h> 

3. struct tm介绍
struct tm 所存的时间格式为GMT时间格式。
struct tm { 
　　int tm_sec; /* 秒–取值区间为[0,59] */ 
　　int tm_min; /* 分 - 取值区间为[0,59] */ 
　　int tm_hour; /* 时 - 取值区间为[0,23] */
　　int tm_mday; /* 一个月中的日期 - 取值区间为[1,31] */ 
　　int tm_mon; /* 月份（从一月开始，0代表一月） - 取值区间为[0,11] */ 
　　int tm_year; /* 年份，其值从1900开始 */ 
　　int tm_wday; /* 星期–取值区间为[0,6]，其中0代表星期天，1代表星期一，以此类推 */ 
　　int tm_yday; /* 从每年的1月1日开始的天数–取值区间为[0,365]，其中0代表1月1日，1代表1月2日，以此类推 */ 
　　int tm_isdst; /* 夏令时标识符，实行夏令时的时候，tm_isdst为正。不实行夏令时的进候，tm_isdst为0；不了解情况时，tm_isdst()为负。*/ 
　　long int tm_gmtoff; /*指定了日期变更线东面时区中UTC东部时区正秒数或UTC西部时区的负秒数*/ 
　　const char *tm_zone; /*当前时区的名字(与环境变量TZ有关)*/ 
}; 
