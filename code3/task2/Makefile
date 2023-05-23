ifneq ($(KERNELRELEASE),)
	obj-m := kthread_bind_cores.o
else
	KERNELDIR ?=/usr/lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif
.PHONY:clean
install:
	insmod kthread_bind_cores.ko
uninstall:
	rmmod kthread_bind_cores
clean:
	-rm *.mod.c *.o *.order *.symvers *.ko
