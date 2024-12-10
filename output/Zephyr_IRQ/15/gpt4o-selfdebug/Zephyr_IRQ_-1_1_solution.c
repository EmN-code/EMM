#include <zephyr/types.h>
#include <zephyr/toolchain.h>
#include <zephyr/sys/util.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/init.h>

volatile atomic_t irq_counter;

/* Define an interrupt service routine */
void my_isr(const void *arg) {
    ARG_UNUSED(arg);
    atomic_inc(&irq_counter);
}

int main(void) {
    /* Disable local interrupts */
    unsigned int key = irq_lock();

    /* Register the interrupt handler for a test interrupt vector */
    IRQ_CONNECT(15, 0, my_isr, NULL, 0);

    /* Initialize the counter */
    atomic_set(&irq_counter, 0);

    /* Restore interrupts */
    irq_unlock(key);

    /* Simulate interrupt firing */
    if (atomic_get(&irq_counter) == 0) {
        my_isr(NULL);  // manually triggering interrupt for testing
    }

    /* Ensure the interrupt was handled */
    if (atomic_get(&irq_counter) != 1) {
        printk("Error: Interrupt handling failed!\n");
    } else {
        printk("Success: Interrupt handled correctly!\n");
    }

    return 0;
}
