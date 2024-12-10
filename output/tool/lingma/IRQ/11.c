#include "xtimer.h"
#include "irq.h"
#include "atomic.h"
#include "periph_cpu.h"
#include "periph_conf.h"

// Atomic variable to check interrupt status
atomic_t interrupt_flag = ATOMIC_VAR_INIT(0);

// Function to handle the interrupt
void handle_interrupt(void *arg) {
    (void)arg;
    // Set the atomic variable to indicate the interrupt occurred
    atomic_store(&interrupt_flag, 1);
}

// Function to enable the interrupt
void enable_interrupt(void) {
    // Enable the interrupt (example: using a timer interrupt)
    // Replace this with the actual interrupt source you want to use
    xtimer_set_interrupt(&handle_interrupt, NULL, 1000000, NULL);
}

// Function to disable the interrupt
void disable_interrupt(void) {
    // Disable the interrupt (example: using a timer interrupt)
    // Replace this with the actual interrupt source you want to use
    xtimer_remove_interrupt();
}

int main(void) {
    // Initialize the system
    xtimer_init();

    // Enable the interrupt
    enable_interrupt();

    // Main loop
    while (1) {
        // Check if the interrupt has occurred
        if (atomic_load(&interrupt_flag)) {
            // Clear the interrupt flag
            atomic_store(&interrupt_flag, 0);

            // Print a message indicating the interrupt was handled
            printf("Interrupt handled\n");

            // Disable the interrupt
            disable_interrupt();

            // Re-enable the interrupt after a delay
            xtimer_usleep(500000); // 500 ms
            enable_interrupt();
        }

        // Yield to other tasks
        thread_yield();
    }

    return 0;
}