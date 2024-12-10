
#include "event/periodic_callback.h"
#include "ztimer.h"
#include "xtimer.h"

#define EVENT_QUEUE_SIZE (8)
#define EVENT_STACK_SIZE (THREAD_STACKSIZE_DEFAULT)

static event_queue_t event_queue;
static char event_stack[EVENT_STACK_SIZE];

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_periodic_callback_t periodic_event;

    /* Initialize the event queue */
    event_thread_init(&event_queue, event_stack, sizeof(event_stack), PRIORITY_MAIN - 1);

    /* Initialize the periodic callback event */
    event_periodic_callback_init(&periodic_event, ZTIMER_MSEC, 1000, &event_queue, periodic_callback);

    /* Start the periodic callback */
    event_periodic_callback_start(&periodic_event.periodic);

    /* Keep the main thread alive */
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
