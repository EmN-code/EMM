#include "irq.h"
#include "irq_arch.h"
#include "mma8x5x_regs.h"
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void my_interrupt_handler(void) {
    // This simulates an interrupt handler function
    printf("Interrupt handled\n");
}

int main(void) {
    atomic_bool interrupt_disabled = ATOMIC_VAR_INIT(false);

    // Disable interrupts
    unsigned int state = irq_disable();
    atomic_store(&interrupt_disabled, true);
    printf("Interrupts disabled\n");

    // Simulate checking if interrupts are really disabled
    if (atomic_load(&interrupt_disabled)) {
        // Simulate calling the interrupt handler while interrupts are disabled
        printf("Simulating interrupt being ignored due to disabled state\n");
    } else {
        my_interrupt_handler();
    }

    // Restore interrupts
    irq_restore(state);
    atomic_store(&interrupt_disabled, false);
    printf("Interrupts restored\n");

    // Simulate checking if interrupts are enabled
    if (!atomic_load(&interrupt_disabled)) {
        my_interrupt_handler();
    }

    return 0;
}
