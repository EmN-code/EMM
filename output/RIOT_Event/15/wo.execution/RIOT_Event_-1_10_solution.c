#include "xtimer.h"
#include "ztimer.h"
#include "event/thread.h"
#include "event/periodic.h"
#include "msg.h"
#include "stdio.h"

#define INTERVAL (1U * US_PER_SEC)

static void callback(void *arg)
{
    printf("Callback function executed every second.\n");
}

int main(void)
{
    evtimer_periodic_t periodic_event;
    evtimer_periodic_init(&periodic_event, INTERVAL, callback, NULL);
    evtimer_periodic_start(&periodic_event);
    
    // Keep the main function running
    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}