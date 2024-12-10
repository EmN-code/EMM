
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <mma8x5x_regs.h>
#include <MQTTClient.h>
#include <event/thread.h>
#include <thread.h>

// Define a simple atomic variable
volatile int atomic_var = 0;

// Function to simulate some work
void do_work(void) {
    for (int i = 0; i < 1000000; i++) {
        // Simulate work
    }
}

// Function to handle interrupts
void irq_handler(void) {
    atomic_var = 1; // Set atomic variable to indicate interrupt occurred
}

int main(void) {
    unsigned state;

    // Disable interrupts and save the state
    state = irq_disable();

    // Perform some work while interrupts are disabled
    do_work();

    // Check if the atomic variable was modified (should not be)
    if (atomic_var == 0) {
        printf("Interrupts were disabled correctly.\n");
    } else {
        printf("Interrupts were not disabled correctly.\n");
    }

    // Restore the interrupt state
    irq_restore(state);

    // Enable interrupts and trigger an interrupt
    irq_enable();
    irq_trigger();

    // Perform some work while interrupts are enabled
    do_work();

    // Check if the atomic variable was modified (should be)
    if (atomic_var == 1) {
        printf("Interrupts were enabled and handled correctly.\n");
    } else {
        printf("Interrupts were not handled correctly.\n");
    }

    return 0;
}
