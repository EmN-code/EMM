
#include "event/periodic_callback.h"
#include "event/callback.h"
#include "ztimer.h"
#include "thread.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_queue_t main_queue;
    event_periodic_callback_t periodic_event;

    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    event_queue_init(&main_queue);

    event_periodic_callback_create(&periodic_event, ZTIMER_MSEC, 1000, &main_queue, periodic_callback, NULL);
    event_periodic_callback_start(&periodic_event, 1000);

    while (1) {
        event_queue_dispatch(&main_queue);
    }

    return 0;
}
