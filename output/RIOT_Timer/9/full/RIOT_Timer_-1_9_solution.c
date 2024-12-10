#include "xtimer.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "stddef.h"
#include "unistd.h"
#include "stdatomic.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "net/sock/udp.h"
#include "netinet/in.h"
#include "kernel_defines.h"
#include "event/evtimer_mbox.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "dht.h"
#include "dht_params.h"
#include "mutex.h"
#include "time_units.h"
#include "timex.h"
#include "mma8x5x_regs.h"

#define INTERVAL (1U) // Interval in seconds

static msg_t queue[8];

void *led_toggle_handler(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        // Simulate LED toggle (stub)
        printf("LED toggled\n");
    }
    return NULL;
}

int main(void) {
    // Initialize message queue
    msg_init_queue(queue, 8);
    
    // Create a thread for handling LED toggle
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  led_toggle_handler, NULL, "toggle_handler");

    // Event timer setup
    evtimer_t evtimer;
    evtimer_msg_event_t event;

    evtimer_init_msg(&evtimer);
    evtimer_mbox_event_init(&event, THREAD_CREATE_STACKTEST);

    // Main loop to send event every second
    while (1) {
        evtimer_add_msg(&evtimer, &event, sched_active_pid);
        printf("Event sent\n");
        sleep(INTERVAL);
    }

    return 0;
}