
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static void event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    event_periodic_t periodic_event;
    kernel_pid_t event_thread_pid;

    /* Initialize the periodic event */
    event_periodic_init(&periodic_event, xtimer_ticks32(), xtimer_s2ticks(1), event_periodic_callback, NULL);

    /* Add the periodic event */
    event_periodic_add(&periodic_event);

    /* Initialize the event thread */
    event_thread_pid = thread_create(NULL, EVENT_THREAD_STACKSIZE, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, event_thread_init, NULL, "event_thread");

    return 0;
}
