#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/reset.h>
#include <linux/pm_runtime.h>
#include <mali_kbase.h>
#include <mali_kbase_defs.h>
#include <mali_kbase_config.h>
#include "mali_kbase_config_platform.h"
#include "mali_kbase_config_thor.h"

#define DEFAULT_GPU_DMC_MODE            0

static int thor_gpu_l2_bisr_enable(struct thor_platform_data *pdata)
{
	unsigned int val;
	unsigned int err = 0;
	int retry_cnt = 0;

	gpu_reg_write(pdata, BISR_MODE0, 0x00000001);
	gpu_reg_write(pdata, BISR_MODE1, 0x00000001);
	gpu_reg_write(pdata, BISR_MODE2, 0x00000001);
	gpu_reg_write(pdata, BISR_MODE3, 0x00000001);
	gpu_reg_write(pdata, BISR_MODE4, 0x00000001);

	gpu_reg_write(pdata, BISR0, 0x00000002);
	gpu_reg_write(pdata, BISR1, 0x00000002);
	gpu_reg_write(pdata, BISR2, 0x00000002);
	gpu_reg_write(pdata, BISR3, 0x00000002);
	gpu_reg_write(pdata, BISR4, 0x00000002);

	gpu_reg_write(pdata, BISR0, 0x00000003);
	gpu_reg_write(pdata, BISR1, 0x00000003);
	gpu_reg_write(pdata, BISR2, 0x00000003);
	gpu_reg_write(pdata, BISR3, 0x00000003);
	gpu_reg_write(pdata, BISR4, 0x00000003);

retry:
	retry_cnt++;
	if (retry_cnt > 200)
		return -ETIME;
	usleep_range(5000, 6000);
	val = gpu_reg_read(pdata, BISR_DONE0);
	if (~val & BIT(0))
		goto retry;
	err |= val;
	val = gpu_reg_read(pdata, BISR_DONE1);
	if (~val & BIT(0))
		goto retry;
	err |= val;
	val = gpu_reg_read(pdata, BISR_DONE2);
	if (~val & BIT(0))
		goto retry;
	err |= val;
	val = gpu_reg_read(pdata, BISR_DONE3);
	if (~val & BIT(0))
		goto retry;
	err |= val;
	val = gpu_reg_read(pdata, BISR_DONE4);
	if (~val & BIT(0))
		goto retry;
	err |= val;
	if (err & BIT(4))
		return -EFAULT;

	gpu_reg_write(pdata, BISR0, 0x00000007);
	gpu_reg_write(pdata, BISR1, 0x00000007);
	gpu_reg_write(pdata, BISR2, 0x00000007);
	gpu_reg_write(pdata, BISR3, 0x00000007);
	gpu_reg_write(pdata, BISR4, 0x00000007);

	gpu_reg_write(pdata, BISR_MODE0, 0x00000000);
	gpu_reg_write(pdata, BISR_MODE1, 0x00000000);
	gpu_reg_write(pdata, BISR_MODE2, 0x00000000);
	gpu_reg_write(pdata, BISR_MODE3, 0x00000000);
	gpu_reg_write(pdata, BISR_MODE4, 0x00000000);
	return 0;
}

int thor_gpu_power_on(struct kbase_device *kbdev)
{
	struct thor_platform_data *pdata = kbdev->platform_context;
	struct device *dev = pdata->dev;
	int ret;

	dev_dbg(pdata->dev, "%s\n", __func__);

	reset_control_deassert(pdata->rstc);
	clk_prepare_enable(kbdev->clock);

	/* we don't do ip power on here, let driver control it */

	/* force reset */
	reset_control_assert(pdata->rstc);
	clk_disable(kbdev->clock);
	clk_enable(kbdev->clock);
	reset_control_deassert(pdata->rstc);

	/* set dram access mode to dmc */
	if (pdata->dmc)
		gpu_reg_write(pdata, CTI_DMC_SEL, 0x00000003);

	/* workaround for THOR A00 */
	if (pdata->no_cg) {
#define SC_CLOCK_GATE_OVERRIDE       (1ul << 0)
#define SC_TEX_CLOCK_GATE_OVERRIDE   (1ul << 24)
		kbdev->hw_quirks_sc |= SC_CLOCK_GATE_OVERRIDE;
		kbdev->hw_quirks_sc |= SC_TEX_CLOCK_GATE_OVERRIDE;
	}

	ret = thor_gpu_l2_bisr_enable(pdata);
	if (ret) {
		dev_err(dev, "thor_gpu_l2_bisr_enable() returns %d\n", ret);
		return ret;
	}

	pdata->power = 1;
	return 0;
}

