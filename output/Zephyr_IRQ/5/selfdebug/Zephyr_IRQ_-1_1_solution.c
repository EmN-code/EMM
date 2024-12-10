#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/sys/atomic.h>

void main(void) {
    struct k_sem sync_sem;
    atomic_val_t atomic_var = 0;
    unsigned int key;

    // Initialize semaphore used for synchronization
    k_sem_init(&sync_sem, 0, UINT_MAX);

    /*
     * Simulate start of a critical section by locking interrupts.
     * This is typically where shared data would be accessed.
     */
    key = irq_lock();

    // Simulate critical section: modify atomic variable
    atomic_set(&atomic_var, 1);

    // End of critical section, restore previous interrupt state
    irq_unlock(key);

    // Infinite loop or other application specific tasks...
    while (1) {
        k_sleep(K_MSEC(1000));
    }
}
