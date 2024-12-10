
#include <stdlib.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <unistd.h>
#include <xtimer.h>
#include <thread.h>
#include <irq.h>

// Define a global atomic variable to test interrupt handling
atomic_int interrupt_flag = ATOMIC_VAR_INIT(0);

// Function to simulate an interrupt handler
void interrupt_handler(void)
{
    // Set the interrupt flag
    atomic_store(&interrupt_flag, 1);
}

int main(void)
{
    // Disable interrupts
    unsigned state = irq_disable();

    // Simulate some critical section code
    printf("Critical section: Interrupts are disabled\n");

    // Restore interrupts to the previous state
    irq_restore(state);

    // Enable interrupts
    irq_enable();

    // Simulate an interrupt occurring
    interrupt_handler();

    // Check if the interrupt flag was set
    if (atomic_load(&interrupt_flag) == 1) {
        printf("Interrupt was successfully handled\n");
    } else {
        printf("Interrupt handling failed\n");
    }

    // Yield to other threads
    thread_yield();

    return 0;
}
