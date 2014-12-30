#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/interrupt.h>

static int irq;
static char *devname;
module_param(irq,int,0644);
module_param(devname,charp,0644);

static irqreturn_t my_irq_handler(int irq,void *dev)
{
	printk(KERN_ALERT"This is the handler of  irq %d\n",irq);
	return IRQ_HANDLED;
}

static int __init my_irq_init(void)
{
	int ret;
	ret = request_irq(irq,my_irq_handler,IRQF_SHARED,devname,&irq);
	if(0!=ret){
		printk("%s request IRQ %d failed..\n",devname,irq);
		return -1;
	}
	else
		printk("%s request IRQ %d success..\n",devname,irq);
	return 0;
}

static void __exit my_irq_exit(void)
{
	free_irq(irq,&irq);
	printk("my module exit\n");
}

module_init(my_irq_init);
module_exit(my_irq_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dqiyue");
