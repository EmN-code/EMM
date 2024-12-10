
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
    event_init(&periodic_event, periodic_event_cb, NULL);

    xtimer_t periodic_timer;
    xtimer_init(&periodic_timer, &periodic_event);

    xtimer_set_periodic(&periodic_timer, 1 * US_PER_SEC, 1 * US_PER_SEC);

    while (1) {
        msg_receive(&_main_msg_queue[0]);
    }

    return 0;
}
