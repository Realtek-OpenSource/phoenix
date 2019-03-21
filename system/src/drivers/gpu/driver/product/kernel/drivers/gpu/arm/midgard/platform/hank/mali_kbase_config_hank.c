#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/reset.h>
#include <linux/pm_runtime.h>
#include <mali_kbase.h>
#include <mali_kbase_defs.h>
#include <mali_kbase_config.h>
#include "mali_kbase_config_platform.h"
#include "mali_kbase_config_hank.h"

int hank_gpu_power_on(struct kbase_device *kbdev)
{
	struct hank_platform_data *pdata = kbdev->platform_context;
	struct device *dev = pdata->dev;
	int ret;

	dev_dbg(pdata->dev, "%s\n", __func__);

	reset_control_deassert(pdata->rstc);
	clk_prepare_enable(kbdev->clock);

	/* we don't do ip power on here, let driver control it */

	udelay(200);
	pdata->power = 1;
	return 0;
}

int hank_gpu_power_off(struct kbase_device *kbdev)
{
	struct hank_platform_data *pdata = kbdev->platform_context;

	dev_dbg(pdata->dev, "%s\n", __func__);
	clk_disable_unprepare(kbdev->clock);
	reset_control_assert(pdata->rstc);
	pdata->power = 0;

	return 0;
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
	int err;

	pdata = devm_kzalloc(dev, sizeof(*pdata), GFP_KERNEL);
	if (!pdata)
		return -ENOMEM;
	kbdev->platform_context = pdata;

	pdata->dev  = dev;
	pdata->base = kbdev->reg;
	pdata->rstc = devm_reset_control_get(dev, NULL);
	if (IS_ERR_OR_NULL(pdata->rstc)) {
		dev_err(dev, "reset_control_get() returns %ld\n", PTR_ERR(pdata->rstc));
		return -EINVAL;
	}


	/*
	 * We set the rpm status as active when enable rpm, so we should finish
	 * the power on flow.
	 *
	 * the power items and current status
	 *  1. sram_pow [on] by genpd
	 *  2. iso_pow [on] by power-chain of sram_pow
	 *  3. clk_en [on] by mali power control
	 *  4. rstn [off]
	 *
	 * If hank_gpu_power_on() is used, the clk_prepare_enable has been
	 * execute twice. Undo once.
	 */
	hank_gpu_power_on(kbdev);
	clk_disable_unprepare(kbdev->clock);

	dev_info(dev, "%s: done\n", __func__);
	return 0;
}

static void kbase_platform_term(struct kbase_device *kbdev)
{
	struct hank_platform_data *pdata = kbdev->platform_context;

	kfree(pdata);
	kbdev->platform_context = NULL;
}

struct kbase_platform_funcs_conf platform_funcs = {
	.platform_init_func = kbase_platform_init,
	.platform_term_func = kbase_platform_term,
};
