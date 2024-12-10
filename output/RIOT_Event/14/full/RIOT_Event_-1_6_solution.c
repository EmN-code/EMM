#include <xtimer.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stddef.h>
#include <ztimer.h>
#include <unistd.h>
#include <msg.h>
#include <kernel_defines.h>
#include <netinet/in.h>
#include <event/thread.h>
#include <event/callback.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <mbox.h>
#include <timex.h>
#include <paho_mqtt.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <MQTTClient.h>
#include <arpa/inet.h>
#include <thread.h>
#include <time_units.h>

void evtimer_callback(struct evtimer_event *event) {
    printf("Event timer callback triggered.\n");
}

int main(void) {
    evtimer_t my_event_timer;

    // Initialize event timer
    evtimer_init_msg(&my_event_timer);
    my_event_timer.callback = evtimer_callback;

    // Create an event
    evtimer_event_t my_event;
    my_event.event.timer = 0; // init evtimer_event_t

    // Set timer for 1 second (1,000,000 microseconds)
    uint32_t interval = 1000000;

    // Add event to evtimer
    evtimer_add_event(&my_event_timer, &my_event, interval);

    // Run evtimer, enter loop
    while (true) {
        evtimer_run(&my_event_timer);
        usleep(interval);
    }

    return 0;
}