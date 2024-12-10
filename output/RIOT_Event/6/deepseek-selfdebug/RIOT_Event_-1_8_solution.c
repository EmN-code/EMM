
#include "xtimer.h"
#include "thread.h"
#include "event.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static char stack[STACKSIZE];
static event_queue_t event_queue;

static void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic callback triggered\n");
}

int main(void) {
    event_queue_init(&event_queue);

    event_periodic_t periodic_event;
    event_periodic_init(&periodic_event, xtimer_ticks_from_usec(1000000), periodic_callback, NULL);

    event_periodic_add(&periodic_event, &event_queue);

    while (1) {
        event_process(&event_queue);
    }

    return 0;
}
