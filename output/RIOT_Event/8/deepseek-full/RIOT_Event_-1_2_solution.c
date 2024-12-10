
#include <stdio.h>
#include "evtimer_mbox.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "ztimer.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static evtimer_t evtimer;
static evtimer_msg_t event;

void periodic_callback(void *arg) {
    (void)arg;
    puts("Periodic event triggered!");
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    // Initialize the event timer
    evtimer_init(&evtimer);

    // Set up the periodic event
    event.event.handler = (event_handler_t)periodic_callback;
    event.event.arg = NULL;
    event.offset = 0;
    event.period = ZTIMER_MSEC(1000); // 1 second period

    // Add the periodic event to the event timer
    evtimer_add_msg(&evtimer, &event);

    // Main loop to handle messages
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        // Handle the received message if needed
    }

    return 0;
}
