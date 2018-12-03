#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/reset.h>
#include <linux/pm_runtime.h>
#include <mali_kbase.h>
#include <mali_kbase_defs.h>
#include <mali_kbase_config.h>
#include "mali_kbase_config_platform.h"
#include "mali_kbase_config_hank.h"

static int pm_callback_power_on(struct kbase_device *kbdev)
{
	return 1;
}

static void pm_callback_power_off(struct kbase_device *kbdev)
{
}

struct kbase_pm_callback_conf pm_callbacks = {
	.power_on_callback = pm_callback_power_on,
	.power_off_callback = pm_callback_power_off,
	.power_suspend_callback  = NULL,
	.power_resume_callback = NULL
};

static void hank_fpga_init(struct hank_platform_data *pdata)
{
	u32 val;

	rbus_reg_write(pdata, 0x7b70, 0x00000f00);
	udelay(1000);
	val = rbus_reg_read(pdata, 0x7b74);
	if (!(val & 0x4)) {
		dev_warn(pdata->dev, "sram int not set\n");
	}

	rbus_reg_write(pdata, 0x0050, 0x00008000);
	rbus_reg_write(pdata, 0x0000, 0x00200000);

	rbus_reg_write(pdata, 0x7fd0, 0x07ff0000);

	rbus_reg_write(pdata, 0x0000, 0x00300000);
	rbus_reg_write(pdata, 0x0050, 0x0000c000);
}


static struct kbase_platform_config dummy_platform_config;

struct kbase_platform_config *kbase_get_platform_config(void)
{
	return &dummy_platform_config;
}

static int kbase_platform_init(struct kbase_device *kbdev)
{
	struct device *dev = kbdev->dev;
	struct hank_platform_data *pdata;

	pdata = devm_kzalloc(dev, sizeof(*pdata), GFP_KERNEL);
	if (!pdata)
		return -ENOMEM;
	kbdev->platform_context = pdata;

	pdata->rbus_base = ioremap(0x98000000, 0x200000);
	WARN_ON(!pdata->rbus_base);
	pdata->dev  = dev;

	hank_fpga_init(pdata);

	dev_info(dev, "%s: done\n", __func__);
	return 0;
}

static void kbase_platform_term(struct kbase_device *kbdev)
{
	struct hank_platform_data *pdata = kbdev->platform_context;

	iounmap(pdata->rbus_base);
	kbdev->platform_context = NULL;
}

struct kbase_platform_funcs_conf platform_funcs = {
	.platform_init_func = kbase_platform_init,
	.platform_term_func = kbase_platform_term,
};
