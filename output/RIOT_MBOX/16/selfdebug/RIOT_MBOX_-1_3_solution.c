#include <ztimer.h>
#include <xtimer.h>
#include <stdio.h>
#include <event.h>
#include <evtimer.h>
#include <msg.h>

#define MSG_QUEUE_SIZE 8
static msg_t msg_queue[MSG_QUEUE_SIZE];

static void _evtimer_handler(evqueue_t *queue, const evtimer_event_t *event)
{
    printf("Timer expired. Event received.\n");
}

int main(void)
{
    evtimer_event_t event;
    evtimer_t evtimer;

    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    
    evtimer_init(&evtimer, _evtimer_handler);

    // Initialize event timer with a periodic event
    evtimer_add_periodic(&evtimer, &event, 1000000U);

    while (1) {
        msg_t msg;
        if (msg_receive(&msg)) {
            // Handle received message
        }
    }

    return 0;
}