#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/sched.h>
#include<linux/kthread.h>
#include<linux/delay.h>

static struct task_struct *my_task = NULL;

int my_thread_handle(void *data){
	while(!kthread_should_stop()){
		printk("data:%s\n",(char *)data);
		msleep(100);
	}
	return 0;
}

static int __init my_kthread_init(void)
{
	int err;
	my_task = kthread_run(my_thread_handle, "Dqiyue", "my_task");
	if(IS_ERR(my_task)){
		printk("unable to start kernel thread.\n");
		err = PTR_ERR(my_task);
		my_task = NULL;
		return err;
	}
	wake_up_process(my_task);
	return 0;
}

static void __exit my_kthread_exit(void)
{
	if(my_task){
		kthread_stop(my_task);
		my_task = NULL;
	}
	printk(KERN_ALERT"my kthread module exit\n");
}

module_init(my_kthread_init);
module_exit(my_kthread_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dqiyue");
