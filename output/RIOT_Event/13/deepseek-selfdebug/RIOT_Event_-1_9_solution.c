
#include "xtimer.h"
#include "event.h"
#include "thread.h"
#include "msg.h"

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

    xtimer_t timer;
    xtimer_init(&timer);

    event_t periodic_event;
    event_init(&periodic_event, periodic_event_cb, NULL);

    while (1) {
        xtimer_set(&timer, 1000000); // 1 second
        event_post(&event_queue, &periodic_event);
        xtimer_sleep(1);
    }

    return 0;
}
