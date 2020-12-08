	obj-m=work.o
	KERNELDIR=/lib/modules/$(shell uname -r)/build
	PED=$(shell pwd)

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

