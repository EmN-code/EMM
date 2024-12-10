
#include "event.h"
#include "thread.h"
#include "ztimer.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static void periodic_callback(void *arg)
{
    (void)arg;
    puts("Periodic callback triggered");
}

int main(void)
{
    event_queue_t main_queue;
    event_periodic_callback_t periodic_event;

    /* Initialize the main message queue */
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    /* Initialize the event queue */
    event_queue_init(&main_queue);

    /* Initialize the periodic event */
    event_periodic_callback_init(&periodic_event, ZTIMER_USEC, &main_queue, periodic_callback, NULL);

    /* Start the periodic event with a 1-second interval */
    event_periodic_callback_start(&periodic_event, 1000000);

    /* Process events */
    while (1) {
        event_process(&main_queue);
    }

    return 0;
}
