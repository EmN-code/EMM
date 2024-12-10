#include "xtimer.h"
#include "evtimer.h"
#include "event.h"
#include <stdio.h>

static evtimer_event_t periodic_event;

void callback_function(void *arg)
{
    (void)arg;
    printf("Callback function executed!\n");
}

int main(void)
{
    /* Initialize the periodic event with the callback function */
    evtimer_init(&periodic_event, callback_function);
    periodic_event.period = 1000000; // 1 second in microseconds

    printf("Starting the periodic event\n");

    while (1) {
        /* Call the callback function every second */
        evtimer_add(&periodic_event);
        xtimer_sleep(1);
    }

    return 0;
}