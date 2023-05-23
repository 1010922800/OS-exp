ifneq ($(KERNELRELEASE),)
	obj-m := kthread_stu_id.o
else
	KERNELDIR ?=/usr/lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif
.PHONY:clean
install:
	insmod kthread_stu_id.ko
uninstall:
	rmmod kthread_stu_id
clean:
	-rm *.mod.c *.o *.order *.symvers *.ko
