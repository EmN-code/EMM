#include <stdio.h>  // Include for printf
#include "irq_arch.h" // Include for IRQ handling in RIOT
#include <stdatomic.h> // Include for atomic operations

atomic_int irq_disabled_count;

void irq_test_handler(void)
{
    // Simulate disabling of interrupts
    unsigned state = irq_enable();
    printf("Interrupts enabled, previous state: %u\n", state);
    printf("Performing critical operation with interrupts disabled.\n");

    // Simulate atomic operation
    atomic_fetch_add(&irq_disabled_count, 1);

    // Simulate restoring interrupts
    irq_restore(state);
    printf("Interrupts restored to state: %u\n", state);

    // Check if the critical section was accessed
    printf("IRQ disabled count: %d\n", irq_disabled_count);
}

int main(void)
{
    // Initialize the atomic variable
    atomic_init(&irq_disabled_count, 0);

    printf("Starting interrupt handling test.\n");

    // Call the test handler
    irq_test_handler();

    return 0;
}