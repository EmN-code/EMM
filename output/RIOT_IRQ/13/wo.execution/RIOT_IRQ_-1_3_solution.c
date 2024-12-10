#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "irq_arch.h"
#include "irq.h"
#include "kernel_defines.h"
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <architecture.h>
#include <stdatomic.h>
#include <stddef.h>

atomic_bool irq_flag = ATOMIC_VAR_INIT(false);

void irq_enable(void) {
    // Simulate enabling interrupts
    printf("Interrupts enabled\n");
    atomic_store(&irq_flag, true);
}

void irq_disable(void) {
    // Simulate disabling interrupts
    printf("Interrupts disabled\n");
    atomic_store(&irq_flag, false);
}

unsigned irq_restore(unsigned state) {
    // Simulate restoring interrupts based on previous state
    printf("Restoring interrupts state to: %u\n", state);
    if (state) {
        irq_enable();
    } else {
        irq_disable();
    }
    return state;
}

int main(void) {
    printf("Starting application...\n");
    // Initial state
    irq_enable();
    unsigned initial_state = atomic_load(&irq_flag);

    // Disable IRQ
    irq_disable();
    // Some critical section code could be here

    // Restore previous IRQ state
    irq_restore(initial_state);
    printf("Application finished.\n");
    return 0;
}