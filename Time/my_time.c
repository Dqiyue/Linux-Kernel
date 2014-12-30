#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/rtc.h>
#include<linux/timer.h>

static struct timeval start;
static struct timeval tvn;
static char *weekday[7]={"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};

void showtime(void){
	struct rtc_time tm;
	do_gettimeofday(&tvn);
	rtc_time_to_tm(tvn.tv_sec+8*60*60,&tm);
	printk("Today is %d/%d/%d %s, now the time is %d:%d:%d  \n",tm.tm_year+1900,tm.tm_mon+1,tm.tm_mday,weekday[tm.tm_wday],tm.tm_hour,tm.tm_min,tm.tm_sec);
}

unsigned long int gettimepass(void)
{
	do_gettimeofday(&tvn);
	return (tvn.tv_sec - start.tv_sec)*1000000 + tvn.tv_usec - start.tv_usec;
}

static int __init my_time_init(void)
{
	do_gettimeofday(&start);
	printk(KERN_ALERT"my time module start,showtime:\n");
	showtime();
	printk("time past from module start is %lu usec\n",gettimepass());
	return 0;
}

static void __exit my_time_exit(void)
{
	printk(KERN_ALERT"my time module exit,showtime:\n");
	showtime();
	printk("time past from module start is %lu usec\n",gettimepass());
}

module_init(my_time_init);
module_exit(my_time_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dqiyue");
