
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

    event_t periodic_event;
    event_periodic_t periodic_event_timer;

    event_periodic_init(&periodic_event_timer, &periodic_event, periodic_event_cb, NULL, 1000000U);
    event_periodic_start(&periodic_event_timer);

    while (1) {
        msg_receive(NULL);
    }

    return 0;
}
