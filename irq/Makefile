
obj-m := my_irq.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD :=	$(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	rm *.o *.ko *.symvers *.mod.c .*.cmd *.order
	rm -r .tmp_versions
