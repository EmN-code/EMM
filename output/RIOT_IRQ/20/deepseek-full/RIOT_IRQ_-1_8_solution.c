
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <unistd.h>
#include <timex.h>
#include <xtimer.h>
#include <evtimer_msg.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <sys/socket.h>
#include <ztimer.h>

// Define a simple atomic variable
atomic_int atomic_var = ATOMIC_VAR_INIT(0);

// Function to simulate some work
void simulate_work(void) {
    for (int i = 0; i < 1000000; i++) {
        // Simulate some computational work
    }
}

// Function to handle interrupts
void interrupt_handler(void) {
    atomic_fetch_add(&atomic_var, 1);
}

int main(void) {
    // Initialize the timer for interrupt simulation
    evtimer_msg_t evtimer;
    evtimer_msg_init(&evtimer);

    // Set up the interrupt handler
    evtimer_msg_add_msg(&evtimer, (void (*)(void *))interrupt_handler, NULL);

    // Disable interrupts initially
    unsigned state = irq_disable();

    // Simulate some work while interrupts are disabled
    simulate_work();

    // Restore interrupts
    irq_restore(state);

    // Enable interrupts
    irq_enable();

    // Simulate some work while interrupts are enabled
    simulate_work();

    // Check the atomic variable to verify interrupt handling
    if (atomic_load(&atomic_var) > 0) {
        printf("Interrupts were handled correctly.\n");
    } else {
        printf("Interrupts were not handled correctly.\n");
    }

    return 0;
}
