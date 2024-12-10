#include <ztimer.h> // Ensure correct timer utilities are included for handling time
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mutex.h>
#include <mbox.h>
#include <msg.h>
#include <kernel_defines.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <evtimer.h>
#include <esp_system.h>

// Define evtimer object and event
static evtimer_t evtimer;
static evtimer_event_t event1;

// Function to be called periodically
void periodic_callback(void *arg) {
    printf("Periodic event triggered\n");
    // Implement your recurring task here
}

int main(void) {
    // Initialize evtimer
    evtimer_init(&evtimer, periodic_callback);

    // Set the event structure
    event1.period = 1 * US_PER_SEC; // Set period to 1 second

    // Initialize and add event
    evtimer_add(&evtimer, &event1);

    while (1) {
        // Process events
        evtimer_periodic_run(&evtimer);

        // Perform other tasks or sleep to save CPU
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return 0;
}