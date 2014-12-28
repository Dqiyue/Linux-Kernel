#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/uaccess.h>

#define maxsize 1024
#define maxturn 1000

char  buffer[maxsize] = "Author:Dqiyue;Email:2470782476@qq.com.commit:This is a test file.";
static int __init
my_module_init(void)
{
	struct file *fp;
	mm_segment_t old_fs;
	int flag = 0,res,turn;
	loff_t pos;
	
	 fp = filp_open("./test",O_RDWR,0);
 	 if(IS_ERR(fp)){
		 flag = 1;
		 fp = filp_open("./test",O_RDWR|O_CREAT,0644);
		 if(IS_ERR(fp)){
			 printk(KERN_ALERT"open file test failed\n");
			 return -1;
		 }
	 }

	old_fs = get_fs();
	set_fs(KERNEL_DS);
	/***************vfs operation***********/
	if(0 == flag){
		pos = 0;
		res = 1;
		turn = maxturn;
		memset(buffer,0,maxsize);
		printk("read file,file content is\n");
		while(0!=res&&turn > 0){
			res = vfs_read(fp,buffer,sizeof(buffer),&pos);
			printk("%s",buffer);
			turn--;
		}
		if(0 == turn){
			printk("vfs_read err\n");
		}
		else{
			printk("vfs_read successfully\n");
		}
	}
	else{
		pos = 0;
		vfs_write(fp,buffer,sizeof(buffer),&pos);
		printk("please check the test file,weather the file content is :%s\n",buffer);
		printk("sizeof(buffer) = %lu\n",sizeof(buffer));
	}
	set_fs(old_fs);
	filp_close(fp,NULL);
	return 0;
}

static void __exit
my_module_exit(void)
{
	printk("my module exit\n");
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dqiyue");
