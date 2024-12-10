#include <zephyr/toolchain.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/kernel.h>
#include <zephyr/kernel.h>

volatile atomic_t interrupt_handled;

void my_isr_handler(void *arg)
{
    ARG_UNUSED(arg);
    atomic_inc(&interrupt_handled);
    /* Code to handle interrupt */
}

int main(void)
{
    /* Initialize atomic variable */
    atomic_set(&interrupt_handled, 0);

    /* Disable interrupts */
    unsigned int key = irq_lock();

    /* Setup of interrupts and other initializations */
    
    /* Restore interrupts */
    irq_unlock(key);

    /* Main loop */
    while (1) {
        if (atomic_get(&interrupt_handled)) {
            /* Handle the occurrence of an interrupt */
            printk("Interrupt handled!\n");
            atomic_clear(&interrupt_handled);
        }
        k_sleep(K_MSEC(100));
    }

    return 0;
}