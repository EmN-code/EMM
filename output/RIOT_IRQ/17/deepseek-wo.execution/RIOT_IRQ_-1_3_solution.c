
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include "irq.h"

// Define a simple atomic variable
volatile int atomic_var = 0;

// Interrupt handler function
void irq_handler(void) {
    atomic_var++;
    printf("Interrupt handled, atomic_var incremented to: %d\n", atomic_var);
}

int main(void) {
    // Enable interrupts globally
    irq_enable();

    // Set the interrupt handler for the specific IRQ number
    irq_set_exclusive_handler(ESP32_IRQ_NUM, irq_handler);

    // Enable the specific interrupt
    irq_enable_interrupt(ESP32_IRQ_NUM);

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        printf("Main loop iteration: %d\n", i);
        // Trigger the interrupt (this is a simulation, actual triggering depends on hardware)
        irq_trigger(ESP32_IRQ_NUM);
        // Disable interrupts to check atomicity
        unsigned state = irq_disable();
        if (atomic_var != i + 1) {
            printf("Atomicity check failed!\n");
        }
        irq_restore(state);
    }

    // Disable the specific interrupt
    irq_disable_interrupt(ESP32_IRQ_NUM);

    return 0;
}
