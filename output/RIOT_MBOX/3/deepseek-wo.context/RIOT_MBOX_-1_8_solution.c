
#include "msg.h"
#include "thread.h"
#include "ztimer.h"
#include "event.h"
#include "event/callback.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static void _event_callback(void *arg)
{
    (void)arg;
    puts("Event triggered!");
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    event_queue_t event_queue;
    event_queue_init(&event_queue);

    event_periodic_callback_t periodic_event;
    event_periodic_callback_create(&periodic_event, ZTIMER_MSEC, 1000, &event_queue, _event_callback);

    event_periodic_callback_start(&periodic_event);

    while (1) {
        event_loop(&event_queue);
    }

    return 0;
}
