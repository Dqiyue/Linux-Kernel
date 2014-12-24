#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/proc_fs.h>
#include<linux/seq_file.h>
#include<linux/fs.h>
#include<linux/string.h>
#include<linux/rtc.h>
#include<linux/timer.h>

static int date_time = 0;
module_param(date_time, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(date_time,"--- this is the module_param  date_time\n");

static int date_day = 0;
module_param(date_day, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(date_day,"--- this is the module_param  date_day\n");

static struct proc_dir_entry *proc_ky;
static struct proc_dir_entry *student,*date;

static int task_show(struct seq_file *m, void *v){
	seq_printf(m,"create my proc dir and file\n");
	return 0;
}

static int member_show(struct seq_file *m, void *v){
	seq_printf(m,"nj_duhui   nj_zhaopei\n");
	return 0;
}

static int day_show(struct seq_file *m, void *v){
	struct timeval tv;
	struct rtc_time tm;
	int year,mon,day,wk;
	char *week[7] = {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
	do_gettimeofday(&tv);
	rtc_time_to_tm(tv.tv_sec + 8*60*60, &tm);
	year = tm.tm_year + 1900;
	mon = tm.tm_mon + 1;
	day = tm.tm_mday;
	wk = tm.tm_wday;
	switch(date_day){
		case 1:	seq_printf(m,"%d/%d/%d %s\n",year,mon,day,week[wk]);break;
		case 2: seq_printf(m,"%d-%d-%d %s\n",year,mon,day,week[wk]);break;
		case 3: seq_printf(m,"%d.%d.%d %s\n",year,mon,day,week[wk]);break;
		default:
				seq_printf(m,"%d %d %d %s\n",year,mon,day,week[wk]);break;
	};
	return 0;
}

static int time_show(struct seq_file *m, void *v){
	struct timeval tv;
	struct rtc_time tm;
	int hour,min,sec;
	do_gettimeofday(&tv);
	rtc_time_to_tm(tv.tv_sec + 8*60*60, &tm);
	hour = tm.tm_hour;
	min = tm.tm_min;
	sec = tm.tm_sec;
	switch(date_time){
		case 1:seq_printf(m,"%d-%d-%d\n",hour,min,sec);break;
		case 2:seq_printf(m,"%d/%d/%d\n",hour,min,sec);break;
		default:seq_printf(m,"%d:%d:%d\n",hour,min,sec);break;
	};

	return 0;
}

static int ky_show(struct seq_file *m, void *v){
	seq_printf(m,"I am your nightmare\n");
	return 0;
}
static int ky_open(struct inode *inode,struct file *file){
	if(!strncmp("task", file->f_dentry->d_name.name, 4))
		return single_open(file, task_show, NULL);
	if(!strncmp("member", file->f_dentry->d_name.name, 6))
	    return single_open(file, member_show, NULL);
	if(!strncmp("day", file->f_dentry->d_name.name, 3))
	  return single_open(file, day_show, NULL);
	if(!strncmp("time", file->f_dentry->d_name.name, 4))
	  return single_open(file, time_show, NULL);

	return single_open(file, ky_show, NULL);
}

static const struct file_operations myproc_fops = {
	.open = ky_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

static int __init
proc_init(void)
{
	printk("module start\n");
//	proc_create(char *filename,);
	proc_ky = proc_mkdir("proc_ky",NULL);
	student = proc_mkdir("student",proc_ky);
	date = proc_mkdir("date",proc_ky);
	if(student == NULL || date == NULL)
	  return -ENOMEM;
	proc_create("task",0,proc_ky,&myproc_fops);
	proc_create("member",0,student,&myproc_fops);
	proc_create("day",0,date,&myproc_fops);
	proc_create("time",0,date,&myproc_fops);
//	proc_symlink("info2",proc_ky,"");
//	proc_remove(sub1);
	return 0;
}

static void __exit
proc_exit(void)
{
	proc_remove(date);
	proc_remove(student);
	proc_remove(proc_ky);
	printk("remove proc\n");
	printk("module exit\n");
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dqiyue");
