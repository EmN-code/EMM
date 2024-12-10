
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

/* Define an atomic variable */
atomic_t counter = ATOMIC_INIT(0);

/* Interrupt handler function */
void irq_handler(void *arg)
{
    (void)arg;
    atomic_add(&counter, 1);
}

int main(void)
{
    /* Disable interrupts */
    unsigned state = irq_disable();

    /* Increment the counter atomically */
    atomic_add(&counter, 1);

    /* Restore interrupts */
    irq_restore(state);

    /* Set the interrupt handler */
    irq_set_handler(IRQ_NUM, irq_handler, NULL);

    /* Trigger the interrupt */
    irq_trigger(IRQ_NUM);

    /* Wait for the interrupt to be handled */
    thread_sleep();

    /* Check the counter value */
    if (atomic_get(&counter) == 2) {
        printf("Interrupt handling and atomic operations worked correctly.\n");
    } else {
        printf("Interrupt handling or atomic operations failed.\n");
    }

    return 0;
}
