
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

// Thread function to simulate work
void worker_thread(void)
{
    while (1) {
        printf("Worker thread running...\n");
        xtimer_sleep(1);
    }
}

int main(void)
{
    // Initialize the worker thread
    thread_create(NULL, 1024, THREAD_PRIORITY_MAIN - 1, 0, worker_thread, NULL, "worker");

    // Disable interrupts
    unsigned state = irq_disable();

    // Set the interrupt handler
    irq_set_handler(IRQ_NUM, interrupt_handler);

    // Enable interrupts
    irq_restore(state);

    // Simulate an interrupt by calling the handler directly
    interrupt_handler();

    // Verify if the interrupt was handled
    if (interrupt_handled) {
        printf("Interrupt was handled correctly.\n");
    } else {
        printf("Interrupt handling failed.\n");
    }

    // Main loop
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
