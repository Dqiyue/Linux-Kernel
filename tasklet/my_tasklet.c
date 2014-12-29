#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/interrupt.h>

static struct tasklet_struct my_tasklet;
static void tasklet_handler(unsigned long data)
{
	printk(KERN_ALERT"this is my tasklet_handler.\n");
}

static int __init my_tasklet_init(void)
{
	printk(KERN_ALERT"my module start\n");
	tasklet_init(&my_tasklet,tasklet_handler,0);
	tasklet_schedule(&my_tasklet);
	return 0;
}

static void __exit my_tasklet_exit(void){
	tasklet_kill(&my_tasklet);
	printk(KERN_ALERT"my module exit.\n");
}

module_init(my_tasklet_init);
module_exit(my_tasklet_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dqiyue");
