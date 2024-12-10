
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <xtimer.h>
#include <ztimer.h>
#include <irq.h>

atomic_int counter = ATOMIC_VAR_INIT(0);

void interrupt_handler(void)
{
    atomic_fetch_add(&counter, 1);
}

int main(void)
{
    unsigned state = irq_disable();

    // Simulate some work
    for (int i = 0; i < 1000000; i++) {
        // Busy loop
    }

    irq_restore(state);

    // Enable interrupts and set the handler
    irq_enable();
    irq_set_handler(IRQ_NUM, interrupt_handler);

    // Wait for some time to let the interrupt handler be called
    ztimer_sleep(ZTIMER_MSEC, 1000);

    // Check the counter value
    int final_count = atomic_load(&counter);
    printf("Counter value: %d\n", final_count);

    return 0;
}
