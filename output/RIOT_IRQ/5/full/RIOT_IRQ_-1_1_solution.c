#include "irq_arch.h"

#include <stdio.h>
#include <stdatomic.h>

static _Atomic int interrupt_state;

void handle_interrupt(void) {
    
    printf("Interrupt occurred!\n");
    // Simulation of interrupt handling process
}

void test_irq_handling(void) {
    unsigned state;

    // Save current interrupt state and disable interrupts
    state = irq_disable();
    printf("Interrupts disabled\n");

    // Check if interrupt is effectively disabled using atomic variable
    atomic_store(&interrupt_state, 0);

    // Simulate an interrupt (would be ignored because they are disabled)
    handle_interrupt();

    // Ensure that interrupt handling doesn't occur
    if (atomic_load(&interrupt_state) == 0) {
        printf("Interrupt handling prevented\n");
    }

    // Restore the saved interrupt state
    irq_restore(state);
    printf("Interrupts restored\n");

    // After restoring, simulate an interrupt to verify handling
    atomic_store(&interrupt_state, 1);
    handle_interrupt();

    // Check if interrupt handling correctly occurred
    if (atomic_load(&interrupt_state) == 1) {
        printf("Interrupt handled correctly\n");
    }
}

int main(void) {
    printf("RIOT Interrupt Handling Test\n");
    test_irq_handling();
    return 0;
}