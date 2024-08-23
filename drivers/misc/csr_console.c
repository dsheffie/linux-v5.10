// SPDX-License-Identifier: GPL-2.0
/*
 * Simple kernel console driver for STM devices
 * Copyright (c) 2014, Intel Corporation.
 *
 * STM console will send kernel messages over STM devices to a trace host.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/console.h>
#include <linux/slab.h>

static int csr_console_setup(struct console *c, char *opts);
static void csr_console_write(struct console *c, const char *buf, unsigned len);

static struct console csr_console = {
  .name = "csr_console",
  .write		= csr_console_write,
  .setup		= csr_console_setup,
  .flags		= CON_PRINTBUFFER,
  .index		= -1,
};

static int csr_console_setup(struct console *c, char *opts) {
	return 0;
}

static void csr_print(char *buf, int len) {
  int i;
  for(i = 0; i < len; i++) {
    while(csr_read(0xc03) != 0) {}
    csr_write(0xc03, buf[i]);
  }
}

static void csr_console_write(struct console *c, const char *buf, unsigned len) {
  csr_print(buf, len);
}

static void __exit csr_console_exit(void) {
  printk(KERN_INFO "HERE %s : %d\n", __PRETTY_FUNCTION__, __LINE__);  
  unregister_console(&csr_console);
}

static int __init csr_console_init(void) {
  printk(KERN_INFO "HERE %s : %d\n", __PRETTY_FUNCTION__, __LINE__);
  register_console(&csr_console);
  return 0;
}

module_init(csr_console_init);
module_exit(csr_console_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("csr_console driver");
MODULE_AUTHOR("David Sheffield");
