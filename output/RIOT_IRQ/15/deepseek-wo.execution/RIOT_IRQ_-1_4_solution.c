
#include <stdlib.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <xtimer.h>
#include <unistd.h>
#include <irq.h>

// Define an atomic variable to verify interrupt handling
atomic_int interrupt_flag = ATOMIC_VAR_INIT(0);

// Function to simulate an interrupt handler
void custom_interrupt_handler(void)
{
    // Set the interrupt flag
    atomic_store(&interrupt_flag, 1);
}

int main(void)
{
    // Disable interrupts initially
    unsigned state = irq_disable();

    // Simulate enabling an interrupt
    custom_interrupt_handler();

    // Restore the interrupt state
    irq_restore(state);

    // Check if the interrupt flag was set
    if (atomic_load(&interrupt_flag) == 1) {
        printf("Interrupt was handled correctly.\n");
    } else {
        printf("Interrupt handling failed.\n");
    }

    // Keep the program running
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
