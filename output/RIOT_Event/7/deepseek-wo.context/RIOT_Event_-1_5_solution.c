
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define EVENT_QUEUE_SIZE     (8)
#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static event_queue_t event_queue;
static char event_thread_stack[EVENT_THREAD_STACKSIZE];

static void periodic_event_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    event_thread_init(&event_queue, event_thread_stack, sizeof(event_thread_stack), THREAD_PRIORITY_MAIN - 1);

    xtimer_t timer;
    timer.callback = periodic_event_callback;
    timer.arg = NULL;

    xtimer_set(&timer, 1000000); // 1 second in microseconds

    while (1) {
        event_wait(&event_queue);
    }

    return 0;
}
