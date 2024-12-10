#include <ztimer.h>
#include <msg.h>
#include <event.h>
#include <stdio.h>

#define MSG_QUEUE_SIZE (8U)
#define TIMER_INTERVAL (1000000U) /* 1 second */

static evtimer_t evtimer;
static msg_t queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

void _evtimer_handler(evqueue_t *queue, evtimer_event_t *event)
{
    (void)queue;
    msg_t msg;
    msg.content.value = event->period;
    printf("Event occurred with period: %ld\n", msg.content.value);
    msg_send(&msg, mbox);
}

int main(void)
{
    mbox_init(&mbox, queue, MSG_QUEUE_SIZE);

    evtimer_event_t periodic_event;
    periodic_event.event.handler = _evtimer_handler;
    periodic_event.period = TIMER_INTERVAL;

    evtimer_init(&evtimer, _evtimer_handler);
    evtimer_add_periodic(&evtimer, &periodic_event);

    printf("Setup complete. Events will be sent every %lus.\n", TIMER_INTERVAL / 1000000);

    msg_t msg;
    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Received message with period: %ld\n", msg.content.value);
        }
    }

    return 0;
}