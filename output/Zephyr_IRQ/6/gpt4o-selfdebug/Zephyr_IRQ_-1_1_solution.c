#include <zephyr/types.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>

void interrupt_handler(const void *arg) {
    printk("Interrupt triggered!\n");
    // Perform interrupt-related operations here.
}

void configure_interrupt(void) {
    int key;
    atomic_t interrupt_status = ATOMIC_INIT(false);
    
    // Lock interrupts
    key = irq_lock();

    // Simulate critical section
    printk("Critical section start.\n");
    atomic_set(&interrupt_status, true);
    printk("Interrupt status set to %d\n", atomic_get(&interrupt_status));
    
    // Unlock interrupts
    irq_unlock(key);
    printk("Critical section end.\n");
    printk("Interrupts restored.\n");
}

int main(void) {
    printk("ESP32 Zephyr Interrupt Handling Application.\n");
    configure_interrupt();
    return 0;
}