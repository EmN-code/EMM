
#include "xtimer.h"
#include "thread.h"
#include "event.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static char stack[STACKSIZE];
static event_queue_t event_queue;

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    event_queue_init(&event_queue);

    event_t event;
    event.handler = periodic_callback;
    event.arg = NULL;

    xtimer_t timer;
    timer.callback = (xtimer_callback_t)event_post;
    timer.arg = &event;

    xtimer_set(&timer, 1000000); // 1 second

    while (1) {
        event_process(&event_queue);
    }

    return 0;
}
