#include "xtimer.h"
#include "irq.h"
#include "atomic.h"

// Define the interrupt handler
void my_irq_handler(void *arg) {
    (void)arg;
    atomic_t *flag = (atomic_t *)arg;

    // Atomically set the flag to indicate the interrupt occurred
    atomic_set(flag, 1);

    // Print a message to indicate the interrupt occurred
    printf("Interrupt occurred!\n");
}

// Main function
int main(void) {
    atomic_t flag = ATOMIC_INIT(0); // Initialize the atomic flag to 0

    // Enable the interrupt
    irq_enable(10); // Replace 10 with the actual interrupt number for your use case

    // Set the interrupt handler
    irq_set_handler(10, my_irq_handler, &flag); // Pass the flag as an argument

    // Disable interrupts
    irq_disable();

    // Simulate some work
    xtimer_usleep(1000000); // Sleep for 1 second

    // Re-enable interrupts
    irq_restore();

    // Check if the interrupt occurred
    if (atomic_read(&flag) == 1) {
        printf("Interrupt was handled successfully.\n");
    } else {
        printf("Interrupt was not handled.\n");
    }

    // Disable the interrupt
    irq_disable(10);

    return 0;
}