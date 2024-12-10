
#include "thread.h"
#include "event.h"
#include "ztimer.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
static char event_thread_stack[EVENT_THREAD_STACKSIZE];

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_queue_t main_queue;
    event_queue_init(&main_queue);

    event_thread_init(&main_queue, event_thread_stack, EVENT_THREAD_STACKSIZE, THREAD_PRIORITY_MAIN - 1);

    ztimer_periodic_callback_t periodic_event;
    ztimer_periodic_callback_init(&periodic_event, ZTIMER_SEC, &main_queue, periodic_callback);
    ztimer_periodic_callback_set_interval(&periodic_event, 1 * US_PER_SEC);
    ztimer_periodic_callback_start(&periodic_event);

    while (1) {
        event_process(&main_queue);
    }

    return 0;
}
