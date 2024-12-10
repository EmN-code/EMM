
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
static event_queue_t event_queue;

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_queue_init(&event_queue);

    xtimer_t timer;
    xtimer_init(&timer);

    while (1) {
        xtimer_set(&timer, 1000000); // 1 second
        event_t event;
        event.handler = periodic_callback;
        event.arg = NULL;
        event_post(&event_queue, &event);
        xtimer_usleep(1000000); // 1 second
    }

    return 0;
}
