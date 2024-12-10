#include "ztimer.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "evtimer.h"

#define INTERVAL (1U * ZTIMER_MSEC_PER_SEC)

static void _callback(void *arg)
{
    (void)arg;
    printf("Callback triggered every second.\n");
}

int main(void)
{
    evtimer_t timer;
    evtimer_event_t event;

    evtimer_init(&timer, NULL);
    evtimer_set(&event, _callback, NULL);
    evtimer_add(&timer, &event, INTERVAL);

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, INTERVAL);
    }

    return 0;
}