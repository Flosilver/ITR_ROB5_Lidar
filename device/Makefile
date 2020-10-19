obj-m:=encoder.o
KDIR := /lib/modules/$(shell uname -r)/build

default:
	$(MAKE) -C $(KDIR) M=$(shell pwd) modules

clean:
	rm -f *.[oas] .*.flags *.ko .*.cmd .*.d .*.tmp *.mod.c *.order *.symvers *.mod

install:
	sudo insmod encoder.ko

uninstall:
	sudo rmmod encoder.ko

reinstall: uninstall install

info:
	modinfo encoder.ko
