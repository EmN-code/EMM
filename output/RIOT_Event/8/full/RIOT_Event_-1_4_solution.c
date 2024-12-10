#include <xtimer.h>
#include <ztimer.h>
#include <timex.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <kernel_defines.h>
#include <msg.h>
#include <mbox.h>
#include <event/thread.h>
#include <event/callback.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <MQTTClient.h>
#include <thread.h>
#include <time_units.h>

// Define a kernel PID for the event timer; it must be a valid short integer
static kernel_pid_t evtimer_pid = KERNEL_PID_UNDEF; // Set to undefined initially

// Define the event timer structure
evtimer_t evtimer;

// Callback function for periodic execution
void event_periodic(void *arg) {
    (void)arg;    // Suppress unused parameter warning
    printf("Periodic event triggered!\n");
}

int main(void) {
    puts("RIOT event timer example
");

    // Initialize the message bus
    evtimer_msg_event_t event;
    msg_t msg_queue[8];

    // Define and initialize the event queue
    msg_init_queue(msg_queue, 8);

    // Initialize the event timer with message handling
    evtimer_init_msg(&evtimer);

    // Start a periodic event every 1 second
    evtimer_pid = thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_STACKTEST, 
                                (void *)&event, "Periodic Event Thread");

    if (evtimer_pid == KERNEL_PID_UNDEF) {
        puts("[Error] Unable to create event thread");
        return 1;
    }

    // Attempt to add event to the timer
    int res = evtimer_add_msg(&evtimer, &event.super, evtimer_pid);
    if (res != 1) {
        puts("[Error] Failed to add event to evtimer");
        return 1;
    }

    // Continuously wait for messages and trigger events
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        msg_send(&msg, evtimer_pid);
        xtimer_sleep(1);
    }

    return 0;
}