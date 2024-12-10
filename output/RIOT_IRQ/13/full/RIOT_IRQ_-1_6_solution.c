#include <ztimer.h>
#include <xtimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>

// Atomic variable used to verify if interrupts are enabled or disabled
atomic_bool interrupt_state;

// Simulate interrupt enable
void enable_interrupt(void) {
    atomic_store(&interrupt_state, true);
    puts("Interrupt enabled");
}

// Simulate interrupt disable
void disable_interrupt(void) {
    atomic_store(&interrupt_state, false);
    puts("Interrupt disabled");
}

int main(void) {
    // Initially disable interrupts
    disable_interrupt();
    
    // Check if interrupts are currently enabled
    if (!atomic_load(&interrupt_state)) {
        puts("Interrupts are disabled as expected");
    }
    
    // Enable interrupts
    enable_interrupt();

    // Verify if interrupts are enabled
    if (atomic_load(&interrupt_state)) {
        puts("Interrupts are enabled as intended");
    }

    return 0;
}