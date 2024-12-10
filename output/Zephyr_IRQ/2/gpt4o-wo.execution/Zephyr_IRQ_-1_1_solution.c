#include <zephyr/toolchain.h> 
#include <zephyr/types.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

/* Variable to hold the old interrupt lock key */
unsigned int key;

/* Atomic variable for interrupt status verification */
atomic_t interrupt_occurred;

/* Interrupt handler function */
void irq_handler(const void *param) {
    atomic_set(&interrupt_occurred, 1);
    printk("Interrupt occurred!\n");
}

int main(void) {
    /* Initialize atomic variable */
    atomic_set(&interrupt_occurred, 0);
    
    printk("Starting interrupt test on ESP32\n");
    
    /* Disable interrupts and save the key */
    key = irq_lock();
    printk("Interrupts disabled\n");
    
    /* Simulating some critical section */
    k_sleep(K_MSEC(1000));
    
    /* Forcefully call the interrupt handler */
    irq_handler(NULL);
    
    /* Check if interrupt was handled during disabled state */
    if (!atomic_get(&interrupt_occurred)) {
        printk("Interrupt was correctly deferred!\n");
    } else {
        printk("Unexpected interrupt occurrence!\n");
    }

    /* Restore interrupts */
    irq_unlock(key);
    printk("Interrupts restored\n");

    /* Reset interrupt_occurred for second test */
    atomic_set(&interrupt_occurred, 0);

    /* Now enable interrupt and check again */
    irq_handler(NULL);

    if (atomic_get(&interrupt_occurred)) {
        printk("Interrupt handled correctly after restoring!\n");
    } else {
        printk("Interrupt was not processed correctly!\n");
    }

    return 0;
}