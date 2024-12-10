#include "irq.h"
#include "atomic_utils.h"
#include "xtimer.h"
#include "thread.h"
#include <stdio.h>

static volatile uint32_t interrupt_counter = 0;
static volatile uint32_t atomic_var = 0;

void isr_handler(void *arg)
{
    (void)arg;
    /* Increment the interrupt counter atomically */
    atomic_fetch_add_u32(&interrupt_counter, 1);

    /* Perform some operations */
    atomic_fetch_add_u32(&atomic_var, 1);
    printf("ISR: interrupt_counter=%u, atomic_var=%u\n", interrupt_counter, atomic_var);
}

void init_interrupt(void)
{
    /* Disable interrupts */
    uint32_t state = irq_disable();

    /* Set up the interrupt handler */
    irq_add(IRQ_TIMER0, isr_handler, NULL);
    irq_enable(IRQ_TIMER0);

    /* Restore interrupts */
    irq_restore(state);
}

int main(void)
{
    /* Initialize the interrupt */
    init_interrupt();

    /* Simulate an interrupt */
    irq_trigger(IRQ_TIMER0);

    /* Wait for a while to let the interrupt handler execute */
    xtimer_sleep(1);

    /* Print the final values */
    printf("Main: interrupt_counter=%u, atomic_var=%u\n", interrupt_counter, atomic_var);

    return 0;
}