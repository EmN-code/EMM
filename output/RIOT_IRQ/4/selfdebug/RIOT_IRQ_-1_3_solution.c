#include "thread_flags.h"  // For irq functions
#include "stdatomic.h"  // For atomic operations
#include <stdio.h>        // For standard input-output functions

// Global atomic variable
atomic_int interrupt_count;

// Function to simulate interrupt enabling and disabling
void simulate_interrupt_handling(void) {
    // Disable interrupts
    irq_disable();
    
    // Critical section, increment interrupt count
    atomic_store(&interrupt_count, atomic_load(&interrupt_count) + 1);

    // Re-enable interrupts
    irq_restore();
}

int main(void) {
    // Initialize the atomic variable
    atomic_init(&interrupt_count, 0);

    printf("Initial interrupt count: %d\n", atomic_load(&interrupt_count));

    // Simulate interrupt handling
    simulate_interrupt_handling();

    printf("Interrupt count after simulation: %d\n", atomic_load(&interrupt_count));

    return 0;
}