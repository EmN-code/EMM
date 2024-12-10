#include <xtimer.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <event/thread.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <stdio.h>
#include <msg.h>
#include <stdlib.h>
#include <stddef.h>
#include <ztimer.h>
#include <unistd.h>
#include <evtimer_msg.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <time_units.h>
#include <thread.h>
#include <timex.h>

/* Callback function to be called every second */
static void callback(void *arg) {
    (void)arg;
    puts("Periodic event triggered.");
}

int main(void) {   
    evtimer_t evtimer;
    evtimer_msg_event_t msg_event;

    /* Initialize the event timer to use messages for notifying events */
    evtimer_init_msg(&evtimer);

    /* Setup the msg_event structure */
    msg_event.super.event.handler = callback;
    msg_event.super.event.handler_arg = NULL;
    msg_event.super.event.next = NULL;

    /* Add a periodic event that triggers every second */
    evtimer_add_msg(&evtimer, &msg_event.super.event, 1, 1, msg_pid);

    /* Run the scheduler */
    while (1) {
        evtimer_run();
        thread_yield();
    }
    return 0;
}