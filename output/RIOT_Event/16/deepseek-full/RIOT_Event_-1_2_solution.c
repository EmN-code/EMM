
#include "xtimer.h"
#include "timex.h"
#include "ztimer.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_t evtimer;
static mbox_t mbox;
static msg_t msg_array[8];

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    // Initialize the mailbox
    mbox_init(&mbox, msg_array, sizeof(msg_array) / sizeof(msg_array[0]));

    // Initialize the event timer
    evtimer_init_mbox(&evtimer);

    // Set up the periodic event
    evtimer_event_t event;
    event.event.handler = periodic_callback;
    event.event.arg = NULL;
    event.interval = xtimer_ticks32(PERIODIC_INTERVAL);

    // Add the periodic event to the event timer
    evtimer_add_event(&evtimer, &event);

    // Main loop to keep the application running
    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        msg_reply(&mbox, &msg, &msg);
    }

    return 0;
}
