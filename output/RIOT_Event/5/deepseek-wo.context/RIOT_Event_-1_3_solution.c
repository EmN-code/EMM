
#include "event/periodic_callback.h"
#include "thread.h"
#include "xtimer.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered\n");
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    event_periodic_t periodic;
    event_callback_t callback;

    event_callback_init(&callback, periodic_callback, NULL);
    event_periodic_init(&periodic, &callback, (1 * US_PER_SEC));

    event_periodic_start(&periodic);

    while (1) {
        msg_receive(&_main_msg_queue[0]);
    }

    return 0;
}
