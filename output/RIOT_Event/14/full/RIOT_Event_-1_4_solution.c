#include <xtimer.h>
#include <ztimer.h>
#include <unistd.h>
#include <timex.h>
#include <thread.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdbool.h>
#include <msg.h>
#include <event/thread.h>
#include <event/callback.h>
#include <kernel_defines.h>
#include <time_units.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <mbox.h>

void event_callback(void *arg) {
    /* Callback function for handling events */
    printf("Event triggered\n");
}

int main(void) {
    printf("Initializing event timer...\n");
    
    evtimer_t my_timer;
    evtimer_event_t event;

    evtimer_init_msg(&my_timer);

    event.callback = event_callback;
    event.data = NULL;

    evtimer_add_event(&my_timer, &event, 1);

    while (1) {
        evtimer_run(&my_timer);
        xtimer_sleep(1);
    }

    return 0;
}