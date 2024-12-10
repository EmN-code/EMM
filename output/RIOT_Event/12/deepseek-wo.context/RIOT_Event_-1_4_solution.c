
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static void periodic_event_cb(void *arg)
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
    event_periodic_init(&periodic_event, periodic_event_cb, NULL);

    event_periodic_add_periodic(&event_queue, &periodic_event, 1000000U);

    while (1) {
        event_loop(&event_queue);
    }

    return 0;
}
