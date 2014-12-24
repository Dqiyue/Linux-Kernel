#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/kprobes.h>
#include<linux/kallsyms.h>
#include<linux/sched.h>
#include<linux/time.h>

static struct kprobe kp;
static struct timeval start,end;
static int schedule_counter = 0;

static int my_pre_handler(struct kprobe *p, struct pt_regs *regs){
	printk(KERN_ALERT"current task on cpu#%d:%s(before scheduling),preempt_count=%d\n",smp_processor_id(),current->comm,preempt_count());
	schedule_counter++;
	return 0;
}
static void my_post_handler(struct kprobe *p, struct pt_regs *regs, unsigned long flags){
	printk(KERN_ALERT"current task on CPU#%d:%s(after scheduling),preempt_count=%d\n",smp_processor_id(),current->comm,preempt_count());
}
static int my_fault_handler(struct kprobe *p,struct pt_regs *regs, int trapnr){
	printk(KERN_ALERT"this is my kprobe fault_handler, a fault happened during probing.\n");
	return 0;
}

int init_module(void){
	int ret;
	kp.addr = (kprobe_opcode_t*)kallsyms_lookup_name("schedule");
	kp.pre_handler = my_pre_handler;
	kp.post_handler = my_post_handler;
	kp.fault_handler = my_fault_handler;
	if((ret = register_kprobe(&kp) < 0)){
		printk("register_kprobe failed,returned %d\n",ret);
		return -1;
	}
	do_gettimeofday(&start);
	printk("kprobe registere\n");
	return 0;
}

void cleanup_module(void)
{
	unregister_kprobe(&kp);
	do_gettimeofday(&end);
	printk("Scheduling times is %d during of %ld milliseconds.\n", schedule_counter, ((end.tv_sec - start.tv_sec)*1000000 + (end.tv_usec - start.tv_usec))/1000);
	printk(KERN_ALERT"kprobe unregistered\n");
}
MODULE_LICENSE("GPL");
