
#include <stdio.h>
#include <xtimer.h>
#include <thread.h>
#include <ztimer.h>
#include <event.h>

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static void *event_periodic_callback(void *arg)
{
    (void)arg;
    puts("Event periodic callback triggered");
    return NULL;
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    event_queue_t event_queue;
    event_queue_init(&event_queue);

    ztimer_t timer;
    ztimer_init(&timer, ZTIMER_MSEC, event_periodic_callback, NULL);

    while (1) {
        ztimer_set(&timer, 1000);
        event_loop(&event_queue);
    }

    return 0;
}
