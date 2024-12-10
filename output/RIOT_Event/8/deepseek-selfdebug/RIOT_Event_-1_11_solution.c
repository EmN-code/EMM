
#include "xtimer.h"
#include "thread.h"
#include "event.h"
#include "periph/gpio.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
static event_queue_t event_queue;

static void event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    event_queue_init(&event_queue);
    event_thread_init(&event_queue, stack, sizeof(stack), PRIORITY);

    xtimer_t timer;
    timer.callback = event_periodic_callback;
    timer.arg = NULL;

    xtimer_set_periodic(&timer, XTIMER_TICKS(1000000)); // 1 second period

    return 0;
}
