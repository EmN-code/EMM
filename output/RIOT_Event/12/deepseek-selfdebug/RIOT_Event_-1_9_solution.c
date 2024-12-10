
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define EVENT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char event_thread_stack[EVENT_THREAD_STACKSIZE];
static event_queue_t event_queue;

static void periodic_event_handler(void *arg)
{
    (void)arg; // Unused argument
    printf("Periodic event triggered!\n");
}

int main(void)
{
    // Initialize the event queue
    event_thread_init(&event_queue, event_thread_stack, sizeof(event_thread_stack), EVENT_THREAD_PRIORITY);

    // Create a periodic event
    event_t periodic_event;
    periodic_event.handler = (event_handler_t)periodic_event_handler;

    // Set the periodic event to trigger every second
    xtimer_periodic_event(&periodic_event, XTIMER_TICKS(1 * US_PER_SEC), &event_queue);

    // Start the event thread
    event_thread_start(&event_queue);

    return 0;
}
