# If KERNELRELEASE is defined, we've been invoked from the
# kernel build system and can use its language.
ifneq ($(KERNELRELEASE),)
	obj-m := g13.o
# Otherwise we were called directly from the command
# line; invoke the kernel build system.
else
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
endif

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

install: default
	sudo rmmod usbhid || true
	sudo rmmod g13 || true
	sudo insmod g13.ko

clean:
	rm -rf *.o *.ko *.mod.c .*.cmd modules.order Module.symvers .tmp_versions

