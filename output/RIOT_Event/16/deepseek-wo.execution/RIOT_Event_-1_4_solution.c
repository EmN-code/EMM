
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <msg.h>
#include <thread.h>

#define MSG_QUEUE_SIZE 8

static msg_t msg_queue[MSG_QUEUE_SIZE];
static evtimer_msg_t evtimer;

void event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Event triggered!\n");
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    evtimer_init_msg(&evtimer);

    evtimer_msg_event_t event;
    event.event.handler = event_periodic_callback;
    event.event.arg = NULL;

    timex_t interval = timex_set(1, 0); // 1 second interval

    while (1) {
        evtimer_add_msg(&evtimer, &event, thread_getpid());
        xtimer_sleep(1); // Sleep for 1 second
    }

    return 0;
}
