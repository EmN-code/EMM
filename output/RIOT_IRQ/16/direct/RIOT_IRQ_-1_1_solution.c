#include <xtimer.h> // For delay functions
#include <stdio.h> // For printf
#include <irq.h> // For interrupt handling
#include <atomic_ops.h> // For atomic operations

// Define a global atomic variable
atomic_t interrupt_disabled;

// Interrupt handler function
void irq_handler(void)
{
    // Atomically set the interrupt_disabled flag
    atomic_set(&interrupt_disabled, 1);

    // Simulate some work done in interrupt handler
    printf("Interrupt handler executed.\n");

    // Simulate end of work and clear flag
    atomic_clear(&interrupt_disabled);
}

// Function to simulate interrupt trigger
void trigger_interrupt(void)
{
    printf("Triggering interrupt...\n");
    irq_enable();
    irq_handler();
    irq_disable();
}

int main(void)
{
    // Initialize atomic variable
    atomic_init(&interrupt_disabled, 0);

    // Trigger the interrupt
    trigger_interrupt();

    // Check if the interrupt was handled correctly
    if (atomic_read(&interrupt_disabled) == 0) {
        printf("Interrupt successfully handled!\n");
    } else {
        printf("Interrupt handling failed!\n");
    }

    return 0;
}