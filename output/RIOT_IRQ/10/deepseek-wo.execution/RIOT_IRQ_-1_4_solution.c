
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <time_units.h>
#include <xtimer.h>
#include <ztimer.h>
#include <kernel_defines.h>
#include <timex.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>
#include <event/thread.h>

// Define a simple atomic variable
atomic_int atomic_var = ATOMIC_VAR_INIT(0);

// Function to simulate an interrupt handler
void interrupt_handler(void)
{
    atomic_fetch_add(&atomic_var, 1);
    printf("Interrupt handled, atomic_var incremented to: %d\n", atomic_var);
}

int main(void)
{
    // Save the current interrupt state
    unsigned int state = irq_disable();

    // Simulate some work with interrupts disabled
    printf("Disabling interrupts...\n");
    xtimer_sleep(1);

    // Restore the interrupt state
    irq_restore(state);
    printf("Interrupts restored.\n");

    // Trigger the interrupt handler
    interrupt_handler();

    // Verify the atomic variable
    if (atomic_load(&atomic_var) == 1) {
        printf("Interrupt handling verified successfully.\n");
    } else {
        printf("Interrupt handling verification failed.\n");
    }

    return 0;
}
