
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define EVENT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char event_thread_stack[EVENT_THREAD_STACKSIZE];
static event_queue_t event_queue;
static kernel_pid_t event_thread_pid;

static void event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event callback triggered!\n");
}

int main(void)
{
    // Initialize the event queue
    event_queue_init(&event_queue);

    // Create the event thread
    event_thread_pid = thread_create(event_thread_stack, sizeof(event_thread_stack),
                                     EVENT_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                                     event_thread, &event_queue, "event_thread");

    // Set up a periodic event
    xtimer_t timer;
    timer.callback = event_periodic_callback;
    timer.arg = NULL;
    xtimer_set_periodic(&timer, 1 * US_PER_SEC);

    return 0;
}
