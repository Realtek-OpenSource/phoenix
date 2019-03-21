#ifndef __MALI_KBASE_CONFIG_HANK_H
#define __MALI_KBASE_CONFIG_HANK_H

struct hank_platform_data {
	struct reset_control *rstc;
	struct reset_control *rstc_s;
	void *base;
	struct device *dev;
	unsigned int inited;
	unsigned int power;
	unsigned int spm_suspended;
};

#define INITED_SYSFS_GROUP              0x1

static inline void gpu_reg_write(struct hank_platform_data *pdata,
	int offset, u32 val)
{
	dev_dbg(pdata->dev, "%s: offset=%04x, val=%08x\n", __func__, offset, val);
	writel(val, pdata->base + offset);
}

static inline u32 gpu_reg_read(struct hank_platform_data *pdata, int offset)
{
	u32 val;

	val = readl(pdata->base + offset);
	dev_dbg(pdata->dev, "%s: offset=%04x, val=%08x\n", __func__, offset, val);
	return val;
}

#include <mali_kbase_config.h>

int hank_gpu_power_on(struct kbase_device *kbdev);
int hank_gpu_power_off(struct kbase_device *kbdev);

static inline struct kbase_device *to_kbase_device(struct device *dev)
{
	return dev_get_drvdata(dev);
}

#endif
