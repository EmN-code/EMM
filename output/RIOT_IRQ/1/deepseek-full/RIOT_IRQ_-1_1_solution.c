
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
    thread_create(NULL, 512, THREAD_PRIORITY_MAIN - 1, 0, worker_thread, NULL, "worker");

    // Disable interrupts
    unsigned state = irq_disable();

    // Set the interrupt handler
    irq_set_handler(interrupt_handler);

    // Simulate some work
    printf("Main thread running...\n");
    xtimer_sleep(2);

    // Check if the interrupt was handled while interrupts were disabled
    if (interrupt_handled) {
        printf("Interrupt was handled while interrupts were disabled!\n");
    } else {
        printf("Interrupt was not handled while interrupts were disabled.\n");
    }

    // Restore interrupts
    irq_restore(state);

    // Simulate some work
    printf("Main thread running again...\n");
    xtimer_sleep(2);

    // Check if the interrupt was handled after restoring interrupts
    if (interrupt_handled) {
        printf("Interrupt was handled after restoring interrupts.\n");
    } else {
        printf("Interrupt was not handled after restoring interrupts.\n");
    }

    return 0;
}
