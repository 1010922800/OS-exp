ifneq ($(KERNELRELEASE),)
	obj-m := kthread_sleep_monitor.o
else
	KERNELDIR ?=/usr/lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif
.PHONY:clean
install:
	insmod kthread_sleep_monitor.ko
uninstall:
	rmmod kthread_sleep_monitor
clean:
	-rm *.mod.c *.o *.order *.symvers *.ko
