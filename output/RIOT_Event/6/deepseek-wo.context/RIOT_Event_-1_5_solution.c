
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static void *event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Event periodic callback triggered\n");
    return NULL;
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    event_queue_t event_queue;
    event_queue_init(&event_queue);

    xtimer_t timer;
    timer.callback = (xtimer_callback_t)event_periodic_callback;
    timer.arg = NULL;

    xtimer_set(&timer, 1000000); // 1 second in microseconds

    while (1) {
        event_wait(&event_queue);
    }

    return 0;
}
