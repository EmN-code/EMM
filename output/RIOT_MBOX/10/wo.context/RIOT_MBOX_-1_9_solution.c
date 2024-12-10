#include "msg.h"
#include "evtimer.h"
#include "xtimer.h"
#include "stdio.h"

#define MSG_QUEUE_SIZE (8)
#define INTERVAL (1U * US_PER_SEC)

static msg_t msg_queue[MSG_QUEUE_SIZE];
static evtimer_t evtimer;
static evtimer_msg_event_t evtimer_event;

void timer_handler(void)
{
    msg_t msg;
    msg.content.value = 42;  // Arbitrary value to signify an event
    printf("Sending message with content value: %d\n", msg.content.value);
    msg_send(&msg, sched_active_thread());
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    evtimer_init_msg(&evtimer);

    evtimer_msg_event_init(&evtimer_event);
    evtimer_set(&timer, INTERVAL);
    evtimer_add_msg(&evtimer, &evtimer_event, sched_active_thread(), NULL);

    while (1) {
        msg_t incoming_msg;
        if (msg_receive(&incoming_msg) == 1) {
            printf("Received event with content value: %d\n", incoming_msg.content.value);
            xtimer_usleep(INTERVAL);
        }
    }

    return 0;
}