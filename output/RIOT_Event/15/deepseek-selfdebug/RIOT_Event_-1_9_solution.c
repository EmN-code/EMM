
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
static event_queue_t event_queue;

static void periodic_event_handler(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    event_queue_init(&event_queue);

    xtimer_t timer;
    timer.callback = (xtimer_callback_t)periodic_event_handler;
    timer.arg = NULL;

    xtimer_set_periodic(&timer, 1 * US_PER_SEC);

    thread_create(stack, sizeof(stack), PRIORITY, THREAD_CREATE_STACKTEST, event_thread, &event_queue, "event_thread");

    return 0;
}
