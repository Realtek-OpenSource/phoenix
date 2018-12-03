#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x88b338af, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xf27041ec, __VMLINUX_SYMBOL_STR(param_ops_int) },
	{ 0xa7b6e309, __VMLINUX_SYMBOL_STR(usb_deregister) },
	{ 0x48d2dd69, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0xf5f7e7b2, __VMLINUX_SYMBOL_STR(usb_register_driver) },
	{ 0xb67520f0, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0xc0da7146, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0x5b17fde3, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0x2ee4f31b, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0xb566ae26, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0x29537c9e, __VMLINUX_SYMBOL_STR(alloc_chrdev_region) },
	{ 0x893fb9d9, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0x8e8140a, __VMLINUX_SYMBOL_STR(usb_driver_release_interface) },
	{ 0xac1a55be, __VMLINUX_SYMBOL_STR(unregister_reboot_notifier) },
	{ 0x7681946c, __VMLINUX_SYMBOL_STR(unregister_pm_notifier) },
	{ 0x3517383e, __VMLINUX_SYMBOL_STR(register_reboot_notifier) },
	{ 0x9cc4f70a, __VMLINUX_SYMBOL_STR(register_pm_notifier) },
	{ 0xdfd61436, __VMLINUX_SYMBOL_STR(_raw_write_unlock) },
	{ 0xa87ec259, __VMLINUX_SYMBOL_STR(__mutex_init) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0xd100acbd, __VMLINUX_SYMBOL_STR(_raw_write_lock) },
	{ 0x805ecc18, __VMLINUX_SYMBOL_STR(usb_driver_claim_interface) },
	{ 0xc3b8cd1b, __VMLINUX_SYMBOL_STR(usb_ifnum_to_if) },
	{ 0x275ef902, __VMLINUX_SYMBOL_STR(__init_waitqueue_head) },
	{ 0xde17c400, __VMLINUX_SYMBOL_STR(devm_kmalloc) },
	{ 0x11c8e026, __VMLINUX_SYMBOL_STR(usb_disable_autosuspend) },
	{ 0x999e8297, __VMLINUX_SYMBOL_STR(vfree) },
	{ 0x71c90087, __VMLINUX_SYMBOL_STR(memcmp) },
	{ 0xd6ee688f, __VMLINUX_SYMBOL_STR(vmalloc) },
	{ 0xd17355c6, __VMLINUX_SYMBOL_STR(release_firmware) },
	{ 0x887c1627, __VMLINUX_SYMBOL_STR(request_firmware) },
	{ 0x45032e3c, __VMLINUX_SYMBOL_STR(filp_close) },
	{ 0x91d33909, __VMLINUX_SYMBOL_STR(vfs_read) },
	{ 0xe0876e2, __VMLINUX_SYMBOL_STR(filp_open) },
	{ 0x7ce8fb5d, __VMLINUX_SYMBOL_STR(usb_get_from_anchor) },
	{ 0xf9a482f9, __VMLINUX_SYMBOL_STR(msleep) },
	{ 0x1a1431fd, __VMLINUX_SYMBOL_STR(_raw_spin_unlock_irq) },
	{ 0x3507a132, __VMLINUX_SYMBOL_STR(_raw_spin_lock_irq) },
	{ 0x48bf2a84, __VMLINUX_SYMBOL_STR(usb_interrupt_msg) },
	{ 0x16ece344, __VMLINUX_SYMBOL_STR(usb_control_msg) },
	{ 0x9b68001c, __VMLINUX_SYMBOL_STR(nonseekable_open) },
	{ 0x8e9b68fd, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0x6f29956c, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0xf4fa543b, __VMLINUX_SYMBOL_STR(arm_copy_to_user) },
	{ 0x1cfb04fa, __VMLINUX_SYMBOL_STR(finish_wait) },
	{ 0x344b7739, __VMLINUX_SYMBOL_STR(prepare_to_wait_event) },
	{ 0x1000e51, __VMLINUX_SYMBOL_STR(schedule) },
	{ 0x622598b1, __VMLINUX_SYMBOL_STR(init_wait_entry) },
	{ 0xfa2a45e, __VMLINUX_SYMBOL_STR(__memzero) },
	{ 0x13cfc419, __VMLINUX_SYMBOL_STR(skb_pull) },
	{ 0x28cc25db, __VMLINUX_SYMBOL_STR(arm_copy_from_user) },
	{ 0x88db9f48, __VMLINUX_SYMBOL_STR(__check_object_size) },
	{ 0x8adcc47d, __VMLINUX_SYMBOL_STR(usb_set_interface) },
	{ 0x676bbc0f, __VMLINUX_SYMBOL_STR(_set_bit) },
	{ 0xca54fee, __VMLINUX_SYMBOL_STR(_test_and_set_bit) },
	{ 0x5200c3b4, __VMLINUX_SYMBOL_STR(usb_scuttle_anchored_urbs) },
	{ 0xebbb558, __VMLINUX_SYMBOL_STR(usb_autopm_put_interface) },
	{ 0xd9632398, __VMLINUX_SYMBOL_STR(usb_autopm_get_interface) },
	{ 0x49ebacbd, __VMLINUX_SYMBOL_STR(_clear_bit) },
	{ 0x4205ad24, __VMLINUX_SYMBOL_STR(cancel_work_sync) },
	{ 0x2a3aa678, __VMLINUX_SYMBOL_STR(_test_and_clear_bit) },
	{ 0xd1f0a011, __VMLINUX_SYMBOL_STR(usb_kill_anchored_urbs) },
	{ 0x51d559d1, __VMLINUX_SYMBOL_STR(_raw_spin_unlock_irqrestore) },
	{ 0x598542b2, __VMLINUX_SYMBOL_STR(_raw_spin_lock_irqsave) },
	{ 0xb2d48a2e, __VMLINUX_SYMBOL_STR(queue_work_on) },
	{ 0x2d3385d3, __VMLINUX_SYMBOL_STR(system_wq) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0xd85cd67e, __VMLINUX_SYMBOL_STR(__wake_up) },
	{ 0x97d68d30, __VMLINUX_SYMBOL_STR(skb_push) },
	{ 0xda2dc9b6, __VMLINUX_SYMBOL_STR(__pskb_copy_fclone) },
	{ 0x4def3418, __VMLINUX_SYMBOL_STR(__alloc_skb) },
	{ 0xe4689576, __VMLINUX_SYMBOL_STR(ktime_get_with_offset) },
	{ 0x9d669763, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0x992d3709, __VMLINUX_SYMBOL_STR(skb_put) },
	{ 0x8f678b07, __VMLINUX_SYMBOL_STR(__stack_chk_guard) },
	{ 0x5df57a3f, __VMLINUX_SYMBOL_STR(kfree_skb) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x54e379d3, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0x6903ad6, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x526c3a6c, __VMLINUX_SYMBOL_STR(jiffies) },
	{ 0xf2b41037, __VMLINUX_SYMBOL_STR(usb_unanchor_urb) },
	{ 0x5bbaaf90, __VMLINUX_SYMBOL_STR(usb_free_urb) },
	{ 0x29b748dd, __VMLINUX_SYMBOL_STR(usb_submit_urb) },
	{ 0x17fa4707, __VMLINUX_SYMBOL_STR(usb_anchor_urb) },
	{ 0x12da5bb2, __VMLINUX_SYMBOL_STR(__kmalloc) },
	{ 0xfc9fa229, __VMLINUX_SYMBOL_STR(usb_alloc_urb) },
	{ 0x8e865d3c, __VMLINUX_SYMBOL_STR(arm_delay_ops) },
	{ 0x2e5810c6, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr1) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x5fc56a46, __VMLINUX_SYMBOL_STR(_raw_spin_unlock) },
	{ 0x9c0bd51f, __VMLINUX_SYMBOL_STR(_raw_spin_lock) },
	{ 0xb1ad28e0, __VMLINUX_SYMBOL_STR(__gnu_mcount_nc) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("usb:v0BDAp*d*dc*dsc*dp*icE0isc01ip01in*");
MODULE_ALIAS("usb:v13D3p*d*dc*dsc*dp*icE0isc01ip01in*");

MODULE_INFO(srcversion, "4C30D64B50E2AF8F5E434D2");
