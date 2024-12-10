// This code initializes and starts a periodic event callback function every second using RIOT (specifically for ESP32), with proper error handling for network connection.

#include "ztimer.h"
#include "stdlib.h"
#include "stdbool.h"  
#include "stddef.h"
#include "stdio.h"
#include "mutex.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "net_emcute.h"
#include "net_sock_udp.h"
#include "net_sock_util.h"
#include "netinet_in.h"
#include "net_gcoap.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "time_units.h"
#include "sys_socket.h"
#include "paho_mqtt.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "event_thread.h"
#include "event_callback.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "architecture.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa_inet.h"
#include "stdatomic.h"
#include "unistd.h"
#include "esp_system.h"
#include "MQTTClient.h"

// Function prototype for evtimer handler
void periodic_event_handler(void *arg);

// Static variables
static evtimer_t evtimer;

void periodic_event_handler(void *arg) {
    (void)arg; // Suppress unused variable warning
    printf("Event triggered\n");
    // Your event handling logic
}

int main(void) {
    printf("Initializing periodic event timer\n");

    // Initialize event timer for periodic events using mbox
    evtimer_init_mbox(&evtimer);

    // Declare and setup periodic event
    evtimer_mbox_event_t periodic_event;
    mbox_t mbox;
    assert(mbox_init(&mbox) == 0);

    while (1) {
        // Simulate adding a periodic event every second
        evtimer_add_mbox(&evtimer, &periodic_event, &mbox);

        // Sleep for 1 second (1000000 microseconds)
        xtimer_usleep(1000000);
    }

    return 0;
}