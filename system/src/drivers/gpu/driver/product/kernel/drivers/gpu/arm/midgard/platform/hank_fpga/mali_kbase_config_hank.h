#ifndef __MALI_KBASE_CONFIG_HANK_H__
#define __MALI_KBASE_CONFIG_HANK_H__

struct hank_platform_data {
	void *base;
	struct device *dev;
	void *rbus_base;
};

static inline void rbus_reg_write(struct hank_platform_data *pdata,
	int offset, u32 val)
{
	dev_dbg(pdata->dev, "%s: offset=%04x, val=%08x\n", __func__, offset, val);
	writel(val, pdata->rbus_base + offset);
}

static inline u32 rbus_reg_read(struct hank_platform_data *pdata, int offset)
{
	u32 val;

	val = readl(pdata->rbus_base + offset);
	dev_dbg(pdata->dev, "%s: offset=%04x, val=%08x\n", __func__, offset, val);
	return val;
}

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

#endif
