
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "mutex.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "net/sock/util.h"
#include "stdlib.h"
#include "net/sock/udp.h"
#include "sys/socket.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "arpa/inet.h"
#include "thread.h"
#include "net/emcute.h"
#include "unistd.h"
#include "stdatomic.h"
#include "esp_wifi.h"
#include "MQTTClient.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_t periodic_timer;
static evtimer_event_t periodic_event;

void periodic_callback(void *arg) {
    printf("Periodic callback triggered!\n");
    // Re-add the event to the timer to repeat the callback
    evtimer_add(&periodic_timer, &periodic_event);
}

int main(void) {
    // Initialize the event timer
    evtimer_init(&periodic_timer);

    // Set up the periodic event
    periodic_event.event.callback = periodic_callback;
    periodic_event.event.arg = NULL;

    // Add the event to the timer with the specified interval
    ztimer_set(ZTIMER_USEC, &periodic_event.event.timer, PERIODIC_INTERVAL);

    // Start the periodic timer
    evtimer_add(&periodic_timer, &periodic_event);

    // Main loop to keep the application running
    while (1) {
        // Yield to allow other threads to run
        thread_yield();
    }

    return 0;
}
