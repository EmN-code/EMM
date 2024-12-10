#include <zephyr/types.h>
#include <zephyr/toolchain.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>

/* Description of the code solution */

// The purpose of this code is to demonstrate interrupt handling using atomic variables 
// to check the correct disabling and restoring of interrupts on an ESP32 with Zephyr OS.
// We use 'irq_lock' to disable interrupts and 'irq_unlock' to restore them. Atomic variables 
// are used to ensure that operations related to interrupt status change are performed 
// correctly in a multithreaded environment.

static atomic_t interrupt_status = ATOMIC_INIT(0);

void test_interrupt_handling(void) {
    unsigned int key;
    
    // Lock interrupts
    key = irq_lock();
    
    // Critical section starts here
    if (!atomic_set(&interrupt_status, 1)) {
        printk("Interrupts are locked\n");
    }
    
    // Simulate some work in the critical section
    k_busy_wait(1000);
    
    // Critical section ends here
    
    // Unlock interrupts
    irq_unlock(key);

    // Check interrupt status after unlocking
    if (atomic_set(&interrupt_status, 0)) {
        printk("Interrupts are unlocked\n");
    }
}

int main(void) {
    printk("Starting interrupt handling test on ESP32\n");
    
    // Call the function to test interrupt handling
    test_interrupt_handling();

    return 0;
}