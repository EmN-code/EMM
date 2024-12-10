#include <xtimer.h>
#include <stdio.h>
#include <msg.h>
#include <evtimer.h>

#define MESSAGE_QUEUE_SIZE 8
#define TIMER_INTERVAL (1U * US_PER_SEC)

static msg_t msg_queue[MESSAGE_QUEUE_SIZE];
static evtimer_t evtimer;
static evtimer_msg_event_t event;

void timer_callback(void *arg)
{
    (void)arg;
    puts("Timer triggered an event");
    msg_t msg;
    msg_send(&msg, sched_active_pid);
}

int main(void)
{
    msg_init_queue(msg_queue, MESSAGE_QUEUE_SIZE);

    evtimer_init_msg(&evtimer);
    evtimer_msg_event_init(&event);

    xtimer_t timer = {
        .callback = timer_callback,
    };

    xtimer_set(&timer, TIMER_INTERVAL);

    while(1) {
        msg_t msg;
        msg_receive(&msg);
        puts("Main loop received a message");
    }

    return 0;
}