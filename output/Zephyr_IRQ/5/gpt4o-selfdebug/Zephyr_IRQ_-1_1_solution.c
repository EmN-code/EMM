#include <zephyr/types.h>
#include <zephyr/toolchain.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/sys/slist.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/pm/state.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/init.h>

atomic_t interrupt_flag;

void interrupt_handler(void)
{
    /* Simulate interrupt handling */
    printk("Interrupt occurred!\n");
    atomic_set(&interrupt_flag, 1);  // Set the flag to indicate interrupt occurrence
}

int main(void)
{
    printk("Starting Zephyr Interrupt Handling Example\n");

    atomic_clear(&interrupt_flag, 0); // Initialize atomic variable

    unsigned int key;

    /* Simulate disabling interrupts */
    key = irq_lock();
    printk("Interrupts disabled\n");

    /* Check atomic variable before restoring interrupts */
    if (!atomic_get(&interrupt_flag)) {
        printk("No interrupt occurred while interrupts were disabled.\n");
    } else {
        printk("Interrupt was incorrectly fired while disabled!\n");
    }

    /* Simulate restoring interrupts */
    irq_unlock(key);
    printk("Interrupts enabled\n");

    /* Manually trigger an interrupt to test handling */
    interrupt_handler();

    /* Check atomic variable after interrupt */
    if (atomic_get(&interrupt_flag)) {
        printk("Interrupt handling verified!\n");
    }

    return 0;
}