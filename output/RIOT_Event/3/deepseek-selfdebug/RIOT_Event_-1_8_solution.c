
#include "xtimer.h"
#include "event.h"
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
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    event_queue_t main_queue;
    event_queue_init(&main_queue);

    event_periodic_t periodic_event;
    event_periodic_callback_t periodic_cb_event;

    periodic_cb_event.periodic = periodic_event;
    periodic_cb_event.event.handler = (event_handler_t)periodic_callback;
    periodic_cb_event.event.arg = NULL;

    xtimer_periodic_event_init(&periodic_event, &periodic_cb_event.event, 1 * US_PER_SEC);

    while (1) {
        event_loop(&main_queue);
    }

    return 0;
}
