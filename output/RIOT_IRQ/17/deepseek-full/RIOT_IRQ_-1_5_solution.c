
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <unistd.h>
#include <timex.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <xtimer.h>
#include <ztimer.h>

// Define an atomic variable
static atomic_int atomic_var = ATOMIC_VAR_INIT(0);

// Function to handle interrupts
void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_fetch_add(&atomic_var, 1);
    printf("Interrupt handled, atomic_var incremented to %d\n", atomic_var);
}

int main(void)
{
    // Initialize the event timer
    evtimer_msg_t evtimer;
    evtimer_init_msg(&evtimer);

    // Set up the interrupt handler
    ztimer_set(ZTIMER_MSEC, &evtimer.timer, 1000);
    evtimer.handler = interrupt_handler;

    // Main loop to check the atomic variable
    while (1) {
        if (atomic_load(&atomic_var) > 0) {
            printf("Atomic variable verified, value: %d\n", atomic_var);
            atomic_store(&atomic_var, 0);
        }
        xtimer_sleep(1);
    }

    return 0;
}
