
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static void periodic_event_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    event_queue_t event_queue;
    event_queue_init(&event_queue);

    event_t periodic_event;
    event_periodic_t periodic_event_data;

    periodic_event_data.interval = 1000000U; // 1 second in microseconds
    periodic_event_data.event.handler = periodic_event_callback;
    periodic_event_data.event.arg = NULL;

    event_periodic_init(&periodic_event, &periodic_event_data);
    event_periodic_start(&periodic_event);

    while (1) {
        event_t *event = event_get(&event_queue);
        if (event) {
            event->handler(event->arg);
        }
    }

    return 0;
}
