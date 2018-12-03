#ifndef __MALI_KBASE_CONFIG_THOR_H__
#define __MALI_KBASE_CONFIG_THOR_H__

#define BISR_MODE0   0x4350
#define BISR_DONE0   0x4354
#define BISR0        0x4358
#define BISR0_0      0x435c

#define BISR_MODE1   0x4360
#define BISR_DONE1   0x4364
#define BISR1        0x4368
#define BISR1_0      0x436c

#define BISR_MODE2   0x4370
#define BISR_DONE2   0x4374
#define BISR2        0x4378
#define BISR2_0      0x437c

#define BISR_MODE3   0x4380
#define BISR_DONE3   0x4384
#define BISR3        0x4388
#define BISR3_0      0x438c

#define BISR_MODE4   0x4390
#define BISR_DONE4   0x4394
#define BISR4        0x4398
#define BISR4_0      0x439c

#define CTI_DMC_SEL  0x5090

struct thor_platform_data {
	struct reset_control *rstc;
	void *base;
	struct device *dev;
	unsigned int inited;
	unsigned int dmc;
	unsigned int no_cg;
	unsigned int power;
	unsigned int spm_suspended;
};

#define INITED_SYSFS_GROUP              0x1

static inline void gpu_reg_write(struct thor_platform_data *pdata,
	int offset, u32 val)
{
	dev_dbg(pdata->dev, "%s: offset=%04x, val=%08x\n", __func__, offset, val);
	writel(val, pdata->base + offset);
}

static inline u32 gpu_reg_read(struct thor_platform_data *pdata, int offset)
{
	u32 val;

	val = readl(pdata->base + offset);
	dev_dbg(pdata->dev, "%s: offset=%04x, val=%08x\n", __func__, offset, val);
	return val;
}

#include <mali_kbase_config.h>

int thor_gpu_power_on(struct kbase_device *kbdev);
int thor_gpu_power_off(struct kbase_device *kbdev);

static inline struct kbase_device *to_kbase_device(struct device *dev)
{
	return dev_get_drvdata(dev);
}

#endif
