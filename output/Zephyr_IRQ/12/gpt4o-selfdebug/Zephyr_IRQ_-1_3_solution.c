#include <zephyr/types.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>

/* Description:
This program demonstrates interrupt handling using atomic variables on an ESP32 board running Zephyr OS.
Atomic variables are used to safely verify disabling and restoring of interrupts without using GPIO. */

/* Forward declarations */
void my_isr(int parameter);

/* Definition of atomic variable */
atomic_t my_atomic_var = ATOMIC_INIT(0);

/* Interrupt Service Routine (ISR) */
void my_isr(int parameter) {
    atomic_inc(&my_atomic_var);  // Increment the atomic variable
}

/* Main Function */
int main(void) {
    int key;

    /* Disable interrupts */
    key = irq_lock();
    
    /* Check atomic variable incremented in ISR */
    printk("Atomic variable before ISR: %d\n", (int)atomic_get(&my_atomic_var));

    /* Simulate interrupt calling */
    my_isr(0);

    /* Enable interrupts */
    irq_unlock(key);

    /* Verify if ISR executed */
    printk("Atomic variable after ISR: %d\n", (int)atomic_get(&my_atomic_var));

    return 0;
}