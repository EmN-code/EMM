#include <xtimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <event/timeout.h>
#include <event/callback.h>
#include <event/event.h>
#include <kernel_defs.h>

#define INTERVAL (1U * US_PER_SEC)
#define QUEUE_SIZE (8)

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static evtimer_t evtimer;
static evtimer_msg_event_t event;

void event_callback(void *arg)
{
    (void)arg;
    printf("Event triggered!\n");
}

int main(void)
{
    msg_init_queue(msg_queue, QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);
    evtimer_init_mbox(&evtimer);

    event.event.timeout = INTERVAL;
    event.event.callback = event_callback;
    
    evtimer_add_mbox(&evtimer, &event, &mbox);

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}