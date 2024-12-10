#include "unistd.h"
#include "time_units.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdlib.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "nimble/nimble_npl.h"
#include "kernel_defines.h"
#include "irq_arch.h"
#include "irq.h"
#include "architecture.h"

atomic_int irq_status;

void handle_interrupt(void) {
    // Critical section code
    printf("Handling interrupt\n");
}

int main(void) {
    // Initialize atomic variable
    atomic_store(&irq_status, 0);

    // Disable interrupts and save state
    unsigned state = irq_disable();
    if (state) {
        atomic_store(&irq_status, 1);
        printf("Interrupts disabled\n");
    }

    // Perform critical operations while interrupts are disabled
    handle_interrupt();

    // Restore previous interrupt state
    if (atomic_load(&irq_status)) {
        irq_restore(state);
        printf("Interrupts restored\n");
    }

    // Simulate external API interaction with exception handling
    while (true) {
        int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc == 0) {
            printf("Connected to network successfully\n");
            break;
        } else {
            printf("Network connection failed, retrying...\n");
            usleep(1000000); // retry after 1 second
        }
    }

    return 0;
}

// Dummy function definition to simulate NetworkConnect
typedef struct { } network_t;
int NetworkConnect(network_t *network, const char *ip, int port) {
    // Simulate network connection attempt
    return -1; // Simulate failure
}