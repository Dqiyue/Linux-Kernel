obj-m := my_time.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	rm *.o *.ko *.order *.symvers *.mod.c .*.cmd
	rm -r .tmp_versions
