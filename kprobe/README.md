kprobe简介
kprobe是一个动态地收集调试和性能信息的工具，它从Dprobe项目派生而来，是一种非破坏性工具，用户用它几乎可以跟踪任何函数或被执行的指令以及一些异步事件（如timer）。它的基本工作机制是：用户指定一个探测点，并把一个用户定义的处理函数关联到该探测点，当内核执行到该探测点时，相应的关联函数被执行，然后继续执行正常的代码路径。

kprobe实现了三种类型的探测点: kprobes, jprobes和kretprobes (也叫返回探测点)。 kprobes是可以被插入到内核的任何指令位置的探测点，jprobes则只能被插入到一个内核函数的入口，而kretprobes则是在指定的内核函数返回时才被执行。
一般，使用kprobe的程序实现作一个内核模块，模块的初始化函数来负责安装探测点，退出函数卸载那些被安装的探测点。kprobe提供了接口函数（APIs）来安装或卸载探测点。目前kprobe支持如下架构：i386、x86_64、ppc64、ia64(不支持对slot1指令的探测)、sparc64 (返回探测还没有实现)。

jprobe通过注册kprobes在被探测函数入口的来实现，它能无缝地访问被探测函数的参数。jprobe处理函数应当和被探测函数有同样的原型，而且该处理函数在函数末必须调用kprobe提供的函数jprobe_return()。当执行到该探测点时，kprobe备份CPU寄存器和栈的一些部分，然后修改指令寄存器指向jprobe处理函数，当执行该jprobe处理函数时，寄存器和栈内容与执行真正的被探测函数一模一样，因此它不需要任何特别的处理就能访问函数参数， 在该处理函数执行到最后时，它调用jprobe_return()，那导致寄存器和栈恢复到执行探测点时的状态，因此被探测函数能被正常运行。需要注意，被探测函数的参数可能通过栈传递，也可能通过寄存器传递，但是jprobe对于两种情况都能工作，因为它既备份了栈，又备份了寄存器，当然，前提是jprobe处理函数原型必须与被探测函数完全一样。

kretprobe也使用了kprobes来实现，当用户调用register_kretprobe()时，kprobe在被探测函数的入口建立了一个探测点，当执行到探测点时，kprobe保存了被探测函数的返回地址并取代返回地址为一个trampoline的地址，kprobe在初始化时定义了该trampoline并且为该trampoline注册了一个kprobe,当被探测函数执行它的返回指令时，控制传递到该trampoline，因此kprobe已经注册的对应于trampoline的处理函数将被执行，而该处理函数会调用用户关联到该kretprobe上的处理函数，处理完毕后，设置指令寄存器指向已经备份的函数返回地址，因而原来的函数返回被正常执行。

被探测函数的返回地址保存在类型为kretprobe_instance的变量中，结构kretprobe的maxactive字段指定了被探测函数可以被同时探测的实例数，函数register_kretprobe()将预分配指定数量的kretprobe_instance。如果被探测函数是非递归的并且调用时已经保持了自旋锁（spinlock），那么maxactive为1就足够了； 如果被探测函数是非递归的且运行时是抢占失效的，那么maxactive为NR_CPUS就可以了；如果maxactive被设置为小于等于0, 它被设置到缺省值（如果抢占使能， 即配置了 CONFIG_PREEMPT，缺省值为10和2*NR_CPUS中的最大值，否则缺省值为NR_CPUS）。

如果maxactive被设置的太小了，一些探测点的执行可能被丢失，但是不影响系统的正常运行，在结构kretprobe中nmissed字段将记录被丢失的探测点执行数，它在返回探测点被注册时设置为0，每次当执行探测函数而没有kretprobe_instance可用时，它就加1。
