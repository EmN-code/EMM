
#include <stdlib.h>
#include <irq.h>
#include <thread.h>
#include <stdatomic.h>
#include <stdio.h>

atomic_int counter = ATOMIC_VAR_INIT(0);

void interrupt_handler(void)
{
    atomic_fetch_add(&counter, 1);
}

int main(void)
{
    unsigned state;

    // Disable interrupts
    state = irq_disable();

    // Simulate some work
    for (int i = 0; i < 1000000; i++) {
        // Busy loop
    }

    // Restore interrupts
    irq_restore(state);

    // Enable interrupt handling
    irq_set_exclusive_handler(ESP_INTR_NUM, interrupt_handler);
    irq_enable_interrupt(ESP_INTR_NUM);

    // Wait for interrupts to occur
    xtimer_sleep(5);

    // Print the counter value
    printf("Counter value: %d\n", atomic_load(&counter));

    return 0;
}
