PWD=$(shell pwd)
ifndef TOP
	TOP = ../..
endif
include $(TOP)/make.include

ifndef TARGET_PLATFORM
TARGET_PLATFORM=thor
endif

all: install

install:
	KDIR=$(PWD)/$(LINUX_KERNEL_PATH) \
	CONFIG_MALI_PLATFORM_NAME="$(TARGET_PLATFORM)" \
	CONFIG_MALI_MIDGARD=m \
	make -C driver/product/kernel/drivers/gpu/arm/midgard
	$(CROSS_COMPILE)strip driver/product/kernel/drivers/gpu/arm/midgard/mali_kbase.ko --strip-unneeded

clean:
	$(MAKE) -C driver/product/kernel/drivers/gpu/arm/midgard


