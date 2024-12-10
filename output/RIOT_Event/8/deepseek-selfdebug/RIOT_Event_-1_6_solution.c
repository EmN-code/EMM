
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define EVENT_THREAD_PRIO (THREAD_PRIORITY_MAIN - 1)

static char event_thread_stack[EVENT_THREAD_STACKSIZE];
static event_t event;

void event_periodic_callback(void *arg)
{
    (void)arg; // Ensure arg is used to avoid compiler warnings
    printf("Event periodic callback triggered\n");
}

int main(void)
{
    // Initialize the event structure
    event_init(&event, event_periodic_callback, NULL);

    // Create an event thread
    thread_create(event_thread_stack, sizeof(event_thread_stack),
                  EVENT_THREAD_PRIO, THREAD_CREATE_STACKTEST,
                  event_thread, NULL, "event_thread");

    // Set the event to trigger every second
    xtimer_set_event_periodic(&event, 1 * US_PER_SEC);

    return 0;
}
