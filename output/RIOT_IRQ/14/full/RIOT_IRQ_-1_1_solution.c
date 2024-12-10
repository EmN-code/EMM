#include <stdio.h>
#include "irq_arch.h"

static volatile unsigned counter = 0;

void perform_irq_operations(void) {
    unsigned state;
    // Disable interrupts and save the state
    state = irq_disable();

    // Critical section
    counter++;

    // Restore interrupts with the saved state
    irq_restore(state);
}

int main(void) {
    int attempts = 0;
    int max_attempts = 5;

    printf("Starting interrupt handling demonstration\n");

    while (attempts < max_attempts) {
        try {
            perform_irq_operations();
            printf("Successfully performed IRQ operations. Counter = %u\n", counter);
            break;
        } catch (const std::exception& e) {
            printf("Error in IRQ operations: %s\n", e.what());
            attempts++;
            if (attempts >= max_attempts) {
                printf("Max attempts reached. Exiting with failure.\n");
                return -1;
            }
            printf("Retrying IRQ operations. Attempt %d\n", attempts + 1);
        }
    }

    printf("Program completed successfully. Final counter value: %u\n", counter);
    return 0;
}