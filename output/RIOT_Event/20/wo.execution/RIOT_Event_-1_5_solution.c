#include "ztimer.h"
#include "xtimer.h"
#include "thread.h"
#include "msg.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "timex.h"
#include "stdbool.h"
#include "stderr.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdlib.h"
#include "time_units.h"
#include "unistd.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "mbox.h"

// Callback function for the event timer
void periodic_callback(void *arg) {
    printf("Periodic callback executed\n");
}

int main(void) {
    evtimer_t periodic_timer;
    evtimer_msg_event_t periodic_event;

    // Initialize the event timer with a callback
    evtimer_init_msg(&periodic_timer);

    // Set base time and callback
    periodic_timer.base = ztimer_now(ZTIMER_SEC);
    periodic_timer.callback = periodic_callback;

    // Configure event
    periodic_event.event.callback = periodic_callback;
    periodic_event.event.period = 1000000ULL; // 1000000 microseconds = 1 second

    // Add event to the timer
    evtimer_add_msg(&periodic_timer, &periodic_event, thread_getpid());

    // Run the event timer to handle events
    while (1) {
        evtimer_run(&periodic_timer);
        thread_yield();
    }

    return 0;
}