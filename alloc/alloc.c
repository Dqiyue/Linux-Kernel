#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/slab.h>
#include<linux/gfp.h>

char *k1,*k8,*v8,*vm,*v64;

static int __init
my_alloc_init(void)
{
	k1 = (char*)kmalloc(1024,GFP_KERNEL);
	k8 = (char*)kmalloc(8*1024,GFP_KERNEL);
	v8 = (char*)vmalloc(8*1024);
	vm = (char*)vmalloc(1024*1024);
	v64 = (char*)vmalloc(64*1024*1024);
	printk(KERN_ALERT"kmalloc 1kb address:%p,kmalloc 8kb address:%p,address1 - address2 = %lu\n",k1,k8,k8-k1);
	printk(KERN_ALERT"vmalloc 8kb address:%p,vmalloc 1MB address:%p,vmalloc 64MB address:%p\n",v8,vm,v64);
	return 0;
}

static void __exit
my_alloc_exit(void)
{
	kfree(k1);
	kfree(k8);
	vfree(v8);
	vfree(vm);
	vfree(v64);
	printk(KERN_ALERT"module exit\n");
}

module_init(my_alloc_init);
module_exit(my_alloc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dqiyue/2470782476@qq.com");
MODULE_VERSION("1.0");

