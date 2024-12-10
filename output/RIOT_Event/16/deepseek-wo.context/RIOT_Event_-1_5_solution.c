
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define EVENT_THREAD_PRIO (THREAD_PRIORITY_MAIN - 1)

static char event_thread_stack[EVENT_THREAD_STACKSIZE];
static event_queue_t event_queue;

void periodic_event_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    // Initialize the event queue
    event_queue_init(&event_queue);

    // Create the event thread
    thread_create(event_thread_stack, sizeof(event_thread_stack),
                  EVENT_THREAD_PRIO, THREAD_CREATE_STACKTEST,
                  event_thread, &event_queue, "event_thread");

    // Create a periodic event
    event_periodic_t periodic_event;
    event_periodic_init(&periodic_event, xtimer_ticks(1 * US_PER_SEC),
                        (event_callback_t)periodic_event_callback, NULL);

    // Add the periodic event to the queue
    event_periodic_add(&event_queue, &periodic_event);

    // Main loop to process events
    while (1) {
        event_t *event = event_wait(&event_queue);
        event->handler(event);
    }

    return 0;
}
