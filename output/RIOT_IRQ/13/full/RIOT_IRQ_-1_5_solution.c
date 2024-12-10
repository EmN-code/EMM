#include "xtimer.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "network_thread.h"
#include "net_thread.h"

// Function to handle interrupts
void handle_interrupt(unsigned *state) {
    // Check if interrupts are enabled and disable them
    if (irq_is_enabled()) {
        *state = irq_disable();
    }
    
    // Simulate some critical section code execution
    printf("Interrupts are disabled, executing critical section.\n");

    // Restore the interrupts state
    irq_restore(*state);
    printf("Interrupts are restored.\n");
}

int main(void) {
    unsigned state;

    // Define network parameters
    Network network;
    int rc;

    while (1) {
        rc = NetworkConnect(&network, "192.168.1.1", 80);

        // Validate network connection attempt
        if (rc != 0) {
            printf("Network connection failed, retrying...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Network connected successfully.\n");
            break;
        }
    }

    // Proceed to handle interrupts after successful network connection
    handle_interrupt(&state);

    return 0;
}