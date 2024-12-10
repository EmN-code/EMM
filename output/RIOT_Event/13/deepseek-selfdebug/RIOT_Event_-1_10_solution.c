
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

    xtimer_t timer;
    event_t event;

    event.handler = periodic_event_callback;
    event.arg = NULL;

    xtimer_init(&timer);
    xtimer_set_periodic(&timer, 1 * US_PER_SEC, (void (*)(void *))event_post, &event);

    while (1) {
        event_wait(&event);
    }

    return 0;
}
