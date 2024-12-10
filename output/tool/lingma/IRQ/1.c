#include <stdio.h>
#include <atomic.h>
#include <xtimer.h>
#include <irq.h>

// Define the interrupt handler function
void my_interrupt_handler(void *arg) {
    (void)arg;
    printf("Interrupt occurred!\n");
}

// Atomic variable to check if the interrupt was handled
atomic_t interrupt_handled = ATOMIC_INIT(0);

// Function to simulate an interrupt
void trigger_interrupt(void) {
    // Simulate an interrupt by calling the handler directly
    my_interrupt_handler(NULL);
}

int main(void) {
    // Initialize the atomic variable
    atomic_set(&interrupt_handled, 0);

    // Disable interrupts
    irq_disable();

    // Trigger the interrupt
    trigger_interrupt();

    // Check if the interrupt was handled while interrupts were disabled
    if (atomic_get(&interrupt_handled) == 0) {
        printf("Interrupt was not handled while interrupts were disabled.\n");
    } else {
        printf("Interrupt was handled while interrupts were disabled (unexpected).\n");
    }

    // Restore interrupts
    irq_restore();

    // Trigger the interrupt again
    trigger_interrupt();

    // Check if the interrupt was handled after restoring interrupts
    if (atomic_get(&interrupt_handled) == 1) {
        printf("Interrupt was handled after restoring interrupts.\n");
    } else {
        printf("Interrupt was not handled after restoring interrupts (unexpected).\n");
    }

    // Wait for a while to observe the output
    xtimer_sleep(5);

    return 0;
}