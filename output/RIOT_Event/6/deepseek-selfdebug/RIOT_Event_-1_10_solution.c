
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)

static char stack[STACK_SIZE];
static event_queue_t event_queue;

static void periodic_event_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    // Initialize the event queue
    event_queue_init(&event_queue);

    // Create a periodic event
    xtimer_t periodic_timer;
    periodic_timer.callback = (xtimer_callback_t)periodic_event_callback;
    periodic_timer.arg = NULL;

    // Set the periodic timer to trigger every second
    xtimer_set(&periodic_timer, 1000000);

    // Main loop to process events
    while (1) {
        event_t event;
        event_wait(&event_queue, &event);
        event.handler(&event);
    }

    return 0;
}
