#include <zephyr/types.h>
#include <zephyr/sys/util.h>
#include <zephyr/kernel.h>

void main(void) {
    // Initialize an atomic variable
    atomic_t irq_count = 0;

    // Disable interrupts and check the state
    unsigned int key = irq_lock();
    atomic_inc(&irq_count);
    printk("Interrupts disabled, count: %d\n", atomic_get(&irq_count));

    // Do some critical work
    k_busy_wait(USEC_PER_SEC);  // Simulates doing work for 1 second

    // Restore interrupts to their previous state
    irq_unlock(key);
    atomic_dec(&irq_count);
    printk("Interrupts restored, count: %d\n", atomic_get(&irq_count));

    while (1) {
        // Idle loop
        k_sleep(K_MSEC(1000));
    }
}