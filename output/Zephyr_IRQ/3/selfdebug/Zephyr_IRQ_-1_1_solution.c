#include <zephyr/types.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/kernel.h>

// Define a simple atomic variable
atomic_t my_atomic_var = ATOMIC_INIT(0);

// Function to handle the interrupt
void my_interrupt_handler(void *arg) {
    // Atomic operation to increment
    atomic_inc(&my_atomic_var);
}

int main(void) {
    // Declare an irq lock key
    unsigned int key;
    
    // Lock the interrupt
    key = irq_lock();

    // Check and log the atomic variable value
    int value_before = atomic_get(&my_atomic_var);
    printk("Value before interrupt: %d\n", value_before);
    
    // Simulate an interrupt call
    my_interrupt_handler(NULL);

    // Unlock the interrupt
    irq_unlock(key);

    // Check and log the atomic variable value
    int value_after = atomic_get(&my_atomic_var);
    printk("Value after interrupt: %d\n", value_after);

    return 0;
}