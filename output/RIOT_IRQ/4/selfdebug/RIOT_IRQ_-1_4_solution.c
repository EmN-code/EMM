#include "irq_arch.h"
#include <stdatomic.h>
#include <stdio.h>

int main(void) {
    atomic_int interrupt_counter = 0;

    // Atomically increment the interrupt counter
    atomic_fetch_add(&interrupt_counter, 1);

    // Disable interrupts
    unsigned state = irq_disable();

    // Simulate critical section
    printf("Critical section started\n");
    // Atomically increment the interrupt counter
    atomic_fetch_add(&interrupt_counter, 1);
    printf("Critical section ended\n");

    // Restore interrupts
    irq_restore(state);

    // Atomically increment the interrupt counter
    atomic_fetch_add(&interrupt_counter, 1);

    printf("Interrupt counter: %d\n", interrupt_counter);

    return 0;
}
