#include "xtimer.h"
#include "irq.h"
#include "atomic.h"
#include "thread.h"

// Atomic variable to check if the interrupt handler has been called
atomic_t interrupt_triggered = ATOMIC_VAR_INIT(0);

// Function to handle the interrupt
void interrupt_handler(void *arg) {
    (void)arg;
    atomic_store(&interrupt_triggered, 1);
    puts("Interrupt triggered!");
}

// Main function
int main(void) {
    // Initialize the system
    puts("Starting interrupt handling example");

    // Enable global interrupts
    irq_enable();

    // Register the interrupt handler
    // Note: This is a simplified example. In a real scenario, you would register
    // the interrupt handler with the specific hardware peripheral.
    irq_register_handler(interrupt_handler, NULL);

    // Simulate an interrupt by manually triggering it
    puts("Simulating interrupt...");
    irq_trigger();

    // Wait for a short period to allow the interrupt to be processed
    xtimer_usleep(100000); // 100 ms

    // Check if the interrupt was handled
    if (atomic_load(&interrupt_triggered)) {
        puts("Interrupt was successfully handled");
    } else {
        puts("Interrupt was not handled");
    }

    // Disable global interrupts
    irq_disable();

    // Main loop
    while (1) {
        xtimer_usleep(1000000); // 1 second
    }

    return 0;
}