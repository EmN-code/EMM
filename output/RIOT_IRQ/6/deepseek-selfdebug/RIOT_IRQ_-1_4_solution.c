
#include "thread.h"
#include "irq.h"
#include "atomic_utils.h"
#include "xtimer.h"
#include <stdio.h>

atomic_int counter = ATOMIC_VAR_INIT(0);

void interrupt_handler(void)
{
    atomic_fetch_add(&counter, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Disable interrupts
    unsigned state = irq_disable();

    // Simulate an interrupt
    interrupt_handler();

    // Restore interrupts
    irq_restore(state);

    // Check the counter value
    int current_value = atomic_load(&counter);
    printf("Counter value after interrupt: %d\n", current_value);

    // Sleep for a while to simulate the program running
    xtimer_sleep(5);

    return 0;
}
