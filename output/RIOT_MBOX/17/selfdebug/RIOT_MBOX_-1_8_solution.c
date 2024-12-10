#include <stdio.h>
#include <msg.h>
#include <evtimer.h>
#include <event.h>

#define QUEUE_SIZE 8
static msg_t queue[QUEUE_SIZE];

static evtimer_t evtimer;
static evtimer_event_t event;

static void _event_handler(void *arg)
{
    (void)arg;
    printf("Event triggered!\n");
}

int main(void)
{
    msg_init_queue(queue, QUEUE_SIZE);

    evtimer_init(&evtimer, _event_handler);

    event.event.time = 5000000;  // 5 seconds
    evtimer_add(&evtimer, &event);

    while (1) {
        msg_t msg;
        msg_receive(&msg);  // Wait for messages
    }

    return 0;
}