
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <mma8x5x.h>
#include <stdatomic.h>
#include <irq.h>

// Define a custom interrupt handler
void custom_interrupt_handler(void) {
    printf("Interrupt handled!\n");
}

// Function to print the result
void print_result(bool success) {
    if (success) {
        printf("Interrupt handling test passed!\n");
    } else {
        printf("Interrupt handling test failed!\n");
    }
}

int main(void) {
    // Initialize atomic variable to track interrupt handling
    atomic_bool interrupt_handled = ATOMIC_VAR_INIT(false);

    // Disable interrupts
    unsigned state = irq_disable();

    // Set the custom interrupt handler
    set_custom_interrupt_handler();

    // Enable interrupts
    irq_restore(state);

    // Simulate some work
    printf("Simulating work...\n");

    // Check if the interrupt was handled
    if (atomic_load(&interrupt_handled)) {
        print_result(true);
    } else {
        print_result(false);
    }

    // Restore interrupts
    irq_restore(state);

    return 0;
}
