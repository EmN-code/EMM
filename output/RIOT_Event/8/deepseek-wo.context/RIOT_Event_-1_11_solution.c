
#include "xtimer.h"
#include "evtimer.h"
#include "msg.h"

#define PERIODIC_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static evtimer_t periodic_timer;
static evtimer_msg_event_t periodic_event;

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    evtimer_init(&periodic_timer);

    periodic_event.event.callback = periodic_callback;
    periodic_event.event.arg = NULL;

    periodic_event.event.offset = 0;
    periodic_event.event.interval = PERIODIC_INTERVAL;

    evtimer_add_msg(&periodic_timer, &periodic_event, thread_getpid());

    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}
