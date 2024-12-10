#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/irq.h>
#include <zephyr/types.h>

static volatile atomic_t interrupt_counter = ATOMIC_INIT(0);

void dummy_irq_handler(const void *param) {
    ARG_UNUSED(param);
    atomic_inc(&interrupt_counter);
}

int main(void) {
    const unsigned int test_irq = 3; // Example IRQ line
    unsigned long flags;

    // Set interrupt handler
    irq_connect_dynamic(test_irq, 0, dummy_irq_handler, NULL, 0);

    // Enable the IRQ
    irq_enable(test_irq);

    // Disable interrupts and perform a critical operation
    flags = irq_lock();
    if (irq_is_enabled(test_irq)) {
        printk("Interrupts are not properly disabled.\n");
    }

    // Critical section here

    // Restore interrupts
    irq_unlock(flags);
    if (!irq_is_enabled(test_irq)) {
        printk("Interrupts are not properly restored.\n");
    }

    printk("Interrupts test completed, interrupt counter: %d\n", atomic_get(&interrupt_counter));

    return 0;
}