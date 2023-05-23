ifneq ($(KERNELRELEASE),)
	obj-m := proc_sys_info.o
else
	KERNELDIR ?=/usr/lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif
install:
	insmod proc_sys_info.ko
uninstall:
	rmmod proc_sys_info
.PHONY:clean
clean:
	-rm *.mod.c *.o *.order *.symvers *.ko
