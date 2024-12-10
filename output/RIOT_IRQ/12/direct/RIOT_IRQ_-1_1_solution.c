#include <ztimer.h>
#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <irq.h>
#include <inttypes.h>
#include <stdatomic.h>

// Atomic variable to detect interrupt handling
atomic_int interrupt_occurred = 0;

// Interrupt service routine (ISR) simulation
void irq_handler(void *arg) {
    (void)arg;
    // Set the atomic flag to indicate interrupt handling
    atomic_store(&interrupt_occurred, 1);
}

int main(void) {
    // Initialize interrupt
    printf("Setting up interrupt test on ESP32 using RIOT\n");

    // Disable IRQs
    irq_disable();
    printf("Interrupts disabled\n");

    // Simulate an interrupt by calling the interrupt handler manually
    irq_handler(NULL);

    // Check if interrupts were ignored
    if (atomic_load(&interrupt_occurred) == 0) {
        printf("Interrupt handling works!\n");
    } else {
        printf("Interrupt failed, interrupt was not ignored.\n");
    }

    // Reset the atomic variable
    atomic_store(&interrupt_occurred, 0);

    // Enable IRQs
    irq_enable();
    printf("Interrupts enabled\n");

    // Simulate an interrupt by calling the interrupt handler manually again
    irq_handler(NULL);

    // Check if interrupts are being handled now
    if (atomic_load(&interrupt_occurred) == 1) {
        printf("Interrupt handling works properly when enabled.\n");
    } else {
        printf("Interrupt failed, interrupt was ignored when it shouldn't be.\n");
    }

    return 0;
}