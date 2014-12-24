#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>


static int __init
hello_init(void)
{
	printk(KERN_ALERT"my module start ,hello world\n");
	return 0;
}

static void __exit
hello_exit(void)
{
	printk(KERN_ALERT"my module exit, hello world\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dqiyue");
MODULE_DESCRIPTION("This is a simple example!\n");
MODULE_ALIAS("A simplest example");
MODULE_VERSION("1.0");
