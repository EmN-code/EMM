#include <zephyr/types.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/kernel.h>

#define MY_STACK_SIZE 512
#define MY_PRIORITY 5

atomic_t irq_disabled;

void irq_handler(const void *parameter) {
  printk("Interrupt occurred\n");
  if (atomic_get(&irq_disabled)) {
    printk("IRQ already disabled\n");
  } else {
    atomic_set(&irq_disabled, 1);
    printk("Disabling IRQ\n");
    irq_disable(IRQ_NUM);
  }
}

void main(void) {
  atomic_set(&irq_disabled, 0);

  // Simulate an interrupt to test handling
  irq_handler(NULL);
  
  // Post interrupt processing logic
  if (atomic_get(&irq_disabled)) {
    printk("Restoring IRQ\n");
    irq_enable(IRQ_NUM);
    atomic_set(&irq_disabled, 0);
  }
}