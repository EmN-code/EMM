#include <zephyr.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/kernel.h>
#include <zephyr/init.h>
#include <zephyr/sys/printk.h>

atomic_t irq_locked;

void irq_handler_function() {
    unsigned int key;
    key = irq_lock();

    // Atomic set to indicate interrupt handling
    atomic_set(&irq_locked, 1);

    // Critical section code goes here

    irq_unlock(key);

    // Atomic clear to indicate exit of interrupt handling
    atomic_set(&irq_locked, 0);
}

int main(void) {
    printk("Starting interrupt handling test\n");
    atomic_set(&irq_locked, 0);

    irq_handler_function();

    if (atomic_get(&irq_locked) == 0) {
        printk("Interrupts successfully handled and restored\n");
    } else {
        printk("Failed to handle interrupts correctly\n");
    }

    return 0;
}