
#include "xtimer.h"
#include "msg.h"
#include "kernel_defines.h"

#define PERIODIC_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static msg_t msg_queue[8];
static evtimer_t evtimer;

static void event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    msg_init_queue(msg_queue, ARRAY_SIZE(msg_queue));

    evtimer_init(&evtimer, event_periodic_callback, NULL);

    evtimer_event_t event;
    xtimer_t timer;

    timer.callback = (xtimer_callback_t)evtimer_event_callback;
    timer.arg = &event;

    while (1) {
        xtimer_set(&timer, PERIODIC_INTERVAL);
        msg_receive(&msg_queue[0]);
    }

    return 0;
}
