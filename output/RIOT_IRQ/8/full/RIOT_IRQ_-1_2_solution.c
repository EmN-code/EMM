#include "irq_arch.h"
#include "architecture.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdlib.h"
#include "irq.h"
#include "stdbool.h"

volatile atomic_bool interrupt_flag = false;

static void handle_interrupt(void) {
    // Set the flag to true when interrupt occurs
    atomic_store(&interrupt_flag, true);
}

static void simulate_external_event(void) {
    // Simulate an external event that triggers an interrupt
    handle_interrupt();
}

int main(void) {
    // Assume irq_init() is a function to initialize interrupts
    irq_enable();

    // Simulate an external event
    simulate_external_event();

    // Check if interrupt was handled
    if (atomic_load(&interrupt_flag)) {
        printf("Interrupt handled successfully!\n");
    } else {
        printf("Interrupt handling failed!\n");
    }

    return 0;
}