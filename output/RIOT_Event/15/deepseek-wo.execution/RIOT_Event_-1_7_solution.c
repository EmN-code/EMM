
#include "xtimer.h"
#include "evtimer.h"
#include "thread.h"
#include "msg.h"

#define PERIODIC_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static evtimer_t periodic_timer;
static msg_t periodic_msg;

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
    evtimer_add_msg(&periodic_timer, &periodic_msg, PERIODIC_INTERVAL);
}

int main(void)
{
    msg_t msg_array[8];
    msg_init_queue(msg_array, 8);

    evtimer_init(&periodic_timer, periodic_callback, NULL);
    evtimer_add_msg(&periodic_timer, &periodic_msg, PERIODIC_INTERVAL);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}
