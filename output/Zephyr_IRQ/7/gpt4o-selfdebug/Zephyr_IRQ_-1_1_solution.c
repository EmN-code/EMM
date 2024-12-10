#include <zephyr/tools.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/atomic.h>

#define MY_IRQ 6

volatile atomic_t irq_handled = ATOMIC_INIT(false);

void my_irq_handler(const void *args)
{
    /* Disable interrupts */
    unsigned int key = irq_lock();

    /* Critical section start */
    printk("Interrupt handled.\n");
    atomic_set(&irq_handled, true);
    /* Critical section end */

    /* Restore interrupts */
    irq_unlock(key);
}

void configure_irq(void)
{
    /* Assuming MY_IRQ is a mock IRQ to demonstrate */
    IRQ_CONNECT(MY_IRQ, 0, my_irq_handler, NULL, 0);
    irq_enable(MY_IRQ);
}

int main(void)
{
    printk("Configuring IRQ.\n");
    configure_irq();

    while(!atomic_get(&irq_handled)) {
        /* Waiting for the interrupt to be handled */
    }

    printk("IRQ has been handled successfully.\n");
    return 0;
}