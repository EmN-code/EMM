
#include "xtimer.h"
#include "thread.h"
#include "mbox.h"
#include "evtimer_mbox.h"

#define MSG_QUEUE_SIZE (8)

static msg_t msg_queue[MSG_QUEUE_SIZE];
static evtimer_t evtimer;

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    evtimer_init_mbox(&evtimer);

    evtimer_event_t event;
    event.event.handler = (event_handler_t)periodic_callback;
    event.event.arg = NULL;

    xtimer_ticks32_t interval = xtimer_ticks_from_usec(1000000); // 1 second
    evtimer_add_absolute(&evtimer, &event, xtimer_now() + interval);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        // Handle other messages if needed
    }

    return 0;
}
