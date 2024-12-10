#include <zephyr/types.h>
#include <zephyr/kernel.h>
#include <zephyr/irq.h>

static atomic_t interrupt_counter;

/* Interrupt handler function */
void my_interrupt_handler(void)
{
    atomic_add(&interrupt_counter, 1);
    printk("Interrupt occurred! Counter: %d\n", atomic_get(&interrupt_counter));
}

int main(void)
{
    /* Disable interrupts */
    unsigned int key = irq_lock();
    printk("Interrupts disabled\n");

    /* Simulating some critical section code */
    k_busy_wait(1000);

    /* Register an interrupt handler */
    IRQ_CONNECT(DT_IRQN(DT_ALIAS(sw0)),
                DT_IRQ(DT_ALIAS(sw0), priority),
                my_interrupt_handler,
                NULL,
                DT_IRQ(DT_ALIAS(sw0), flags));

    /* Enable interrupts */
    irq_unlock(key);
    printk("Interrupts restored\n");

    while (1) {
        /* Busy-wait doing nothing */
        k_busy_wait(1000);
    }

    return 0;
}