
#include "xtimer.h"
#include "msg.h"
#include "evtimer.h"

#define PERIODIC_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static evtimer_t periodic_timer;
static evtimer_msg_event_t periodic_event;

static void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void) {
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    evtimer_init_msg(&periodic_timer);

    periodic_event.event.callback = periodic_callback;
    periodic_event.event.arg = NULL;

    periodic_event.msg.type = 0;
    periodic_event.msg.content.ptr = NULL;

    while (1) {
        evtimer_add_msg(&periodic_timer, &periodic_event, PERIODIC_INTERVAL, thread_get_active_pid());
        xtimer_sleep(1);
    }

    return 0;
}
