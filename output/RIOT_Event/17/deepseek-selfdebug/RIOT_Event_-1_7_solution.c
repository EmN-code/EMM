
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define EVENT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static event_queue_t event_queue;
static char event_thread_stack[EVENT_THREAD_STACKSIZE];

static void periodic_event_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

static void event_periodic_init(void)
{
    event_queue_init(&event_queue);
    event_thread_init(&event_queue, event_thread_stack, sizeof(event_thread_stack), EVENT_THREAD_PRIORITY);

    xtimer_t timer;
    timer.callback = periodic_event_callback;
    timer.arg = NULL;

    xtimer_set(&timer, 1000000); // 1 second interval
}

int main(void)
{
    event_periodic_init();

    while (1) {
        // Main loop can be used for other tasks
    }

    return 0;
}
