obj-m	:=	my_kthread.o

KDIR	:=	/lib/modules/$(shell uname -r)/build
PWD	:=	$(shell pwd)

deafult:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	-rm *.o *.ko *.order *.symvers *.mod.c .*.cmd
	-rm -r .tmp_versions
