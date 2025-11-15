# Kernel source
KERNEL_SRC := /home/ostingo/tuya-bsp-gpl-public-components/linux-4.4.192

# Module to build
obj-m := isp_stub_driver.o

# Objects included in the module
isp_stub_driver-objs := isp_stub_driver_core.o isp_stub_globals.o isp_stub_symbols.o

# Build rule
all:
	$(MAKE) -C $(KERNEL_SRC) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNEL_SRC) M=$(PWD) clean
	rm -f *.o *.ko *.mod.o *.mod.c modules.order Module.symvers




