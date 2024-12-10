
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static char stack[STACKSIZE];
static event_queue_t event_queue;

static void periodic_callback(void *arg)
{
    (void)arg; // To suppress the unused parameter warning
    printf("Periodic event triggered!\n");
}

int main(void)
{
    event_queue_init(&event_queue);

    event_t periodic_event;
    periodic_event.handler = (event_handler_t)periodic_callback;

    while (1) {
        xtimer_sleep(1);
        event_post(&event_queue, &periodic_event);
    }

    return 0;
}
