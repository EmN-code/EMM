#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "stdio.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define INTERVAL (1U * US_PER_SEC)  // 1 second

static evtimer_t evtimer;
static evtimer_event_t event;

void callback_function(void *arg) {
    (void)arg;
    printf("Callback fired every second!\n");
}

int main(void) {
    printf("Starting event timer example.\n");

    /* Initialize the event timer */
    evtimer_init(&evtimer, ZTIMER_MSEC, callback_function, NULL);

    /* Schedule periodic callback every 1 second */
    evtimer_add(&evtimer, &event);

    /* Start the loop to wait for event callbacks */
    while (1) {
        evtimer_set(&evtimer, &event.event, INTERVAL);
        evtimer_event_wait(&evtimer);
    }

    return 0;
}