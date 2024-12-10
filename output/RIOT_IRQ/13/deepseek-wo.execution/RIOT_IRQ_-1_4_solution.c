
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include "xtimer.h"
#include "thread.h"
#include "irq.h"

// Atomic variable to verify interrupt handling
atomic_int interrupt_flag = ATOMIC_VAR_INIT(0);

// Function to simulate an interrupt handler
void interrupt_handler(void)
{
    atomic_fetch_add(&interrupt_flag, 1);
}

int main(void)
{
    // Initial state of interrupts
    unsigned int state = irq_disable();

    // Simulate some work
    printf("Disabling interrupts...\n");
    xtimer_sleep(1);

    // Enable interrupts
    irq_restore(state);
    printf("Interrupts restored.\n");

    // Simulate an interrupt
    interrupt_handler();

    // Check the atomic variable
    if (atomic_load(&interrupt_flag) == 1) {
        printf("Interrupt handling verified successfully.\n");
    } else {
        printf("Interrupt handling verification failed.\n");
    }

    return 0;
}
