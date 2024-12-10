
#include "xtimer.h"
#include "thread.h"
#include "event.h"
#include "timex.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static char stack[STACKSIZE];
static event_queue_t event_queue;

void event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    event_thread_init(&event_queue, stack, STACKSIZE, THREAD_PRIORITY_MAIN - 1);

    xtimer_t timer;
    timer.callback = event_periodic_callback;
    timer.arg = NULL;

    xtimer_set(&timer, 1 * US_PER_SEC);

    return 0;
}