int thor_gpu_power_off(struct kbase_device *kbdev)
{
	struct thor_platform_data *pdata = kbdev->platform_context;

	dev_dbg(pdata->dev, "%s\n", __func__);
	clk_disable_unprepare(kbdev->clock);
	reset_control_assert(pdata->rstc);
	pdata->power = 0;

	return 0;
}

static ssize_t set_dmc(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct kbase_device *kbdev;
	struct thor_platform_data *pdata;
	unsigned int dmc_val;

	kbdev = to_kbase_device(dev);
	if (!kbdev)
		return -ENODEV;

	pdata = kbdev->platform_context;
	if (!pdata)
		return -EINVAL;

	if ((kstrtoint(buf, 0, &dmc_val) != 0) && (dmc_val != 0 || dmc_val != 1))
		return -EINVAL;

	pdata->dmc = dmc_val;
	return count;
}

static ssize_t show_dmc(struct device *dev, struct device_attribute *attr, char * const buf)
{
	struct kbase_device *kbdev;
	struct thor_platform_data *pdata;

	kbdev = to_kbase_device(dev);
	if (!kbdev)
		return -ENODEV;

	pdata = kbdev->platform_context;
	if (!pdata)
		return -EINVAL;

	return scnprintf(buf, PAGE_SIZE, "%u\n", pdata->dmc);
}

static DEVICE_ATTR(dmc, S_IRUGO | S_IWUSR, show_dmc, set_dmc);

static ssize_t set_no_cg(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct kbase_device *kbdev;
	struct thor_platform_data *pdata;
	unsigned int no_cg_val;

	kbdev = to_kbase_device(dev);
	if (!kbdev)
		return -ENODEV;

	pdata = kbdev->platform_context;
	if (!pdata)
		return -EINVAL;

	if ((kstrtoint(buf, 0, &no_cg_val) != 0) && (no_cg_val != 0 || no_cg_val != 1))
		return -EINVAL;

	pdata->no_cg = no_cg_val;
	return count;
}

static ssize_t show_no_cg(struct device *dev, struct device_attribute *attr, char * const buf)
{
	struct kbase_device *kbdev;
	struct thor_platform_data *pdata;

	kbdev = to_kbase_device(dev);
	if (!kbdev)
		return -ENODEV;

	pdata = kbdev->platform_context;
	if (!pdata)
		return -EINVAL;

	return scnprintf(buf, PAGE_SIZE, "%u\n", pdata->no_cg);
}

static DEVICE_ATTR(no_cg, S_IRUGO | S_IWUSR, show_no_cg, set_no_cg);

static struct attribute *thor_attrs[] = {
	&dev_attr_dmc.attr,
	&dev_attr_no_cg.attr,
	NULL,
};

static const struct attribute_group thor_attr_group = {
	        .attrs = thor_attrs,
};

static struct kbase_platform_config dummy_platform_config;

struct kbase_platform_config *kbase_get_platform_config(void)
{
	return &dummy_platform_config;
}

static int kbase_platform_init(struct kbase_device *kbdev)
{
	struct device *dev = kbdev->dev;
	struct thor_platform_data *pdata;
	int err;

	pdata = devm_kzalloc(dev, sizeof(*pdata), GFP_KERNEL);
	if (!pdata)
		return -ENOMEM;
	kbdev->platform_context = pdata;

	err = sysfs_create_group(&kbdev->dev->kobj, &thor_attr_group);
	if (err) {
		dev_err(dev, "SysFS group creation failed\n");
		return err;
	}
	pdata->inited |= INITED_SYSFS_GROUP;

	pdata->dmc  = DEFAULT_GPU_DMC_MODE;
	pdata->dev  = dev;
	pdata->base = kbdev->reg;
	pdata->rstc = devm_reset_control_get(dev, NULL);
	if (IS_ERR_OR_NULL(pdata->rstc)) {
		dev_err(dev, "reset_control_get() returns %ld\n", PTR_ERR(pdata->rstc));
		return -EINVAL;
	}

	/* workaround for THOR A00 */
	pdata->no_cg = 1;

	thor_gpu_power_on(kbdev);
	clk_disable_unprepare(kbdev->clock);

	dev_info(dev, "%s: done\n", __func__);
	return 0;
}

static void kbase_platform_term(struct kbase_device *kbdev)
{
	struct thor_platform_data *pdata = kbdev->platform_context;

	if (pdata->inited & INITED_SYSFS_GROUP)
		sysfs_remove_group(&kbdev->dev->kobj, &thor_attr_group);

	kfree(pdata);
	kbdev->platform_context = NULL;
}

struct kbase_platform_funcs_conf platform_funcs = {
	.platform_init_func = kbase_platform_init,
	.platform_term_func = kbase_platform_term,
};
