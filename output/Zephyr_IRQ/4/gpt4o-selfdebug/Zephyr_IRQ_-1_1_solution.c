#include <zephyr/types.h>
#include <zephyr/kernel.h>
#include <zephyr/init.h>

/* Atomic variable for testing interrupts */
volatile atomic_t interrupt_occurred = (atomic_t)0;

/* ISR handler function */
void my_isr_handler(const void *arg) {
    /* Set the interrupt_occurred flag to 1 */
    atomic_set(&interrupt_occurred, 1);
}

/* Main function */
int main(void) {
    unsigned int key;

    /* Disable interrupts and save the interrupt state */
    key = irq_lock();

    /* Check that interrupts are disabled by attempting to set the atomic variable */
    if (atomic_get(&interrupt_occurred) == 0) {
        printk("Interrupts successfully disabled.\n");
    } else {
        printk("Failed to disable interrupts.\n");
    }

    /* Register ISR handler */
    irq_connect_dynamic(24, 0, my_isr_handler, NULL, 0);

    /* Enable interrupts by restoring the interrupt state */
    irq_unlock(key);

    /* Simulate interrupt trigger (this would normally be done by hardware) */
    irq_offload(my_isr_handler, NULL);

    /* Check if the ISR was executed and the atomic variable was set */
    if (atomic_get(&interrupt_occurred) == 1) {
        printk("Interrupts successfully enabled and ISR executed.\n");
    } else {
        printk("Failed to enable interrupts or execute ISR.\n");
    }

    return 0;
}
