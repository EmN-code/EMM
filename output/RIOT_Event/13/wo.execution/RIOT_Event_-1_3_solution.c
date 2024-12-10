#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "ztimer.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"

#define INTERVAL_SEC 1

static msg_t msg_queue[8];  // Define a message queue
static ztimer_clock_t *clock = ZTIMER_MSEC;

evtimer_event_t periodic_event;
mbox_t mbox;

void periodic_callback(void *arg) {
    (void)arg;
    puts("Periodic event triggered!");

    // Re-trigger the event
    ztimer_set(clock, &periodic_event.t.expiry, INTERVAL_SEC * MS_PER_SEC);
    evtimer_mbox_trigger(&periodic_event);
}

int main(void) {
    msg_init_queue(msg_queue, ARRAY_SIZE(msg_queue));

    // Initialize and setup mbox
    mbox_init(&mbox, msg_queue, ARRAY_SIZE(msg_queue));

    // Setup the evtimer structure
    evtimer_mbox_init(&periodic_event, periodic_callback, NULL, &mbox);

    periodic_event.t.expiry = ztimer_now(clock) + INTERVAL_SEC * MS_PER_SEC;

    // Add to evtimer and start it
    ztimer_set(clock, (ztimer_t *)&periodic_event, INTERVAL_SEC * MS_PER_SEC);
    evtimer_mbox_trigger(&periodic_event);

    return 0;
}