
obj-m := my_tasklet.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	rm *.o *.ko *.symvers .*.cmd *.order *.mod.c
	rm -r .tmp_versions
