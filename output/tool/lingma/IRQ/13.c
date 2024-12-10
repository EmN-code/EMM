#include "xtimer.h"
#include "irq.h"
#include "atomic.h"
#include "periph/irq.h"

// Atomic variable to track interrupt status
atomic_t interrupt_triggered = ATOMIC_VAR_INIT(0);

// Interrupt handler function
void my_interrupt_handler(void *arg) {
    (void)arg; // Unused argument
    atomic_store(&interrupt_triggered, 1);
    xtimer_usleep(1000); // Simulate some work in the interrupt handler
}

// Function to enable the interrupt
void enable_my_interrupt(void) {
    // Enable the interrupt
    irq_enable(IRQ_NUMBER); // Replace IRQ_NUMBER with the actual interrupt number
}

// Function to disable the interrupt
void disable_my_interrupt(void) {
    // Disable the interrupt
    irq_disable(IRQ_NUMBER); // Replace IRQ_NUMBER with the actual interrupt number
}

int main(void) {
    // Initialize the interrupt
    irq_install(IRQ_NUMBER, my_interrupt_handler, NULL); // Replace IRQ_NUMBER with the actual interrupt number

    while (1) {
        // Disable interrupts
        irq_disable(IRQ_NUMBER);

        // Perform some critical section operations
        xtimer_usleep(1000); // Simulate some work in the critical section

        // Re-enable interrupts
        irq_enable(IRQ_NUMBER);

        // Check if the interrupt was triggered
        if (atomic_load(&interrupt_triggered)) {
            printf("Interrupt was triggered!\n");
            atomic_store(&interrupt_triggered, 0); // Reset the atomic variable
        }

        // Sleep for a while
        xtimer_sleep(1);
    }

    return 0;
}