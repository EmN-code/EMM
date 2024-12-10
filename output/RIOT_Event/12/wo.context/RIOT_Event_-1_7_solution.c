#include "xtimer.h"
#include "stdio.h"
#include "evtimer.h"
#include "msg.h"

#define INTERVAL (1U * US_PER_SEC)

static void my_periodic_callback(evtimer_event_t *event)
{
    printf("Callback executed.\n");
}

int main(void)
{
    evtimer_t evtimer;
    evtimer_event_t event;

    msg_t queue[8];
    msg_init_queue(queue, 8);

    evtimer_init(&evtimer, _evtimer_msg_handler);

    event.callback = my_periodic_callback;
    event.period = INTERVAL;
    event.event.offset = 0;

    evtimer_add_msg(&evtimer, &event, sched_active_pid);

    while (1) {
        msg_t m;
        msg_receive(&m);
    }

    return 0;
}