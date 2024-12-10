
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <timex.h>
#include <xtimer.h>
#include <evtimer_msg.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <net/sock/util.h>
#include <ztimer.h>
#include <stdatomic.h>

// Define a simple atomic variable
static atomic_int atomic_var = ATOMIC_VAR_INIT(0);

// Function to simulate some work
void simulate_work(void) {
    printf("Simulating work...\n");
    xtimer_sleep(2); // Sleep for 2 seconds to simulate work
}

// Function to handle interrupts
void interrupt_handler(void) {
    printf("Interrupt occurred!\n");
    atomic_fetch_add(&atomic_var, 1); // Increment atomic variable
}

int main(void) {
    printf("Starting interrupt handling test...\n");

    // Disable interrupts
    unsigned state = irq_disable();

    // Set the interrupt handler
    irq_set_handler(UART_IRQ_NUM, interrupt_handler);

    // Enable interrupts
    irq_enable();

    // Simulate some work
    simulate_work();

    // Restore interrupts to their previous state
    irq_restore(state);

    // Check the atomic variable to verify if interrupts were handled correctly
    if (atomic_load(&atomic_var) > 0) {
        printf("Interrupts were handled correctly. Atomic variable: %d\n", atomic_load(&atomic_var));
    } else {
        printf("Interrupts were not handled correctly. Atomic variable: %d\n", atomic_load(&atomic_var));
    }

    return 0;
}
