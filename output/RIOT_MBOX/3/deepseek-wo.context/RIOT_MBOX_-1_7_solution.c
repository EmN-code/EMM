
#include <stdio.h>
#include <thread.h>
#include <msg.h>
#include <evtimer.h>
#include <ztimer.h>

#define MSG_QUEUE_SIZE 16
#define INTERVAL_MS 1000

static msg_t msg_queue[MSG_QUEUE_SIZE];
static evtimer_t evtimer;
static evtimer_msg_event_t ev_event;

void event_callback(void *arg)
{
    (void)arg;
    puts("Event triggered!");
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    evtimer_init(&evtimer);

    ev_event.event.callback = event_callback;
    ev_event.event.arg = NULL;

    ztimer_t interval = { .callback = evtimer_add_msg, .arg = &ev_event };
    ztimer_set(ZTIMER_MSEC, &interval, INTERVAL_MS);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        puts("Message received!");
    }

    return 0;
}
