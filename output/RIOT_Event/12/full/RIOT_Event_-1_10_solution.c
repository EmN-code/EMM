#include "xtimer.h"
#include "stdlib.h"
#include "stdio.h"
#include "event.h"
#include "evtimer.h"
#include "net.h"
#include "kernel.h"

#define INTERVAL (1LU * US_PER_SEC) // 1 second interval

// Dummy NetworkConnect function
int NetworkConnect(Network *n, char* address_ip, int port_number) {
    // Simulate successful connection
    return 0;
}

void on_timeout(void *arg) {
    (void)arg;
    printf("Periodic event triggered.\n");
}

int main(void) {
    printf("RIOT application started.\n");

    evtimer_t evtimer;
    evtimer_event_t event;

    // Initialize the evtimer with the correct handler
    evtimer_init(&evtimer, on_timeout);

    // Trigger event every INTERVAL (1 second)
    evtimer_add(&evtimer, &event, INTERVAL);

    // Network connection handling
    Network network;
    int rc;

    while (1) {
        rc = NetworkConnect(&network, "192.168.1.1", 1883);
        if (rc != 0) {
            printf("Connection failed, retrying...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Connected successfully!\n");
            break;
        }
    }

    // Main loop
    while (1) {
        evtimer_sleep(1);
    }

    return 0;
}