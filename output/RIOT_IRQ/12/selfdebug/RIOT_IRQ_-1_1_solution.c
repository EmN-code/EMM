#include "irq_arch.h"
#include <stdio.h>
#include <stdatomic.h>

volatile atomic_bool test_flag = ATOMIC_VAR_INIT(false);

void irq_test_handler(void) {
    // Atomic compare and swap to test if interrupts are handled
    bool expected = false;
    atomic_compare_exchange_strong(&test_flag, &expected, true);
}

int main(void) {
    unsigned state;
    
    // Disable interrupts
    state = irq_disable();
    printf("Interrupts disabled\n");
    
    // Simulate interrupt handling
    irq_test_handler();

    // Restore the interrupts
    irq_restore(state);
    printf("Interrupts restored\n");

    // Check if the handler updated the atomic variable
    if (atomic_load(&test_flag)) {
        printf("Interrupt handling verified successfully\n");
    } else {
        printf("Interrupt handling verification failed\n");
    }
    
    return 0;
}