
#include <stdio.h>
#include <stdbool.h>
#include "irq.h"
#include "thread.h"
#include "xtimer.h"

// Atomic variable to verify interrupt handling
volatile bool interrupt_handled = false;

// Function to handle the interrupt
void interrupt_handler(void)
{
    interrupt_handled = true;
}

// Thread function to simulate some work
void worker_thread(void)
{
    while (1) {
        printf("Worker thread running...\n");
        xtimer_sleep(1);
    }
}

int main(void)
{
    // Create a worker thread
    thread_create(NULL, 512, THREAD_PRIORITY_MAIN - 1, 0, worker_thread, NULL, "worker");

    // Disable interrupts
    unsigned state = irq_disable();

    // Simulate some critical section code
    printf("Critical section: Disabling interrupts...\n");
    xtimer_sleep(2);

    // Verify interrupts are disabled
    if (irq_is_in()) {
        printf("Interrupts are disabled as expected.\n");
    } else {
        printf("Error: Interrupts are not disabled.\n");
    }

    // Restore interrupts
    irq_restore(state);

    // Verify interrupts are restored
    if (!irq_is_in()) {
        printf("Interrupts are restored as expected.\n");
    } else {
        printf("Error: Interrupts are not restored.\n");
    }

    // Set up the interrupt handler
    irq_set_handler(interrupt_handler);

    // Enable interrupts
    irq_enable();

    // Simulate an interrupt
    printf("Simulating an interrupt...\n");
    irq_set(true);

    // Wait for the interrupt to be handled
    while (!interrupt_handled) {
        xtimer_usleep(100);
    }

    // Verify the interrupt was handled
    if (interrupt_handled) {
        printf("Interrupt was handled successfully.\n");
    } else {
        printf("Error: Interrupt was not handled.\n");
    }

    return 0;
}
