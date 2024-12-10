
#include "xtimer.h"
#include "event/periodic.h"
#include "thread.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    event_periodic_t periodic_event;
    event_periodic_event_t periodic_event_data;

    event_periodic_init(&periodic_event, ZTIMER_MSEC, periodic_callback, NULL);
    event_periodic_event_init(&periodic_event_data, &periodic_event);

    event_periodic_add_event(&periodic_event, &periodic_event_data, 1000);
    event_periodic_start(&periodic_event);

    return 0;
}
