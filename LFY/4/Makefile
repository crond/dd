#if ( "$#" < "2" )
#	exit(0)
#obj-m:= $1.o
obj-m:= ofd.o
KERNLDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNLDIR) M=$(PWD) modules
clean:
	rm *.o *.ko *.o* *.sy*  *.mod*
