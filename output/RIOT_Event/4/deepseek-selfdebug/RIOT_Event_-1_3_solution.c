
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
    event_periodic_callback_t periodic_event;
    event_queue_t main_queue = { NULL };

    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    event_periodic_callback_init(&periodic_event, ZTIMER_USEC, &main_queue, _periodic_callback, NULL);
    event_periodic_callback_start(&periodic_event, 1000000);  // 1 second interval

    while (1) {
        msg_receive(&_main_msg_queue[0]);
    }

    return 0;
}
