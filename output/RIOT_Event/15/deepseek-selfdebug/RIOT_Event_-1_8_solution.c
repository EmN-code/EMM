
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
static event_queue_t event_queue;

static void periodic_event_cb(void *arg)
{
    (void)arg;
    printf("Periodic event callback triggered!\n");
}

int main(void)
{
    event_queue_init(&event_queue);
    event_thread_init(&event_queue, stack, sizeof(stack), PRIORITY);

    xtimer_t timer;
    xtimer_init(&timer);

    while (1) {
        xtimer_set(&timer, 1000000); // 1 second
        event_post(&event_queue, (event_t *)periodic_event_cb, NULL);
        xtimer_usleep(1000000); // 1 second
    }

    return 0;
}
