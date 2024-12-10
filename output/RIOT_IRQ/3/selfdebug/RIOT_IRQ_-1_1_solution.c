#include "irq_arch.h"
#include <stdio.h>
#include <stdatomic.h>

// Atomic variable to demonstrate critical section handling
atomic_int shared_counter = 0;

// Function to demonstrate disabling and restoring interrupts
void critical_section_example(void) {
    unsigned state;

    // Disable interrupts and save the state
    state = irq_disable();

    // Perform critical section operations
    for (int i = 0; i < 1000; i++) {
        atomic_fetch_add(&shared_counter, 1);
    }

    // Restore interrupts
    irq_restore(state);
}

int main(void) {
    printf("Starting interrupt handling test\n");

    // Before entering the critical section
    printf("Counter before critical section: %d\n", shared_counter);

    critical_section_example();

    // After exiting the critical section
    printf("Counter after critical section: %d\n", shared_counter);

    return 0;
}