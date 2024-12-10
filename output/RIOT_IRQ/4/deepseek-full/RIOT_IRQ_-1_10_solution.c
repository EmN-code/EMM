
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include "xtimer.h"

// Define a simple atomic variable
atomic_int interrupt_flag = ATOMIC_VAR_INIT(0);

// Function to simulate an interrupt handler
void interrupt_handler(void)
{
    atomic_store(&interrupt_flag, 1);
}

int main(void)
{
    puts("RIOT interrupt handling example");

    // Simulate enabling interrupts
    puts("Enabling interrupts...");
    unsigned state = irq_disable();

    // Simulate an interrupt occurring
    puts("Simulating interrupt...");
    interrupt_handler();

    // Verify the interrupt flag
    if (atomic_load(&interrupt_flag) == 1) {
        puts("Interrupt flag set correctly.");
    } else {
        puts("Interrupt flag not set.");
    }

    // Simulate restoring interrupts
    puts("Restoring interrupts...");
    irq_restore(state);

    // Wait for a while to observe the state
    xtimer_sleep(2);

    return 0;
}
