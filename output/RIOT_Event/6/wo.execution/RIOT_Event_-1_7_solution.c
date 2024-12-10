#include "xtimer.h"
#include "ztimer.h"
#include "event.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "stdio.h"
#include "stdlib.h"

static evtimer_t evtimer;
static evtimer_event_t event;

void callback(void *arg) {
    printf("Event triggered\n");
}

int main(void) {
    puts("RIOT event timer example started");

    /* Initialize the event timer without extra arguments */
    evtimer_init(&evtimer, (evtimer_callback_t) callback);

    /* Create the event with a period of 1 second */
    evtimer_set(&evtimer, &event, 1000000);

    /* Start the timer */
    evtimer_event_wait(&event);

    return 0;
}