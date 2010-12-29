# If KERNELRELEASE is defined, we've been invoked from the
# kernel build system and can use its language.
ifneq ($(KERNELRELEASE),)
	obj-m += g13.o
# Otherwise we were called directly from the command
# line; invoke the kernel build system.
else
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
	KCPPFLAGS := -DKERNELBUILD=1
endif

default: kernel-build

kernel-build: clean
	$(MAKE) -C $(KERNELDIR) M=$(PWD) KCPPFLAGS=$(KCPPFLAGS) modules

install:
	rmmod usbhid || true
	rmmod g13 || true
	insmod g13.ko

clean:
	rm -rf *.o *.ko *.mod.c *.c.* .*.cmd .*.c.* modules.order Module.symvers .tmp_versions test
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

test-build: clean
	gcc -std=c99 -pedantic -g -Wall -Wstrict-prototypes -o test RunTests.c CuTest.c g13Test.c g13_util.c

test-run: test-build
	./test

test: test-build test-run
 
