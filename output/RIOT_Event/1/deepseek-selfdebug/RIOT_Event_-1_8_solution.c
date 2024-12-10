
#include "event/periodic_callback.h"
#include "ztimer.h"
#include "thread.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static void _periodic_callback(void *arg)
{
    (void)arg;
    puts("Periodic callback triggered");
}

int main(void)
{
    event_queue_t main_queue = { NULL, 0 };
    event_periodic_callback_t periodic_event;

    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    event_periodic_callback_init(&periodic_event, ZTIMER_MSEC, &main_queue, _periodic_callback, NULL);
    event_periodic_callback_set_interval(&periodic_event, 1000);
    event_periodic_callback_start(&periodic_event);

    while (1) {
        event_loop(&main_queue);
    }

    return 0;
}
