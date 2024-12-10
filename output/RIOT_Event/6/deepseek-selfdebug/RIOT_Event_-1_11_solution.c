
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)

static char stack[STACK_SIZE];
static event_queue_t event_queue;

void event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    event_queue_init(&event_queue);

    xtimer_t timer;
    timer.callback = event_periodic_callback;
    timer.arg = NULL;

    xtimer_set(&timer, 1000000); // 1 second interval

    while (1) {
        event_wait(&event_queue);
    }

    return 0;
}
