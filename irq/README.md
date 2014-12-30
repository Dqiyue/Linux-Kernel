模块参数module_param(name,type,perm)
    在用户态下编程可以通过main()来传递命令行参数，而编写一个内核模块则通过module_param()来实现参数的传递。
参数用 module_param 宏定义来声明, 它定义在 moduleparam.h.
module_param(name,type,perm);
module_param 使用了 3 个参数: 变量名, 它的类型, 以及一个权限掩码用来做一个辅助的 sysfs 入口. 这个宏定义应当放在任何函数之外, 典型地是出现在源  文件的前面.定义如：
    #include <linux/init.h>
    #include <linux/module.h>
    #include <linux/moduleparam.h>

    static char *who = "world";
    static int times = 1;
    module_param (times, int, S_IRUSR);
    module_param (who, charp, S_IRUSR);
    static int hello_init (void)
   {
      int i;
      for (i = 0; i < times; i++)
      printk (KERN_ALERT "(%d) hello, %s!\n", i, who);
    return 0;
   }
      static void hello_exit (void)
  {
      printk (KERN_ALERT "Goodbye, %s!\n", who);
  }
module_init (hello_init);
module_exit (hello_exit);
编译生成内核模块hello.ko,然后在加载模块时输入参数insmod hello.ko who="world" times=5


中断号的分配情况如下：
IRQ　说明
0　定时器
1　键盘
2 串行设备控制器
3　COM2
4　COM1
5 LPT2
6　软盘控制器
7　LPT1
8　实时时钟
9　PC网络
10　可用（Available）
11　可用（Available）
12 PS/2 鼠标
13　数学协处理器
14 硬盘控制器
15 可用（Available）


中断注册
   在内核模块中中断注册的工作是通过request_irq函数来实现的。下面来分析一下request_irq函数。
int request_irq(unsigned int irq,  irq_handler_t handler,unsigned long flags, const char *devname, void *dev_id)
    第一个参数irq: 表示要分配的中断号。对于一些设备(系统时钟或键盘)它的值是预先固定的，而对于大多数设备来说，这个值是动态确定的。
    第二个参数handler：表示要挂入到中断请求对列中的中断服务例程，这个中断服务函数的原型是static irqreturn_t handler(int , void *);
中断处理程序的前缀为static,因为它从来不会被别的文件中的代码直接调用。
    第三个参数flags:为标志位。可以取IRQF_DISABLED、IRQF_SHARED和IRQF_SAMPLE_RANDOM之一。在本实例程序中取 IRQF_SHARED，该标志表示多个中断处理程序共享irq中断线。一般某个中断线上的中断服务程序在执行时会屏蔽请求该线的其他中断，如果取 IRQF_DISABLED标志，则在执行该中断服务程序时会屏蔽所有其他的中断。取IRQF_SAMPLE_RANDOM则表示设备可以被看做是事件随见的发生源。
    第四个参数devname:是请求中断的设备的名称。当你加载模块成功后可以在/proc/interrupts中查看到具体设备的名称，与此同时也可以看到这个设备对应的中断号以及请求次数。
    第五个参数dev_id:为一个指针型变量。注意该参数为void型，也就是说通过强制转换可以转换为任意类型。dev_id主要用于共享中断线，对每个注册的中断处理程序来说，dev_id参数必须唯一(指向任一设备结构的指针就可以满足此要求，选择设备结构因为它是唯一的，而且中断处理程序可能会用到它)如果无需共享中断线，则将该参数赋值为NULL。
requset_irq()函数成功执行后返回0。如果返回非0值，就表示错误发生。此时，指定的中断处理程序不会被注册。

1.软中断与tasklet
    tasklet是linux中断处理机制中的软中断延迟机制。引入tasklet，最主要的是考虑支持SMP，提高SMP多个cpu的利用率；不同的tasklet可以在不同的cpu上运行。tasklet可以理解为softirq的派生，所以它的调度时机和软中断一样。对于内核中需要延迟执行的多数任务都可以用tasklet来完成，由于同类tasklet本身已经进行了同步保护，所以使用tasklet比软中断要简单的多，tasklet不需要考虑SMP下的并行问题，而又比workqueues有着更好的性能。tasklet是作为中断下半部的一个很好的选择，它在性能和易用性之间有着很好的平衡。
    tasklet通常作为中断的下半部来使用，在中断中调用tasklet_schedule(t)。每次中断都会触发一次tasklet_schedule(t)，但是每次中断它只会向其中的一个CPU注册，而不是所有的CPU。完成注册后的tasklet由tasklet_action()来执行，在SMP环境下，它保证同一时刻，同一个tasklet只有一个副本在运行，这样就避免了使用softirq所要考虑的互斥的问题。再者，tasklet在执行tasklet->func()前，再一次允许tasklet可调度（注册），但是在该tasklet已有一个副本在其他CPU上运行的情况下，它只能退后执行。总之，同一个硬中断引起的一个tasklet_schedule()动作只会使一个tasklet被注册，而不同中断引起的tasklet则可能在不同的时刻被注册而多次被执行。
     由于同一个tasklet不能有多个副本同时运行，所以不需要在多CPU之间互斥。在tasklet运行的过程中，它会被硬中断打断（这也是软中断的优点），所以如果tasklet和其他中断之间的互斥有可能存在。
2.中断的上半部和下半部
    中断服务程序一般都是在中断请求关闭的条件下执行的,以避免嵌套而使中断控制复杂化。但是，中断是一个随机事件，它随时会到来，如果关中断的时间太长，CPU就不能及时响应其他的中断请求，从而造成中断的丢失。因此，Linux内核的目标就是尽可能快的处理完中断请求，尽其所能把更多的处理向后推迟。例如，假设一个数据块已经达到了网线，当中断控制器接受到这个中断请求信号时，Linux内核只是简单地标志数据到来了，然后让处理器恢复到它以前运行的状态，其余的处理稍后再进行（如把数据移入一个缓冲区，接受数据的进程就可以在缓冲区找到数据）。因此，内核把中断处理分为两部分：上半部（tophalf）和下半部（bottomhalf），上半部（就是中断服务程序）内核立即执行，而下半部（就是一些内核函数）留着稍后处理，
     首先，一个快速的“上半部”来处理硬件发出的请求，它必须在一个新的中断产生之前终止。通常，除了在设备和一些内存缓冲区（如果你的设备用到了DMA，就不止这些）之间移动或传送数据，确定硬件是否处于健全的状态之外，这一部分做的工作很少。
     下半部运行时是允许中断请求的，而上半部运行时是关中断的，这是二者之间的主要区别。但是，内核到底什时候执行下半部，以何种方式组织下半部？这就是我们要讨论的下半部实现机制，这种机制在内核的演变过程中不断得到改进，在以前的内核中，这个机制叫做bottomhalf(简称bh)，在2.4以后的版本中有了新的发展和改进，改进的目标使下半部可以在多处理机上并行执行，并有助于驱动程序的开发者进行驱动程序的开发。

3.关于tasklet的介绍和例子可以参考博文  blog.chinaunix.net/uid-24467128-id-3234379.html ，edsionte.com/techblog/archives/1547


