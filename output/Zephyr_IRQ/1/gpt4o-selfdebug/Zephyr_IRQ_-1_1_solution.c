#include <zephyr/types.h>
#include <zephyr/sys/util.h>
#include <zephyr/kernel.h>
#include <zephyr/init.h>

/* Atomic variable to verify interrupt disabling and restoring */
static atomic_t irq_key;
static atomic_t irq_disabled;

/* Interrupt handler function */
void my_irq_handler(void)
{
    atomic_set(&irq_disabled, 1);
    k_busy_wait(100); /* simulate some work in the handler */
    atomic_set(&irq_disabled, 0);
    printk("Interrupt handled and restored\n");
}

/* Function to disable interrupts, perform some processing, and then restore interrupts */
void disable_and_restore_irq(void) {
    /* Disable interrupts and save the state */
    unsigned int key = irq_lock();
    atomic_set(&irq_key, key);
    printk("Interrupts disabled\n");

    /* Perform some work while interrupts are disabled */
    k_busy_wait(200);

    /* Restore interrupts */
    irq_unlock(key);
    printk("Interrupts restored\n");
}


void main(void) {
    /* Initialize atomic variables */
    atomic_set(&irq_key, 0);
    atomic_set(&irq_disabled, 0);

    printk("Starting interrupt handling example\n");

    /* Simulate my_irq_handler being triggered */
    my_irq_handler();

    /* Disable and restore interrupts for testing */
    disable_and_restore_irq();
}
