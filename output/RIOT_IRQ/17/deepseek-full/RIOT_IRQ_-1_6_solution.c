
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <unistd.h>
#include <evtimer_msg.h>
#include <kernel_defines.h>
#include <timex.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <dht_params.h>
#include <time_units.h>
#include <event/thread.h>
#include <MQTTClient.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <paho_mqtt.h>
#include <dht.h>
#include <stddef.h>
#include <ztimer.h>

// Define an atomic variable to verify interrupt handling
static atomic_int atomic_var = ATOMIC_VAR_INIT(0);

// Event timer for handling interrupts
static evtimer_msg_t evtimer;

// Function to handle the interrupt event
static void _interrupt_handler(void *arg)
{
    (void)arg;
    atomic_fetch_add(&atomic_var, 1);
    printf("Interrupt handled, atomic_var incremented to %d\n", atomic_var);
}

int main(void)
{
    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Set up the interrupt handler
    evtimer_add_msg(&evtimer, _interrupt_handler, NULL, 0);

    // Simulate an interrupt by triggering the event timer
    evtimer_add_msg(&evtimer, _interrupt_handler, NULL, 1000); // Trigger after 1 second

    // Main loop to monitor the atomic variable
    while (true) {
        if (atomic_var > 0) {
            printf("Atomic variable is greater than 0, interrupt handling verified.\n");
            break;
        }
        usleep(100000); // Sleep for 100ms
    }

    return 0;
}
