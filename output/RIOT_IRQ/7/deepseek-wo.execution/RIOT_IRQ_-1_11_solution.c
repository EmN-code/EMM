
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "stdatomic.h"

atomic_int counter = ATOMIC_VAR_INIT(0);

void my_interrupt_handler(void)
{
    atomic_fetch_add(&counter, 1);
}

int main(void)
{
    unsigned state;

    // Disable interrupts
    state = irq_disable();

    // Set the interrupt handler
    irq_set_exclusive_handler(ESP32_IRQ_NUM, my_interrupt_handler);

    // Enable the interrupt
    irq_enable(ESP32_IRQ_NUM);

    // Restore interrupts
    irq_restore(state);

    // Simulate some work
    xtimer_sleep(1);

    // Check the counter value
    printf("Counter value: %d\n", atomic_load(&counter));

    return 0;
}
