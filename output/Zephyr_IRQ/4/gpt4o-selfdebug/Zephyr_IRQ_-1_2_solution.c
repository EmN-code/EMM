#include <zephyr/types.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>

void irq_off(void);
void irq_restore(unsigned int key);

static atomic_t interrupt_counter;

// Function to disable interrupts
void irq_off(void) {
    unsigned int key;
    key = irq_lock();  // Lock interrupts
    atomic_inc(&interrupt_counter);  // Increment the atomic variable
    irq_unlock(key);   // Unlock interrupts
}

// Function to restore interrupts
void irq_restore(unsigned int key) {
    irq_unlock(key);  // Unlock interrupts
    atomic_dec(&interrupt_counter);  // Decrement the atomic variable
}

// Thread function to test interrupt handling
void test_interrupts(void) {
    unsigned int key;
    while (1) {
        key = irq_lock();  // Lock interrupts
        printk("Interrupts disabled\n");
        irq_unlock(key);  // Unlock interrupts
        printk("Interrupts restored\n");
        k_sleep(K_MSEC(1000));  // Sleep for 1000 ms
    }
}

int main(void) {
    k_thread_create(&thread_data, thread_stack, STACKSIZE,
                    (k_thread_entry_t)test_interrupts, NULL, NULL, NULL,
                    PRIORITY, 0, K_NO_WAIT);
    return 0;
}

K_THREAD_DEFINE(test_interrupts_thread, 1024, test_interrupts, NULL, NULL, NULL,
                7, 0, 0);