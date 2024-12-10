
#include "xtimer.h"
#include "event.h"
#include "thread.h"
#include "periph/gpio.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE)

static char event_thread_stack[EVENT_THREAD_STACKSIZE];
static event_queue_t event_queue;

static void event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered\n");
}

static void event_loop(void)
{
    while (1) {
        event_t event;
        event_wait(&event);
        event.handler(&event);
    }
}

int main(void)
{
    event_thread_init(&event_queue, event_thread_stack, sizeof(event_thread_stack), THREAD_PRIORITY_MAIN - 1);

    xtimer_t timer;
    event_t periodic_event;
    periodic_event.handler = event_periodic_callback;

    xtimer_init(&timer, &periodic_event, NULL);
    xtimer_set_periodic(&timer, 1000000);

    event_loop();

    return 0;
}
