PWD=$(shell pwd)
ifndef TOP
	TOP = ../..
endif
include $(TOP)/make.include

MALI_DEVICEDRV_PATH=$(PWD)/driver/src/devicedrv/mali


all: install

install: build

build:
	$(MAKE) -C $(MALI_DEVICEDRV_PATH) KDIR=$(PWD)/$(LINUX_KERNEL_PATH) \
		 BUILD=release USING_PROFILING=0 USING_DT=1 USING_DVFS=1 MALI_DMA_BUF_MAP_ON_ATTACH=0 \
		 USING_DEVFREQ=1 MALI_PLATFORM=hercules
	$(CROSS_COMPILE)strip $(PWD)/driver/src/devicedrv/mali/mali.ko --strip-unneeded

clean:
	$(MAKE) -C $(MALI_DEVICEDRV_PATH) KDIR=$(PWD)/$(LINUX_KERNEL_PATH) clean
