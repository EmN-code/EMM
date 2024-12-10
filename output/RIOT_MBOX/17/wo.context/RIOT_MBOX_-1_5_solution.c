#include <xtimer.h>
#include <stdio.h>
#include <stdint.h>
#include <event/callback.h>

#define INTERVAL (500000U)

static void event_callback(void *arg) {
    (void)arg;
    printf("Event received!\n");
}

int main(void) {
    evtimer_t evtimer;
    evtimer_event_t event;
    
    /* Configure the event callback */
    event.callback = event_callback;

    /* Initialize the event timer */
    evtimer_init(&evtimer, _evtimer_mbox_handler);

    /* Set the timer to periodically trigger the event */
    evtimer_set_msg(&evtimer, &event, THREAD_PID_MAIN, INTERVAL);

    while (1) {
        /* This simulates other operations in the application */
        xtimer_sleep(1);
    }

    return 0;
}