#include <stdio.h>
#include <kernel_defines.h>
#include <xtimer.h>
#include <evtimer.h>
#include <msg.h>

#define INTERVAL 10U
#define ONE_SECOND (1000000U)

static evtimer_t evtimer;
static msg_t msg_queue[8];

static void _evtimer_callback(void *arg)
{
    (void)arg;
    puts("Event triggered!");
}

int main(void)
{
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    evtimer_init(&evtimer);
    evtimer_msg_event_t evtmsg;
    evtimer_msg_event_init(&evtmsg);

    evtimer_set(&evtimer, INTERVAL * ONE_SECOND, _evtimer_callback, NULL);
    evtimer_add_event(&evtimer, &evtmsg.super);

    while (1) {
        // Populate with your custom message processing logic
        msg_t msg;
        msg_receive(&msg);
    }
    
    return 0;
}