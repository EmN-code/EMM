#include "irq_arch.h"
#include <stdio.h>

volatile unsigned interrupts_disabled;
volatile unsigned restored_state;

void disable_interrupts(void) {
    interrupts_disabled = irq_disable();
}

void restore_interrupts(void) {
    irq_restore(restored_state);
}

int main(void) {
    printf("Starting interrupt handling test\n");

    // Initial state of the atomic variable before disabling interrupts
    interrupts_disabled = 0;
    restored_state = 0;

    // Disable interrupts
    disable_interrupts();
    printf("Interrupts disabled, state: %u\n", interrupts_disabled);

    // Here you could check the state of your application,
    // perform critical operations, etc.

    // Restore interrupts
    restore_interrupts();
    printf("Interrupts restored to state: %u\n", restored_state);

    return 0;
}