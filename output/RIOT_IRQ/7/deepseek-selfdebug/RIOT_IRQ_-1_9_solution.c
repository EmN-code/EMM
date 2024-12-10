
#include <stdio.h>
#include "irq.h"
#include "thread.h"
#include "atomic_utils.h"

atomic_t counter = ATOMIC_INIT(0);

void irq_handler(void *arg)
{
    atomic_inc(&counter);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Disable interrupts
    unsigned state = irq_disable();

    // Set the interrupt handler
    irq_set_handler(IRQ_NUM, irq_handler, NULL);

    // Enable interrupts
    irq_restore(state);

    // Simulate some work
    for (int i = 0; i < 1000000; i++) {
        // Busy loop
    }

    // Check the counter value
    printf("Counter value: %d\n", atomic_get(&counter));

    return 0;
}
