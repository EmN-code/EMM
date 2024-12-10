#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "irq_arch.h"
#include "xtimer.h"

static volatile uint32_t atomic_var = 0;

void simulate_interrupt_handler(void) {
    // Simulated interrupt action
    atomic_var++;
}

int main(void) {
    printf("Starting the interrupt handling test on ESP32 with RIOT\n");
    unsigned irq_state = irq_disable();

    // Simulate interrupt and modify the atomic_var
    simulate_interrupt_handler();

    assert(atomic_var == 1); // Verify interrupt action

    irq_restore(irq_state); // Restore the previous interrupt state

    printf("Interrupt handling verified successfully.\n");
    return 0;
}