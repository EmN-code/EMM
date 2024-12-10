
#include "event/periodic.h"
#include "ztimer.h"
#include "thread.h"
#include "xtimer.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static void _periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered\n");
}

int main(void)
{
    event_periodic_callback_t periodic_event;
    event_queue_t main_event_queue;

    /* Initialize the main message queue */
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    /* Initialize the main event queue */
    event_queue_init(&main_event_queue);

    /* Initialize the periodic event with the correct number of arguments */
    event_periodic_callback_init(&periodic_event, ZTIMER_USEC, &main_event_queue, _periodic_callback, NULL);

    /* Start the periodic event with a 1-second interval */
    event_periodic_callback_start(&periodic_event, US_PER_SEC);

    /* Run the event loop */
    while (1) {
        event_t *event = event_get(&main_event_queue);
        if (event) {
            event->handler(event);
        }
    }

    return 0;
}
