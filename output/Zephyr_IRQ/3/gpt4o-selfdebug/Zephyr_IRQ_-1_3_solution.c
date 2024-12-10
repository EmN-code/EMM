#include <zephyr/sys/util.h>  // Replacing atomic.h with another header
#include <zephyr/kernel.h>
#include <zephyr/irq.h>

int main(void) {
    volatile int irq_lock_key;
    
    // Disable interrupts
    irq_lock_key = irq_lock();

    // Simulate critical section by printing
    printk("Interrupts disabled\n");

    // Restore interrupts
    irq_unlock(irq_lock_key);
    printk("Interrupts restored\n");

    return 0;
}