
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
    // Set the atomic variable to true
    interrupt_handled = true;
}

// Thread function to simulate interrupt handling
void *interrupt_simulator(void *arg)
{
    (void)arg;

    // Simulate an interrupt by calling the handler
    interrupt_handler();

    return NULL;
}

int main(void)
{
    // Create a thread to simulate the interrupt
    kernel_pid_t pid = thread_create(
        thread_stack, sizeof(thread_stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        interrupt_simulator, NULL, "interrupt_simulator");

    // Disable interrupts
    unsigned state = irq_disable();

    // Check if the interrupt was handled while interrupts were disabled
    if (interrupt_handled) {
        printf("Interrupt was handled while interrupts were disabled!\n");
    } else {
        printf("Interrupt was not handled while interrupts were disabled.\n");
    }

    // Restore interrupts
    irq_restore(state);

    // Wait for the interrupt simulator thread to finish
    thread_join(pid);

    // Check the atomic variable after interrupts are restored
    if (interrupt_handled) {
        printf("Interrupt was handled after interrupts were restored.\n");
    } else {
        printf("Interrupt was not handled after interrupts were restored.\n");
    }

    return 0;
}
