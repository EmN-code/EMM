#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "thread.h"
#include "timex.h"
#include "msg.h"
#include "mbox.h"
#include "event/thread.h"
#include "event/callback.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define INTERVAL_SEC 1

evtimer_msg_event_t event;

void my_callback(event_t *event) {
    printf("Periodic event callback executed.\n");
}

int main(void) {
    evtimer_t evtimer;

    // Initialize the event timer for message-based events
    evtimer_init_msg(&evtimer);

    // Configure the event to be periodic every INTERVAL_SEC
    event.super.handler = my_callback;
    evtimer_add_msg(&evtimer, &event, INTERVAL_SEC * US_PER_SEC, NULL);

    // This is a simple infinite loop to keep the main thread alive,
    // In real applications, there might be more logic here.
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}