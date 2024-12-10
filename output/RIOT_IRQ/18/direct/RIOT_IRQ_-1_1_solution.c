#include <xtimer.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <irq.h>
#include <atomic.h>

// Atomic variable to keep track of interrupts status
atomic_t interrupt_status = 0;

// Simple function to simulate interrupt handling
void handle_interrupt(void) {
    printf("Interrupt handled!\n");
}

int main(void) {
    printf("Starting system...\n");

    // Disable interrupts and update atomic variable
    uint32_t state = irq_disable();
    atomic_store(&interrupt_status, 1);
    printf("Interrupts disabled.\n");

    // Simulate some processing
    xtimer_sleep(1);

    // Check if interrupts were disabled by checking the atomic variable
    if (atomic_load(&interrupt_status)) {
        handle_interrupt();
    }

    // Restore interrupts
    irq_restore(state);
    atomic_store(&interrupt_status, 0);
    printf("Interrupts restored.\n");

    return 0;
}