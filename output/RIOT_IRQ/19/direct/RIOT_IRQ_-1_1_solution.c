#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <xtensa/core-macros.h>
#include <xtensa/config/core-isa.h>
#include "irq.h"
#include "xtimer.h"

volatile uint32_t atomic_var = 0;  // Atomic variable for demonstration

void isr_function(void) {
    // Increment the atomic variable to demonstrate interrupt handling
    atomic_var++;
}

int main(void) {
    printf("Starting ESP32 Interrupt Handling Example\n");

    // Setup the interrupt service routine (ISR)
    irq_disable();  // Disable all interrupts
    irq_connect_dynamic(XCHAL_TIMER0_INTERRUPT, ISR_FLAG, isr_function, NULL);
    irq_enable();  // Enable all interrupts

    // Simulate an interrupt event by manually invoking the ISR
    printf("Simulating an interrupt...\n");
    isr_function();

    if (atomic_var == 1) {
        printf("Interrupt handled successfully. Atomic var: %u\n", atomic_var);
    } else {
        printf("Interrupt handling failed. Atomic var: %u\n", atomic_var);
    }

    return 0;
}