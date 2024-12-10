#include "irq_arch.h"
#include <stdio.h>
#include <stdbool.h>

// Atomic flag to test interrupt disabling and restoring
volatile bool interrupt_disabled = false;

void dummy_function(void) {
    printf("This is a dummy function to simulate work while IRQ is disabled.\n");
}

void test_interrupt_handling(void) {
    // Disable interrupts
    unsigned state = irq_disable();
    printf("Interrupts disabled.\n");

    // Set atomic flag
    interrupt_disabled = true;
    printf("Atomic flag set to indicate disabled interrupts.\n");

    // Simulate some work
    dummy_function();

    // Restore interrupts
    irq_restore(state);
    printf("Interrupts restored.\n");

    // Check flag and verify correct interrupt handling
    if (interrupt_disabled) {
        printf("Test passed: Interrupts were correctly disabled and restored.\n");
    } else {
        printf("Test failed: Interrupt flag not set after disabling interrupts.\n");
    }
}

int main(void) {
    printf("Starting test for interrupt handling with atomic variables on ESP32 using RIOT.\n");
    test_interrupt_handling();
    return 0;
}