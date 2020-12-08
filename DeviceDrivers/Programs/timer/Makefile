	obj-m:=timer.o
	KERNELDIR=/lib/modules/$(shell uname -r)/build
	PWD=$(shell pwd)

module:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

